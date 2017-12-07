/********************************************************************
* Filename:			CTimer.h
* Date:				10/24/2012
* Mod. Date:		11/16/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This class is a timer that allows us to keep
track fo time for our application
********************************************************************/

#ifndef CLUAFUNCTIONS_H
#define CLUAFUNCTIONS_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

#include "../Globals.h"
#include "../Core/CWorldManager.h"


void RegisterFunctions(lua_State *_pluaState);

int NextSection(lua_State *_pluaState);
int PrevSection(lua_State *_pluaState);
int GoToLevel(lua_State * _pLuaState);
int ReloadSection(lua_State *_pluaState);
int ReloadEnemies(lua_State *_pluaState);
int EverlastingRage(lua_State *_pluaState);
int SetWizardHP(lua_State *_pluaState);
int Render(lua_State *_pluaState);

/********************************************************************
* LoadVec3():			Loads in a vector from a lua file
*
* Ins:					lua_State*
*
* Outs:					vec3f&
*
* Returns:				void
*
* Mod. Date:			11/27/2012
* Mod. Initials:		MB
********************************************************************/
void LoadVec3(lua_State *_pLuaState, vec3f &_vPos);


void LoadVec4(lua_State *_pLuaState, vec3f &_vPos, float &_fExtraData);

void LoadVec(lua_State *_pLuaState, vector<int> &_vSpawnSet);

/********************************************************************
* MakeMeleeMinion():	Makes a melee minion from a vec in a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			11/27/2012
* Mod. Initials:		MB
********************************************************************/
int MakeMeleeMinion(lua_State *_pLuaState);

/********************************************************************
* MakeRangedMinion():	Makes a ranged minion from a vec in a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			11/28/2012
* Mod. Initials:		MB
********************************************************************/
int MakeRangedMinion(lua_State *_pLuaState);

/********************************************************************
* MakeExplosiveMinion():	Makes an explosive minion from a vec in a
*							lua file
*
* Ins:						lua_State*
*
* Outs:						void
*
* Returns:					int
*
* Mod. Date:				11/28/2012
* Mod. Initials:			MB
********************************************************************/
int MakeExplosiveMinion(lua_State *_pLuaState);

/********************************************************************
* MakeIceBeast():		Makes a ice beast from a vec in a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			11/27/2012
* Mod. Initials:		MB
********************************************************************/
int MakeIceBeast(lua_State *_pLuaState);

/********************************************************************
* MakeFireBeast():		Makes a fire beast from a vec in a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			11/27/2012
* Mod. Initials:		MB
********************************************************************/
int MakeFireBeast(lua_State *_pLuaState);

/********************************************************************
* MakeIceTrap():		Makes an ice trap from a vec in a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			11/27/2012
* Mod. Initials:		MB
********************************************************************/
int MakeIceTrap(lua_State *_pLuaState);

/********************************************************************
* MakeFireTrap():		Makes a fire trap from a vec in a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			11/27/2012
* Mod. Initials:		MB
********************************************************************/
int MakeFireTrap(lua_State *_pLuaState);

/********************************************************************
* MakeEnviromentObject():	Makes an enviroment object from a vec in a lua file
*
* Ins:						lua_State*
*
* Outs:						void
*
* Returns:					int
*
* Mod. Date:				12/3/2012
* Mod. Initials:			MB
********************************************************************/
int MakeEnviromentObject(lua_State *_pLuaState);

/********************************************************************
* MakeSceneryObject():	Makes a Scenery object from date in lua files
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			1/18/2012
* Mod. Initials:		MB
********************************************************************/
int MakeSceneryObject(lua_State *_pLuaState);

/********************************************************************
* MakeSpawner():	Makes a spawner from a lua file
*
* Ins:					lua_State*
*
* Outs:					void
*
* Returns:				int
*
* Mod. Date:			01/12/2013
* Mod. Initials:		BRG
********************************************************************/
int MakeSpawner(lua_State *_pLuaState);

int MakeWizard(lua_State *_pLuaState);

int ForceBreak(lua_State *_pLuaState);

#endif CLUAFUNCTIONS_H