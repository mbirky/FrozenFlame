/********************************************************************
* Filename:			CLevel.h
* Date:				11/8/2012
* Mod. Date:		2/23/2013
* Mod. Initials:	JC
* Author:			Matthew G. Birky
* Purpose:			This is a level in the game
********************************************************************/

#ifndef CLEVEL_H
#define	CLEVEL_H

#include "../Globals.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/Wall.h"
#include "../Objects/SceneryObject.h"

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
};

#include <conio.h>

class CLevel
{
private:
	// vector of active enemies on the level
	vector<CEnemy*> m_pvActiveEnemies;
	// vector of active beasts
	vector<CEnemy*> m_pvActiveBeasts;
	// vector of environment objects on the level
	vector<CEnvironmentObject*> m_pvEnviromentObjects;
	// vector of scenery objects on the level
	vector<CSceneryObject*> m_pvSceneryObjects;

	// If this float is greater or equal to zero, enemies should be popping
	// If enemies should be popping, this timer will represent the time passed since the last enemy was popped
	// As an enemy gets popped, the timer will set back to zero and will be updated
	// Once all the enemies are popped this will be set to -1
	float m_fPoppingEnemiesTimer;
	// This represents the time waited between one pop and another
	float m_fPoppingEnemiesCooldown;

	// level wall
	CWall* m_pWall;
	// z position of the level "entrance"
	float m_fPosZ;
	// x position of the level center
	float m_fCenterX;
	// extent of the level on the z
	float m_fExtentZ;
	// extent of the level on the x
	float m_fExtentX;
	// position of the level on the y
	float m_fPosY;
	// true is the enemies are active
	bool m_bActive;
	// id of the level in the world manager
	int m_nLevelID;
	// used for a single time "intro" to this level piece - JC
	// gets set to true when camera starts following player or camera is far enough away during transition.
	bool m_bIntroDone;

	static int sm_nSoundCount;

	// Keeps track of the ground of the level
	CEnvironmentObject* m_pGround;

	int m_nBackgroundID;

	vector<int> m_vDeleteVector;

	// Used in conjuction with camera transition and intro bool to make anthing happen once
	// during the camera transition introducting the level - JC
	void DoIntroActions(float fElapsedTime);
	// the boss level doesn't transition and we may want some special cinematic flare for this stage only -SMS
	void DoBossLevelIntroActions(float fElapsedTime);
	// Used for manually cleaning up any madness you did in intro actions. -JC
	void CleanupIntroActions(float fElapsedTime);

public:
	CLevel(void);

	~CLevel(void);

	bool ActiveEnemies(void);

	void UnloadLevel(void);

	void Update(float fElapsedTime);

	void ActivateEnemies(void);

	CBaseObject* PushEnemy(vec3f vPos, int nObjType, int _nAttackElement = 0, int nSubType = -1);
	void PushEnemyIntoActiveVector(CEnemy* pEnemy);
	CBaseObject* PushEnvironment(vec3f vPos, int nObjType, float _fElapsedTime = 0.0f, int nSubType = -1, const char* szText = 0);
	CBaseObject* PushScenery(vec3f vPos, float fYRot, float fScale, bool bUpdatable, const char* szModelName);

	void MakeGround(void);
	void MakeWizardGround(void);
	void MakeBridge(void);
	void MakeCliff(void);
	void MakeEndGround(void);
	void MakeWall(vec3f vPos, float fAngle);

	bool IsInside(CBaseObject* pObj) const;
	void KeepInside(CBaseObject* pObj);

	void ActivateSpawners();
	void StopSpawners();

	// This function will set all the enemies on this level to start popping / exploding
	// fPopCooldown - time waited between enemies popping
	// Returns the number of enemies currently active on the level
	int PopEnemies(float fPopCooldown);


	/***************
	*  Accessors
	***************/
	inline float GetPosZ(void) const					{ return m_fPosZ; }
	inline float GetPosY(void) const					{ return m_fPosY; }
	inline float GetCenterX(void) const					{ return m_fCenterX; }
	inline float GetCenterZ(void) const					{ return (m_fPosZ + (m_fExtentZ / 2)); }
	inline float GetExtentZ(void) const					{ return m_fExtentZ; }
	inline float GetExtentX(void) const					{ return m_fExtentX; }
	inline int	 GetBackgroundID(void) const			{ return m_nBackgroundID; }
	inline bool  IsActive(void) const					{ return m_bActive; }
//	inline int   GetNewestSoundID()						{ return sm_nSoundCount++; }
	inline unsigned int GetActiveEnemyCount(void) const { return m_pvActiveEnemies.size(); }
	inline unsigned int GetBeastCount(void) const		{ return m_pvActiveBeasts.size(); }
	vector<CEnemy*>* GetActiveEnemies(void)				{ return &m_pvActiveEnemies; }
	vector<CEnvironmentObject*>* GetEnvironmentObjects() {return &m_pvEnviromentObjects;}
	/***************
	* Mutators
	***************/
	
	// deprecated function, if you want to activate a level call ActivateEnemies()
	
	//void SetActive(bool _bActive) {}// 	{ m_bActive = _bActive; }
	void ResetActive() {m_bActive = false;}

	void SetLevelID(int _nLevelID)	{ m_nLevelID = _nLevelID; }
};

#endif		// CLEVEL_H