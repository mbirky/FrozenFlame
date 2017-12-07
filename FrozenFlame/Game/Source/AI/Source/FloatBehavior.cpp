#include "../Header/FloatBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/Interpolator.h"
#include "../../Animations/Animation.h"
#include "../../Core/CWorldManager.h"


CFloatBehavior::CFloatBehavior()
{
	m_bStopWandering = false;
	m_bReturnToCenter = false;
	m_bDirectionSet = false;
	m_tStrafePoint = vec3f(0.0f, 0.0f, 0.0f);
}

void CFloatBehavior::Enter()
{
	m_fWanderAmount = (float)rand() / (float)RAND_MAX;
	m_fFloatTimer = 0.0f;
	m_tPointToAdvance = GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
	vec3f tempVec = GetOwner()->GetAgent()->GetWorldMatrix()->axis_z;
	tempVec = tempVec.normalize();
	m_tPointToAdvance += tempVec * GetOwner()->GetAgent()->GetMoveSpeed();
	m_nFloatCount = 0;
	//m_tOriginalPosition = GetOwner()->GetAgent()->GetPos();
	m_bStrafeLeft = true;
	m_bPositionSet = false;
	GetOwner()->GetAgent()->SetIsMoving(false);
	GetOwner()->GetCooldown() = true;
}

void CFloatBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if(this->GetOwner() && !this->GetOwner()->GetIsChasing())
		return;

	if(((CWizardEnemy*)this->GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS && ((CWizardEnemy*)this->GetOwner()->GetAgent())->GetWizardForm() == 4)
	{
		GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
	}

	int nCurrWizrdLevel = CWorldManager::GetInstance()->GetLevelForObject(GetOwner()->GetAgent());

	if(nCurrWizrdLevel > -1 && nCurrWizrdLevel < (int)(CWorldManager::GetInstance()->GetTotalCurrentLevels()-2))
	{
		//int i = CWorldManager::GetInstance()->GetLevelForObject(GetOwner()->GetAgent());
		GetOwner()->TransitionTo(RESET_BEHAVIOR);
	}

	if(GetOwner()->GetAgent()->GetIsAttacking())
	{
		GetOwner()->GetAgent()->SetIsAttacking(false);
		GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);
	}
	else
	{
		GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);
	}

	m_fSinFloatTimer += _fElapsedTime;
	if (!m_bPositionSet)
	{
		m_tOriginalPosition = GetOwner()->GetAgent()->GetPos();
		m_bPositionSet = true;
	}
	/*if (!m_bPositionSet)
	{
		m_tCenterPoint = GetOwner()->GetAgent()->GetPos();
		m_bPositionSet = true;
	}
	if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::FROZEN && GetOwner()->GetAgent()->GetType() != ICE_OBJECT)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		return;
	}
	else */if (GetOwner()->GetCooldown())
	{
		if (GetOwner()->GetAgent()->GetPos().y < 5.0f)
		{
			if(((CWizardEnemy*) GetOwner()->GetAgent())->GetWizardSubtype() != FOURTH_STAGE && ((CWizardEnemy*) GetOwner()->GetAgent())->GetWizardForm() != 3)
			{
				vec3f targetVec = _pcPlayer->GetPos();
				targetVec.y = GetOwner()->GetAgent()->GetPos().y;

				targetVec.x += 8.0f;
				targetVec.z -= 8.0f;

				this->SoftAttach(targetVec,_fElapsedTime);
			}
			

			GetOwner()->GetAgent()->SetPos(vec3f(GetOwner()->GetAgent()->GetPos().x, GetOwner()->GetAgent()->GetPos().y + 5.0f * _fElapsedTime, GetOwner()->GetAgent()->GetPos().z));
		}
		else
		{
			GetOwner()->GetCooldown() = false;
			GetOwner()->GetCooldownTimer() = 0.0f;
		}
		GetOwner()->GetCooldownTimer() += _fElapsedTime;

		if (GetOwner()->GetCooldownTimer() >= 3.0f)
		{
			GetOwner()->GetCooldown() = false;
			GetOwner()->GetCooldownTimer() = 0.0f;
		}
	}
	else
	{
		m_fFloatTimer += _fElapsedTime;

		vec3f targetVec;// = _pcPlayer->GetPos();

		if(((CWizardEnemy*) GetOwner()->GetAgent())->GetWizardSubtype() == FOURTH_STAGE || ((CWizardEnemy*) GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS)
		{
			CWorldManager * pWorldMan = CWorldManager::GetInstance();
			targetVec.x = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterX();// + 10.0f;
			targetVec.z = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterZ();// - 35.0f;
			targetVec.y = 15.0f;
		}
		else
		{
			targetVec = _pcPlayer->GetPos();
			targetVec.y = GetOwner()->GetAgent()->GetPos().y;

			targetVec.x += 8.0f;
			targetVec.z -= 8.0f;
		}
		this->SoftAttach(targetVec,_fElapsedTime);

		//GetOwner()->GetAgent()->SetPos(targetVec);

		//vec3f playerVec = _pcPlayer->GetPos() - GetOwner()->GetAgent()->GetPos();

		//if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == SECOND_STAGE)
		//{
		//	GetOwner()->GetAgent()->SetPos( GetOwner()->GetAgent()->GetPos() + vec3f(0.0f,0.0f,-15.0f * _fElapsedTime));
		//}
		//if (playerVec.magnitude() <= 20.0f && playerVec.magnitude() > 2.0f)
		//{
		//	TurnTo(_pcPlayer->GetPos(), _fElapsedTime);
		//	vec3f distanceVec = m_tOriginalPosition - GetOwner()->GetAgent()->GetPos();
		//	if (distanceVec.magnitude() < 1.0f)
		//	{
		//		vec3f xAxis = GetOwner()->GetAgent()->GetWorldMatrix()->axis_x;
		//		xAxis.normalize();
		//		xAxis.y = 0.0f;
		//		if (m_bStrafeLeft)
		//		{
		//			xAxis *= -1.0f;
		//		}
		//		GetOwner()->GetAgent()->SetVelocity(xAxis * GetOwner()->GetAgent()->GetMoveSpeed());
		//	}
		//	else
		//	{
		//		m_bStrafeLeft = !m_bStrafeLeft;
		//	}
		//	//vec3f sinVec = vec3f(0.0f, sin(m_fSinFloatTimer), 0.0f);
		//	//sinVec.y *= 2.0f;
		//	//GetOwner()->GetAgent()->SetVelocity(GetOwner()->GetAgent()->GetVelocity() + sinVec);

		//	/*if (!m_bStopWandering)
		//	{
		//		m_fFloatTimer -= _fElapsedTime;
		//		TurnTo(_pcPlayer->GetMatrix()->axis_pos, _fElapsedTime);
		//		
		//		vec3f radVec = GetOwner()->GetAgent()->GetPos() - m_tCenterPoint;
		//		if (radVec.magnitude() >= 30.0f)
		//		{
		//			m_tPointToAdvance = m_tCenterPoint;
		//		}

		//		vec3f moveVec = m_tPointToAdvance - GetOwner()->GetAgent()->GetMatrix()->axis_pos;
		//		moveVec = moveVec.normalize();
		//		moveVec.y = 0.0f;
		//		GetOwner()->GetAgent()->SetVelocity(moveVec * GetOwner()->GetAgent()->GetMoveSpeed());
		//	}
		//	else
		//	{
		//		m_fFloatTimer += _fElapsedTime;
		//		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		//	}
		//	vec3f sinVec = vec3f(0.0f, sin(m_fSinFloatTimer), 0.0f);
		//	sinVec.y *= 2.0f;
		//	GetOwner()->GetAgent()->SetVelocity(GetOwner()->GetAgent()->GetVelocity() + sinVec);

		//	if (m_fFloatTimer <= 0.0f)
		//	{
		//		vec3f distanceVec = _pcPlayer->GetMatrix()->axis_pos - GetOwner()->GetAgent()->GetMatrix()->axis_pos;
		//		
		//		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		//		m_bStopWandering = true;
		//		GetOwner()->GetAgent()->SetIsMoving(false);
		//		m_nFloatCount += 1;
		//	}
		//	else if (m_fFloatTimer >= m_fWanderAmount)
		//	{
		//		m_bStopWandering = false;
		//		GetOwner()->GetAgent()->SetIsMoving(true);
		//		GetOwner()->GetAgent()->SetIsAttacking(false);

		//		float r = (float)rand()/(float)RAND_MAX;
		//		m_fWanderAmount = (float)rand() / (float)RAND_MAX;
		//		m_fFloatTimer = m_fWanderAmount;
		//		matrix4f tempMat = *GetOwner()->GetAgent()->GetMatrix();
		//		tempMat.rotate_y_pre(r * 180.0f * _fElapsedTime);
		//		m_tPointToAdvance = GetOwner()->GetAgent()->GetMatrix()->axis_pos;
		//		vec3f tempVec = tempMat.axis_z;
		//		tempVec = tempVec.normalize();
		//		m_tPointToAdvance += tempVec * GetOwner()->GetAgent()->GetMoveSpeed();
		//	}*/
		//}
		//else if (playerVec.magnitude() > 20.0f)
		//{
		//	TurnTo(_pcPlayer->GetPos(), _fElapsedTime);
		//	playerVec = playerVec.normalize();
		//	playerVec.y = 0.0f;
		//	GetOwner()->GetAgent()->SetVelocity(playerVec * GetOwner()->GetAgent()->GetMoveSpeed());
		//}
		//else
		//{
		//	TurnTo(_pcPlayer->GetPos(), _fElapsedTime);
		//	playerVec = playerVec.normalize();
		//	playerVec.y = 0.0f;
		//	playerVec *= -1.0f;
		//	GetOwner()->GetAgent()->SetVelocity(playerVec * GetOwner()->GetAgent()->GetMoveSpeed());
		//}

		if (m_fFloatTimer >= 2.0f)
		{
			GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			int nWizardSubtype = ((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype();

			switch (nWizardSubtype)
			{
			case FINAL_BOSS:
				{
					switch(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardForm())
					{
					case 1:
						{
							GetOwner()->TransitionTo(TRAP_SPAWN_BEHAVIOR);
							break;
						}
					case 2:
						{
							GetOwner()->TransitionTo(HYPERORB_BEHAVIOR);
							break;
						}
					case 3:
						{
							GetOwner()->TransitionTo(HYPERSNOWBALL_BEHAVIOR);
							break;
						}
					case 4:
						{
							GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
							break;
						}
					};
					//if (((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardForm() != 3)
					//	GetOwner()->TransitionTo(RANGED_BEHAVIOR);
					//else
					//	GetOwner()->TransitionTo(HYPERORB_BEHAVIOR);
					break;
				}
			case FIRST_STAGE:
				{
					GetOwner()->TransitionTo(EXHAUST_BEHAVIOR);
					break;
				}
			case SECOND_STAGE:
				{
					GetOwner()->TransitionTo(RANGED_BEHAVIOR);
					break;
				}
			case FOURTH_STAGE:
				{
					GetOwner()->TransitionTo(HYPERORB_BEHAVIOR);
					break;
				}
			default:
				{
					//LOG("ERROR: INVALID SUBTYPE FOR WIZARD TYPE! \n COULD NOT FIND VALIT TRANSITION FROM FLOAT BEHAVIOR IN UPDATE OF FLOAT.CPP!\n");
					break;
				}
			}
		}
	}
}

void CFloatBehavior::Exit()
{
	GetOwner()->GetCooldown() = true;
	GetOwner()->GetAgent()->SetIsMoving(false);
}