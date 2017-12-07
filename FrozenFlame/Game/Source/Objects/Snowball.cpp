/***********************************************
 * Filename:  		Snowball.cpp
 * Date:      		11/17/2012
 * Mod. Date: 		11/17/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the Snowball object
 ************************************************/

#include "Snowball.h"

#include "../Renderer/View.h"
#include "../Renderer/Emitter.h"
#include "../CGame.h"
#include "../Sound/CSoundManager.h"
#include "../Core/CWorldManager.h"
#include "../Renderer/CEmitterManager.h"


/*****************************************************************
*	CSnowball():	Default constructor for the Snowball object
*						
*
* Ins:					void
*
* Outs:					void
*
* Returns:				n/a
*
* Mod. Date:		    11/17/2012
* Mod. Initials:	    CM
*****************************************************************/
CSnowball::CSnowball(void)
{
	SetType(SNOWBALL_OBJECT);
	m_pcFireEffect = nullptr;
	m_bHitByFire = false;
}

/*****************************************************************
*	~CSnowball():	Default destructor for the enemy base object
*						
*
* Ins:					void
*
* Outs:					void
*
* Returns:				n/a
*
* Mod. Date:		    11/17/2012
* Mod. Initials:	    CM
*****************************************************************/
CSnowball::~CSnowball(void)
{
	if(m_pcFireEffect != nullptr)
	{
		m_pcFireEffect->SetActive(false);
		CView::GetEmitterManager().KillEmitter(m_pcFireEffect);
		m_pcFireEffect = nullptr;
	}
}

/*****************************************************************
*	Initialize():		The default funciton that sets all starting values for
*						this instance of the class
*
* Ins:					nType
*
* Outs:					void
*
* Returns:				void
*
* Mod. Date:		    11/17/2012
* Mod. Initials:	    CM
*****************************************************************/
void CSnowball::Initialize(void)
{
	// Call the paren'ts Initialize
	CCollidableObject::Initialize();

	// Push it to the view
	SetRenderID(CView::Push(this, "Snowball"));
	CView::SetRenderInstanceFlags(GetRenderID(),RENDER_NOT_SHADOWCASTER | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);

	// Initialize values
	SetOwner(nullptr);
	SetLifeTime(0.0f);
	SetDeflected(false);
	SetVelocity(vec3f(0, 0, 0));
	SetAttackElement(ICE);
	m_pcFireEffect = nullptr;
	m_bHitByFire = false;
	m_pcIceExplosionSmall = nullptr;
	m_pIceExplosionLarge = nullptr;
	this->SetSphereRad(this->GetSphereRad()/2.0f);
	m_tShadow.m_nImageID = CView::LoadSprite("WizardShadow.png", &m_tShadow.m_nImageWidth, &m_tShadow.m_nImageHeight);
	m_nShadowRenderID = CView::DynamicShapeCreate(GetRenderID(), true);
	m_nMoltenRenderID = -1;//CView::DynamicShapeCreate(GetRenderID(), false, vec3f(1.3f, 1.0f, 1.3f));
	CView::DynamicShapeMakePlane(m_nShadowRenderID, 3, 3);
	CView::DynamicShapeSetTextures(m_nShadowRenderID, m_tShadow.m_nImageID, -1);

	m_bWasThrown = false;
}

void CSnowball::SetAttackElement(int _nAttackElement)
{
	CCollidableObject::SetAttackElement(_nAttackElement);
}

/*****************************************************************
*	Reinitialize():		The default funciton that resets all starting values for
*						this instance of the class
*
* Ins:					void
*
* Outs:					void
*
* Returns:				void
*
* Mod. Date:		    11/17/2012
* Mod. Initials:	    CM
*****************************************************************/
void CSnowball::Reinitialize()
{
	if(!GetInitialized())
	{
		Initialize();
	}
}

/*****************************************************************
*	Update():			Updates all data for the class based off of time
*
* Ins:					fElapsedTime
*
* Outs:					void
*
* Returns:				void
*
* Mod. Date:		    11/17/2012
* Mod. Initials:	    CM
*****************************************************************/
void CSnowball::Update(float fElapsedTime)
{
	// If the snowball wasn't thrown yet, do nothing
	if(!m_bWasThrown)
		return;

	// Apply gravity to the velocity
	SetVelocity(GetVelocity() + EARTH_GRAVITY * fElapsedTime * 1.4f);
	
	// Set position based on velocity
	SetPos(GetPos() + GetVelocity() * fElapsedTime/* * 1.4f*/);
	
	// Update collision data
	//UpdateCollisionData();
	CCollidableObject::Update(fElapsedTime);

	if(!GetInitialized())
		return;

	this->SetSphereRad(this->GetSphereRad()/1.5f);

	// update shadow
	if(GetRenderID() != -1 && m_nShadowRenderID != -1)
	{
		matrix4f mShadowMatrix;// = *GetWorldMatrix();
		mShadowMatrix.make_identity();
		mShadowMatrix.axis_pos.y -= GetPos().y - 0.75f;
		//mShadowMatrix.make_translation(GetPos().x,CWorldManager::GetInstance()->GetGroundHeight() + 0.5f, GetPos().z);
		CView::DynamicShapeSetLocalMatrix(m_nShadowRenderID, mShadowMatrix);
	}

	// When it hits the ground, uninitialize
	if(GetPos().y <= -3.0f || m_bHitByFire)
	{
	//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_Crash");
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_CRASH, GetSoundID());
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SQUISH, GetSoundID());

		if (this->GetAttackElement() == FIRE)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_METEOR_LAND, GetSoundID());
		}

		Uninitialize();
	}

	if (m_pcFireEffect)
	{
		m_pcFireEffect->SetEmittPos(GetPos());
	}

	if (GetPos().y < 0)
	{
		// if this is a meteor
		if (GetAttackElement() == FIRE)
		{
			ExplodeMeteor();
		}
		else
		{
			ExplodeSnowball();
		}
	}

}

void CSnowball::Uninitialize()
{
	CView::DynamicShapeRemove(m_nShadowRenderID);
	m_nShadowRenderID = -1;
	// Play an explosion effect
	//CEmitter * pcExplosion = new CEmitter();
	//pcExplosion->SetLoop(false);
	//pcExplosion->SetActive(true);
	//pcExplosion->SetEmittPos(this->GetPos());
	//pcExplosion->LoadParticle("Resource/Particles/IceExplosion.txt");
	//pcExplosion->SetEmittPos(this->GetPos());
	//CView::AddEmitter(pcExplosion);
	
	// Add a snow patch where the object fell

	// if this is a meteor
	if (GetAttackElement() == FIRE)
	{
		//CView::DynamicShapeSetAsOrphan(m_nMoltenRenderID);
		vec3f vMoltenPos(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight(), GetPos().z);
		CSceneryObject* pMoltenGround = dynamic_cast<CSceneryObject*>(CWorldManager::GetInstance()->MakeObject(vMoltenPos, SCENERY_OBJECT, -1, "FireTrap"));
		if(pMoltenGround)
		{			
			matrix4f moltenMatrix = *(pMoltenGround->GetWorldMatrix());
			// strech or skew on the X and Z axis to randomize/distort the shape a little
			moltenMatrix.scale_post(RAND_FLOAT(1.2f, 1.4f), 1.0f, RAND_FLOAT(1.2f, 1.4f));
			// give a random rotation about the Y axis
			moltenMatrix.rotate_y_pre(float(rand() % 360));			
			pMoltenGround->SetWorldMatrix(moltenMatrix);

			pMoltenGround->SetLifetime(5.0f);
			CView::SetRenderInstanceFlags(pMoltenGround->GetRenderID(), CView::GetRenderInstance(pMoltenGround->GetRenderID())->GetFlags() | RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
			CWorldManager::GetInstance()->BoundsKeeping(pMoltenGround);
		}
	}
	// meteors shouldn't make snowpatches =P
	 if(!m_bHitByFire && GetAttackElement() != FIRE)
	{
		vec3f vSnowPatchPos(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight(), GetPos().z);
		CSceneryObject* pSnowpatch = dynamic_cast<CSceneryObject*>(CWorldManager::GetInstance()->MakeObject(vSnowPatchPos, SCENERY_OBJECT, -1, "SnowPatch"));
		if(pSnowpatch)
		{			
			matrix4f snowMatrix = *(pSnowpatch->GetWorldMatrix());
			// strech or skew on the X and Z axis to randomize/distort the shape a little
			snowMatrix.scale_post(RAND_FLOAT(0.8f, 1.2f), 1.0f, RAND_FLOAT(0.8f, 1.2f));
			// give a random rotation about the Y axis
			snowMatrix.rotate_y_pre(float(rand() % 360));			
			pSnowpatch->SetWorldMatrix(snowMatrix);

			pSnowpatch->SetLifetime(5.0f);
			CView::SetRenderInstanceFlags(pSnowpatch->GetRenderID(), CView::GetRenderInstance(pSnowpatch->GetRenderID())->GetFlags() | RENDER_FULLY_TRANSLUCENT);
			CWorldManager::GetInstance()->BoundsKeeping(pSnowpatch);
		}

	}

	
	if(m_pcFireEffect != nullptr)
	{
		m_pcFireEffect->SetActive(false);
		CView::GetEmitterManager().KillEmitter(m_pcFireEffect);
		m_pcFireEffect = nullptr;
	}

	// Call the parent's Uninitialize()
	CCollidableObject::Uninitialize();
}

void CSnowball::MakeMeteor()
{
	SetAttackElement(FIRE);
	SetColor(vec3f(0.1f,0.1f,0.1f));
	m_pcFireEffect = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/FireModel.txt");
	m_pcFireEffect->SetActive(true);
	this->RegisterWithSoundManager();
	//CView::SetRenderInstanceFlags(m_pcFireEffect->GetRenderID(),RENDER_NOT_SHADOWCASTER | RENDER_TWOD_PARTICLE);
}

void CSnowball::ExplodeSnowball()
{
	if(!m_pcIceExplosionSmall)
	{
		// the snowball doesn't actually explode until it's about 2 below the ground so set the emitter to about the ground
		vec3f smallPosition = vec3f(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight() + 0.5f, GetPos().z);
		m_pcIceExplosionSmall = CView::GetEmitterManager().AddEmitter(false, true, smallPosition, "Resource/Particles/SnowballExplosionSmall.txt", true);
	}
	if(!m_pIceExplosionLarge)
	{
		// the snowball doesn't actually explode until it's about 2 below the ground so set the emitter to about the ground
		vec3f largePosition = vec3f(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight() + 1.0f, GetPos().z);
		m_pIceExplosionLarge = CView::GetEmitterManager().AddEmitter(false, true, largePosition, "Resource/Particles/SnowballExplosionLarge.txt", true);
	}
}

void CSnowball::ExplodeMeteor()
{
	if(!m_pcIceExplosionSmall)
	{
		// the snowball doesn't actually explode until it's about 2 below the ground so set the emitter to about the ground
		vec3f smallPosition = vec3f(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight() + 0.5f, GetPos().z);
		m_pcIceExplosionSmall = CView::GetEmitterManager().AddEmitter(false, true, smallPosition, "Resource/Particles/MeteorExplosionSmall.txt", true);
	}
	if(!m_pIceExplosionLarge)
	{
		// the snowball doesn't actually explode until it's about 2 below the ground so set the emitter to about the ground
		vec3f largePosition = vec3f(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight() + 1.0f, GetPos().z);
		m_pIceExplosionLarge = CView::GetEmitterManager().AddEmitter(false, true, largePosition, "Resource/Particles/MeteorExplosionLarge.txt", true);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	/*CView::DynamicShapeMakePlane(m_nMoltenRenderID, 2.0f, 2.0f);
	matrix4f moltenMatrix;

	moltenMatrix.make_identity();
	moltenMatrix.rotate_y_post(float(rand() % 360));
	moltenMatrix.scale_post(vec3f(2.0f, 1.0f, 2.0f));
	moltenMatrix.axis_pos = vec3f(GetPos().x, CWorldManager::GetInstance()->GetGroundHeight(), GetPos().z);

	CView::DynamicShapeSetLifetime(m_nMoltenRenderID, 3.0f);
	CView::DynamicShapeSetLocalMatrix(m_nMoltenRenderID, moltenMatrix);
	CView::DynamicShapeSetTextures(m_nMoltenRenderID, CView::LoadSprite("Firetrap_Diffuse_8BB.png"), -1);*/
}

