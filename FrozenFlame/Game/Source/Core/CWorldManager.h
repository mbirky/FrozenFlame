/********************************************************************
* Filename:			CWorldManager.h
* Date:				11/7/2012
* Mod. Date:		11/28/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Manager of the game world
********************************************************************/

#ifndef CWORLDMANAGER_H
#define CWORLDMANAGER_H

#include "../Globals.h"
#include "../Objects/ObjectManager.h"
#include "../AI/Header/CBehaviorManager.h"
#include "../Util/CInput.h"
#include "CLevel.h"
#include "../Util/CluaFunctions.h"
#include "../Achievements/Header/CAchievementManager.h"

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
};

#include <conio.h>

class CWorldManager
{
private:
	bool m_bResetFade;
	bool m_bSectionComplete;
	bool m_bBossLoaded;

	static const int m_nNumMelee				= 30;
	static const int m_nNumRanged				= 30;
	static const int m_nNumExplosive			= 30;
	static const int m_nNumIceBeast				= 30;
	static const int m_nNumFireBeast			= 30;
	static const int m_nNumCollidableObjects	= 50;
	static const int m_nNumProjectiles			= 30;
	static const int m_nNumFireTraps			= 50;
	static const int m_nNumIceTraps				= 30;
	static const int m_nNumEnvironmentObjects	= 300;

	
	float			m_fEtime;
	int				m_nCurrSection;
	int				m_nCurrentLevel;
	int				m_nCurrLoadLevel;
	vector<CLevel>	m_vLevels;
	//RECT			m_rCurrentBounds;

	vector<AABB>	m_vWallAABBs;

	lua_State		*m_pLuaState;

	CObjectManager		m_cObjectManager;
	CBehaviorManager	m_cBehaviorManager;

	/********************************************************************
	* CWorldManager():		The constructor for the world manager
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CWorldManager(void);

	/********************************************************************
	* CWorldManager():		The copy constructor for the World manager
	*
	* Ins:					const CWorldManager&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CWorldManager(const CWorldManager&);

	/********************************************************************
	* operator=():			The assignment operator for the world manager
	*
	* Ins:					CWorldManager&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	CWorldManager& operator=(const CWorldManager&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the world manager
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	~CWorldManager(void);

	/********************************************************************
	* Loadlevel():			Loads in a level from a lua file
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:			12/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	bool LoadLevel(void);

	void CalculateWallAABBs(void);
public:

	// Sets the current level to start popping enemies
	// Returns the number of active enemies on the level
	// fCooldown - Time between one and another minion "explosion"
	int PopCurrentLevelEnemies(float fCooldown);

	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CWorldManager*
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CWorldManager* GetInstance()
	{
		static CWorldManager instance;
		return &instance;
	}

	/********************************************************************
	* Init():				This is a function that inits the class
	*
	* Ins:					bool
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Init(void);

	/********************************************************************
	* Reset():				This is a function that Resets the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Reset(bool _bTotal);

	/********************************************************************
	* Update():				Updates any classes in the World manager
	*						that need updated
	*
	* Ins:					double
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/8/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Update(float _fElapsedTime);

	/********************************************************************
	* Lost():				Checks if the player has lost the game yet
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:			11/8/2012
	* Mod. Initials:		MB
	********************************************************************/
	bool Lost(void);

	/********************************************************************
	* GetHealthScale():		Returns the scale of how much health we have
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				double
	*
	* Mod. Date:			11/8/2012
	* Mod. Initials:		MB
	********************************************************************/
	float GetHealthScale(void);

	/********************************************************************
	* GetRageScale():		Returns the scale of how much Rage we have
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				double
	*
	* Mod. Date:			11/8/2012
	* Mod. Initials:		MB
	********************************************************************/
	float GetRageScale(void);

	/********************************************************************
	* Shutdown():			This shuts down the world manager
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/7/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Shutdown(void);

	/********************************************************************
	* MakeIceTrap():		This function makes an object of the specifed
	*						at the position passed in
	*
	* Ins:					int, vec3f
	*
	* Outs:					void
	*
	* Returns:				Pointer to the created object
	*
	* Mod. Date:			11/8/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CBaseObject* MakeObject(vec3f _vPos, int _nObjType, int _nSubType = -1, const char* szModelName = NULL);

	/********************************************************************
	* CheckStack():			This function is used for debugging the stack
	*						that lua uses
	*
	* Ins:					lua_State*
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/27/2012
	* Mod. Initials:		MB
	********************************************************************/
	void CheckStack(lua_State *L);

	bool LoadCurrentSection(void);

	void ActivateObjectLevel(CBaseObject* pObj);
	void BoundsKeeping(CBaseObject* pObj);

	/***************
	*  Accessors
	***************/
	float GetGroundHeight(void);
	inline CObjectManager* GetObjectManager(void)		{ return &m_cObjectManager; }
	inline CBehaviorManager* GetBehaviorManager(void)	{ return &m_cBehaviorManager; }
	inline int GetCurrLoadLevel(void)					{ return m_nCurrLoadLevel; }
	inline CLevel* GetLevel(int _nLevelNum)				{ return &m_vLevels[_nLevelNum]; }
	inline int GetCurrentLevel()						{return m_nCurrentLevel;}
	inline lua_State* GetluaState(void)					{return m_pLuaState;}
	inline int GetCurrentSection(void)					{return m_nCurrSection;}
	inline unsigned int GetTotalCurrentLevels(void)		{return m_vLevels.size();}
	inline bool GetBoss(void)							{return m_bBossLoaded;}

	int GetLevelForObject(CBaseObject* pObj);
	inline float GetExtent(void)
	{
		if(m_nCurrentLevel > -1 && m_nCurrentLevel < (int)m_vLevels.size())
		{
			return m_vLevels[m_nCurrentLevel].GetExtentZ();
		}
		return 0.0f;
	}
	inline vector<AABB>& GetWallAABBs(void) { return m_vWallAABBs; }
	int GetCurBackground(void)
	{
		if(m_nCurrentLevel >= 0 && m_nCurrentLevel < (int)m_vLevels.size())
		{
			return m_vLevels[m_nCurrentLevel].GetBackgroundID();
		}
		return -1;
	}
	inline bool GetSectionComplete(void)	{return m_bSectionComplete;}

	/***************
	* Mutators
	***************/
	void SetCurrentSection(int _nCurrSection)				{m_nCurrSection = _nCurrSection;}
	void SetSectionComplete(bool _bSectionComplete)			{m_bSectionComplete = _bSectionComplete;}
	inline void SetBossLevelLoaded(bool bLoaded)			{m_bBossLoaded = bLoaded; }
};

#endif	// CWORLDMANAGER_H
