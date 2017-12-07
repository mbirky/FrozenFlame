
#include "../Header/MeleeBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/Interpolator.h"
#include "../../Animations/Animation.h"

CMeleeBehavior::CMeleeBehavior()
{
	m_fAttackTimer = 0.0f;
	m_bAttacked = false;
}

void CMeleeBehavior::Enter()
{
	m_bShakeLeft = true;
	m_bFirstShake = true;
	m_fAttackTimer = 0.0f;
	m_tOriginialPosition = GetOwner()->GetAgent()->GetPos();
}

void CMeleeBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if ((GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::FROZEN && GetOwner()->GetAgent()->GetType() != ICE_OBJECT) || GetOwner()->GetAgent()->GetHealth() <= 0)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		return;
	}

	//if(GetOwner()->GetCooldown())
	//{
	//	GetOwner()->GetCooldownTimer() += _fElapsedTime;

	//	if (GetOwner()->GetCooldownTimer() >= 0.15f + (0.15f * GetOwner()->GetAgent()->GetTimerOffset()))
	//	{
	//		GetOwner()->GetCooldown() = false;
	//		GetOwner()->GetCooldownTimer() = 0.0f;
	//	}
	//}

	vec3f approachVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

	if (approachVec.magnitude() < APPROACH_DISTANCE /*|| m_fAttackTimer >= GetOwner()->GetAgent()->GetAttackSpeed()/1.25f*/)
	{
		if(!GetOwner()->GetAgent()->GetIsAttacking())
		{
			TurnTo(_pcPlayer->GetWorldMatrix()->axis_pos, _fElapsedTime);
		}

		GetOwner()->GetAgent()->SetPos(m_tOriginialPosition);
		if( !GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_ATTACK) )
		{
			//if(!GetOwner()->GetCooldown())
			{
				GetOwner()->GetAgent()->SetIsMoving(false);
				GetOwner()->GetAgent()->SetIsAttacking(true);


				// That's how I roll - Dan
				if(CIceEnemy* pIce = dynamic_cast<CIceEnemy*>(GetOwner()->GetAgent()))
				{
					pIce->Punch();
				}
			}
		}
	}

	else
	{
		if (GetOwner()->GetAgent()->GetType() != FIRE_OBJECT)
		{
			GetOwner()->TransitionTo(HOMING_BEHAVIOR);
		}
		else
		{
			if(!GetOwner()->GetAgent()->GetIsAttacking())
			{
				if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::COLD)
				{
					GetOwner()->TransitionTo(HOMING_BEHAVIOR);
				}
				else
				{
					GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
				}
			}
		}
	}

	if(m_bAttacked)
	{
		m_fAttackTimer += _fElapsedTime;
	}

	if( GetOwner()->GetAgent()->GetIsAttacking() && !m_bAttacked)
	{

		m_fAttackTimer += _fElapsedTime;// * GetOwner()->GetAgent()->GetAnimationPlayer()->GetAnimationSpeed();


		//if(!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_ATTACK))
		if (GetOwner()->GetAgent()->GetType() == MELEE_OBJECT)
		{
			//if( GetOwner()->GetAgent()->GetAnimationPlayer()->GetFrameID() == 13 )

			// This line has been causing problems for a while and for some reason my fixes keep getting deleted.
			// Now that I looked at it better, I finally understand the real problem that is going on here.
			// GetCurAnimation() sometimes returns nullptr because the curAnimation is -1. However to get inside here
			// we have "IsAttacking" set to true, which means that the attack animation SHOULD BE PLAYING.
			// So there's something wrong related to  how this part of the system is working and I'm not sure what.
			// - Dan
			//CAnimation* curAnimation = GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimation();
			if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.50f)
			{
				//m_bAttacked = true;
				if (IceBreathCheck(GetOwner()->GetAgent(), _pcPlayer))
				{
					_pcPlayer->OnAttack(GetOwner()->GetAgent()->GetDamage(), GetOwner()->GetAgent()->GetAttackElement());
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_3D_Punches");
					m_bAttacked = true;
					if (CInput::GetInstance()->GetIfUsingController())
					{
						// if the minion is on the left side of(or directly above or below) the player, rumble the left moter
						if (GetOwner()->GetAgent()->GetPos().z >= _pcPlayer->GetPos().z)
						{
							_pcPlayer->InitiateMeleeMinionMeleeRumble(GetOwner()->GetAgent()->GetDamage(), true);
						}
						else // if the minion is on the right side of the player, rumble the right motor
						{
							_pcPlayer->InitiateMeleeMinionMeleeRumble(GetOwner()->GetAgent()->GetDamage(), false);
						}
					}
				}
			}
		}


		else
		{

			if (GetOwner()->GetAgent()->GetType() == FIRE_OBJECT)
			{
				if(m_fAttackTimer < GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.55f)
				{
					GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(0.4f);// + (0.4f * GetOwner()->GetAgent()->GetTimerOffset()));
				}

				// This number is what we need to show for Frame 17 of the Fire Beast's melee attack, which until this frame, was basically a windup for a punch
				if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.55f)
				{
					GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(1.0f);// + (1.0f * GetOwner()->GetAgent()->GetTimerOffset()));
				}
			}

			if (GetOwner()->GetAgent()->GetType() == ICE_OBJECT)
			{
				// This is kind of hacked in here now but I'm changing stuff for the Ice Beast
				// so that it works a little better
				IceBeastPunch(_pcPlayer);
			}
			else
			{
				if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration())
				{
					if (IceBreathCheck(GetOwner()->GetAgent(), _pcPlayer))
					{
						_pcPlayer->OnAttack(GetOwner()->GetAgent()->GetDamage(), GetOwner()->GetAgent()->GetAttackElement());
						CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_3D_Punches");
						// if a beast is using a melee attack to hit the player	
						if (CInput::GetInstance()->GetIfUsingController())
						{
							_pcPlayer->InitiateBeastMeleeRumble(GetOwner()->GetAgent()->GetDamage());
						}
					}
					m_bAttacked = true;
				}
			}


		}
	}

	// Dan was here
	// This was crashing so I added a safety check
	if(m_fAttackTimer >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration()/* + GetOwner()->GetAgent()->GetTimerOffset()*/ && m_bAttacked)
	{
		GetOwner()->GetAgent()->SetIsAttacking(false);
		m_fAttackTimer = 0.0f;
		m_bAttacked = false;
	}
}

void CMeleeBehavior::Exit()
{
	GetOwner()->GetAgent()->SetIsAttacking(false);
}

void CMeleeBehavior::AgentShake(float _fShakeAmount)
{
	matrix4f tempMat = *GetOwner()->GetAgent()->GetWorldMatrix();
	vec3f xAxis = tempMat.axis_x;
	xAxis.normalize();
	if (m_bShakeLeft)
	{
		if (m_bFirstShake)
		{
			xAxis *= _fShakeAmount / 2.0f;
			m_bFirstShake = false;
		}
		xAxis *= -1.0f;
	}
	m_bShakeLeft = !m_bShakeLeft;
	tempMat.axis_pos += xAxis;
	GetOwner()->GetAgent()->SetWorldMatrix(tempMat);
}


void CMeleeBehavior::IceBeastPunch(CPlayerObject* pTarget)
{
	GetOwner()->GetAgent()->SetIsMoving(false);
	
	// I know this value shouldn't be hardcoded but I need it like this now. - Dan
	if(m_fAttackTimer >= 1.7f)// + (1.7f * GetOwner()->GetAgent()->GetTimerOffset()))
	{
		if (IceBreathCheck(GetOwner()->GetAgent(), pTarget))
		{
			pTarget->OnAttack(GetOwner()->GetAgent()->GetDamage(), GetOwner()->GetAgent()->GetAttackElement());
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_3D_Punches");
			// if a beast is using a melee attack to hit the player	
			if (CInput::GetInstance()->GetIfUsingController())
			{
				pTarget->InitiateBeastMeleeRumble(GetOwner()->GetAgent()->GetDamage());
			}
		}
		m_bAttacked = true;
	}
}