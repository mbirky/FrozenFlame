/***********************************************
* Filename:  		ObjectManager.h
* Date:      		10/31/2012
* Mod. Date: 		1/9/2013
* Mod. Initials:	JC
* Author:    		Charles Meade
* Purpose:   		All function definitions and member declarations for the object manager 
*			
************************************************/


#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "BaseObject.h"
#include "EnvironmentObject.h"
#include "ProjectileObject.h"
#include "PlayerObject.h"
#include "Enemy.h"
#include "ExplosiveEnemy.h"
#include "FireEnemy.h"
#include "IceEnemy.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "WizardEnemy.h"
#include "CollidableObject.h"
#include "AttackObject.h"
#include "CFireTrap.h"
#include "CIceTrap.h"
#include <vector>
#include "Wall.h"
#include "Snowball.h"
#include "Portal.h"
#include "CPickupObject.h"
#include "Spawner.h"
#include "SceneryObject.h"
#include "Signpost.h"

class CObjectManager
{
	// bool to keep track of if this class was initialized
	bool m_bInitialized;
	// player pointer
	CPlayerObject * m_pcPlayer;
	
	// wizard pointer
	CWizardEnemy * m_pcWizardEnemy;

	CPortal* m_pcPortal;

	CSignpost* m_pcCurrSign;
	
	// bool to keep track of if wizard should be updated
	bool m_bWizardActive;

	// number of each object currently allocated
	unsigned int m_unNumEnvironmentObjects;
	unsigned int m_unNumProjectiles;
	unsigned int m_unNumExplosiveEnemies;
	unsigned int m_unNumMeleeEnemies;
	unsigned int m_unNumRangedEnemies;
	unsigned int m_unNumFireEnemies;
	unsigned int m_unNumIceEnemies;
	unsigned int m_unNumCollidableObjects;
	unsigned int m_unNumAttackObjects;
	unsigned int m_unNumIceTraps;
	unsigned int m_unNumFireTraps;
	unsigned int m_unNumWalls;
	unsigned int m_unNumSnowballs;
	unsigned int m_unNumPortals;
	unsigned int m_unNumPickups;
	unsigned int m_unNumSpawners;
	unsigned int m_unNumScenery;
	unsigned int m_unNumSigns;

	// vectors to keep track of active objects
	std::vector<CEnvironmentObject*> m_vpcActiveEnvironmentObjects;
	std::vector<CProjectileObject*>	m_vpcActiveProjectiles;
	std::vector<CExplosiveEnemy*>	m_vpcActiveExplosiveEnemies;
	std::vector<CMeleeEnemy*>		m_vpcActiveMeleeEnemies;
	std::vector<CRangedEnemy*>		m_vpcActiveRangedEnemies;
	std::vector<CFireEnemy*>		m_vpcActiveFireEnemies;
	std::vector<CIceEnemy*>			m_vpcActiveIceEnemies;
	//std::vector<CCollidableObject*> m_vpcActiveCollidableObjects;
	std::vector<CAttackObject*>		m_vpcActiveAttackObjects;
	std::vector<CIceTrap*>			m_vpcActiveIceTraps;
	std::vector<CFireTrap*>			m_vpcActiveFireTraps;
	std::vector<CWall*>				m_vpcActiveWalls;
	std::vector<CSnowball*>			m_vpcActiveSnowballs;
	std::vector<CPortal*>			m_vpcActivePortals;
	std::vector<CPickupObject*>		m_vpcActivePickups;
	std::vector<CSpawner*>			m_vpcActiveSpawners;
	std::vector<CSceneryObject*>	m_vpcActiveSceneryObjects;
	std::vector<CSignpost*>			m_vpcActiveSignPosts;

	// vectors to keep track of inactive objects
	std::vector<CEnvironmentObject*> m_vpcInactiveEnvironmentObjects;
	std::vector<CProjectileObject*> m_vpcInactiveProjectiles;
	std::vector<CExplosiveEnemy*>	m_vpcInactiveExplosiveEnemies;
	std::vector<CMeleeEnemy*>		m_vpcInactiveMeleeEnemies;
	std::vector<CRangedEnemy*>		m_vpcInactiveRangedEnemies;
	std::vector<CFireEnemy*>		m_vpcInactiveFireEnemies;
	std::vector<CIceEnemy*>			m_vpcInactiveIceEnemies;
	//std::vector<CCollidableObject*> m_vpcInactiveCollidableObjects;
	std::vector<CAttackObject*>		m_vpcInactiveAttackObjects;
	std::vector<CIceTrap*>			m_vpcInactiveIceTraps;
	std::vector<CFireTrap*>			m_vpcInactiveFireTraps;
	std::vector<CWall*>				m_vpcInactiveWalls;
	std::vector<CSnowball*>			m_vpcInactiveSnowballs;
	std::vector<CPortal*>			m_vpcInactivePortals;
	std::vector<CPickupObject*>		m_vpcInactivePickups;
	std::vector<CSpawner*>			m_vpcInactiveSpawners;
	std::vector<CSceneryObject*>	m_vpcInactiveSceneryObjects;
	std::vector<CSignpost*>			m_vpcInactiveSignPosts;

	// vector for checking collisions and making sure that there are not duplicate checks
	std::vector<CCollidableObject*> m_vpcCollisionCheckVector;

	//vector for containing all traps, for ckecks with ai to avoid traps
	std::vector<CCollidableObject*> m_vpcTrapVector;

public:

	/*****************************************************************
	* CObjectManager():	The constructor for the object manager. DOES NOT INITIALIZE CLASS!
	*					INITIALIZE MUST BE CALLED PRIOR TO USING THIS CLASS!
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:		    n/a
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	CObjectManager(void);

	/*****************************************************************
	* ~CObjectManager():	Destructor for the object manager. will call ShutDown if it was not already called,
	*					however, the ShutDown function should be called prior to this in case something has fallen out of scope.
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:		    n/a
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	~CObjectManager(void);

	/*****************************************************************
	* Initialize():		Initializes the object manager to have the max number of each type of object.
	*					For tuning/balancing purposes, there has to be passed in a number for each type of enemy.
	*					Returns true if loading was sucessful, false otherwise.
	*
	* Ins:				nNumMelee
	*					nNumRanged
	*					nNumExplosive
	*					nNumIce
	*					nNumFire
	*					nNumCollidable
	*					nNumProjectile
	*					nNumPickups
	*
	* Outs:				void
	*
	* Returns:		    bool
	*
	* Mod. Date:		12/22/2012
	* Mod. Initials:	JM
	*****************************************************************/
	bool Initialize(int nNumMelee,int nNumRanged, int nNumExplosive, int nNumIce, int nNumFire, int nNumCollidable, int nNumEnvironment, int nNumProjectile, int nNumFireTraps, int nNumIceTraps,int nNumWalls = 10,int nNumSnowballs = 10,int nNumPortals = 10, int nNumPickups = 50);

	/*****************************************************************
	* GetPlayer():		Returns a pointer to the player object
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:		    CPlayerObject*
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	CPlayerObject* GetPlayer();

	/*****************************************************************
	* GetWizard():		Returns a pointer to the wizard object
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:		    CWizardEnemy*
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	CWizardEnemy* GetWizard();

	/*****************************************************************
	* NewObject():		takes in an int for the enum of the type of object that is needed and 
	*					returns a reinitialized object of that type. will return nullptr if no objects are available
	*
	* Ins:				nObjectType
	*
	* Outs:				void
	*
	* Returns:		    CBaseObject*
	*
	* Mod. Date:		11/01/2012
	* Mod. Initials:	CM
	*****************************************************************/
	CBaseObject* NewObject(int nObjectType, int nSubType = -1, const char* szName = nullptr);

	/*****************************************************************
	* Update():			updates all objects and calls private functions to handle all collisions
	*
	* Ins:				fElapsedTime
	*
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	void Update(float fElapsedTime);

	/*****************************************************************
	* ExpandOjectList():	makes the array of objects larger for the specified type.
	*						CAUTION!!! VERY SLOW FUNCTION THAT CALLS NEW ALOT!!!
	*						SHOULD ONLY BE CALLED DURING TESTING IF MORE OBJECTS ARE NEEDED!!
	*						ON RELEASE, WE SHOULD KNOW FROM THE BEGINNING THE AMOUNT WE NEED!!
	*
	* Ins:				nObjectType
	*					nNumToAdd
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	void ExpandObjectList(int nObjectType, int nNumToAdd);

	/*****************************************************************
	* Shutdown():		shuts down the manager and deletes all memory used for objects
	*
	* Ins:				void
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	void Shutdown();

	bool CheckActiveEnemies();

	void SetWizardActive(bool bIsActive) { m_bWizardActive = bIsActive; }

	CPortal* GetPortal(void) {return m_pcPortal;}
	void SetPortal(CPortal* _pcPortal) {m_pcPortal = _pcPortal;}

	CSignpost* GetCurrentSign(void) {return m_pcCurrSign;}
	void SetCurrSign(CSignpost* _pcSign) {m_pcCurrSign = _pcSign;}

	const vector<CCollidableObject *>& GetTraps() {return m_vpcTrapVector;}

	CEnemy * GetCrystalGuardian();
	void ClearScenery(void);
	void ClearPickups(void);
	void ClearEnvironmental(void);
	void ClearFireTraps(void);
	void ClearSnowballs(void);
	void ClearIceTraps(void);

private:

	/*****************************************************************
	* LoadLevel():		loads the level from the passed in file
	*
	* Ins:				szFileName
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	bool LoadLevel(char* szFileName);

	/*****************************************************************
	* CheckCollisions():checks all collisions for all collidable objects
	*
	* Ins:				void
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		11/02/2012
	* Mod. Initials:	CM
	*****************************************************************/
	void CheckCollisions();

	/*****************************************************************
	* PlayerCollisions(): checks all environment player collisions and handles average correction
	*					  called inside CheckCollisions
	* Ins:				void
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		1/9/2013
	* Mod. Initials:	JC
	*****************************************************************/
	void PlayerCollisions();

	

	/*****************************************************************
	* CreateObjects():	function to move all of the loops from initialize to another function out of the way
	*
	* Ins:				void
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	void CreateObjects();

	/*****************************************************************
	* DeleteObjects():	function to move all of the loops from deletion to another function out of the way
	*
	* Ins:				void
	*					
	* Outs:				void
	*
	* Returns:		    void
	*
	* Mod. Date:		10/31/2012
	* Mod. Initials:	CM
	*****************************************************************/
	void DeleteObjects();



	public:
	void UninitializeAllObjects();

};

#endif