/***********************************************
 * Filename:		Spawner.h		
 * Date:      		01/09/2013
 * Mod. Date: 		01/09/2013
 * Mod. Initials:	BRG
 * Author:    		Brett R. Green
 * Purpose:   		Spawner fuction declerations
 ************************************************/
#ifndef SPAWNER_H_
#define SPAWNER_H_

class CWorldManager;
class CLevel;
class CWizardEnemy;

#include "EnvironmentObject.h"
#include "Enemy.h"
#include "../Renderer/Emitter.h"
#include "../Renderer/View.h"
#include "SceneryObject.h"
#include "../Sound/CSoundManager.h"

struct tBobbingData
{
	float fBobbingSpeed;				// the speed factor for how fast the object bobs up and down
	float fBobbingDistance;			// the object will float this far up before coming back down (and vice versa)
	float fCurrentFloatDistance;		// the object has floated this far since it started floating
	bool  bBobbingUp;					// true if the object is floating up, false if the object is floating down	
};

class CSpawner : public CEnemy // HACK Should be changed to Enemy.
{
public:
	CEmitter* m_cEmit;
	CEmitter* m_pcIceEmitter;
	CEmitter* m_pcFireEmitter;
	CEmitter* m_pcWizardBeamEmitter;
	CEmitter* m_pcShatterEmitter;
	bool m_bIsEmitterActive;
	bool m_bIsDestructable;
	bool m_bEnergyTrails; // Do we want this spawner to emit energy trails to the wizard?

	// Data for Spawning Enemies
	vector<int>		m_vSpawnList;	// The list of enemies the spawner will spawn in its wave
	bool			m_bIsSpawningEnemies;	// Wheter the spawner is active
	CEnemy*			m_pGaurd;
	CLevel*			m_pLevelOwner;	// The Level that contains this spawner

	float m_fSpawnDelay;	// The delay between spawn waves
	float m_fCurrentDelay;	// The current time passed between waves
	float m_fInternalDelay;	// The current time passed between individual spawns
	float m_fReformDelay;	// The delay before the crystal reforms.
	bool  m_bShieldCooldown;
	float m_fShieldCooldown;
	float m_fElementSwitchDelay;
	bool  m_bSpawnOnce;
	bool  m_bWizardSpawner;

	tBobbingData m_bobbingData;	// contains the data for causing the spawner to float up and down in the air

	unsigned int m_nMinimunReinforce;
	int m_nExplosiveType;	// Alternating value for explosive enemies
	int m_nExplosiveChance;

	CSceneryObject * m_pcBubble;
	CSceneryObject * GetBubble(void) { return m_pcBubble; }


	vector<int>::iterator m_CurrentSpawn;	// The currently active creature to spawn

#define SPAWNER_WAIT_TIME	0.75f	// The default delay between individual spawns

	float fRotateRadians;	// rotate this many degrees (will get converted to radians) each frame
	float fRotateDuration;	// rotates along the axes for this long before switching rotation direction
	float fRotateTimer;		// how long the spawner has been rotating in a particular direction

public:

	// Look how beautiful those accessors are :D - Dan
	inline const CEmitter*				GetEmitter(void)			const { return m_cEmit; }
	inline bool							GetIsDestructable(void)		const { return m_bIsDestructable; }
	inline const vector<int>&			GetSpawnList(void)			const { return m_vSpawnList; }
	inline bool							GetIsSpawningEnemies(void)	const { return m_bIsSpawningEnemies; }
	inline const CEnemy*				GetGuard(void)				const { return m_pGaurd; }
	inline const CLevel*				GetLevelOwner(void)			const { return m_pLevelOwner; }
	inline float						GetSpawnDelay(void)			const { return m_fSpawnDelay; }
	inline float						GetCurrentDelay(void)		const { return m_fCurrentDelay; }
	inline float						GetInternalDelay(void)		const { return m_fInternalDelay; }
	inline bool							GetSpawnOnce(void)			const { return m_bSpawnOnce; }
	inline unsigned int					GetMinimunReinforce(void)	const { return m_nMinimunReinforce; }
	inline int							GetExplosiveType(void)		const { return m_nExplosiveType; }
	inline int							GetExplosiveChance(void)	const { return m_nExplosiveChance; }
	inline const vector<int>::iterator& GetCurrentSpawn(void)		const { return m_CurrentSpawn; }



	/**************************************************************
	* 
	* Ins				Void
	* Outs				Void
	* Mod. Date: 		01/09/2013
	* Mod. Initials:	BRG
	***************************************************************/
	CSpawner();

	/**************************************************************
	* 
	* Ins				Void
	* Outs				Void
	 * Mod. Date: 		01/09/2013
	* Mod. Initials:	BRG
	***************************************************************/
	~CSpawner();

	/**************************************************************
	* 
	* Ins				Void
	* Outs				Void
	 * Mod. Date: 		01/09/2013
	* Mod. Initials:	BRG
	***************************************************************/
	void Initialize(void);

	void Uninitialize(void);

	/**************************************************************
	* 
	* Ins				Void
	* Outs				Void
	 * Mod. Date: 		01/09/2013
	* Mod. Initials:	BRG
	***************************************************************/
	void Reinitialize(void);

	/**************************************************************
	* 
	* Ins				fElapsedTime	- The elapsed time of frame
	* Outs				Void
	 * Mod. Date: 		01/09/2013
	* Mod. Initials:	BRG
	***************************************************************/
	void Update(float fElapsedTime);
	
	/**************************************************************
	* 
	* Ins				Void
	* Outs				Void
	 * Mod. Date: 		01/09/2013
	* Mod. Initials:	BRG
	***************************************************************/
	void Spawn(void);

	void AddBubble(void);


	/*********************************************
	*
	*					Mutators
	*
	**********************************************/
	void SetWizardSpawner(bool in)						{m_bWizardSpawner = in;}
	void SetEnergyTrails(bool in)						{m_bEnergyTrails = in;}

	/*********************************************
	*
	*					Accessors
	*
	**********************************************/
	void OnAttack(int nDamage,int nElementType);
};

#endif