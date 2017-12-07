#include "../Header/HyperOrbBehavior.h"
#include "../Header/CFSM.h"
#include "../../Core/CWorldManager.h"
#include "../../Animations/Animation.h"

CHyperOrbBehavior::CHyperOrbBehavior()
{
	m_bAttacked = false;
	m_bReady = false;
	m_fDelay = 0.0f;

	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:	{
			m_fDifficultyModifier = 0.7f;
		}
		break;

	case DIFF_MEDIUM: {
			m_fDifficultyModifier = 0.9f;	
		}
		break;

	case DIFF_HARD: {
			m_fDifficultyModifier = 1.0f;
		}
		break;
	}
}

void CHyperOrbBehavior::Enter()
{
	m_fAttackTimer = 0.0f;
	m_nOrbCount = 0;

	// commented out to make him only track the player. to make the wizard use different attack patterns again, change it back to rand mod 4
	// CEM
	m_nAttackType = 2;//rand()%4;

	if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS)
	{
		m_fPhaseModifier = 2.0f;
	}
	else
	{
		m_fPhaseModifier = 1.0f;
	}
	switch (m_nAttackType)
	{
	case 0:
		{
			CWorldManager * pWorldMan = CWorldManager::GetInstance();
			int nLevel = pWorldMan->GetTotalCurrentLevels()-1;
			m_vNextAttackPos.x = pWorldMan->GetLevel(nLevel)->GetCenterX();
			m_vNextAttackPos.z = pWorldMan->GetLevel(nLevel)->GetCenterZ();
			m_vNextAttackPos.y = MAP_HEIGHT;

			m_fAmountToAddX = pWorldMan->GetLevel(nLevel)->GetExtentX()*0.20f/m_fPhaseModifier;
			m_fAmountToAddZ = pWorldMan->GetLevel(nLevel)->GetExtentZ()*0.20f/m_fPhaseModifier;

			m_vNextAttackPos.x -= m_fAmountToAddX*2*m_fPhaseModifier;
			m_vNextAttackPos.z -= m_fAmountToAddZ*2*m_fPhaseModifier;

			m_nOrbChangeDirCount = 0;

			break;
		}
	case 1:
		{
			CWorldManager * pWorldMan = CWorldManager::GetInstance();
			int nLevel = pWorldMan->GetTotalCurrentLevels()-1;
			m_vNextAttackPos.x = pWorldMan->GetLevel(nLevel)->GetCenterX();
			m_vNextAttackPos.z = pWorldMan->GetLevel(nLevel)->GetCenterZ();
			m_vNextAttackPos.y = MAP_HEIGHT;

			m_fAmountToAddX = pWorldMan->GetLevel(nLevel)->GetExtentX()*0.20f/m_fPhaseModifier;
			m_fAmountToAddZ = pWorldMan->GetLevel(nLevel)->GetExtentZ()*0.20f/m_fPhaseModifier;

			m_vNextAttackPos.x -= m_fAmountToAddX*2*m_fPhaseModifier;
			m_vNextAttackPos.z -= m_fAmountToAddZ*2*m_fPhaseModifier;

			m_nOrbChangeDirCount = 0;

			break;
		}
	case 2:
		{
			m_vNextAttackPos.make_zero();
		}
	case 3:
		{
			CWorldManager * pWorldMan = CWorldManager::GetInstance();
			int nLevel = pWorldMan->GetTotalCurrentLevels()-1;
			m_vNextAttackPos.x = pWorldMan->GetLevel(nLevel)->GetCenterX();
			m_vNextAttackPos.z = pWorldMan->GetLevel(nLevel)->GetCenterZ();
			m_vNextAttackPos.y = MAP_HEIGHT;

			m_fAmountToAddX = pWorldMan->GetLevel(nLevel)->GetExtentX()*0.20f/m_fPhaseModifier;
			m_fAmountToAddZ = pWorldMan->GetLevel(nLevel)->GetExtentZ()*0.20f/m_fPhaseModifier;

			m_vNextAttackPos.x -= m_fAmountToAddX*2*m_fPhaseModifier;
			m_vNextAttackPos.z -= m_fAmountToAddZ*2*m_fPhaseModifier;

			m_nOrbChangeDirCount = 0;
		}
	};

	if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() != FINAL_BOSS)
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_HYPERORB_CHARGE, GAME_OBJECT_ID_PLAYER);//GetOwner()->GetAgent()->GetSoundID());
}

void CHyperOrbBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{

	if(this->GetOwner()->GetCooldown() && ((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS)
	{
		this->GetOwner()->GetCooldownTimer() += _fElapsedTime;
		if(GetOwner()->GetCooldownTimer() < 3.0f)
		{
			return;
		}
		else
		{
			GetOwner()->GetCooldown() = false;
			GetOwner()->GetCooldownTimer() = 0.0f;
			if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS)
			{
				this->Enter();
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_HYPERORB_CHARGE, GAME_OBJECT_ID_PLAYER);//GetOwner()->GetAgent()->GetSoundID());
				return;
			}
			else
			{
				GetOwner()->TransitionTo(EXHAUST_BEHAVIOR);
			}
		}
	}

	if(!m_bAttacked)
	{
		//m_fDelay < 
		if(false == m_bReady)
		{
			GetOwner()->GetAgent()->GetAnimationPlayer()->PlayAnimation(ANIM_ATTACK);
			m_fDelay += _fElapsedTime;
		}

		if(m_fDelay >= GetOwner()->GetAgent()->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.80f)
		{
			m_bReady = true;
			m_fAttackTimer += _fElapsedTime;
			GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(0.0f);

			

			if (m_fAttackTimer >= (0.25f / m_fPhaseModifier))
			{
				//vec3f vTarget = _pcPlayer->GetWorldMatrix()->axis_pos;
			
				CProjectileObject * pcProjectile = (CProjectileObject*)GetOwner()->GetObjectManager()->NewObject(PROJECTILE_OBJECT);				
			
				matrix4f projectileMatrix = *(pcProjectile->GetWorldMatrix());
				projectileMatrix.axis_pos = GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
				projectileMatrix.axis_pos.y += 3;
				pcProjectile->SetWorldMatrix(projectileMatrix);
//				pcProjectile->GetMatrix()->axis_pos = GetOwner()->GetAgent()->GetMatrix()->axis_pos;
//				pcProjectile->GetMatrix()->axis_pos.y += 3;
				vec3f vProjVelocity = m_vNextAttackPos - pcProjectile->GetPos();// = vTarget - pcProjectile->GetWorldMatrix()->axis_pos;

				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_HYPERORB, GAME_OBJECT_ID_PLAYER);
				
				switch (m_nAttackType)
				{
				case 0: // horizontal pattern
					{

						m_vNextAttackPos.z += m_fAmountToAddZ;

						m_nOrbChangeDirCount++;

						if(m_nOrbChangeDirCount >= 5*m_fPhaseModifier)
						{
							m_nOrbChangeDirCount = 0;
							m_vNextAttackPos.x += m_fAmountToAddX*m_fPhaseModifier;
							m_fAmountToAddZ *= -1;
							m_vNextAttackPos.z += m_fAmountToAddZ*m_fPhaseModifier;
						}

						break;
					}
				case 1: // vertical pattern
					{
						m_vNextAttackPos.x += m_fAmountToAddX;

						m_nOrbChangeDirCount++;

						if(m_nOrbChangeDirCount >= 5*m_fPhaseModifier)
						{
							m_nOrbChangeDirCount = 0;
							m_vNextAttackPos.z += m_fAmountToAddZ*m_fPhaseModifier;
							m_fAmountToAddX *= -1;
							m_vNextAttackPos.x += m_fAmountToAddX*m_fPhaseModifier;
						}
						break;
					}
				case 2: // track player
					{
						vProjVelocity = _pcPlayer->GetPos() - pcProjectile->GetPos(); 
					}
				case 3: // cross pattern
					{
						
						m_nOrbChangeDirCount++;

						int nStage = (int)m_nOrbChangeDirCount/(int)(5*m_fPhaseModifier);

						switch (nStage)
						{
						case 0:
							{
								m_vNextAttackPos.x += m_fAmountToAddX;
								m_vNextAttackPos.z += m_fAmountToAddZ;
								break;
							}
						case 1:
							{
								if(m_nOrbChangeDirCount == 5*m_fPhaseModifier)
								{
									m_vNextAttackPos.z -= m_fAmountToAddZ * 5*m_fPhaseModifier;
									m_vNextAttackPos.x += m_fAmountToAddX;//*m_fPhaseModifier;// * 5;
									m_fAmountToAddX *= -1;
								}

								m_vNextAttackPos.x += m_fAmountToAddX;
								m_vNextAttackPos.z += m_fAmountToAddZ;

								break;
							}
						case 2:
							{
								if(m_nOrbChangeDirCount == 10*m_fPhaseModifier)
								{
									m_vNextAttackPos.z -= m_fAmountToAddZ * 5*m_fPhaseModifier;
									m_vNextAttackPos.x -= m_fAmountToAddX * 2*m_fPhaseModifier;
									//m_fAmountToAddX *= -1;
								}

								//m_vNextAttackPos.x += m_fAmountToAddX;
								m_vNextAttackPos.z += m_fAmountToAddZ;

								break;
							}
						case 3:
							{
								if(m_nOrbChangeDirCount == 15*m_fPhaseModifier)
								{
									//m_vNextAttackPos.z -= m_fAmountToAddZ * 5;
									//m_vNextAttackPos.x -= m_fAmountToAddX * 2;
									m_fAmountToAddZ *= -1;
									m_vNextAttackPos.z -= m_fAmountToAddZ*m_fPhaseModifier;// * 2;

								}

								//m_vNextAttackPos.x += m_fAmountToAddX;
								m_vNextAttackPos.z += m_fAmountToAddZ;
								break;
							}
						case 4:
							{
								if(m_nOrbChangeDirCount == 20*m_fPhaseModifier)
								{
									//m_vNextAttackPos.z -= m_fAmountToAddZ * 5;
									//m_vNextAttackPos.x -= m_fAmountToAddX * 2;
									m_fAmountToAddZ *= -1;
									m_vNextAttackPos.z -= m_fAmountToAddZ*m_fPhaseModifier;// *3;
								}

								//m_vNextAttackPos.x += m_fAmountToAddX;
								m_vNextAttackPos.z += m_fAmountToAddZ;
								break;
							}
						}

					}
				};

				vProjVelocity = vProjVelocity.normalize();
				vProjVelocity *= 2.0f;
				pcProjectile->SetVelocity(vProjVelocity);
				


				pcProjectile->SetOwner(GetOwner()->GetAgent());
				pcProjectile->SetDamage(GetOwner()->GetAgent()->GetDamage());
				if(GetOwner()->GetAgent()->GetAttackElement() == 0)
				{
					//rand being used to calculate values of 1 or -1
					pcProjectile->SetAttackElement(((rand() % 2)*2)-1);
				}
				else
				{
					pcProjectile->SetAttackElement(GetOwner()->GetAgent()->GetAttackElement());
				}
				
				if(pcProjectile->GetAttackElement() == 1)
				{
					pcProjectile->SetColor(vec3f(1.0f,0.0f,0.0f));
					// CView::AttachLight(pcProjectile->GetRenderID(), vec4f(1.0f, 0.0f, 0.0f, 1.0f), 30.0f);
					CView::MakeLight(pcProjectile->GetRenderID(), true, vec3f(0, 0, 0), vec4f(1, 0, 0, 1), 30.0f);
				}
				else
				{
					pcProjectile->SetColor(vec3f(0.0f,0.0f,1.0f));
					// CView::AttachLight(pcProjectile->GetRenderID(), vec4f(0.0f, 0.0f, 1.0f, 1.0f), 30.0f);
					CView::MakeLight(pcProjectile->GetRenderID(), true, vec3f(0, 0, 0), vec4f(0, 0, 1, 1), 30.0f);
				}

				CView::SetRenderInstanceFlags(pcProjectile->GetRenderID(), CView::GetRenderInstance(pcProjectile->GetRenderID())->GetFlags() | RENDER_INVISIBLE);

				m_fAttackTimer = 0.0f;
				GetOwner()->GetAgent()->SetIsAttacking(true);
			//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_2D_WeaponWhoosh");
				m_nOrbCount += 1;
			}
			


			if (m_nOrbCount == (int)(25 * m_fPhaseModifier * m_fDifficultyModifier))
			{
				//CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrentLevel())->ActivateEnemies();
				if(GetOwner()->GetAgent()->GetAnimationPlayer()->GetAnimationSpeed() < 1.0f)
				{
					GetOwner()->GetAgent()->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
				}
			
				m_bAttacked = false;
				m_fDelay = 0.0f;
				m_bReady = true;
				m_bReady = false;
				//GetOwner()->TransitionTo(EXHAUST_BEHAVIOR);
				//this->Enter();
				//return;
				if(((CWizardEnemy*)GetOwner()->GetAgent())->GetWizardSubtype() == FINAL_BOSS)
				{
					this->GetOwner()->GetCooldown() = true;
					this->GetOwner()->GetCooldownTimer() = 0.0f;
				}
				else
				{
					GetOwner()->TransitionTo(EXHAUST_BEHAVIOR);
				}

			}
		}
	}
}

void CHyperOrbBehavior::Exit()
{

}