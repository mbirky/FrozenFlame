#include "../Header/AlignBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/AnimPlayer.h"
#include "../../Animations/Animation.h"

void CAlignBehavior::Enter()
{
	m_bReadyToRoll = false;
	m_nDegreesToSpin = 720;
	m_bPlaySpinSound = true;
}

void CAlignBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if(this->GetOwner() && !this->GetOwner()->GetIsChasing())
		return;

	if (GetOwner()->GetAgent()->GetType() == WIZARD_OBJECT)
	{
		vec3f pos = GetOwner()->GetAgent()->GetPos();
		GetOwner()->GetAgent()->SetPos(vec3f(pos.x,MAP_HEIGHT,pos.z));

		matrix4f agentMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
		agentMatrix.rotate_y_post(D3DXToRadian(15));
		GetOwner()->GetAgent()->SetWorldMatrix(agentMatrix);
//					GetOwner()->GetAgent()->GetMatrix()->rotate_y_post(D3DXToRadian(20));
		m_nDegreesToSpin -= 15;

		if (m_bPlaySpinSound)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_HYPERDASH_SPIN, GAME_OBJECT_ID_PLAYER);
			m_bPlaySpinSound = false;
		}

		if (m_nDegreesToSpin == 0)
		{
			GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_DEATH);
			GetOwner()->TransitionTo(HYPERDASH_BEHAVIOR);
		}
		return;
	}

	if(((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRolling())
	{
	
		if( !((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp() )
		{
			GetOwner()->GetAgent()->GetAnimationPlayer()->Stop();
			GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
			
			
			//float fDur = GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimation()->GetDuration();
			//GetOwner()->GetAgent()->GetAnimationPlayer()->SetCurTime(CAssetManager::GetAnimation(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimationID())->;
			GetOwner()->GetAgent()->GetAnimationPlayer()->SetCurTime(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration());
			
			GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(-1.0f);
			//GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
			((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(true);
			
		}

		//if(!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_A))
		else if(((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp() && GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurTime() <= 0.0f)
		{
			GetOwner()->GetAgent()->GetAnimationPlayer()->Stop();
			GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
			((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
			GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
			((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRolling(false);
			GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
			
		}
	}
	else
	{

		vec3f distanceVec = _pcPlayer->GetPos() - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
		if(distanceVec.magnitude() >= ROLL_RANGE)
		{
			//((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
			GetOwner()->TransitionTo(HOMING_BEHAVIOR);
			//return;
		}
		if (GetOwner()->GetCooldown())
		{
			GetOwner()->GetCooldownTimer() += _fElapsedTime;

			if (GetOwner()->GetCooldownTimer() >= 2.0f)
			{
				GetOwner()->GetCooldown() = false;
				GetOwner()->GetCooldownTimer() = 0.0f;
			}
		}
		else if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::COLD && !m_bReadyToRoll)
		{
			GetOwner()->TransitionTo(HOMING_BEHAVIOR);
			return;
		}
		else
		{
			if (!m_bReadyToRoll)
			{
				//TurnTo(_pcPlayer->GetWorldMatrix()->axis_pos, _fElapsedTime);
				LookAt(_pcPlayer->GetWorldMatrix()->axis_pos, _fElapsedTime);
				vec3f zAxis = GetOwner()->GetAgent()->GetWorldMatrix()->axis_z;
				vec3f xAxis = GetOwner()->GetAgent()->GetWorldMatrix()->axis_x;
				zAxis.normalize();
				vec3f targetVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
				targetVec.normalize();
				targetVec.y = 0.0f;
				if (dot_product(targetVec, xAxis) < FLT_EPSILON*10.0f && dot_product(targetVec, xAxis) > -FLT_EPSILON*10.0f)
				{
					m_bReadyToRoll = true;
				}
			}
			else
			{
				//matrix4f worldMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
				//worldMatrix.rotate_y_post(D3DXToRadian(20));
				//GetOwner()->GetAgent()->SetWorldMatrix(worldMatrix);
				//GetOwner()->GetAgent()->GetMatrix()->rotate_y_post(D3DXToRadian(20));

				

				

				
				if (!((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp())
				{
					if(!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_A))
					{
						GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
						((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(true);
					}
				}

				if (!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_A) && ((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp())
				{
					GetOwner()->TransitionTo(ROLL_BEHAVIOR);
				}

					
				
			}
		}
	}
}

void CAlignBehavior::Exit()
{

}
