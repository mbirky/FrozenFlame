/***********************************************
 * Filename:  		Emitter.cpp
 * Date:      		11/16/2012
 * Mod. Date: 		11/16/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		Emitter to render particles and defenitions
 ************************************************/
#include "Emitter.h"
#include <iostream>

using std::cout;

const string g_szParticlePath = "../../Resource/Textures/Particles/";

/*******************************
* Private Functions
********************************/
void CEmitter::InitModifiers(tModifiers& mod)
{
	mod.m_bToDestination = false;
	mod.m_bAreaRotation = true;
	mod.m_bDestinationTrack = false;
	mod.m_pDestObj		= nullptr;
	mod.m_nSourceBlend	= -1;
	mod.m_nDestinationBlend = -1;
	mod.m_fMinLife		= 0.0f;
	mod.m_fMaxLife		= 0.0f;
	mod.m_fRangeW		= 0.0f;
	mod.m_fRangeH		= 0.0f;
	mod.m_vEmitterRotation = D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
	mod.m_cStartColor	= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mod.m_cMidColor		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mod.m_cEndColor		= D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);	
	mod.m_vMinGravity	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vMaxGravity	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vVel1			= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vVel2			= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vVelocity		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vStartPosition = D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vEndPosition	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vOffset		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_vRotation		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	mod.m_fStartScale	= D3DXVECTOR3(1.0f,1.0f,1.0f);
	mod.m_fEndScale		= D3DXVECTOR3(1.0f,1.0f,1.0f);	
}

/********************************
* Public Functions
*********************************/
CEmitter::CEmitter(void)
{
	SetType(PARTICLE_OBJECT);

	m_bToDestination = false;
	m_bDestinationTrack = false;
	m_bAreaRotation = true;
	m_pSourceAttach = nullptr;
	m_pDestinationAttach = nullptr;
	InitModifiers(m_tMods);
	m_nRenderType = -1;
	m_vParticles = nullptr;
	m_nNumParticles = 0;
	m_nAliveCount = 0;
	m_nAliveLastFrame = 0;
	m_bIsActive = false;
	m_nDestBlend = -1;
	m_nSourceBlend = -1;
	m_nShader = -1;
	m_nMinToSpit = 1;
	m_nMaxToSpit = 10;
	m_nTotalSpat = 0;
	m_fEmitterLifetime = 0.0f;
	m_fCurrentLifetime = 0.0f;
	m_fRangeW = 0.0f;
	m_fRangeH = 0.0f;
	m_fGravityStrength = 1;	
	//SetShader("Particles.fx");	 // Default particle shader. - JC
}

CEmitter::~CEmitter(void)
{
	if(m_vParticles != nullptr)
	{
		if(m_nRenderType == THREE_D)
		{
			for(int i = 0; i < m_nNumParticles; i++)
			{
				CView::Pop(m_vParticles[i].GetRenderID());
			}
		}
		else if(GetType() == TWO_D)
		{
			CView::Pop(this->GetRenderID());
		}


		delete[] m_vParticles;
		m_vParticles = nullptr;
	}

	if(GetRenderID() > -1)
		CView::Pop(this->GetRenderID());
}

void CEmitter::Initialize()
{
	CBaseObject::Initialize();
	m_fRangeW = 0.0f;
	m_fRangeH = 0.0f;

	/*for(int i = 0; i < GetParticleCount(); ++i)
		m_vParticles[i].Reinitialize();*/
}

void CEmitter::Reinitialize()
{
	if(!GetInitialized())
	{
		Initialize();
	}
}

void CEmitter::Uninitialize()
{
	CBaseObject::Uninitialize();
}

void CEmitter::Update(float fElapsedTime)
{
	// Completely gutted how this update works.  Come at me bro. - JC
	if(m_bIsActive)  {

		// Update our emitter position to our source attachment (if any)
		if(m_pSourceAttach) {
			SetEmittPos(m_pSourceAttach->GetPos()); }

		if(m_bToDestination || m_bDestinationTrack ) {
			if(m_pDestinationAttach) {
				m_tMods.m_vEndPosition = D3DXVECTOR3(m_pDestinationAttach->GetPos().x,m_pDestinationAttach->GetPos().y,
					m_pDestinationAttach->GetPos().z); }
		}

		
		// Do that counting thing. - JC
		m_nAliveLastFrame = m_nAliveCount;
		m_nAliveCount = 0;
		m_fCurrentLifetime += fElapsedTime;
		
		if(m_bAreaRotation) {
			MakeEmitterRotation(); }
		
		KillCreatePhase();

		if(!m_bIsActive) {
			return; }

		UpdatePhase(fElapsedTime);
	
		//// Check if we need to kill this emitter.
		if(!m_bIsLooping && m_fCurrentLifetime >= m_fEmitterLifetime && m_nAliveCount <= 0) {
			SetActive(false);
			return;	}
	}
}

void CEmitter::MakeEmitterRotation()
{
	// Pass rotation of emitter's attachment for emission area rotation - JC
		if(m_pSourceAttach) {
			const matrix4f* pObjMat = m_pSourceAttach->GetWorldMatrix();

			// Lotsa-matsa math
			float scaleX = sqrt(pObjMat->m[0][0] * pObjMat->m[0][0] + pObjMat->m[0][1] * pObjMat->m[0][1] + pObjMat->m[0][2] * pObjMat->m[0][2]);
			float scaleZ = sqrt(pObjMat->m[2][0] * pObjMat->m[2][0] + pObjMat->m[2][1] * pObjMat->m[2][1] + pObjMat->m[2][2] * pObjMat->m[2][2]);

			// Create "2x2" rotation matrix - {{rotx,rotx},{rotz,rotz}}
			m_tMods.m_vEmitterRotation = D3DXVECTOR4( pObjMat->m[0][0] / scaleX, pObjMat->m[0][2] / scaleX,
				pObjMat->m[2][0] / scaleZ, pObjMat->m[2][2] / scaleZ);
		}
}

void CEmitter::UpdatePhase(float fElapsedTime)
{
	for(int i = 0; i < m_nNumParticles; ++i) {
		m_vParticles[i].Updates(fElapsedTime,m_tMods,m_nRenderType,m_bIsLooping, m_fGravityStrength); }	
}

void CEmitter::KillCreatePhase(void)
{
	// Find out how many particles we should spit this round.
	int numToSpit = (rand() % (m_nMaxToSpit - m_nMinToSpit)) + m_nMinToSpit; 
	int numSpitThisFrame = 0; // How many were actually spat.

	for(int i = 0; i < m_nNumParticles; ++i) {		
		if(LogansRun(m_vParticles + i)) { // Lets us know we can create a new one with this particle. -JC				
			CreateParticle(numToSpit, m_vParticles + i, numSpitThisFrame); }
	}
}

bool CEmitter::LogansRun(CParticle* in)
{
	// YOU'RE TOO OLD!
	if(in->GetAlive() && in->m_fCurrentlife >= in->m_fLifeTime) {		
		//in->ResetParticle(m_tMods,m_nRenderType);
		in->m_bIsAlive = false; }

	if(!in->GetAlive()) {
		return true;  }
	else {  // You've survived this round...but soon.
		++m_nAliveCount;
		return false; }
}

void CEmitter::CreateParticle(const int numToSpit,CParticle* in,int& numSpitThisFrame)
{
	// Make sure not creating stuff we don't need for non looping emitters
	if(!m_bIsLooping && m_fCurrentLifetime >= m_fEmitterLifetime) {
		return; }

	if(numSpitThisFrame < numToSpit) {
		in->ResetParticle(m_tMods,m_nRenderType);
		in->m_bIsAlive = true;
		++m_nAliveCount;
		++numSpitThisFrame;
		++m_nTotalSpat;	}
}

void CEmitter::PlayAnims(int AnimID, bool loop)
{
	if(m_bIsActive == true && m_nRenderType == THREE_D)
	{
		for(int i = 0; i < m_nNumParticles; i++)
		{
			m_vParticles[i].PlayAnim(AnimID,loop);
		}
	}
}

void CEmitter::SetEmittPos(const D3DVECTOR& vPos)
{
	m_vEmittPosition = vPos;

	m_tMods.m_vStartPosition = m_vEmittPosition;
}

void CEmitter::SetEmittPos(const vec3f& vPos)
{
	m_vEmittPosition.x = vPos.x;
	m_vEmittPosition.y = vPos.y;
	m_vEmittPosition.z = vPos.z;

	m_tMods.m_vStartPosition = m_vEmittPosition;
}

void CEmitter::SetEmittPos(float x, float y, float z)
{
	m_vEmittPosition.x = x;
	m_vEmittPosition.y = y;
	m_vEmittPosition.z = z;

	m_tMods.m_vStartPosition = m_vEmittPosition;
}

void CEmitter::SetDestinationPos( vec3f in )
{
	m_vDestinationPosition.x = in.x;
	m_vDestinationPosition.y = in.y;
	m_vDestinationPosition.z = in.z;

	m_tMods.m_vEndPosition = m_vDestinationPosition;
}

void CEmitter::SetActive(bool bActive)
{
	for(int i = 0; i < m_nNumParticles; i++)
	{
		m_vParticles[i].m_bIsEmitActive = bActive;
	}

	if( bActive && !m_bIsActive ) {		
		ResetParticles();
		m_nTotalSpat = 0;
		m_nAliveCount = 0;
		m_nAliveLastFrame = 0;
		m_fCurrentLifetime = 0.0f; }

	m_bIsActive = bActive;


}

void CEmitter::LoadParticle(const char* cFilename)
{
	fstream in;
	string texture;
	int width;
	int height;
	string model;

	in.open(cFilename,ios_base::in);

	if(in.is_open())
	{
		in >> m_nRenderType;
		in.ignore(INT_MAX,'\n');

		in >> m_nNumParticles;
		in.ignore(INT_MAX,'\n');

		// hey if there is no particles why do all the extra work?
		if(m_nNumParticles == 0)
		{
			in.close();
			LOG("Number of Particles is 0");
			return;
		}
		
		in >> m_nMinToSpit;
		in.ignore(INT_MAX,'\n');

		in >> m_nMaxToSpit;
		in.ignore(INT_MAX,'\n');

		in >> m_fEmitterLifetime;
		in.ignore(INT_MAX,'\n');

		in >> texture;
		in.ignore(INT_MAX,'\n');

		in >> width;
		in.ignore(INT_MAX,'\n');

		in >> height;
		in.ignore(INT_MAX,'\n');

		in >> model;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fStartScale.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fStartScale.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fStartScale.z;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fEndScale.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fEndScale.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fEndScale.z;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vRotation.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vRotation.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vRotation.z;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fMinLife;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_fMaxLife;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cStartColor.r;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cStartColor.g;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cStartColor.b;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cStartColor.a;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cMidColor.r;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cMidColor.g;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cMidColor.b;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cMidColor.a;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cEndColor.r;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cEndColor.g;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cEndColor.b;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_cEndColor.a;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vMinGravity.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vMinGravity.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vMinGravity.z;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vMaxGravity.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vMaxGravity.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vMaxGravity.z;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vVel1.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vVel1.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vVel1.z;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vVel2.x;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vVel2.y;
		in.ignore(INT_MAX,'\n');

		in >> m_tMods.m_vVel2.z;
		in.ignore(INT_MAX,'\n');

		in >> m_fRangeW;		
		in.ignore(INT_MAX, '\n');

		in >> m_fRangeH;		
		in.ignore(INT_MAX, '\n');

		in >> m_nSourceBlend;
		in.ignore(INT_MAX, '\n');

		in >> m_nDestBlend;
		in.ignore(INT_MAX, '\n');

		in >> m_fGravityStrength;
		in.ignore(INT_MAX, '\n');
		
	}
	else
	{
		string error = cFilename;
		error = error + "not Found!";
		LOG(error.c_str());
		in.close();
		return;
	}

	in.close();	

	if(m_nSourceBlend < 0 ) {
 		m_nSourceBlend = 5;		
 	}
 	if(m_nDestBlend < 0 ) {
 		m_nDestBlend = 2;
 	}

	if (m_fGravityStrength <= 0.0f)
		m_fGravityStrength = 1.0f;
 		
 
 	m_tMods.m_nSourceBlend = m_nSourceBlend;
 	m_tMods.m_nDestinationBlend = m_nDestBlend;
	m_tMods.m_bDestinationTrack = false;
	m_tMods.m_pDestObj = nullptr;

	if(m_fRangeW <= 0.0f)
		m_fRangeW = 0.0f;

	if(m_fRangeH <= 0.0f)
		m_fRangeH = 0.0f;

	m_tMods.m_fRangeW = m_fRangeW;
	m_tMods.m_fRangeH = m_fRangeH;

	if(m_vParticles)
	{
		delete[] m_vParticles;
		m_vParticles = nullptr;
	}

	if(m_vParticles == nullptr)
	{
		m_vParticles = new CParticle[m_nNumParticles];
	}

	int textureID = -1;

	// Switch for new TWO_D/THREE_D values
	switch(m_nRenderType)
	{
	case 2:
		m_nRenderType = TWO_D;
		SetType(TWO_D);
		break;
	case 3:
		m_nRenderType = THREE_D;
		SetType(THREE_D);
		break;
	}

	if(texture != "none.png")
	{
		texture = g_szParticlePath + texture;
		textureID = CView::LoadSprite(texture.c_str(),&width,&height);
	}

	if(m_nRenderType == TWO_D)
	{
		model = "none";
		SetRenderID(CView::Push((CBaseObject*)this,model, RENDER_TWOD_PARTICLE)); // Model should be null for 2D - JC
	 }

	// Destination stuff
	m_tMods.m_bToDestination = false;
	m_tMods.m_vEndPosition = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
	for(int i = 0; i < m_nNumParticles; i++)
	{
		m_vParticles[i].Initialize();
		m_vParticles[i].m_bIsAlive = false;
		m_vParticles[i].m_nWidth = width;
		m_vParticles[i].m_nHeight = height;
		m_vParticles[i].m_bIsEmitActive = m_bIsActive;

		m_vParticles[i].ResetParticle(m_tMods, m_nRenderType);

		if(m_nRenderType == TWO_D)
		{
			m_vParticles[i].m_nTextureID = textureID;
		}
		
		if(m_nRenderType == THREE_D)
		{
			m_vParticles[i].SetType(THREE_D);
			m_vParticles[i].SetRenderID(CView::Push((CBaseObject*)&m_vParticles[i],model, RENDER_EFFECT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
			matrix4f worldMatrix = *m_vParticles[i].GetWorldMatrix();
			worldMatrix.scale_post(0.1f, 0.1f, 0.1f);
			worldMatrix.rotate_x_post(D3DXToRadian(90.0f));
			m_vParticles[i].SetWorldMatrix(worldMatrix);			
		}
	}

}

void CEmitter::ScaleParticles(float nAmount,int nAxis)
{
	for(int i = 0; i < m_nNumParticles; i++)
	{
		matrix4f worldMatrix = *m_vParticles[i].GetWorldMatrix();
		switch(nAxis)
		{
		case AXIS_X:
			{
				worldMatrix.xx = nAmount;
				m_vParticles[i].SetWorldMatrix(worldMatrix);	
			}
			break;

		case AXIS_Y:
			{
				worldMatrix.yy = nAmount;
				m_vParticles[i].SetWorldMatrix(worldMatrix);
			}
			break;

		case AXIS_Z:
			{
				worldMatrix.zz = nAmount;
				m_vParticles[i].SetWorldMatrix(worldMatrix);
			}
			break;
		}
	}
}

void CEmitter::ScaleParticles(float x, float y, float z)
{
	for(int i = 0; i < m_nNumParticles; i++)
	{
		matrix4f worldMatrix = *m_vParticles[i].GetWorldMatrix();
		worldMatrix.xx = x;
		worldMatrix.yy = y;
		worldMatrix.zz = z;
		m_vParticles[i].SetWorldMatrix(worldMatrix);
	}
}

void CEmitter::RotateParticles(bool bPrePost,float nAmount,int nAxis)
{
	for(int i = 0; i < m_nNumParticles; i++)
	{
		matrix4f worldMatrix = *m_vParticles[i].GetWorldMatrix();
		switch(nAxis)
		{
		case 0:
			{
				if (bPrePost)
				{
					worldMatrix.rotate_x_post(D3DXToRadian(nAmount));
					m_vParticles[i].SetWorldMatrix(worldMatrix);
				}
				else
				{
					worldMatrix.rotate_x_pre(D3DXToRadian(nAmount));
					m_vParticles[i].SetWorldMatrix(worldMatrix);
				}
			}
			break;
		case 1:
			{
				if (bPrePost)
				{
					worldMatrix.rotate_y_post(D3DXToRadian(nAmount));
					m_vParticles[i].SetWorldMatrix(worldMatrix);
				}
				else
				{
					worldMatrix.rotate_y_pre(D3DXToRadian(nAmount));
					m_vParticles[i].SetWorldMatrix(worldMatrix);
				}
			}
			break;
		case 2:
			{
				if (bPrePost)
				{
					worldMatrix.rotate_z_post(D3DXToRadian(nAmount));
					m_vParticles[i].SetWorldMatrix(worldMatrix);
				}
				else
				{
					worldMatrix.rotate_z_pre(D3DXToRadian(nAmount));
					m_vParticles[i].SetWorldMatrix(worldMatrix);
				}
			}
			break;
		}
	}
}

void CEmitter::ResetParticles(void)
{
	for(int i = 0; i < m_nNumParticles; i++)
	{
		m_vParticles[i].ResetParticle(m_tMods,m_nRenderType);
	}
		
}

void CEmitter::SetShader(string szFileName) 
{
	m_nShader = CAssetManager::LoadShader(szFileName);
}

void CEmitter::ActivateDestination(bool in)
{
	m_bToDestination = in;
	m_tMods.m_bToDestination = in;
}

void CEmitter::SetAttachedDest(CBaseObject* in)
{
	m_pDestinationAttach = in;
	m_tMods.m_pDestObj = in;

	if(in) {
		m_tMods.m_vEndPosition = D3DXVECTOR3(in->GetPos().x,in->GetPos().y,in->GetPos().z); }
	else {
		m_tMods.m_vEndPosition = D3DXVECTOR3(0.0f,0.0f,0.0f); }
}

void CEmitter::ActivateDestinationTracking(bool in)
{
	m_bDestinationTrack = in;
	m_tMods.m_bDestinationTrack = m_bDestinationTrack;
}

void CEmitter::AreaRotation(bool in) 
{ 
	m_bAreaRotation = in; 
	m_tMods.m_bAreaRotation = in; 
}

/******************************
* Particle Class Functions
*******************************/

CEmitter::CParticle::CParticle(void)
{
	SetType(PARTICLE_OBJECT);
}

CEmitter::CParticle::~CParticle(void)
{

}

void CEmitter::CParticle::Initialize(void)
{
	CBaseObject::Initialize();

	m_bIsAlive = false;
	IsMid = false;
	m_fCurrentlife = 0.0f;
	m_fLifeTime = 0.0f;
	m_fRotationX = 0.0f;
	m_fRotationY = 0.0f;
	m_fRotationZ = 0.0f;
	m_cColor = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	m_vScale.x = 0.0f;
	m_vScale.y = 0.0f;
	m_vScale.z = 0.0f;
	m_nTextureID = -1;	

	SetType(PARTICLE_OBJECT);
	SetInitialized(true);
}

void CEmitter::CParticle::Reinitialize(void)
{
	if(!GetInitialized())
	{
		Initialize();
	}
}

void CEmitter::CParticle::Uninitialize(void)
{
	CBaseObject::Uninitialize();
}

void CEmitter::CParticle::Updates(float fElapsedTime,tModifiers& tMods, int nType, bool bIsLooping, float fGravityStrength)
{
	if(m_bIsAlive == true)
	{
		m_fCurrentlife += fElapsedTime;

		if(tMods.m_bDestinationTrack && tMods.m_pDestObj ) {		
			// Get vector to end point and get velocity in that direction - JC
			vec3f toEnd = GetPos() - tMods.m_pDestObj->GetPos();
			toEnd.normalize();
			// Apply our randomized velocity to our direction.
			if (m_vOriginalVelocity.x != 0.0f && m_vOriginalVelocity.y != 0.0f && m_vOriginalVelocity.z != 0.0f)
				this->SetVelocity(m_vOriginalVelocity * toEnd * fGravityStrength); 
			else
				SetVelocity(toEnd * fGravityStrength * -1.0f);
		}

		if(abs(tMods.m_fStartScale.x - tMods.m_fEndScale.x) >= 0.0001f && abs(tMods.m_fStartScale.y - tMods.m_fEndScale.y) >= 0.0001f
			&& abs(tMods.m_fStartScale.z - tMods.m_fEndScale.z) >= 0.0001f)
		{
			D3DXVec3Lerp(&m_vScale,&tMods.m_fStartScale,&tMods.m_fEndScale,m_fCurrentlife / m_fLifeTime);

			if(nType == THREE_D)
			{
				matrix4f worldMatrix = *GetWorldMatrix();
				worldMatrix.xx = m_vScale.x;
				worldMatrix.yy = m_vScale.y;
				worldMatrix.zz = m_vScale.z;
				SetWorldMatrix(worldMatrix);
			}
		}

		m_fRotationX += (tMods.m_vRotation.x * fElapsedTime);
		m_fRotationY += (tMods.m_vRotation.y * fElapsedTime);
		m_fRotationZ += (tMods.m_vRotation.z * fElapsedTime);

		vec3f newVel = GetVelocity();
		newVel.x += m_vGravity.x * fElapsedTime;
		newVel.y += m_vGravity.y * fElapsedTime;
		newVel.z += m_vGravity.z * fElapsedTime;

		SetVelocity(newVel);

		SetPos(vec3f(GetPos().x + (GetVelocity().x * fElapsedTime), 
			  GetPos().y + (GetVelocity().y * fElapsedTime), 
			  GetPos().z + (GetVelocity().z * fElapsedTime)));
		
		if(nType == THREE_D)
		{
			matrix4f worldMatrix = *GetWorldMatrix();
			worldMatrix.rotate_x_post(D3DXToRadian(m_fRotationX));
			worldMatrix.rotate_y_post(D3DXToRadian(m_fRotationY));
			worldMatrix.rotate_z_post(D3DXToRadian(m_fRotationZ));
			SetWorldMatrix(worldMatrix);
			
			vec3f local((float)m_cColor.r,(float)m_cColor.g,(float)m_cColor.b);
			SetColor(local);
		}

		if(IsMid == false)
		{
			D3DXColorLerp(&m_cColor,&tMods.m_cStartColor,&tMods.m_cMidColor,m_fCurrentlife / (m_fLifeTime * 0.5f));

			if(abs(m_cColor.r - tMods.m_cMidColor.r) <= 0.1f && abs(m_cColor.g - tMods.m_cMidColor.g) <= 0.1f 
				&& abs(m_cColor.b - tMods.m_cMidColor.b) <= 0.1f && abs(m_cColor.a - tMods.m_cMidColor.a) <= 0.1f)
			{
				IsMid = true;
			}
		}
		else
		{
			D3DXColorLerp(&m_cColor,&tMods.m_cMidColor,&tMods.m_cEndColor,(m_fCurrentlife -(m_fLifeTime * 0.5f)) / (m_fLifeTime * 0.5f));
		}

		
	}
}

float CEmitter::CParticle::RandomFloatInARange( const float fHigh, const float fLow )
{
	return fLow + (float)rand() / ((float)RAND_MAX / (fHigh - fLow));
}

void CEmitter::CParticle::ResetParticle(tModifiers& tMods, int nType)
{
	//float rangeXMod = RandomFloatInARange(tMods.m_fRangeW,0.0f);
	//float rangeZMod = RandomFloatInARange(tMods.m_fRangeH,0.0f);	

	// Here's where a range is given in which any particle
	float rangeX = RandomFloatInARange(tMods.m_fRangeW,-tMods.m_fRangeW);
	float rangeZ = RandomFloatInARange(tMods.m_fRangeH,-tMods.m_fRangeH);	
		
	//******* MESS WITH ANY OF THIS ROTATION STUFF AND I WILL BURN YOUR HOUSE DOWN **********// -JC
	if(tMods.m_bAreaRotation) {

		// Use 2d rotation matrix from tmods to rotate the range to the object
		D3DXVECTOR3 range(rangeX,1.0f,rangeZ);
		D3DXMATRIX rangeRot;
		D3DXMatrixIdentity(&rangeRot);
		rangeRot.m[0][0] = tMods.m_vEmitterRotation.x;
		rangeRot.m[0][2] = tMods.m_vEmitterRotation.y;
		rangeRot.m[2][0] = tMods.m_vEmitterRotation.z;
		rangeRot.m[2][2] = tMods.m_vEmitterRotation.w;
		
		D3DXVECTOR4 finalRange;
		D3DXVec3Transform(&finalRange,&range,&rangeRot);
		
		rangeX = finalRange.x;
		rangeZ = finalRange.z; }
	

	// Particles can now be offset by an offset from their emitter.  Sometimes you want
	// an emitter to follow an object but the particles to show up somewhere specific in
	// relation to the model.  That's where this comes in. -JC
	SetPos(vec3f(tMods.m_vStartPosition.x + tMods.m_vOffset.x + rangeX, tMods.m_vStartPosition.y + tMods.m_vOffset.y, 
		tMods.m_vStartPosition.z + tMods.m_vOffset.z + rangeZ));

	m_fCurrentlife = 0.0f;

	float min = tMods.m_fMinLife;
	float max = tMods.m_fMaxLife;

	m_fLifeTime = RandomFloatInARange(max,min);

	m_vScale.x = tMods.m_fStartScale.x;
	m_vScale.y = tMods.m_fStartScale.y;
	m_vScale.z = tMods.m_fStartScale.z;

	if(GetRenderID() > -1)
	{
		matrix4f worldMatrix = *GetWorldMatrix();
		worldMatrix.xx = tMods.m_fStartScale.x;
		worldMatrix.yy = tMods.m_fStartScale.y;
		worldMatrix.zz = tMods.m_fStartScale.z;
		SetWorldMatrix(worldMatrix);
	}

	m_fRotationX = tMods.m_vRotation.x;
	m_fRotationY = tMods.m_vRotation.y;
	m_fRotationZ = tMods.m_vRotation.z;

	min = tMods.m_vVel2.x;
	max = tMods.m_vVel1.x;

	
	float velx = RandomFloatInARange(max,min);

	min = tMods.m_vVel2.y;
	max = tMods.m_vVel1.y;

	float vely = RandomFloatInARange(max,min);

	min = tMods.m_vVel2.z;
	max = tMods.m_vVel1.z;

	float velz = RandomFloatInARange(max,min);
	
	vec3f Velocity(velx,vely,velz);
	m_vOriginalVelocity = Velocity;
		
	if(tMods.m_bToDestination || tMods.m_bDestinationTrack ) {
		vec3f end(tMods.m_vEndPosition.x,tMods.m_vEndPosition.y,tMods.m_vEndPosition.z);
		vec3f start(tMods.m_vStartPosition.x, tMods.m_vStartPosition.y, tMods.m_vStartPosition.z);
		// Get vector to end point and get velocity in that direction - JC
		vec3f toEnd = end - start;
		toEnd.normalize();
		// Apply our randomized velocity to our direction.
		Velocity *= toEnd;
	}

	SetVelocity(Velocity);

	min = tMods.m_vMinGravity.x;
	max = tMods.m_vMinGravity.x;

	m_vGravity.x = RandomFloatInARange(max,min);

	min = tMods.m_vMinGravity.y;
	max = tMods.m_vMinGravity.y;

	m_vGravity.y = RandomFloatInARange(max,min);

	min = tMods.m_vMinGravity.z;
	max = tMods.m_vMinGravity.z;

	m_vGravity.z = RandomFloatInARange(max,min);

	m_cColor = tMods.m_cStartColor;
	IsMid = false;

	m_bIsAlive = false;	
}

void CEmitter::CParticle::PlayAnim(int AnimID,bool loop)
{
	// This is not being used so I disabled it for now - Dan
	// CView::GetAnimPlayer(GetRenderID())->Play(AnimID,loop);
}