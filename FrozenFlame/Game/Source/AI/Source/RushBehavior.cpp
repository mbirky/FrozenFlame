#include "../Header/RushBehavior.h"
#include "../Header/CFSM.h"
#include "../../Objects/ExplosiveEnemy.h"

CRushBehavior::CRushBehavior()
{
	m_fExplosionTimer = 0.0f;
	m_bBombLit = false;
	m_nDegreesToSpin = 0;
}

CRushBehavior::~CRushBehavior()
{
}

void CRushBehavior::Enter()
{
	m_bPlayerHit = false;
	m_nSoundCounter = 60;
	m_nDegreesToSpin = 720;
}

void CRushBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	//if (GetOwner()->GetAgent()->GetElementalState() <= CCollidableObject::COLD && !m_bBombLit)
	//{
	//	GetOwner()->TransitionTo(HOMING_BEHAVIOR);
	//}

	static bool isPlaying = false;

	if (GetOwner()->GetCooldown())
	{
		GetOwner()->GetCooldownTimer() += _fElapsedTime;

		GetOwner()->GetAgent()->SetColor(vec3f(0.0f, 0.0f, 0.0f));

		CSoundManager::GetInstance()->StopSound(AK::EVENTS::STOP_FX_EXPLOSIVEMINION_APPROACH,((CExplosiveEnemy*)GetOwner()->GetAgent())->GetSoundID());
		isPlaying = false;
		
		GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));

		if (GetOwner()->GetCooldownTimer() >= 0.5f)// + (0.5f * GetOwner()->GetAgent()->GetTimerOffset()))
		{
			GetOwner()->GetCooldown() = false;
			GetOwner()->GetCooldownTimer() = 0.0f;
			GetOwner()->GetAgent()->SetColor(vec3f(1.0f, 1.0f, 1.0f));
		}
	}
	else if (!m_bBombLit /*&& ((CExplosiveEnemy*)GetOwner()->GetAgent())->GetExplodeScale() == 1.0f*/)
	{
		vec3f distanceVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
		GetOwner()->GetAgent()->SetVelocity(vec3f(1.0f, 0.0f, 1.0f));

		if (distanceVec.magnitude() < APPROACH_DISTANCE /** 3.0f*/)
		{
			//GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			//matrix4f agentMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
			//agentMatrix.rotate_y_pre(D3DXToRadian(30));
			//GetOwner()->GetAgent()->SetWorldMatrix(agentMatrix);
//			GetOwner()->GetAgent()->GetMatrix()->rotate_y_post(D3DXToRadian(20));
			//m_nDegreesToSpin -= 30;

			//if (m_nDegreesToSpin == 0)
			//{
				//m_bBombLit = true;
			if(GetOwner()->GetAgent()->GetAttackElement() == FIRE)
			{
				GetOwner()->GetAgent()->SetElementalState(GetOwner()->GetAgent()->HOT);
				//GetOwner()->GetAgent()->SetColor(vec3f(0.0f, 0.0f, 0.0f));
			}
			else if(GetOwner()->GetAgent()->GetAttackElement() == ICE)
			{
				GetOwner()->GetAgent()->SetElementalState(GetOwner()->GetAgent()->COLD);
				//GetOwner()->GetAgent()->SetColor(vec3f(0.0f, 0.75f, 0.0f));
			}
			m_nSoundCounter = int(60 * GetOwner()->GetAgent()->GetAttackSpeed());
			GetOwner()->GetAgent()->SetVelocity(vec3f(1.0f, 0.0f, 1.0f));
			//}
			
			//if (GetOwner()->GetAgent()->GetAttackElement() == FIRE)
			//	GetOwner()->GetAgent()->SetElementalState(CCollidableObject::HOT);
			//else
			//	GetOwner()->GetAgent()->SetElementalState(CCollidableObject::COLD);
			
		}
		else
		{
			vec3f approachVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
			TurnTo(_pcPlayer->GetWorldMatrix()->axis_pos, _fElapsedTime);
			Move(approachVec, _fElapsedTime);
		}
		m_fExplosionTimer = 0.0f;
	}
	else if (m_bBombLit)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
		//if(GetOwner()->GetAgent()->GetAttackElement() == FIRE)
		//{
		//	GetOwner()->GetAgent()->SetElementalState(GetOwner()->GetAgent()->HOT);
		//	//GetOwner()->GetAgent()->SetColor(vec3f(0.0f, 0.0f, 0.0f));
		//}
		//else if(GetOwner()->GetAgent()->GetAttackElement() == ICE)
		//{
		//	GetOwner()->GetAgent()->SetElementalState(GetOwner()->GetAgent()->COLD);
		//	//GetOwner()->GetAgent()->SetColor(vec3f(0.0f, 0.75f, 0.0f));
		//}

		m_fExplosionTimer += _fElapsedTime;
		m_nSoundCounter -= 1;

		if (m_nSoundCounter % 20 == 0 && isPlaying == false)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_EXPLOSIVEMINION_APPROACH,((CExplosiveEnemy*)GetOwner()->GetAgent())->GetSoundID());
			isPlaying = true;
		}

		if (m_fExplosionTimer >= GetOwner()->GetAgent()->GetAttackSpeed()/1.25f && m_fExplosionTimer < GetOwner()->GetAgent()->GetAttackSpeed())
		{
			GetOwner()->GetAgent()->FlashColor(vec3f(0.0f, 0.0f, 0.0f));
		}
		else if (m_fExplosionTimer >= GetOwner()->GetAgent()->GetAttackSpeed())// * GetOwner()->GetAgent()->GetTimerOffset())
		{
			CSoundManager::GetInstance()->StopSound(AK::EVENTS::STOP_FX_EXPLOSIVEMINION_APPROACH,((CExplosiveEnemy*)GetOwner()->GetAgent())->GetSoundID());

			isPlaying = false;
 			((CExplosiveEnemy*)GetOwner()->GetAgent())->SetExploding(true);
			GetOwner()->GetAgent()->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			Sphere enemySphere, playerSphere;
			GetOwner()->GetAgent()->GetCollisionSphere(enemySphere.m_Center, enemySphere.m_Radius);
			_pcPlayer->GetCollisionSphere(playerSphere.m_Center, playerSphere.m_Radius);
			enemySphere.m_Radius *= 4.0f;
			GetOwner()->GetAgent()->SetCollisionSphere(enemySphere.m_Center, enemySphere.m_Radius);
			if (!CheckCollision(GetOwner()->GetAgent(), _pcPlayer, SPHERE_TO_SPHERE))
			{
				CEnemy* tempEnemy = GetOwner()->GetAgent();
				enemySphere.m_Radius *= 3.0f;
				tempEnemy->SetCollisionSphere(tempEnemy->GetPos(), enemySphere.m_Radius);
				if (CheckCollision(tempEnemy, _pcPlayer, SPHERE_TO_SPHERE))
				{
					CAchievementManager::GetInstance()->ActivateAchievement("Avoid and Evade");
				}
				enemySphere.m_Radius /= 3.0f;
				tempEnemy->SetCollisionSphere(tempEnemy->GetPos(), enemySphere.m_Radius);
			}
			m_bPlayerHit = true;
			m_fExplosionTimer = 0.0f;
			m_bBombLit = false;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_CRASH, ((CExplosiveEnemy*)GetOwner()->GetAgent())->GetSoundID());
			//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(), "FX_3D_Explosion");
		}
	}
}

void CRushBehavior::Exit()
{
	
}