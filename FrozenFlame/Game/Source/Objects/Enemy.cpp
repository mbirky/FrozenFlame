/***********************************************
 * Filename:  		Enemy.cpp
 * Date:      		10/29/2012
 * Mod. Date: 		11/02/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the enemy object
 ************************************************/

#include "Enemy.h"

#include "../AI/Header/CFSM.h"

CEnemy::CEnemy(void) 
{
	SetType(ENEMY_OBJECT);
}


CEnemy::~CEnemy(void)
{
}

void CEnemy::Initialize(void)
{
	CCollidableObject::Initialize();

	m_pcBehavior = nullptr;

	m_nAttackingElement = NORMAL;
	m_nPreviousElement = NEUTRAL;

	m_fMoveSpeed = 2.5f;
	m_fKnockbackTimer = 1.0f;
	m_bHit = false;
	m_fHitCooldown = 0.0f;
	m_bIsStunned = false;
	m_fStunTimer = 0;
	m_fTimerOffset = 0.0f;

	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_y_pre(D3DXToRadian(180));
	SetWorldMatrix(worldMatrix);
//	GetMatrix()->rotate_y_pre(D3DXToRadian(180));

	m_bIsCrystalGuardian = false;

	m_fSeparationRadius = 2.0f;

	//float fOffset = float( ( (rand()% 6) * 1.0f - 3.0f) / 100.0f);
	//float fOffset = 0.0f;
	//this->SetTimerOffset(fOffset);
	//m_fAnimationSpeed = 1.0f * (fOffset + 1.0f);

	//srand(time_t(0));
}

void CEnemy::Reinitialize(void)
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CEnemy::Update(float fElapsedTime)
{
	CCollidableObject::Update(fElapsedTime);

	if(!GetInitialized())
		return;

	if(m_bHit)
	{
		m_fHitCooldown += fElapsedTime;
	}

	if(m_fHitCooldown >= 0.7f)
	{
		m_fHitCooldown = 0.0f;
		m_bHit = false;
	}
}

void CEnemy::UpdateStunTimer(float fElapsedTime)
{
	if (m_bIsStunned)
	{
		m_fStunTimer += fElapsedTime;
		if (m_fStunTimer >= m_fStunDuration)
		{
			m_bIsStunned = false;
		}
	}
}

void CEnemy::Uninitialize(void)
{
	if(GetInitialized())
	{
		CCollidableObject::Uninitialize();

		if(m_pcBehavior != nullptr)
		{
			delete m_pcBehavior;
			m_pcBehavior = nullptr;
		}		
	}
}