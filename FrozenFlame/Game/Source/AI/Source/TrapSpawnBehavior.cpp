#include "../Header/TrapSpawnBehavior.h"
#include "../Header/CFSM.h"
#include "../../Objects/CFireTrap.h"
#include "../../Objects/CIceTrap.h"
#include "../../Core/CWorldManager.h"

CTrapSpawnBehavior::CTrapSpawnBehavior()
{
	m_fSpawnTimer = 0.0f;
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:	{
			m_fSpawnCooldown = 8.0f;
		}
		break;

	case DIFF_MEDIUM: {
			m_fSpawnCooldown = 6.0f;	// I'm actually toning this down slightly from the 5 seconds we've had it at -SMS
		}
		break;

	case DIFF_HARD: {
			m_fSpawnCooldown = 5.0f;
		}
		break;
	}
}

void CTrapSpawnBehavior::Enter()
{
	m_fSpawnTimer = 0.0f;
}

void CTrapSpawnBehavior::Update(CPlayerObject * _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	vec3f targetVec = _pcPlayer->GetPos();
	//targetVec.y = GetOwner()->GetAgent()->GetPos().y;
	targetVec.y = 10.0f;

	targetVec.x += 8.0f;
	targetVec.z -= 8.0f;

	this->SoftAttach(targetVec,_fElapsedTime);

	m_fSpawnTimer += _fElapsedTime;

	if(m_fSpawnTimer >= m_fSpawnCooldown)
	{
		m_fSpawnTimer = 0.0f;

		CFireTrap* pcFire = nullptr;
		CIceTrap* pcIce = nullptr;

		int nTrapType = rand() % 2;

		vec4f vColor;
		int nLightID = -1;

		if(nTrapType)
		{
			vColor = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			vColor = vec4f(0.0f, 0.0f, 1.0f, 1.0f);
		}

		vec3f vPlayerPos = _pcPlayer->GetPos();

		CWorldManager * pWorldMan = CWorldManager::GetInstance();
		CObjectManager * pObjectMan = pWorldMan->GetObjectManager();
		int nLevel = 0;//pWorldMan->GetLevelForObject(this->GetOwner()->GetAgent());
		//CLevel * pLevel = pWorldMan->GetLevel(nLevel);

		float extentX = pWorldMan->GetLevel(nLevel)->GetExtentX() * 0.9f;
		float centerX = pWorldMan->GetLevel(nLevel)->GetCenterX();
		
		float extentZ = pWorldMan->GetLevel(nLevel)->GetExtentZ() * 0.9f;
		float centerZ = pWorldMan->GetLevel(nLevel)->GetCenterZ();

		int nHighX = (int)(centerX + (int)(extentX*0.5f));
		int nLowX = (int)(nHighX - extentX);

		int nHighZ = (int)(centerZ - (int)(extentZ*0.5f));
		int nLowZ = (int)(nHighZ + extentZ);
		
		if(vPlayerPos.x > nHighX)
		{
			vPlayerPos.x = (float)nHighX;
		}
		if(vPlayerPos.z > nHighZ)
		{
			vPlayerPos.z =  (float)nHighZ;
		}
		if(vPlayerPos.x < nLowX)
		{
			vPlayerPos.x =  (float)nLowX;
		}
		if(vPlayerPos.z < nLowZ)
		{
			vPlayerPos.z =  (float)nLowZ;
		}
		// New light system - Dan
		// nLightID = CView::CreateWorldLight(vPlayerPos,vColor);
		nLightID = CView::MakeLight(vPlayerPos, true, vColor);

		vPlayerPos.y -= 3.0f;

		CObjectManager * pcObjectMan = this->GetOwner()->GetObjectManager();

		if(nTrapType)
		{
			pcFire = (CFireTrap*)pcObjectMan->NewObject(FIRETRAP_OBJECT);
			pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(-2.0f,0,-2.0f), nLightID);
			pcFire->RegisterWithSoundManager();
			pcFire->SetSoundPos(pcFire->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_FIRE, GAME_OBJECT_ID_PLAYER);// pcFire->GetSoundID());

			pcFire = (CFireTrap*)pcObjectMan->NewObject(FIRETRAP_OBJECT);
			pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(2.0f,0,-2.0f), nLightID);
			pcFire->RegisterWithSoundManager();
			pcFire->SetSoundPos(pcFire->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_FIRE, GAME_OBJECT_ID_PLAYER);//pcFire->GetSoundID());

			pcFire = (CFireTrap*)pcObjectMan->NewObject(FIRETRAP_OBJECT);
			pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(2.0f,0,2.0f), nLightID);
			pcFire->RegisterWithSoundManager();
			pcFire->SetSoundPos(pcFire->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_FIRE, GAME_OBJECT_ID_PLAYER);//pcFire->GetSoundID());

			pcFire = (CFireTrap*)pcObjectMan->NewObject(FIRETRAP_OBJECT);
			pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(-2.0f,0,2.0f), nLightID);

			pcFire->RegisterWithSoundManager();
			pcFire->SetSoundPos(pcFire->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_FIRE, GAME_OBJECT_ID_PLAYER);//pcFire->GetSoundID());
		}

		else// if(pcIce)
		{
			pcIce = (CIceTrap*)pcObjectMan->NewObject(ICETRAP_OBJECT);
			pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(-1,0,-1), nLightID);
			pcIce->RegisterWithSoundManager();
			pcIce->SetSoundPos(pcIce->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_ICE, GAME_OBJECT_ID_PLAYER);//pcIce->GetSoundID());

			pcIce = (CIceTrap*)pcObjectMan->NewObject(ICETRAP_OBJECT);
			pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(1,0,-1), nLightID);
			pcIce->RegisterWithSoundManager();
			pcIce->SetSoundPos(pcIce->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_ICE, GAME_OBJECT_ID_PLAYER);//pcIce->GetSoundID());

			pcIce = (CIceTrap*)pcObjectMan->NewObject(ICETRAP_OBJECT);
			pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(1,0,1), nLightID);
			pcIce->RegisterWithSoundManager();
			pcIce->SetSoundPos(pcIce->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_ICE, GAME_OBJECT_ID_PLAYER);//pcIce->GetSoundID());

			pcIce = (CIceTrap*)pcObjectMan->NewObject(ICETRAP_OBJECT);
			pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(-1,0,1), nLightID);

			pcIce->RegisterWithSoundManager();
			pcIce->SetSoundPos(pcIce->GetPos());
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WIZARD_SUMMONTRAP_ICE, GAME_OBJECT_ID_PLAYER);//pcIce->GetSoundID());
		}

	}

}

void CTrapSpawnBehavior::Exit()
{

}
