/********************************************************************
* Filename:			CTimer.cpp
* Date:				10/24/2012
* Mod. Date:		11/12/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This class is a timer that allows us to keep
					track fo time for our application
********************************************************************/

#include "CluaFunctions.h"
#include "..\Renderer\CEmitterManager.h"


void RegisterFunctions(lua_State *_pluaState)
{
	lua_register(_pluaState, "MakeMeleeMinion", MakeMeleeMinion);
	lua_register(_pluaState, "MakeRangedMinion", MakeRangedMinion);
	lua_register(_pluaState, "MakeExplosiveMinion", MakeExplosiveMinion);
	lua_register(_pluaState, "MakeIceBeast", MakeIceBeast);
	lua_register(_pluaState, "MakeFireBeast", MakeFireBeast);
	lua_register(_pluaState, "MakeIceTrap", MakeIceTrap);
	lua_register(_pluaState, "MakeFireTrap", MakeFireTrap);
	lua_register(_pluaState, "MakeEnviromentObject", MakeEnviromentObject);
	lua_register(_pluaState, "MakeSceneryObject", MakeSceneryObject);
	lua_register(_pluaState, "MakeSpawner", MakeSpawner);
	lua_register(_pluaState, "NextSection", NextSection);
	lua_register(_pluaState, "PrevSection", PrevSection);
	lua_register(_pluaState, "ReloadSection", ReloadSection);
	lua_register(_pluaState, "ReloadEnemies", ReloadEnemies);
	lua_register(_pluaState, "MakeWizard", MakeWizard);
	lua_register(_pluaState, "ForceBreak", ForceBreak);
	lua_register(_pluaState, "EverlastingRage", EverlastingRage);
	lua_register(_pluaState, "SetWizardHP", SetWizardHP);
	lua_register(_pluaState, "GoToLevel", GoToLevel);
	lua_register(_pluaState, "Render", Render);
}

int NextSection(lua_State *_pluaState)
{
	CWorldManager::GetInstance()->SetCurrentSection(CWorldManager::GetInstance()->GetCurrentSection() + 1);
	CWorldManager::GetInstance()->LoadCurrentSection();

	return 1;
}

int PrevSection(lua_State *_pluaState)
{
	CWorldManager::GetInstance()->SetCurrentSection(CWorldManager::GetInstance()->GetCurrentSection() - 1);
	CWorldManager::GetInstance()->LoadCurrentSection();

	return 1;
}

int GoToLevel(lua_State *_pluaState)
{
	int nStack = lua_gettop(_pluaState);

	if(nStack < 1)
	{
		LOG("Leaving GotToLevel function! Not enough parameters");
		return 0;
	}

	int nLevel = lua_tointeger(_pluaState, -1);
		
	CWorldManager::GetInstance()->SetCurrentSection(nLevel);
	CWorldManager::GetInstance()->LoadCurrentSection();

	return 1;
}

int ReloadSection(lua_State *_pluaState)
{
	CWorldManager::GetInstance()->LoadCurrentSection();

	return 1;
}

int ReloadEnemies(lua_State *_pluaState)
{
	luaL_dofile(CWorldManager::GetInstance()->GetluaState(), "Resource/Scripts/Enemies/MeleeMinion.lua");
	luaL_dofile(_pluaState, "Resource/Scripts/Enemies/ExplosiveMinion.lua");
	luaL_dofile(_pluaState, "Resource/Scripts/Enemies/FireBeast.lua");
	luaL_dofile(_pluaState, "Resource/Scripts/Enemies/IceBeast.lua");

	return 1;
}

void LoadVec3(lua_State *_pLuaState, vec3f &_vPos)
{
	lua_pushnumber(_pLuaState, 1);
	lua_gettable(_pLuaState, -2);
	_vPos.x = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 2);
	lua_gettable(_pLuaState, -3);
	_vPos.y = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 3);
	lua_gettable(_pLuaState, -4);
	_vPos.z = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pop(_pLuaState, 3);
}

void LoadVec4(lua_State *_pLuaState, vec3f &_vPos, float &_fExtraData)
{
	lua_pushnumber(_pLuaState, 1);
	lua_gettable(_pLuaState, -2);
	_vPos.x = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 2);
	lua_gettable(_pLuaState, -3);
	_vPos.y = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 3);
	lua_gettable(_pLuaState, -4);
	_vPos.z = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 4);
	lua_gettable(_pLuaState, -5);
	_fExtraData = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pop(_pLuaState, 4);
}

void LoadVec(lua_State *_pLuaState, vector<int> &_vData)
{
	for(int i = 1; ; ++i)
	{
		lua_pushnumber(_pLuaState, i);
		lua_gettable(_pLuaState, -i - 1);

		if(lua_isnil(_pLuaState, -1))
		{
			lua_pop(_pLuaState, i);
			break;
		}

		_vData.push_back(static_cast<int>(lua_tonumber(_pLuaState, -1)));
	}
}

int MakeMeleeMinion(lua_State *_pLuaState)
{
	// Variables needed
	vec3f vPos;

	// Load in from lua setting needed variables
	lua_pushstring(_pLuaState, "Pos");
	lua_gettable(_pLuaState, -2);
	LoadVec3(_pLuaState, vPos);

	// Pop everything on the lua stack
	lua_pop(_pLuaState, lua_gettop(_pLuaState));

	//printf("Melee Minion Pos = %f, %f, %f\n", vPos.x, vPos.y, vPos.z);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnemy(vPos, MELEE_OBJECT);

	return 1;
}

int MakeRangedMinion(lua_State *_pLuaState)
{
	vec3f vPos;
	float fAttackElement;

	LoadVec4(_pLuaState, vPos, fAttackElement);

	//printf("Ranged Minion Pos = %f, %f, %f\n", vPos.x, vPos.y, vPos.z);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnemy(vPos, RANGED_OBJECT, static_cast<int>(fAttackElement));

	return 1;
}

int MakeExplosiveMinion(lua_State *_pLuaState)
{
	vec3f vPos;
	float fAttackElement;

	LoadVec4(_pLuaState, vPos, fAttackElement);

	//printf("Explosive Minion Pos = %f, %f, %f\n", vPos.x, vPos.y, vPos.z);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnemy(vPos, EXPLOSIVE_OBJECT, static_cast<int>(fAttackElement));

	return 1;
}

int MakeIceBeast(lua_State *_pLuaState)
{
	vec3f vPos;

	LoadVec3(_pLuaState, vPos);

	//printf("Ice Beast Pos = %f, %f, %f\n", vPos.x, vPos.y, vPos.z);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnemy(vPos, ICE_OBJECT);

	return 1;
}

int MakeFireBeast(lua_State *_pLuaState)
{
	vec3f vPos;

	LoadVec3(_pLuaState, vPos);

	//printf("Fire Beast Pos = %f, %f, %f\n", vPos.x, vPos.y, vPos.z);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnemy(vPos, FIRE_OBJECT);

	return 1;
}

int MakeIceTrap(lua_State *_pLuaState)
{
	vec3f vPos;

	LoadVec3(_pLuaState, vPos);

	//printf("Ice Trap Pos = %f, %f, %f\n", vPos.x, vPos.y, vPos.z);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnvironment(vPos, ICETRAP_OBJECT);

	return 1;
}

int MakeFireTrap(lua_State *_pLuaState)
{
	vec3f vPos;
	float fTimeOffset = 0.0;

	LoadVec4(_pLuaState, vPos, fTimeOffset);

	if(fTimeOffset < 0)
	{
		fTimeOffset = (rand() % 600) / 100.0f;
	}

	//printf("Fire Trap Pos = %f, %f, %f, Time Offset = %f\n", vPos.x, vPos.y, vPos.z, fTimeOffset);
	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnvironment(vPos, FIRETRAP_OBJECT, fTimeOffset);

	return 1;
}

int MakeSpawner(lua_State *_pLuaState)
{
	// Variables needed for Spawner
	vec3f vPos;
	float fRespawnDelay		= -1.0f;
	bool bRunOnce			= false;
	bool bIsGuarded			= false;
	int nReinforceMinimum	= -1;
	int nExplosiveChance	= 0;
	vector<int> vSpawnSet;

	// Load in lua variables
	lua_pushstring(_pLuaState, "Pos");
	lua_gettable(_pLuaState, -2);
	LoadVec3(_pLuaState, vPos);

	lua_pushstring(_pLuaState, "RespawnDelay");
	lua_gettable(_pLuaState, -3);
	fRespawnDelay = static_cast<float>(lua_tointeger(_pLuaState, -1));

	lua_pushstring(_pLuaState, "RunOnce");
	lua_gettable(_pLuaState, -4);
	bRunOnce = (lua_toboolean(_pLuaState, -1) ? true : false);

	lua_pushstring(_pLuaState, "IsGuarded");
	lua_gettable(_pLuaState, -5);
	bIsGuarded = (lua_toboolean(_pLuaState, -1) ? true : false);

	lua_pushstring(_pLuaState, "ReinforceMinimum");
	lua_gettable(_pLuaState, -6);
	nReinforceMinimum = static_cast<int>(lua_tointeger(_pLuaState, -1));

	lua_pushstring(_pLuaState, "ExplosiveChance");
	lua_gettable(_pLuaState, -7);
	nExplosiveChance = static_cast<int>(lua_tointeger(_pLuaState, -1));

	// Load in the spawn set according to the current difficulty
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		lua_pushstring(_pLuaState, "Easy");
		break;
	case DIFF_MEDIUM:
		lua_pushstring(_pLuaState, "Medium");
		break;
	case DIFF_HARD:
		lua_pushstring(_pLuaState, "Hard");
		break;
	}
	lua_gettable(_pLuaState, -8);
	LoadVec(_pLuaState, vSpawnSet);

	// Pop everything off of the stack
	lua_pop(_pLuaState, lua_gettop(_pLuaState));
	
	// Create The Spawner with proper values
	CLevel* pLevel = CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel());
	CSpawner* pSpawner = (CSpawner*)(pLevel->PushEnvironment(vPos, SPAWNER_OBJECT));

	pSpawner->m_vSpawnList = vSpawnSet;
	pSpawner->m_CurrentSpawn = pSpawner->m_vSpawnList.begin();
	pSpawner->m_pLevelOwner = pLevel;

	pSpawner->SetMaxHealth(25);
	pSpawner->SetHealth(25);

	pSpawner->m_nMinimunReinforce = nReinforceMinimum;
	pSpawner->m_bSpawnOnce = bRunOnce;
	pSpawner->m_fSpawnDelay = fRespawnDelay;
	pSpawner->m_fCurrentDelay = fRespawnDelay;
	pSpawner->m_nExplosiveChance = nExplosiveChance;

	// All guarded spawners need ENARGEE TRAYLZ. - JC
	pSpawner->SetEnergyTrails(bIsGuarded);

	// Make them invisible to start, for epicosity! - BRG
	CView::SetIsVisible( pSpawner->GetRenderID(), false );
	
	if(CWorldManager::GetInstance()->GetBoss()){
		pSpawner->m_bWizardSpawner = true;

		pSpawner->SetMaxHealth(45);
		pSpawner->SetHealth(45);

		pSpawner->SetElementalState(((rand() % 2)*2)-1);
		if(pSpawner->GetElementalState() == FIRE){
			pSpawner->SetColor(vec3f(1.0f, 0.0f, 0.0f));
		}
		else{
			pSpawner->SetColor(vec3f(0.0f, 0.0f, 0.5f));
		}
	
		pSpawner->m_pcIceEmitter =  CView::GetEmitterManager().AddEmitter( true, false, pSpawner->GetPos(), "Resource/Particles/CrystalIce.txt");
		pSpawner->m_pcFireEmitter =  CView::GetEmitterManager().AddEmitter( true, false, pSpawner->GetPos(), "Resource/Particles/CrystalFire.txt");
	}
	else {
		pSpawner->m_cEmit = CView::GetEmitterManager().AddEmitter( false, false, pSpawner->GetPos(), "Resource/Particles/SpawnerBlocks.txt");
	}
	if( bIsGuarded )
	{
		for(auto iterEnemy = pLevel->GetActiveEnemies()->begin(); iterEnemy != pLevel->GetActiveEnemies()->end(); ++iterEnemy)
		{
			if((*iterEnemy)->GetType() == ICE_OBJECT || (*iterEnemy)->GetType() == FIRE_OBJECT || (*iterEnemy)->GetType() == WIZARD_OBJECT)
			{
				pSpawner->m_pGaurd = (*iterEnemy);
				pSpawner->m_pcWizardBeamEmitter = CView::GetEmitterManager().AddEmitter( true, false, vec3f(0.0f,0.0f,0.0f), "Resource/Particles/CrystalWizardBeam.txt",
					false, pSpawner, pSpawner->m_pGaurd, false, true );
				pSpawner->m_pcWizardBeamEmitter->SetOffset(vec3f(0.0f,2.0f,0.0f));
				pSpawner->AddBubble();
				pSpawner->m_bIsDestructable = false;
			}
		}
	}


	return 1;
}

int MakeEnviromentObject(lua_State *_pLuaState)
{
	int nType;
	vec3f vPos;
	string szType;
	string szText;

	lua_pushnumber(_pLuaState, 1);
	lua_gettable(_pLuaState, -2);
	vPos.x = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 2);
	lua_gettable(_pLuaState, -3);
	vPos.y = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 3);
	lua_gettable(_pLuaState, -4);
	vPos.z = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushnumber(_pLuaState, 4);
	lua_gettable(_pLuaState, -5);
	szType = static_cast<string>(lua_tostring(_pLuaState, -1));

	if(!strcmp(szType.c_str(), "Rock1"))
	{
		nType = CEnvironmentObject::ENVOBJ_ROCK1;
	}
	else if(!strcmp(szType.c_str(), "Rock2"))
	{
		nType = CEnvironmentObject::ENVOBJ_ROCK2;
	}
	else if(!strcmp(szType.c_str(), "Rock3"))
	{
		nType = CEnvironmentObject::ENVOBJ_ROCK3;
	}
	else if(!strcmp(szType.c_str(), "DeadTree1"))
	{
		nType = CEnvironmentObject::ENVOBJ_DEADTREE1;
	}
	else if(!strcmp(szType.c_str(), "DeadTree2"))
	{
		nType = CEnvironmentObject::ENVOBJ_DEADTREE2;
	}
	else if(!strcmp(szType.c_str(), "LivingTree"))
	{
		nType = CEnvironmentObject::ENVOBJ_LIVINGTREE;
	}
	else if(!strcmp(szType.c_str(), "PineTree"))
	{
		nType = CEnvironmentObject::ENVOBJ_PINETREE;
	}
	else if(!strcmp(szType.c_str(), "Shrub"))
	{
		nType = CEnvironmentObject::ENVOBJ_SHRUB;
	}
	else if(!strcmp(szType.c_str(), "Sign"))
	{
		nType = CEnvironmentObject::ENVOBJ_SIGN;
		lua_pushstring(_pLuaState, "Text");
		lua_gettable(_pLuaState, -6);
		CWorldManager::GetInstance()->CheckStack(_pLuaState);
		szText = static_cast<string>(lua_tostring(_pLuaState, -1));
	}

	lua_pop(_pLuaState, lua_gettop(_pLuaState));

	//printf("Rock Pos = %f, %f, %f, Time Offset = %f\n", vPos.x, vPos.y, vPos.z, fTimeOffset);
	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnvironment(vPos, ENVIRONMENT_OBJECT, 0.0f, nType, szText.c_str());
	
	return 1;
}

int MakeSceneryObject(lua_State *_pLuaState)
{
	vec3f vPos;
	float fYRot = -1;
	float fScale = -1;
	string szModelName;
	bool bIsUpdatable = false;

	lua_pushstring(_pLuaState, "Pos");
	lua_gettable(_pLuaState, -2);
	LoadVec3(_pLuaState, vPos);

	lua_pushstring(_pLuaState, "YRot");
	lua_gettable(_pLuaState, -3);
	if(lua_isnil(_pLuaState, -1))
	{
		fYRot = static_cast<float>(rand() % 360);
	}
	else
	{
		fYRot = static_cast<float>(lua_tonumber(_pLuaState, -1));
	}

	lua_pushstring(_pLuaState, "Scale");
	lua_gettable(_pLuaState, -4);
	fScale = static_cast<float>(lua_tonumber(_pLuaState, -1));

	lua_pushstring(_pLuaState, "ModelName");
	lua_gettable(_pLuaState, -5);
	szModelName = lua_tostring(_pLuaState, -1);

	lua_pushstring(_pLuaState, "IsUpdatable");
	lua_gettable(_pLuaState, -6);
	bIsUpdatable = (lua_toboolean(_pLuaState, -1) ? true : false);


	CSceneryObject* pScene = (CSceneryObject*)(CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushScenery(vPos, fYRot, fScale, bIsUpdatable, szModelName.c_str()));

	if(szModelName == "Portal")
	{
		CView::SetRenderInstanceFlags(pScene->GetRenderID(), RENDER_FULLY_TRANSLUCENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		pScene->SetAnimPlayer(CView::GetAnimPlayer(pScene->GetRenderID()));
		pScene->GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Portal_Swirl.anm");
		pScene->GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);
	}

	return 1;
}

int MakeWizard(lua_State *_pLuaState)
{
	vec3f vPos;
	float fType;

	LoadVec4(_pLuaState, vPos, fType);

	CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetCurrLoadLevel())->PushEnemy(vPos, WIZARD_OBJECT, 0, static_cast<int>(fType));

	return 1;
}

int ForceBreak(lua_State *_pLuaState)
{
	char* garbageArray = nullptr;
	//std::cout << garbageArray[2];

	return 1;
}

int EverlastingRage(lua_State *_pluaState)
{
	CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->SetIsEverlastingRage(!CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetIsEverlastingRage());

	return 1;
}

int SetWizardHP(lua_State *_pluaState)
{
	int nStack = lua_gettop(_pluaState);

	if(nStack < 1)
	{
		LOG("Leaving SetWizardHP function! Not enough parameters");
		return 0;
	}

	float fHP = static_cast<float>(lua_tointeger(_pluaState, -1));

	CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->SetHealth(fHP);

	// Commented this out so that game could compile - Dan
	// CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->SetTotalHealth(fHP);

	return 1;
}

int Render(lua_State *_pluaState)
{
	int nStack =  lua_gettop(_pluaState);

	if(nStack < 1)
	{
		LOG("Leaving Render Function! Not Enough parameters");
		return 0;
	}

	string szType = static_cast<string>(lua_tostring(_pluaState, -1));

	if(!strcmp(szType.c_str(), "CollisionBoxes"))
	{
		CView::SetRenderCollisionBoxes(!CView::GetRenderCollisionBoxes());
	}
	else if(!strcmp(szType.c_str(), "CollisionSpheres"))
	{
		CView::SetRenderCollisionSpheres(!CView::GetRenderCollisionSpheres());
	}
	else if(!strcmp(szType.c_str(), "2dParticles"))
	{
		CView::SetRender2dParticles(!CView::GetRender2dParticles());
	}
	else
	{
		return 0;
	}

	return 1;
}