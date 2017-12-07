#include "AttackObject.h"


CAttackObject::CAttackObject(void)
{
	SetType(ATTACK_OBJECT);
}


CAttackObject::~CAttackObject(void)
{
	SetInitialized(false);
}

void CAttackObject::Update(float fElapsedTime)
{
	m_fActiveTimer -= fElapsedTime;

	UpdateCollisionData();

	if(GetHealth() <= 0)
	{
		Uninitialize();
	}
}

void CAttackObject::Initialize(void)
{
	CCollidableObject::Initialize();

	SetRenderID(CView::Push(this, "AttackCube"));

	m_pcOwner = nullptr;
	m_fActiveTimer = 0.5f;
}

void CAttackObject::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CAttackObject::Uninitialize()
{
	CCollidableObject::Uninitialize();
}