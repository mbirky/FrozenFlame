/********************************************************************
* Filename:			CWorldManager.h
* Date:				11/7/2012
* Mod. Date:		11/28/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the cpp of the world manager
********************************************************************/

#include "CWorldManager.h"
#include "../Renderer/View.h"
#include "CLevel.h"
#include "../States/CFadeState.h"

CWorldManager::CWorldManager(void)
{
	CWorldManager::m_nCurrentLevel = 0;
	m_nCurrLoadLevel = -1;
}

void CWorldManager::Init(void)
{
	// Initialize object manager
	m_cObjectManager.Initialize(m_nNumMelee, m_nNumRanged, m_nNumExplosive, m_nNumIceBeast, 
		m_nNumFireBeast, m_nNumCollidableObjects, m_nNumEnvironmentObjects, m_nNumProjectiles, m_nNumFireTraps, m_nNumIceTraps);

	/////////////////////////////////////////////////////////
	/////////////////// PLAYER STUFF ////////////////////////
	/////////////////////////////////////////////////////////
	// This code didn't make any sense so I commented it out - Dan
	// matrix4f worldMatrix = *(m_cObjectManager.GetPlayer()->GetWorldMatrix());
	// worldMatrix.axis_pos = vec3f(-2.5f, 0.0f, 110.0f);	// Not sure why those weird values
	// m_cObjectManager.GetPlayer()->SetWorldMatrix(worldMatrix);

	// Set the player for the behavior manager and the camera
	m_cBehaviorManager.SetPlayer(m_cObjectManager.GetPlayer());
	CView::GetCamera().SetPlayer(m_cObjectManager.GetPlayer());
	/////////////////////////////////////////////////////////

	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);

	// Set up lua functions
	luaL_dofile(CWorldManager::GetInstance()->GetluaState(), "Resource/Scripts/Commands.lua");

	RegisterFunctions(m_pLuaState);

	m_nCurrSection = 0;
	m_bSectionComplete = false;
	m_bResetFade = false;
}

void CWorldManager::Reset(bool _bTotal)
{
	// Load in Enemy scripts
	luaL_dofile(CWorldManager::GetInstance()->GetluaState(), "Resource/Scripts/Enemies/MeleeMinion.lua");
	luaL_dofile(m_pLuaState, "Resource/Scripts/Enemies/ExplosiveMinion.lua");
	luaL_dofile(m_pLuaState, "Resource/Scripts/Enemies/FireBeast.lua");
	luaL_dofile(m_pLuaState, "Resource/Scripts/Enemies/IceBeast.lua");

	m_nCurrentLevel = 0;
	m_bSectionComplete = false;

	if(_bTotal)
	{
		m_nCurrSection = 0;
		m_bBossLoaded = false;
	}
	
	// update the loading bar
	//CGame::GetInstance()->DrawLoading(85);

	// Function Auto sets the game victory bool to true if not able to load the section
	LoadCurrentSection();

	CView::SetCameraToPan();	
	CView::GetCamera().TurnOffShake();	// turn off any camera shaking

	m_cObjectManager.GetPlayer()->Reinitialize();
	m_cObjectManager.GetPlayer()->SetPos(vec3f(0.0f, 0.5f, m_vLevels[m_nCurrentLevel].GetPosZ()-10.0f), false);
}

void CWorldManager::Shutdown(void)
{
	lua_close(m_pLuaState);

	m_cObjectManager.Shutdown();
	//m_cBehaviorManager.Shutdown();
}

CWorldManager::~CWorldManager(void)
{

}

void CWorldManager::Update(float _fElapsedTime)
{
	m_cObjectManager.Update(_fElapsedTime);

	m_cBehaviorManager.SetPlayer(m_cObjectManager.GetPlayer());
	m_cBehaviorManager.Update(_fElapsedTime);

	for(size_t i = 0; i < m_vLevels.size(); ++i)
	{
		m_vLevels[i].Update(_fElapsedTime);
	}

	//vec3f& vPlayerPos = m_cObjectManager.GetPlayer()->GetMatrix()->axis_pos;

	BoundsKeeping(m_cObjectManager.GetPlayer());

	unsigned int nLevel = GetLevelForObject(m_cObjectManager.GetPlayer());

	CEnemy* pGuardian = m_cObjectManager.GetCrystalGuardian();
	if(pGuardian)
	{
		CFSM* pGuardianFSM = pGuardian->GetFSM();
		if(pGuardianFSM)
		{
			if(nLevel <= m_vLevels.size() - 2 && pGuardianFSM->GetIsChasing())
			{
				pGuardianFSM->TransitionTo(WANDER_BEHAVIOR);
				pGuardianFSM->GetIsChasing() = false;
				m_vLevels[m_vLevels.size()-1].ResetActive();
			}
		}
	}

	if(m_cObjectManager.GetWizard())
	{
		BoundsKeeping(m_cObjectManager.GetWizard());
	}
}

bool CWorldManager::Lost(void)
{
	CPlayerObject* pPlayer = m_cObjectManager.GetPlayer();
	if(pPlayer && pPlayer->GetHealth() <= 0)
	{
		CAnimPlayer* pAnimPlayer = pPlayer->GetAnimationPlayer();
		if(pAnimPlayer && pAnimPlayer->GetLastPlayedAnimType() == ANIM_DEATH)
		{
			//pAnimPlayer->SetAnimationSpeed(0.0f);
			//pAnimPlayer->

			//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank2(),"PS_LevelBoss");
			m_bBossLoaded = false;
			//m_cObjectManager.GetPlayer()->Uninitialize();
			m_cObjectManager.SetWizardActive(false);
			//m_cObjectManager.GetWizard()->Uninitialize();
			return true;
		}
	}
	return false;
}

float CWorldManager::GetHealthScale(void)
{
	return m_cObjectManager.GetPlayer()->GetHealth() / m_cObjectManager.GetPlayer()->GetMaxHealth();
}

float CWorldManager::GetRageScale(void)
{
	return static_cast<float>(m_cObjectManager.GetPlayer()->GetRage()) / m_cObjectManager.GetPlayer()->GetMaxPlayerRage();
}

CBaseObject* CWorldManager::MakeObject(vec3f _vPos, int _nObjType, int _nSubType, const char* szModelName)
{
	CBaseObject *tmp = CWorldManager::GetInstance()->GetObjectManager()->NewObject(_nObjType, _nSubType, szModelName);
	if(tmp)
	{
		matrix4f worldMatrix = *(tmp->GetWorldMatrix());
		worldMatrix.axis_pos = _vPos;
		tmp->SetWorldMatrix(worldMatrix);
//		tmp->GetMatrix()->axis_pos = _vPos;

		// if object is an enemy
		if(_nObjType >= MELEE_OBJECT && _nObjType <= WIZARD_OBJECT)
		{
			//CWorldManager::GetInstance()->GetBehaviorManager()->AddFSM(static_cast<CEnemy*>(tmp)->GetFSM());
			CWorldManager::GetInstance()->GetBehaviorManager()->AddEnemy(static_cast<CEnemy*>(tmp));
		}
	}
	return tmp;
}

float CWorldManager::GetGroundHeight(void)
{
	if(m_vLevels.size() >0)
		return m_vLevels[0].GetPosY();
	return 0.0f;
}

bool CWorldManager::LoadCurrentSection(void)
{
	// remove all the positional objects currently held in the sound manager (won't delete player)
	CSoundManager::GetInstance()->RemoveAllPositionalObjects();

	for(size_t i = 0; i < m_vLevels.size(); ++i)
	{
		m_vLevels[i].UnloadLevel();
	}
	m_vLevels.clear();


	// update the loading bar
	if(CGame::GetInstance()->GetSplashing() == false)
	{
		CGame::GetInstance()->DrawLoading(90);
	}

	// updates the object manager to let it get rid of all uninitilized objects
	m_cBehaviorManager.ClearEnemyList();
	m_cObjectManager.ClearPickups();
	m_cObjectManager.ClearEnvironmental();
	m_cObjectManager.ClearFireTraps();
	m_cObjectManager.ClearIceTraps();
	m_cObjectManager.ClearSnowballs();
	//m_cObjectManager.Update(0.0);

	m_nCurrentLevel = 0;
	m_nCurrLoadLevel = 0;
	while(true)
	{
		CLevel cNewLevel;
		m_vLevels.push_back(cNewLevel);

		if(!LoadLevel())
		{
			m_vLevels.pop_back();
			break;
		}
		++m_nCurrLoadLevel;
	}
	if(0 == m_nCurrLoadLevel)
	{
		CGame::GetInstance()->SetVictory(true);
		return false;
	}

	// update the loading bar
	if(CGame::GetInstance()->GetSplashing() == false)
	{
		CGame::GetInstance()->DrawLoading(95);
	}

	m_cObjectManager.GetPlayer()->ResetRotation();

	matrix4f worldMatrix = *(m_cObjectManager.GetPlayer()->GetWorldMatrix());
	worldMatrix.axis_pos = vec3f(0.0f, 0.5f, m_vLevels[CWorldManager::m_nCurrentLevel].GetPosZ()-10);
	m_cObjectManager.GetPlayer()->SetWorldMatrix(worldMatrix);

	CalculateWallAABBs();

	return true;
}

bool CWorldManager::LoadLevel(void)
{
	char buffer[128];

	sprintf_s(buffer, "Resource/Scripts/Levels/Section%i/Level%i.lua", m_nCurrSection, m_nCurrLoadLevel);
	if(!luaL_dofile(m_pLuaState, buffer))
	{
		//m_vLevels[m_nCurrLoadLevel].SetActive(true);
		m_vLevels[m_nCurrLoadLevel].SetLevelID(m_nCurrLoadLevel);

		lua_getglobal(m_pLuaState, "GroundType");
		int nGroundType = static_cast<int>(lua_tointeger(m_pLuaState, -1));
		switch(nGroundType)
		{
		case 0:
			m_vLevels[m_nCurrLoadLevel].MakeGround();
			break;
		case 1:
			m_vLevels[m_nCurrLoadLevel].MakeBridge();
			break;
		case 2:
			m_vLevels[m_nCurrLoadLevel].MakeCliff();
			break;
		default:
			printf("No valid ground type!\n");
			break;
		}

		lua_getglobal(m_pLuaState, "IsWall");
		int nIsWall = static_cast<int>(lua_tointeger(m_pLuaState, -1));
		if(nIsWall != 0)
		{
			CWorldManager::m_vLevels[CWorldManager::m_nCurrLoadLevel].PushEnvironment(vec3f(0.0f,1.0f, CWorldManager::m_vLevels[m_nCurrLoadLevel].GetPosZ()),WALL_OBJECT);
		}

		lua_getglobal(m_pLuaState,"IsEnd");
		int nIsEnd = static_cast<int>(lua_tointeger(m_pLuaState, -1));
		if(nIsEnd != 0)
		{
			float z = CWorldManager::m_vLevels[m_nCurrLoadLevel].GetPosZ();
			CWorldManager::m_vLevels[CWorldManager::m_nCurrLoadLevel].PushEnvironment(vec3f(0.0f,1.0f,z - 30.0f),PORTAL_OBJECT);
		}

		lua_pop(m_pLuaState, 3);

		lua_getglobal(m_pLuaState, "LLoadLevel");

		lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0);

		return true;
	}
	lua_pop(m_pLuaState, 1);

	if(m_nCurrLoadLevel == 0 && !m_bBossLoaded)
	{
		m_bBossLoaded = true;
		if(!luaL_dofile(m_pLuaState, "Resource/Scripts/Levels/BossLevel.lua"))
		{
			//m_vLevels[m_nCurrLoadLevel].SetActive(true);
			m_vLevels[m_nCurrLoadLevel].SetLevelID(m_nCurrLoadLevel);
			m_vLevels[m_nCurrLoadLevel].MakeWizardGround();
			//this->m_cObjectManager.GetWizard() = new CWizardEnemy();
			this->m_cObjectManager.GetWizard()->Uninitialize();
			this->m_cObjectManager.GetWizard()->SetWizardSubtype(FINAL_BOSS);
			this->m_cObjectManager.GetWizard()->Reinitialize();
			this->m_cObjectManager.GetWizard()->GetFSM()->GetIsChasing() = true;
			this->m_cObjectManager.SetWizardActive(true);
			this->m_cObjectManager.GetWizard()->SetPos(vec3f(0.0f, 0.0f, CWorldManager::m_vLevels[m_nCurrLoadLevel].GetCenterZ() - 5.0f));

			m_vLevels[m_nCurrentLevel].PushEnemyIntoActiveVector(m_cObjectManager.GetWizard());
			m_cObjectManager.GetWizard()->GetFSM()->SetObjectManager(&m_cObjectManager);
			m_cBehaviorManager.AddEnemy(m_cObjectManager.GetWizard());
			CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(), "MX_MainMenu");
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(), "PS_LevelBoss");
			
			lua_getglobal(m_pLuaState, "LLoadLevel");			
			lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0);

			m_vLevels[m_nCurrLoadLevel].ActivateSpawners();
			// START FUN
			// Need to turn on the spawners special effect. Use the ReactivateCrystal function.

			vector<CEnvironmentObject*> pObjects = *(m_vLevels[m_nCurrLoadLevel].GetEnvironmentObjects());

			for(unsigned int i = 0; i < pObjects.size();i++)
			{
				if(pObjects[i]->GetType() == SPAWNER_OBJECT)
				{
					//((CSpawner*)pObjects[i])->m_bWizardSpawner = true;
					m_cObjectManager.GetWizard()->PushBackWizardSpawner((CSpawner*)pObjects[i]);
				}
			}

			m_cObjectManager.GetWizard()->ReactivateCrystals();

			return true;
		}
	}
	else if(m_nCurrLoadLevel != 0 && !m_bBossLoaded)
	{
		m_vLevels[m_nCurrLoadLevel - 1].MakeEndGround();
	}

	return false;
}

void CWorldManager::ActivateObjectLevel(CBaseObject* pObj)
{
	int nLevel = GetLevelForObject(pObj);

	if(nLevel != -1)
	{
		m_vLevels[nLevel].ActivateEnemies();
	}
}

int CWorldManager::GetLevelForObject(CBaseObject* pObj)
{
	const vec3f& vPos = pObj->GetWorldMatrix()->axis_pos;

	int numLevels = (int)m_vLevels.size();
	int nResult = -1;
	for(int i = 0; i < numLevels; ++i)
	{
		const CLevel& curLevel = m_vLevels[i];

		float fMaxZ = curLevel.GetPosZ();
		float fMinZ = fMaxZ + curLevel.GetExtentZ();
		float fMaxX = curLevel.GetCenterX() + curLevel.GetExtentX() * 0.5f;
		float fMinX = fMaxX - curLevel.GetExtentX();

		if(curLevel.IsInside(pObj))
		{
			nResult = i;
			break;
		}
	}
	return nResult;
}

void CWorldManager::BoundsKeeping(CBaseObject* pObj)
{
	int nLevel = GetLevelForObject(pObj);

	if(pObj->GetType() == PLAYER_OBJECT)
	{
		if(nLevel != -1)
		{
			if(nLevel > m_nCurrentLevel)
				m_vLevels[nLevel].ActivateEnemies();

			m_nCurrentLevel = nLevel;
		}
		else if(nLevel != m_nCurrentLevel)
			m_vLevels[m_nCurrentLevel].KeepInside(pObj);
	}
	else if(nLevel == -1)
	{
		int nClosestLevel = 0;
		float fLowestDist = FLT_MAX;
		int numLevels = (int)m_vLevels.size();
		const vec3f& vPos = pObj->GetWorldMatrix()->axis_pos;
		for(int i = 0; i < numLevels; ++i)
		{
			const CLevel& curLevel = m_vLevels[i];
			float dz = vPos.z - curLevel.GetCenterZ();
			float dx = vPos.x - curLevel.GetCenterX();
			float fDist = dz * dz + dx * dx; 
			if(fDist < fLowestDist || !i)
			{
				fLowestDist = fDist;
				nClosestLevel = i;
			}
		}
		m_vLevels[nClosestLevel].KeepInside(pObj);
	}
}

void CWorldManager::CheckStack(lua_State *L)
{
	int size = lua_gettop(L);

	if (0 == size)
	{
		//printf_s("\n%s\n", "Nothing is on the stack");
		return;
	}

	//printf_s("\n%s\n", "Checking what is on the stack\n");

	/*for (int i = size * -1; i < 0; ++i)
	{
		printf_s("Stack index: %d ", i);
		switch(lua_type (L, i) )
		{
		case LUA_TNIL:
			printf_s("Nil\n");
			break;
		case LUA_TNUMBER:
			printf_s("Number = %f\n", lua_tonumber(L, i) );
			break;
		case LUA_TBOOLEAN:
			printf_s("Boolean = %d\n", lua_toboolean(L, 1) );
			break;
		case  LUA_TSTRING:
			printf_s("String = %s\n", lua_tostring(L , i) );
			break;
		case LUA_TTABLE:
			printf_s("Table = %#p\n", lua_topointer(L, i) );
			break;
		case LUA_TFUNCTION:
			printf_s("Function =  %#p\n", lua_topointer(L, i) );
			break;
		case LUA_TUSERDATA:
			printf_s("User Data =  %#p\n", lua_topointer(L, i) );
			break;
		case LUA_TTHREAD:
			printf_s("Thread\n");
			break;
		case LUA_TLIGHTUSERDATA:
			printf_s("Light User Data =  %#p\n", lua_topointer(L, i) );
			break;
		default:
			printf_s("ERROR: Unknown type\n");
		};
	}
	printf_s("\n");*/
}

void CWorldManager::CalculateWallAABBs(void)
{
	m_vWallAABBs.clear();

	size_t numLevels = m_vLevels.size();
	for(size_t i = 0; i < numLevels; ++i)
	{
		AABB box;

		CLevel& curLevel = m_vLevels[i];
		float minX = curLevel.GetCenterX() - curLevel.GetExtentX() * 0.5f;
		float maxX = curLevel.GetCenterX() + curLevel.GetExtentX() * 0.5f;

		float minZ = curLevel.GetPosZ();
		float maxZ = minZ + 1.0f;

		float minY = curLevel.GetPosY();
		float maxY = minY + 10.0f;

		box.min = vec3f(minX, minY, minZ);
		box.max = vec3f(maxX, maxY, maxZ);

		m_vWallAABBs.push_back(box);
	}
}

int CWorldManager::PopCurrentLevelEnemies(float fCooldown)
{
	if(m_nCurrentLevel >= 0 && m_nCurrentLevel < (int)m_vLevels.size())
	{
		CLevel& curLevel = m_vLevels[m_nCurrentLevel];
		return curLevel.PopEnemies(fCooldown);
	}
	return -1;
}