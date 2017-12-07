#include "../Header/WanderBehavior.h"
#include "../Header/CFSM.h"
#include "../../Core/CWorldManager.h"
//#include "../Header/CBehaviorManager.h"

CWanderBehavior::CWanderBehavior()
{
	m_bPositionSet = false;
	m_bStopWandering = false;	
}

void CWanderBehavior::Enter()
{
	m_fWanderAmount = (float)rand() / (float)RAND_MAX * 2.0f;
	m_fWanderTimer = m_fWanderAmount;
	m_fIceWalkTimer = 0.0f;
	m_fIceWalkBuffer = 0.45f;
	m_tPointToAdvance = GetOwner()->GetAgent()->GetWorldMatrix()->axis_z;
	m_tPointToAdvance = m_tPointToAdvance.normalize();
	m_tPointToAdvance *= GetOwner()->GetAgent()->GetMoveSpeed();
	if (!m_bPositionSet)
	{
		if ((m_tCenterPoint.x != GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos.x) && (m_tCenterPoint.y != GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos.y) && (m_tCenterPoint.z != GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos.z))
		{
			m_tCenterPoint = GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
		}
		m_bPositionSet = true;
	}
}

void CWanderBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	m_fIceWalkTimer += _fElapsedTime;
	

	//if(((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRolling())
	//{
	//
	//	if( !((CFireEnemy*)(GetOwner()->GetAgent()))->GetIsRollingUp() )
	//	{
	//		GetOwner()->GetAgent()->GetAnimationPlayer()->Stop();
	//		GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
	//		//GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
	//		GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
	//		
	//		//float fDur = GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimation()->GetDuration();
	//		//GetOwner()->GetAgent()->GetAnimationPlayer()->SetCurTime(CAssetManager::GetAnimation(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurAnimationID())->;
	//		GetOwner()->GetAgent()->GetAnimationPlayer()->SetCurTime(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration());
	//		
	//		GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(-1.0f);
	//		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(true);
	//		//GetOwner()->GetIsChasing() = false;
	//		
	//	}
	//
	//	//if(!GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_A))
	//	if(GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurTime() <= 0.0f)
	//	{
	//		GetOwner()->GetAgent()->GetAnimationPlayer()->Stop();
	//		GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
	//		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRollingUp(false);
	//		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
	//		((CFireEnemy*)(GetOwner()->GetAgent()))->SetIsRolling(false);
	//		GetOwner()->TransitionTo(WANDER_BEHAVIOR);
	//		
	//	}
	//}

	if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::FROZEN && GetOwner()->GetAgent()->GetType() != ICE_OBJECT)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		return;
	}

	if (GetOwner()->GetIsChasing())
	{
		/*if (GetOwner()->GetAgent()->GetType() != FIRE_OBJECT)
			GetOwner()->TransitionTo(CBaseBehavior::FLOCK_BEHAVIOR);
		else if (GetOwner()->GetAgent()->GetType() == FIRE_OBJECT)
			GetOwner()->TransitionTo(CBaseBehavior::ALIGN_BEHAVIOR);
		else if (GetOwner()->GetAgent()->GetType() == ICE_OBJECT)
			GetOwner()->TransitionTo(SNOWBALL_BEHAVIOR);*/
		switch (GetOwner()->GetAgent()->GetType())
		{
		case FIRE_OBJECT:
			{
				GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
			}
			break;

		case ICE_OBJECT:
			{
				GetOwner()->TransitionTo(SNOWBALL_BEHAVIOR);
			}
			break;

		case EXPLOSIVE_OBJECT:
			{
				//TurnTo(_pcPlayer->GetPos(), _fElapsedTime);
				GetOwner()->TransitionTo(RUSH_BEHAVIOR);
			}
			break;
		case RANGED_OBJECT:
			{
				GetOwner()->TransitionTo(RANGED_BEHAVIOR);
				break;
			}
		default:
			{
				if (GetOwner()->GetAgent()->GetElementalState() < CCollidableObject::BURNING)
				{
					GetOwner()->TransitionTo(HOMING_BEHAVIOR);
					CWorldManager::GetInstance()->GetBehaviorManager()->AddEnemyToFlock(GetOwner()->GetAgent());
				}
				else
				{
					m_fWanderAmount = (float)rand() / (float)RAND_MAX * 2.0f;
					m_fWanderTimer = m_fWanderAmount;
					return;
				}
			}
			break;
		}
	}

	if(GetOwner()->GetAgent()->GetType() == RANGED_OBJECT)
		return;

	if (!m_bStopWandering)
	{
		m_fWanderTimer -= _fElapsedTime;
		Move(m_tPointToAdvance, _fElapsedTime);
		AvoidTraps(_fElapsedTime);
	}
	else
	{
		m_fWanderTimer += _fElapsedTime;
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
	}

	

	

	if (m_fWanderTimer <= 0.0f)
	{
		vec3f distanceVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

		
		
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		m_bStopWandering = true;
		GetOwner()->GetAgent()->SetIsMoving(false);
	}
	else if (m_fWanderTimer >= m_fWanderAmount)
	{
		// if this is an ice golem, play its walk sound every third of a second while walking
//	if (GetOwner()->GetAgent()->GetType() == ICE_OBJECT && m_fIceWalkTimer > m_fIceWalkBuffer)
//	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_ICEGOLEM_WALK, GetOwner()->GetAgent()->GetSoundID());
		m_fIceWalkTimer = 0.0f;
//	}
		m_bStopWandering = false;
		GetOwner()->GetAgent()->SetIsMoving(true);
		GetOwner()->GetAgent()->SetIsAttacking(false);
		float r = (float)rand()/(float)RAND_MAX;
		m_fWanderAmount = (float)rand() / (float)RAND_MAX * 2.0f;
		m_fWanderTimer = m_fWanderAmount;
		matrix4f worldMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
		worldMatrix.rotate_y_pre(r * 180.0f * _fElapsedTime);
		GetOwner()->GetAgent()->SetWorldMatrix(worldMatrix);
//		GetOwner()->GetAgent()->GetMatrix()->rotate_y_pre(r * 180.0f * _fElapsedTime);
		AvoidTraps(_fElapsedTime);
		
	}
}

void CWanderBehavior::Exit()
{

}
