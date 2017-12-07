#include "../Header/HyperdashBehavior.h"
#include "../../Core/CWorldManager.h"
#include "../../Animations/Animation.h"

void CHyperdashBehavior::Enter()
{
	m_fTrapTimer = 0.0f;
	m_fDashCooldown = 0.0f;
	m_bTargetSet = false;
	m_tEndPosition = vec3f(0.0f, 0.0f, 0.0f);
	m_ucSoundCounter = 0;
	m_nDashCounter = 0;
	m_tStartPosition = GetOwner()->GetAgent()->GetPos();
	m_cTurnDirection = -1;
	GetOwner()->GetAgent()->SetMoveSpeed(GetOwner()->GetAgent()->GetMoveSpeed() * 2.75f);
	GetOwner()->GetCooldown() = false;
	GetOwner()->GetCooldownTimer() = 0.0f;
	GetOwner()->GetAgent()->SetAttackElement(((rand()%2)*2)-1);
	m_fTrailTimer = 0.0f;

	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:	{
			m_fSpeedModifier = 0.7f;
		}
		break;

	case DIFF_MEDIUM: {
			m_fSpeedModifier = 0.9f;	
		}
		break;

	case DIFF_HARD: {
			m_fSpeedModifier = 1.1f;
		}
		break;
	}
}

void CHyperdashBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if(this->GetOwner() && !this->GetOwner()->GetIsChasing())
		return;


	vec3f pos = GetOwner()->GetAgent()->GetPos();
	GetOwner()->GetAgent()->SetPos(vec3f(pos.x,MAP_HEIGHT,pos.z));

	CEnemy* tempEnemy = GetOwner()->GetAgent();

	if (GetOwner()->GetCooldown())
	{
		GetOwner()->GetCooldownTimer() += _fElapsedTime;

		if (GetOwner()->GetCooldownTimer() >= 2.0f)
		{
			GetOwner()->GetCooldown() = false;
			GetOwner()->GetCooldownTimer() = 0.0f;

		}
	}
	else
	{
		m_fDashCooldown += _fElapsedTime;

		if (m_fDashCooldown < 1.0f)
		{
			if (!m_bTargetSet)
			{
				m_tEndPosition = _pcPlayer->GetPos();
				LookAt(m_tEndPosition,_fElapsedTime);
				//GetOwner()->GetAgent()->SetMoveSpeed(GetOwner()->GetAgent()->GetMoveSpeed() * 2.75f);
				m_bTargetSet = true;

				// Create the first of many traps >: D
				if(GetOwner()->GetAgent()->GetAttackElement() == FIRE)
				{
					CFireTrap* pcTrap = (CFireTrap*)GetOwner()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
					if (pcTrap)
					{
						vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
						enemyPos.y = -3.0f;
						pcTrap->SummonTrap(0.25f, enemyPos);
					}
				}
				else
				{
					CIceTrap* pcTrap = (CIceTrap*)GetOwner()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
					if (pcTrap)
					{
						vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
						enemyPos.y = -3.0f;
						pcTrap->SummonTrap(0.25f, enemyPos);
					}
				}
				GetOwner()->GetAgent()->SetIsAttacking(true);
				GetOwner()->GetAgent()->SetIsMoving(false);
			}
			m_ucSoundCounter += 1;

			if(m_nDashCounter == 2)
			{
				vec3f targetVec;// = _pcPlayer->GetPos();
				CWorldManager * pWorldMan = CWorldManager::GetInstance();
				targetVec.x = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterX();// + 10.0f;
				targetVec.z = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterZ();// - 35.0f;
				targetVec.y = MAP_HEIGHT;
				this->LookAt(targetVec,_fElapsedTime);

				targetVec = targetVec - GetOwner()->GetAgent()->GetPos();


				if(targetVec.magnitude() <= 2.0f)
				{
					GetOwner()->TransitionTo(EXHAUST_BEHAVIOR);
				}
			}
			// Time to move the agent in a straight line! :D
			vec3f zAxis = tempEnemy->GetWorldMatrix()->axis_z;
			zAxis = zAxis.normalize();
			zAxis *= tempEnemy->GetMoveSpeed() * m_fSpeedModifier;
			tempEnemy->SetVelocity(zAxis);

			//Move(m_tEndPosition - m_tStartPosition, _fElapsedTime);
			GetOwner()->GetAgent()->SetIsMoving(false);

			m_fTrailTimer += _fElapsedTime;

			if (m_fTrailTimer >= 0.1f)
			{
				m_fTrailTimer = 0.0f;
				if(GetOwner()->GetAgent()->GetAttackElement() == FIRE)
				{
					CFireTrap* pcTrap = (CFireTrap*)GetOwner()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
					if (pcTrap)
					{
						vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
						enemyPos.y = -3.0f;
						pcTrap->SummonTrap(0.25f, enemyPos);
					}
				}
				else
				{
					CIceTrap* pcTrap = (CIceTrap*)GetOwner()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
					if (pcTrap)
					{
						vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
						enemyPos.y = -3.0f;
						pcTrap->SummonTrap(0.25f, enemyPos);
					}
				}
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_HYPERDASH, GAME_OBJECT_ID_PLAYER);
			}
		}
		else
		{
			if (this->m_nDashCounter == 0)
			{				
				vec3f targetPos = _pcPlayer->GetPos() - GetOwner()->GetAgent()->GetPos();
				vec3f xAxis = GetOwner()->GetAgent()->GetWorldMatrix()->axis_x;
				float result = dot_product(targetPos, xAxis);
				// Turn Left
				if (result < 0.0f)
					m_cTurnDirection = -120;
				// Turn Right
				else
					m_cTurnDirection = 120;

				matrix4f worldMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
				worldMatrix.rotate_y_pre(D3DXToRadian(m_cTurnDirection));
				GetOwner()->GetAgent()->SetWorldMatrix(worldMatrix);

				vec3f vWizZ = GetOwner()->GetAgent()->GetWorldMatrix()->axis_z;
				vWizZ.y = 0.0f;
				targetPos.y = 0.0f;

				if(targetPos.magnitude())
					targetPos = targetPos.normalize();
				else
					return;

				if(vWizZ.magnitude())
					vWizZ = vWizZ.normalize();
				else
					return;

				float fAngle = dot_product(targetPos,vWizZ);
				fAngle = D3DXToDegree( acos(fAngle));

				if(fAngle < 30.0f && fAngle > -30.0f)
				{
					m_cTurnDirection = (char)fAngle;
					matrix4f worldMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
					worldMatrix.rotate_y_pre(D3DXToRadian(m_cTurnDirection));
					GetOwner()->GetAgent()->SetWorldMatrix(worldMatrix);
				}

			}
			else
			{
				vec3f targetVec;// = _pcPlayer->GetPos();
				CWorldManager * pWorldMan = CWorldManager::GetInstance();
				targetVec.x = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterX();// + 10.0f;
				targetVec.z = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterZ();// - 35.0f;
				targetVec.y = MAP_HEIGHT;
				m_cTurnDirection = 0;
				
				//targetVec = targetVec - GetOwner()->GetAgent()->GetPos();

				LookAt(targetVec,_fElapsedTime);

			}

			m_nDashCounter += 1;
			



//			GetOwner()->GetAgent()->GetMatrix()->rotate_y_post(D3DXToRadian(m_cTurnDirection));
			m_fDashCooldown = 0.0f;
			float vecLength = (GetOwner()->GetAgent()->GetPos() - m_tStartPosition).magnitude();
			m_tEndPosition = GetOwner()->GetAgent()->GetPos();
			vec3f tempVec = GetOwner()->GetAgent()->GetPos() * vecLength;
			tempVec.normalize();
			m_tEndPosition += tempVec;
			m_tStartPosition = GetOwner()->GetAgent()->GetPos();
		}

		if (m_nDashCounter == 3)
		{
			GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
			m_bTargetSet = false;
			GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);
			GetOwner()->TransitionTo(EXHAUST_BEHAVIOR);
			//GetOwner()->GetCooldown() = true;
		}
	}
}

void CHyperdashBehavior::Exit()
{
	GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
	GetOwner()->GetAgent()->SetMoveSpeed(GetOwner()->GetAgent()->GetMoveSpeed() / 2.75f);
	GetOwner()->GetCooldown() = true;
	GetOwner()->GetAgent()->SetIsAttacking(false);
	GetOwner()->GetAgent()->SetIsMoving(false);
	GetOwner()->GetAgent()->SetAttackElement(0);

}