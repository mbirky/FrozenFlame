#include "BaseObject.h"
#include "EnvironmentObject.h"

CBaseObject::CBaseObject(void)
{
	m_nType = BASE_OBJECT;
	m_bInitialized = false;
	m_nRenderID = -1;
	m_bInvertColors = false;
	m_tColor = vec3f(1.0f, 1.0f, 1.0f);
	m_tPrevColor = vec3f(1.0f, 1.0f, 1.0f);
	m_fTransparency = 1.0f;
	m_bShakeLeft = true;
	m_bIsShaking = false;
	m_fShakeTimer = 0.0f;
	m_nSoundID = -1;
	memset(&m_nSoundPos, 0, sizeof(AkSoundPosition));
	m_fShakeAmount = 0.3f;
	m_nShakeFrameCounter = 0;
}

CBaseObject::~CBaseObject(void)
{
	if(m_nRenderID != -1)
	{
		CView::Pop(m_nRenderID);
	}
}

void CBaseObject::Initialize(void)
{
	m_bInitialized = true;
	m_tColor = vec3f(1.0f, 1.0f, 1.0f);
	m_tVelocity.make_zero();
	m_tPositionData.make_identity();
	m_nToBeDeleted = 0;
	m_fTransparency = 1.0f;
	m_nShakeFrameCounter = 0;
}

void CBaseObject::Reinitialize()
{
	if(m_bInitialized){
		Uninitialize();
	}
	Initialize();
}

void CBaseObject::Uninitialize()
{
	if(m_nRenderID != -1)
	{
		CView::Pop(m_nRenderID);

		m_nRenderID = -1;
	}

	m_bInitialized = false;

	m_tColor.make_zero();
	m_tVelocity.make_zero();

	m_tPositionData.make_identity();

	CSoundManager::GetInstance()->RemoveObject(GetSoundID());
	CSoundManager::GetInstance()->RemoveObjPos(GetSoundPos());
}

void CBaseObject::LookAt(vec3f _tVec, float _fElapsedTime)
{
	// Object Position
	vec3f agentPos = GetWorldMatrix()->axis_pos;
	
	// Vector from us to them
	vec3f vDesired = _tVec - agentPos;
	
	// Normalize vector from us to them
	vDesired = vDesired.normalize();
	
	// Object's Matrix
	matrix4f currMat = *GetWorldMatrix();
	
	currMat.axis_z.x = vDesired.x;
	currMat.axis_z.y = vDesired.y;
	currMat.axis_z.z = vDesired.z;
	
	// Forward Vector
	vec3f front(currMat.axis_z.x, currMat.axis_z.y, currMat.axis_z.z);

	// Top vector
	vec3f top(currMat.axis_y.x, currMat.axis_y.y, currMat.axis_y.z);
	front = front.normalize();
	top = top.normalize();
	vec3f outcome;
	cross_product(outcome, top, front);
	outcome = outcome.normalize();
	
	currMat.axis_x.x = outcome.x;
	currMat.axis_x.y = outcome.y;
	currMat.axis_x.z = outcome.z;
	
	//currMat._42 = 1.0f;
	// Set our world matrix to new world matrix
	SetWorldMatrix(currMat);
//	*GetMatrix() = currMat;
}

void CBaseObject::FullLookAt(const vec3f& vPos)
{
	//	Get the position of the entity and the target
	vec3f posEntity = GetPos();	
	vec3f posTarget = vPos;

	//	Calculate the orientation for each axis of the entity
	//		Create a vector for the Z axis and assign it to the vector
	//		that points from the entity to the object
	vec3f Zaxis = posTarget - posEntity;
	//		Normalize
	Zaxis = Zaxis.normalize();

	//		Create a vector for the X axis and assign it to the
	//		cross product of the Z axis and the world's up vector
	vec3f Xaxis;
	vec3f WorldUpVector(0.0f, 1.0f, 0.0f);
	cross_product(Xaxis, WorldUpVector, Zaxis);
	//		Normalize
	Xaxis = Xaxis.normalize();

	//		Create a vector for the Y axis and assign it to the
	//		cross product of the Z and X axis
	vec3f Yaxis;
	cross_product(Yaxis, Zaxis, Xaxis);
	//		Normalize
	Yaxis = Yaxis.normalize();

	//	Copy orientation information into entity's matrix
	matrix4f mOrientation(	Xaxis.x, Xaxis.y, Xaxis.z, 0,
		Yaxis.x, Yaxis.y, Yaxis.z, 0,
		Zaxis.x, Zaxis.y, Zaxis.z, 0,
		posEntity.x, posEntity.y, posEntity.z, 1);

	SetWorldMatrix(mOrientation);
}


void CBaseObject::FlashColor(vec3f _tColor)
{
	if (!m_bInvertColors)
	{
		m_tPrevColor = m_tColor;
		m_tColor = _tColor;
	}
	else
	{
		m_tColor = m_tPrevColor;
		m_tPrevColor = m_tColor;
	}
	m_bInvertColors = !m_bInvertColors;
}

void CBaseObject::ShakeObject(void)
{
	if (!m_bIsShaking)
		return;

	m_nShakeFrameCounter++;

	if (m_nShakeFrameCounter % 5 == 0)
	{
		vec3f xAxis = m_tPositionData.axis_x;
		xAxis.normalize();

		if (m_bShakeLeft)
		{
			xAxis *= -m_fShakeAmount;
			m_bShakeLeft = false;
		}
		else
		{
			xAxis *= m_fShakeAmount;
			m_bShakeLeft = true;
		}

		SetPos(GetPos() + xAxis);
		m_nShakeFrameCounter = 0;
	}
}

void CBaseObject::SetSoundPos(vec3f position)
{
	m_nSoundPos.Position.X = position.x;
	m_nSoundPos.Position.Y = position.y;
	m_nSoundPos.Position.Z = position.z;
	// match up this position with the object's ID in the sound manager
	CSoundManager::GetInstance()->SetGameObjPos(GetSoundID(), m_nSoundPos);
}

void CBaseObject::SetSoundPos(D3DVECTOR position)
{
	m_nSoundPos.Position.X = position.x;
	m_nSoundPos.Position.Y = position.y;
	m_nSoundPos.Position.Z = position.z;
}

void CBaseObject::SetSoundPos(float x, float y, float z)
{
	m_nSoundPos.Position.X = x;
	m_nSoundPos.Position.Y = y;
	m_nSoundPos.Position.Z = z;
}

void CBaseObject::RegisterWithSoundManager(void)
{
	this->SetSoundID((AkGameObjectID)(CSoundManager::GetInstance()->GetNewestSoundID()));

	CSoundManager::GetInstance()->RegisterObject(m_nSoundID);
	CSoundManager::GetInstance()->AddObject(m_nSoundID);
	CSoundManager::GetInstance()->AddObjPos(m_nSoundPos);
}