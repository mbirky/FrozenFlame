#include "../Header/RangedBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/Interpolator.h"
#include "../../Animations/Animation.h"
#include "../../Renderer/CEmitterManager.h"
#include <ctime>

CRangedBehavior::CRangedBehavior()
{
	m_fAttackTimer = 0.0f;
	m_fDelayTimer = 0.0f;
	m_bAttacked = false;
	m_bReady = true;
	m_pcCastingEmitterFire = nullptr;
	m_pcCastingEmitterIce = nullptr;
	m_bElementSet = false;
	srand(unsigned int(time(0)));
	nProjType = 0;
}

CRangedBehavior::~CRangedBehavior()
{
	if (m_pcCastingEmitterFire)
	{
		CView::GetEmitterManager().KillEmitter(m_pcCastingEmitterFire);
	}
	if (m_pcCastingEmitterIce)
	{
		CView::GetEmitterManager().KillEmitter(m_pcCastingEmitterIce);
	}
	nProjType = 0;
}

void CRangedBehavior::Enter()
{
	int i = 0;
	m_pcCastingEmitterFire = CView::GetEmitterManager().AddEmitter(true, false, GetOwner()->GetAgent()->GetPos(), "Resource/Particles/MagicChargeFire.txt", false, nullptr, GetOwner()->GetAgent(), true, true);
	m_pcCastingEmitterIce = CView::GetEmitterManager().AddEmitter(true, false, GetOwner()->GetAgent()->GetPos(), "Resource/Particles/MagicChargeIce.txt", false, nullptr, GetOwner()->GetAgent(), true, true);
	m_bElementSet = false;
	//this->SoftAttach(vec3f(GetOwner()->GetAgent()->GetPos()),0.0f);
}

void CRangedBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	// ranged minions were firing off another shot while in their death animation
	if (GetOwner()->GetAgent()->GetHealth() <= 0)
		return;

	//if (GetOwner()->GetAgent()->GetType() == WIZARD_OBJECT)
		TurnTo(_pcPlayer->GetWorldMatrix()->axis_pos, _fElapsedTime);
	//else
		//LookAt(_pcPlayer->GetMatrix()->axis_pos, _fElapsedTime);
	
	
	CEnemy * pcEnemy = GetOwner()->GetAgent();

	if(pcEnemy->GetType() == WIZARD_OBJECT)
	{
		vec3f targetVec = _pcPlayer->GetPos();
		targetVec.y = GetOwner()->GetAgent()->GetPos().y;

		targetVec.x += 8.0f;
		targetVec.z -= 8.0f;

		this->SoftAttach(targetVec,_fElapsedTime);
	}

	//if(GetOwner()->GetAgent()->GetType() == WIZARD_OBJECT)
	//{
	//	m_fAttackTimer += _fElapsedTime;
	//	if (m_fAttackTimer >= GetOwner()->GetAgent()->GetAttackSpeed()/2.0f && m_fAttackTimer < GetOwner()->GetAgent()->GetAttackSpeed())
	//	{
	//		GetOwner()->GetAgent()->FlashColor(vec3f(1.0f, 0.5f, 0.0f));
	//	}
	//	else if (m_fAttackTimer >= GetOwner()->GetAgent()->GetAttackSpeed())
	//	{
	//		vec3f vTarget = _pcPlayer->GetWorldMatrix()->axis_pos;
	//
	//		CProjectileObject * pcProjectile = (CProjectileObject*)GetOwner()->GetObjectManager()->NewObject(PROJECTILE_OBJECT);
	//
	//		matrix4f worldMatrix = *(pcProjectile->GetWorldMatrix());
	//		worldMatrix.axis_pos = pcEnemy->GetWorldMatrix()->axis_pos;
	//		worldMatrix.axis_pos.y += 5.0f;
	//		pcProjectile->SetWorldMatrix(worldMatrix);
//	//		pcProjectile->GetMatrix()->axis_pos = pcEnemy->GetMatrix()->axis_pos;
//	//		pcProjectile->GetMatrix()->axis_pos.y += 5.0f;
	//		vec3f vProjVelocity = vTarget - pcProjectile->GetWorldMatrix()->axis_pos;
	//
	//		vProjVelocity = vProjVelocity.normalize();
	//		pcProjectile->SetVelocity(vProjVelocity);
	//		
	//		pcProjectile->SetOwner(pcEnemy);
	//		pcProjectile->SetDamage(pcEnemy->GetDamage());
	//		if(pcEnemy->GetAttackElement() == 0)
	//		{
	//			//rand being used to calculate values of 1 or -1
	//			pcProjectile->SetAttackElement(((rand() % 2)*2)-1);
	//		}
	//		else
	//		{
	//			pcProjectile->SetAttackElement(pcEnemy->GetAttackElement());
	//		}
	//
	//		if(pcProjectile->GetAttackElement() == 1)
	//		{
	//			pcProjectile->SetColor(vec3f(1.0f,0.0f,0.0f));
	//			CView::AttachLight(pcProjectile->GetRenderID(), vec4f(1.0f, 0.0f, 0.0f, 1.0f), 30.0f);
	//		}
	//		else
	//		{
	//			pcProjectile->SetColor(vec3f(0.0f,0.0f,1.0f));
	//			CView::AttachLight(pcProjectile->GetRenderID(), vec4f(0.0f, 0.0f, 1.0f, 1.0f), 30.0f);
	//		}
	//		m_fAttackTimer = 0.0f;
	//		if (GetOwner()->GetAgent()->GetType() == WIZARD_OBJECT)
	//		{
	//			GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
	//		}
	//	}
	//
	//}
	//
	//else
	//{
		if(!GetOwner()->GetAgent()->GetIsAttacking())
		{
			m_fDelayTimer += _fElapsedTime;
			if(pcEnemy->GetType() == WIZARD_OBJECT)
			{
				((CWizardEnemy*)(pcEnemy))->GetBubble()->SetColor(vec3f(1.0f,0.0f,1.0f));
			}
		}

		/*if(m_fDelayTimer >= 0.1f)
		{
			
		}*/

		vec3f vVecToPlayer = _pcPlayer->GetPos() - GetOwner()->GetAgent()->GetPos();

		float fDistanceToPlayer = vVecToPlayer.magnitude();

		if(fDistanceToPlayer < RANGED_ENEMY_RANGE &&  m_fDelayTimer >= GetOwner()->GetAgent()->GetAttackSpeed() /*+ (GetOwner()->GetAgent()->GetAttackSpeed() * GetOwner()->GetAgent()->GetTimerOffset())*/ && !GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_ATTACK) )
		{
			//GetOwner()->GetAgent()->SetIsMoving(false);
			GetOwner()->GetAgent()->SetIsAttacking(true);
			GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_ATTACK);
			//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_SpellCast");

			// ranged minions were playing this sound even while dead because it was playing whenever the timer finished
			if(pcEnemy->GetType() == RANGED_OBJECT && pcEnemy->GetHealth() > 0)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SPELLCAST,((CRangedEnemy*)pcEnemy)->GetSoundID());
			}

			m_fDelayTimer = 0.0f;
			
			
		}
		
		
			//pcEnemy->SetIsAttacking(true);
			
		//}
		if(GetOwner()->GetAgent()->GetIsAttacking() && (GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_ATTACK) ||GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_IDLE) || GetOwner()->GetAgent()->GetAnimationPlayer()->GetLastPlayedAnimType() == ANIM_ATTACK))
		{

			if(GetOwner()->GetAgent()->GetAnimationPlayer()->GetLastPlayedAnimType() == ANIM_ATTACK)
				int i = 0;

			m_fAttackTimer += _fElapsedTime;
			if (m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.10f && m_fAttackTimer < GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.70f)
			{
				if(pcEnemy->GetAttackElement() == 0)
				{
					if (!m_bElementSet)
					{
						nProjType =  ((rand() % 2)*2)-1;
						m_bElementSet = true;
					}
				
					if (nProjType == FIRE)
					{
						if (m_pcCastingEmitterFire)
						{
							m_pcCastingEmitterFire->SetEmittPos(GetOwner()->GetAgent()->GetPos());
							m_pcCastingEmitterFire->SetActive(true);
						}
					}
					else if (nProjType == ICE)
					{
						if (m_pcCastingEmitterIce)
						{
							m_pcCastingEmitterIce->SetEmittPos(GetOwner()->GetAgent()->GetPos());
							m_pcCastingEmitterIce->SetActive(true);
						}
					}
				}
				else if (pcEnemy->GetAttackElement() == FIRE)
				{
					if (m_pcCastingEmitterFire)
					{
						m_pcCastingEmitterFire->SetEmittPos(GetOwner()->GetAgent()->GetPos());
						m_pcCastingEmitterFire->SetActive(true);
					}
				}
				else if (pcEnemy->GetAttackElement() == ICE)
				{
					if (m_pcCastingEmitterIce)
					{
						m_pcCastingEmitterIce->SetEmittPos(GetOwner()->GetAgent()->GetPos());
						m_pcCastingEmitterIce->SetActive(true);
					}
				}
			}

			//if( GetOwner()->GetAgent()->GetAnimationPlayer()->GetFrameID() == 20 )
			if( GetOwner()->GetAgent()->GetType() == RANGED_OBJECT && m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.70 && !m_bAttacked)
			{
				m_bAttacked = true;
				m_bReady = true;
			}
				
			else if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.80 && !m_bAttacked)
			{
				m_bAttacked = true;
				m_bReady = true;
			}

			if(m_bAttacked && m_bReady)
			{
				vec3f vTarget = _pcPlayer->GetWorldMatrix()->axis_pos;

				CProjectileObject * pcProjectile = (CProjectileObject*)GetOwner()->GetObjectManager()->NewObject(PROJECTILE_OBJECT);

				matrix4f worldMatrix = *(pcProjectile->GetWorldMatrix());
				worldMatrix.axis_pos = pcEnemy->GetWorldMatrix()->axis_pos;
				worldMatrix.axis_pos.y += 2.0f;
				pcProjectile->SetWorldMatrix(worldMatrix);
//				pcProjectile->GetMatrix()->axis_pos = pcEnemy->GetMatrix()->axis_pos;
//				pcProjectile->GetMatrix()->axis_pos.y += 5.0f;
				vec3f vProjVelocity = vTarget - pcProjectile->GetWorldMatrix()->axis_pos;

				vProjVelocity = vProjVelocity.normalize();
				pcProjectile->SetVelocity(vProjVelocity);
				
				pcProjectile->SetOwner(pcEnemy);
				pcProjectile->SetDamage(pcEnemy->GetDamage());

				if(pcEnemy->GetAttackElement() == 0)
				{
					//nProjType =  ((rand() % 2)*2)-1;
					//rand being used to calculate values of 1 or -1
					pcProjectile->SetAttackElement(nProjType);
				}
				else
				{
					pcProjectile->SetAttackElement(pcEnemy->GetAttackElement());
				}


				if(pcProjectile->GetAttackElement() == 1)
				{
					// We don't need this anymore - Dan
					// pcProjectile->SetColor(vec3f(0.8f,0.5f,0.0f));

					CView::MakeLight(pcProjectile->GetRenderID(), true, vec3f(0, 0, 0), vec4f(0.8f, 0.5f, 0.0f, 1.0f), 8.0f);
				}
				else
				{
					// We don't need this anymore - Dan
					// pcProjectile->SetColor(vec3f(0.3f,0.3f,1.0f));

					CView::MakeLight(pcProjectile->GetRenderID(), true, vec3f(0, 0, 0), vec4f(0.6f, 0.6f, 1.0f, 1.0f), 8.0f);
				}
				//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_2D_WeaponWhoosh");

				// ranged minions were playing this sound even while dead because it was playing whenever the timer finished
				if(pcEnemy->GetType() == RANGED_OBJECT && pcEnemy->GetHealth() > 0)
				{
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SPELLCAST,((CRangedEnemy*)pcEnemy)->GetSoundID());
				}
				else if(pcEnemy->GetType() == WIZARD_OBJECT)
				{
					if(nProjType == 1)
					{
						((CWizardEnemy*)(pcEnemy))->GetBubble()->SetColor(vec3f(1.0f,0.0f,0.0f));
					}
					else
					{
						((CWizardEnemy*)(pcEnemy))->GetBubble()->SetColor(vec3f(0.0f,.0f,1.0f));
					}
				}
				
				m_bReady = false;
				m_pcCastingEmitterFire->SetActive(false);
				m_pcCastingEmitterIce->SetActive(false);
				m_bElementSet = false;
				nProjType = 0;
			}

			if (GetOwner()->GetAgent()->GetType() == WIZARD_OBJECT)
			{
				if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration())
				{
					m_bAttacked = false;
					m_bReady = true;
					m_fAttackTimer = 0.0f;
					GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
				}
			}

			else
			{
				if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration())
				{
					m_bAttacked = false;
					m_bReady = true;
					m_fAttackTimer = 0.0f;
					
					pcEnemy->SetIsAttacking(false);
					
					m_bElementSet = false;
					nProjType = 0;
				}
			}
			
			
		}
	//}

	//else
	//{
		
	//}
}

void CRangedBehavior::Exit()
{
	if (m_pcCastingEmitterFire)
	{
		CView::GetEmitterManager().KillEmitter(m_pcCastingEmitterFire);
		m_pcCastingEmitterFire = nullptr;
	}

	if (m_pcCastingEmitterIce)
	{
		CView::GetEmitterManager().KillEmitter(m_pcCastingEmitterIce);
		m_pcCastingEmitterIce = nullptr;
	}
}