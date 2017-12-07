#include "../Header/ExhaustBehavior.h"
#include "../../Core/CWorldManager.h"
#include "../../Animations/Interpolator.h"
#include "../../Animations/Animation.h"
#include "../../Objects/WizardEnemy.h"

CExhaustBehavior::CExhaustBehavior()
{
	m_bIsExhausted = false;
}

void CExhaustBehavior::Enter()
{
	m_bTimerStart = false;
	m_bEnemiesSpawned = false;
	m_fExhaustTimer = 0.0f;
	m_fFloatTimer = 0.0f;
	GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
	GetOwner()->GetAgent()->SetKnockbackVelocity(vec3f(0.0f,0.0f,0.0f));
	//m_tOriginalColor = GetOwner()->GetAgent()->GetColor();
	//GetOwner()->GetAgent()->SetColor(vec3f(0.0f, 0.0f, 0.0f));
	GetOwner()->GetAgent()->SetIsAttacking(false);
	GetOwner()->GetAgent()->SetIsMoving(false);

	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->HideBubble();
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetBubbleSheenEmitterActive(false);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetBubbleBurstEmitterActive(true);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetSmokeEmitterActive(true);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetExhausted(true);

	CAnimPlayer* pAnimPlayer = GetOwner()->GetAgent()->GetAnimationPlayer();

	if(pAnimPlayer)
	{
		pAnimPlayer->PlayAnimationToEnd(ANIM_DAMAGED);
		//pAnimPlayer->SetCurTime((((CWizardEnemy*)(this->GetOwner()->GetAgent()))->GetAnimationPlayer()->GetAnimDuration(ANIM_DEATH))-0.001f);
		//pAnimPlayer->SetAnimationSpeed(0.0f);
	}

	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_EXHAUST, GAME_OBJECT_ID_PLAYER);
}

void CExhaustBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	CWizardEnemy * pWiz = (CWizardEnemy*)GetOwner()->GetAgent();

	if(pWiz->GetWizardSubtype() == FINAL_BOSS && pWiz->GetTotalHealth() <= 0)
		return;

	vec3f targetVec;// = _pcPlayer->GetPos();
	CWorldManager * pWorldMan = CWorldManager::GetInstance();
	targetVec.x = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterX();// + 10.0f;
	targetVec.z = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterZ();// - 35.0f;
	targetVec.y = MAP_HEIGHT;

	//if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardForm() != 4)
	{
		SoftAttach(targetVec,_fElapsedTime*20.0f);
	}

	if (!m_bTimerStart)
	{
		m_fExhaustTimer += _fElapsedTime;

		vec3f dist = targetVec - GetOwner()->GetAgent()->GetPos();

		if(dist.magnitude() < 1.0f || m_fExhaustTimer >= 3.0f)
		{
			m_bTimerStart = true;
			m_fExhaustTimer = 0.0f;
			((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SpawnEnemies();
			if(((CWizardEnemy*)(this->GetOwner()->GetAgent()))->GetWizardForm() != 4)
			{
				((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetCrashEmitterActive(true);
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_CRASH, GAME_OBJECT_ID_PLAYER);
			}
			
		}

		//vec3f tempPos = GetOwner()->GetAgent()->GetPos();
		//
		//tempPos.y -= 0.1f;
		//m_fFloatTimer += _fElapsedTime;
		//
		//if (tempPos.y <= MAP_HEIGHT/* || m_fFloatTimer >= 3.0f*/)
		//{
		//	tempPos.y = MAP_HEIGHT;
		//	m_bTimerStart = true;
		//}
		//GetOwner()->GetAgent()->SetPos(tempPos);
		//TurnTo(_pcPlayer->GetPos(), _fElapsedTime);
		//
		//vec3f tempVec = GetOwner()->GetAgent()->GetPos() - _pcPlayer->GetPos();
		/*if (tempVec.magnitude() <= 6.0f || CheckCollision(GetOwner()->GetAgent(), _pcPlayer, SPHERE_TO_SPHERE))
		{
			tempVec.y = 0.0f;
			tempVec *= -1.0f;
			tempVec = tempVec.normalize();
			_pcPlayer->SetVelocity(tempVec * 500.0f);
		}*/

		/*if (CheckCollision(GetOwner()->GetAgent(), _pcPlayer, SPHERE_TO_SPHERE))
		{
			NormalCollisionReaction(GetOwner()->GetAgent(), _pcPlayer);
		}*/
	}
	else
	{
		//if(!m_bIsExhausted && !GetOwner()->GetAgent()->GetAnimationPlayer()->IsPlaying(ANIM_DAMAGED))
		{
			//GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_DAMAGED);
			m_bIsExhausted = true;
		}

		
		//if (!m_bEnemiesSpawned)
		//{
		//	int amountToSpawn = ((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardForm() * 3;
		//	for (int i = 0; i < amountToSpawn; ++i)
		//	{				
		//		vec3f enemyPos = GetOwner()->GetAgent()->GetPos();
		//		enemyPos.z -= 3.0f;
		//		if (i % 2 == 1)
		//		{
		//			enemyPos.x -= 2.0f * i/2;
		//		}
		//		else
		//		{
		//			enemyPos.x += 2.0f * i/2;
		//		}
		//		CMeleeEnemy* tempEnemy = (CMeleeEnemy*)CWorldManager::GetInstance()->GetLevel(0)->PushEnemy(enemyPos, MELEE_OBJECT);
		//		tempEnemy->GetFSM()->RemoveState(WANDER_BEHAVIOR, MELEE_BEHAVIOR);//(MELEE_BEHAVIOR);
		//
		//		//pcEnemy->SetPos(enemyPos);
		//		//pcEnemy->GetFSM()->GetIsChasing() = true;
		//		//CWorldManager::GetInstance()->GetBehaviorManager()->AddFSM(pcEnemy->GetFSM());
		//		
		//	}
		//	m_bEnemiesSpawned = true;
		//	//CWorldManager::GetInstance()->GetLevel(0)->ActivateEnemies();
		//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(), "DX_3D_Aggro");
		//}
		m_fExhaustTimer += _fElapsedTime;

		if (m_fExhaustTimer >= 6.0f)
		{
			m_bIsExhausted = false;
			//GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_DEATH);
			/*vec3f tempVec = GetOwner()->GetAgent()->GetPos() - _pcPlayer->GetPos();
			if (tempVec.magnitude() <= 6.0f)
			{
				tempVec.y = 0.0f;
				tempVec *= -1.0f;
				tempVec = tempVec.normalize();
				_pcPlayer->SetVelocity(tempVec * 500.0f);
			}*/

			if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS && ((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardForm() == 4)
			{
				vec3f tempPos = GetOwner()->GetAgent()->GetPos();
				tempPos.y = MAP_HEIGHT;
				GetOwner()->GetAgent()->SetPos(tempPos);
				GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
				
			}

			//if(false)//((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS)
			//{
			//	switch (((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardForm())
			//	{
			//	case 1:
			//		{
			//			GetOwner()->GetCooldownTimer() = 0.0f;
			//			GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
			//			//((CWizardEnemy*)GetOwner()->GetAgent())->ActivateLiftOffEmitter();
			//			((CWizardEnemy*)GetOwner()->GetAgent())->DeactivateFlowEmitter();
			//		}
			//		break;
			//
			//	case 2:
			//		{
			//			vec3f tempPos = GetOwner()->GetAgent()->GetPos();
			//			tempPos.y = MAP_HEIGHT;
			//			GetOwner()->GetAgent()->SetPos(tempPos);
			//			GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
			//			((CWizardEnemy*)GetOwner()->GetAgent())->DeactivateFlowEmitter();
			//		}
			//		break;
			//
			//	case 3:
			//		{
			//			GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
			//			//((CWizardEnemy*)GetOwner()->GetAgent())->ActivateLiftOffEmitter();
			//		}
			//		break;
			//	}
			//}
			//else
			//{
			//	GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
			//}

			if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() != FINAL_BOSS)
			{
				GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
			}

			m_fExhaustTimer = 0.0f;
			((CWizardEnemy*)GetOwner()->GetAgent())->SetExhausted(false);
			
		}
	}
}

void CExhaustBehavior::Exit()
{
	//GetOwner()->GetAgent()->SetColor(m_tOriginalColor);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->ShowBubble();
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetBubbleSheenEmitterActive(true);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetBubbleBurstEmitterActive(false);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetSmokeEmitterActive(false);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetTakeoffEmitterActive(true);
	((CWizardEnemy*)(this->GetOwner()->GetAgent()))->SetExhausted(false);
	if(((CWizardEnemy*)(this->GetOwner()->GetAgent()))->GetAnimationPlayer())
	{
		((CWizardEnemy*)(this->GetOwner()->GetAgent()))->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
		((CWizardEnemy*)(this->GetOwner()->GetAgent()))->GetAnimationPlayer()->PlayAnimation(ANIM_IDLE,true);
	}
	
}