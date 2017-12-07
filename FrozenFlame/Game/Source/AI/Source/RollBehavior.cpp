#include "../Header/RollBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/AnimPlayer.h"
#include "../../Animations/Animation.h"
//#include "../../Renderer/AssetManager.h"
//#include "../

void CRollBehavior::Enter()
{
	m_bIsRolling = false;
	m_fRollDirection = 0.0f;
	m_fRollCooldown = 0.0f;
	m_tStartPosition = GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
	m_cSoundCounter = 0;
	m_cTrapCounter = 0;
	//m_fTrapTimer = 0.0f;

	m_tEndPosition = GetOwner()->GetAgent()->GetWorldMatrix()->axis_z;
}

void CRollBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	/*if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::COLD)
	{
		GetOwner()->TransitionTo(FLOCK_BEHAVIOR);
		return;
	}*/

	vec3f distanceVec = m_tStartPosition - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
	vec3f distanceToPlayer = _pcPlayer->GetPos() - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

	if(distanceToPlayer.magnitude() >= ROLL_RANGE)
	{
		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
		GetOwner()->TransitionTo(HOMING_BEHAVIOR);
		return;
	}

	else if (GetOwner()->GetAgent()->GetType() == FIRE_OBJECT && GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::COLD)
	{
		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
		GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
		return;
	}

	


	m_fRollCooldown += _fElapsedTime;

	
	if(GetOwner()->GetAgent()->GetType() == FIRE_OBJECT)
	{
		//m_fTrapTimer
		if (m_fRollCooldown < 2.6f  + (2.6f * GetOwner()->GetAgent()->GetTimerOffset()))
		{
			
			if (!((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRolling() && ((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp())
			{
				m_tEndPosition = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
				//m_tEndPosition = m_tEndPosition.normalize();
				//m_tEndPosition *= 5.0f;
				
				((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
				((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsWarmingUp(true);
				((CFireEnemy*)(GetOwner()->GetAgent()))->SetPlayRollUpSound(true);	// he can play the sound again
				//GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_B);
			
			}
			else if(((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsWarmingUp() && !((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRolling())
			{
				
				m_fTrapTimer = 0.0f;
				GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_B, true);
				m_cSoundCounter += 1;

				if (((CFireEnemy*)(GetOwner()->GetAgent()))->CanPlayRollUpSound())
				{
					// the location of the roll up sound will be the player so that the sound can be heard/alert the player
					// even if the fire enemy is far away/off screen
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_FIREGOLEM_CURLUP,((CFireEnemy*)GetOwner()->GetAgent())->GetSoundID());
					((CFireEnemy*)(GetOwner()->GetAgent()))->SetPlayRollUpSound(false);	// only play once
				}

				if (m_cSoundCounter % 10 == 0)
				{
					//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_Roll");
				}
			}
			else
			{
				//GetOwner()->GetAgent()->SetMoveSpeed(GetOwner()->GetAgent()->GetMoveSpeed() * 5.25f);
				GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_B,true);
				if(!((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsWarmingUp() && ((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRolling())
				{
					m_fTrapTimer += _fElapsedTime;
					// Create the first of many traps >: D
					//CFireTrap* pcTrap = (CFireTrap*)GetOwner()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
					//if (pcTrap)
					//{
					//	vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
					//	enemyPos.y = -3.0f;
					//	enemyPos.z -= 0.75f;
					//	pcTrap->SummonTrap(0.25f, enemyPos);
					//}
					
					//GetOwner()->GetAgent()->SetIsAttacking(true);
					GetOwner()->GetAgent()->SetIsMoving(false);

					m_cTrapCounter += 1;

					//if (m_cTrapCounter % 1500 == 0)
					if(m_fTrapTimer >= 0.1f)
					{
						m_fTrapTimer = 0.0f;
						CFireTrap* pcTrap = (CFireTrap*)GetOwner()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
						if (pcTrap)
						{
							vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
							enemyPos.y = -3.0f;
							enemyPos.z -= 0.5f;
							pcTrap->SummonTrap(0.25f, enemyPos);
						}
						//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_Roll");
					//	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_ROLL,GAME_OBJECT_ID_PLAYER);
					}
				}

			}
			m_cSoundCounter += 1;
			Move(m_tEndPosition, _fElapsedTime);
			if (m_cSoundCounter % 10 == 0)
			//if (m_cSoundCounter % 2 == 0)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_ROLL,((CFireEnemy*)GetOwner()->GetAgent())->GetSoundID());
			}
			//if(GetOwner()->GetAgent()->GetPos().x == m_tEndPosition.x && GetOwner()->GetAgent()->GetPos().y == m_tEndPosition.y && GetOwner()->GetAgent()->GetPos().z == m_tEndPosition.z)
			//{
			//	m_fRollCooldown = 0.0f;
			//}
		}
		else
		{

			//if(!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_B) && ((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRolling())
			//{
			//	
			//	if( !((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp() )
			//	{
			//		GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
			//		//float fDur = GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimation()->GetDuration();
			//		//GetOwner()->GetAgent()->GetAnimationPlayer()->SetCurTime(CAssetManager::GetAnimation(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimationID())->;
			//		GetOwner()->GetAgent()->GetAnimationPlayer()->SetCurTime(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimation()->GetDuration());
			//		
			//		GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(-1.0f);
			//		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(true);
			//		
			//	}

			//	//if(!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_A))
			//	if(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurTime() <= 0.0f)
			//	{
			//		GetOwner()->GetAgent()->GetAnimationPlayer()->Stop();
			//		GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
			//		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
			//		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
			//		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRolling(false);
			//		GetOwner()->GetAgent()->SetIsAttacking(false);
			//		GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
			//		m_fRollCooldown = 0.0f;
			//	}
			//}

			//if(distanceVec.magnitude() >= ROLL_RANGE)
			//{
			//	((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
			//	//((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRolling(false);
			//	GetOwner()->TransitionTo(HOMING_BEHAVIOR);
			//	return;
			//}
			//m_fRollCooldown += _fElapsedTime;
			GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
			//else
			//{
			//	GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
			//	((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRolling(false);
			//	GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
			//	m_fRollCooldown = 0.0f;
			//}
		}
	}


}

void CRollBehavior::Exit()
{
	GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
	//GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
	GetOwner()->GetAgent()->SetMoveSpeed(GetOwner()->GetAgent()->GetMoveSpeed() / 5.25f);
	//((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRolling(false);
	GetOwner()->GetCooldown() = true;
	GetOwner()->GetAgent()->SetIsAttacking(true);
}