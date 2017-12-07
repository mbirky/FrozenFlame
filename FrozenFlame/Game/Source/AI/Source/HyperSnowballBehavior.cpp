#include "../Header/HyperSnowballBehavior.h"
#include "../Header/CFSM.h"
#include "../../Objects/Snowball.h"
#include "../../Core/CWorldManager.h"

CHyperSnowballBehavior::CHyperSnowballBehavior()
{
	m_fAttackTimer = 3.5f;
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:	{
			m_unNumMeteors = 30;
			m_fSummonCooldown = 5.0f;
		}
		break;

	case DIFF_MEDIUM: {
			m_unNumMeteors = 50;
			m_fSummonCooldown = 5.0f;
		}
		break;

	case DIFF_HARD: {
			m_unNumMeteors = 100;
			m_fSummonCooldown = 3.5f;
		}
		break;
	}
}

void CHyperSnowballBehavior::Enter()
{
	m_fAttackTimer = 3.5f;
}

void CHyperSnowballBehavior::Update(CPlayerObject * _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	m_fAttackTimer += _fElapsedTime;
	
	CWorldManager * pWorldMan = CWorldManager::GetInstance();
	
	vec3f targetVec;
	targetVec.x = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterX();// + 10.0f;
	targetVec.z = pWorldMan->GetLevel(pWorldMan->GetTotalCurrentLevels() - 1)->GetCenterZ();// - 35.0f;
	targetVec.y = 15.0f;

	SoftAttach(targetVec,_fElapsedTime);

	if(m_fAttackTimer >= m_fSummonCooldown)
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_SUMMON_METEOR, GAME_OBJECT_ID_PLAYER);

		CObjectManager * pObjectMan = pWorldMan->GetObjectManager();
		int nLevel = pWorldMan->GetLevelForObject(this->GetOwner()->GetAgent());
		//CLevel * pLevel = pWorldMan->GetLevel(nLevel);

		float extentX = pWorldMan->GetLevel(nLevel)->GetExtentX();
		float centerX = pWorldMan->GetLevel(nLevel)->GetCenterX();
		
		float extentZ = pWorldMan->GetLevel(nLevel)->GetExtentZ();
		float centerZ = pWorldMan->GetLevel(nLevel)->GetCenterZ();

		int nHighX = (int)(pWorldMan->GetLevel(nLevel)->GetCenterX() + (int)(pWorldMan->GetLevel(nLevel)->GetExtentX()*0.5f));
		int nLowX = (int)(nHighX - pWorldMan->GetLevel(nLevel)->GetExtentX());

		int nHighZ = (int)(pWorldMan->GetLevel(nLevel)->GetCenterZ() - (int)(pWorldMan->GetLevel(nLevel)->GetExtentZ()*0.5f));
		int nLowZ = (int)(nHighZ + pWorldMan->GetLevel(nLevel)->GetExtentZ());

		int nHighY = 150;
		int nLowY = 50;

		for(unsigned int i = 0; i < m_unNumMeteors; i++)
		{
			int x = (rand() % (nHighX - nLowX)) + nLowX;
			int z = (rand() % (nHighZ - nLowZ)) + nLowZ;
			int y = (rand() % (nHighY - nLowY)) + nLowY;
			int nElement = rand() % 2;

			CSnowball * pBall = (CSnowball*)pObjectMan->NewObject(SNOWBALL_OBJECT);
			pBall->SetPos(vec3f((float)x, (float)y, (float)z));
			pBall->SetVelocity(EARTH_GRAVITY);
			pBall->SetWasThrown(true);
			if(nElement == 1)
			{
				pBall->MakeMeteor();
				
			}

		}
		m_fAttackTimer = 0.0f;
	}

}

void CHyperSnowballBehavior::Exit()
{

}

