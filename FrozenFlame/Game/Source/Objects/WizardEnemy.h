/***********************************************
 * Filename:  		WizardEnemy.h
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the wizard enemy object
 ************************************************/

#ifndef WIZARD_ENEMY_H
#define WIZARD_ENEMY_H

class CWorldManager;
class CLevel;
class CWizardSpawner;
class CSpawner;

#include "enemy.h"
#include "../Renderer/Emitter.h"
#include "CIceTrap.h"
#include "CFireTrap.h"
#include "SceneryObject.h"
#include "IceEnemy.h"
#include "FireEnemy.h"
//#include "../Core/CWorldManager.h"

enum WizardAnimations	{RENEMY_IDLE = 3, RENEMY_WALK, RENEMY_ATTACK, RENEMY_DEATH};
enum WizardSubTypes		{FIRST_STAGE,SECOND_STAGE,THIRD_STAGE,FOURTH_STAGE,FINAL_BOSS};

class CWizardEnemy :
	public CEnemy
{
private:

	int m_nNumTrapWavesCreated;
	int m_nWizardForm;
	int m_nWizardSubType;
	bool m_bIsAlive;
	bool m_bExhausted;
	float m_fTrapSpawnTimer;
	bool m_bEscaped;
	bool m_bDefeated;

	float m_fTotalHealth;
	float m_fFinalDeathTimer;
	float m_fDeathTimeLimit;

	bool m_bDeathCharging;
	bool m_bDeathExloding;
	bool m_bDeathExploded;

	TImage m_tShadow;
	int m_nShadowRenderID;

	CSceneryObject * m_pcBubble;

	CEmitter* m_pcFireEmitter;
	CEmitter* m_pcIceEmitter;

	

	vec3f m_tInitialPosition;
	vector<CIceTrap*> m_vIceTraps;
	vector<CFireTrap*> m_vFireTraps;

	CPlayerObject* m_pPlayer;
	CTimer m_tWizTimer;

	int m_nShapeID; // For Health

	// stuff for spawning enemies
	CLevel * m_pcCurrLevel;
	CIceEnemy * m_pcIceSpawned;
	CFireEnemy * m_pcFireSpawned;

	// crystals in final battle
	vector<CSpawner*> m_vpcWizardSpawners;

	// Emitters
	CEmitter * m_pcBubbleBurstEmitter;
	CEmitter * m_pcBubbleSheenEmitter;
	CEmitter * m_pcCrashEmitter;
	CEmitter * m_pcDeathBurstEmitter;
	CEmitter * m_pcDeathFlowEmitter;
	CEmitter * m_pcSmokeEmitter;
	CEmitter * m_pcTakeoffEmitter;
	CEmitter * m_pcSpawnEmitter;
	
public:

	/*****************************************************************
	*	CWizardEnemy():		Default constructor for the wizard enemy object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CWizardEnemy(void);

	/*****************************************************************
	*	~CWizardEnemy():	Default destructor for the wizard enemy object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	~CWizardEnemy(void);

	/*****************************************************************
	*	Initialize():		The default funciton that sets all starting values for
	*						this instance of the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Initialize(void);

	/*****************************************************************
	*	Reinitialize():		The default funciton that resets all starting values for
	*						this instance of the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Reinitialize();

	/*****************************************************************
	*	Update():			Updates all data for the class based off of time
	*
	* Ins:					fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Update(float fElapsedTime);

	/*****************************************************************
	* Uninitialize():		returns the object to the starting state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/03/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Uninitialize();
	
	/*****************************************************************
	*	OnAttack():			runs any logic when this object gets attacked by something
	*
	* Ins:					void
	*
	* Outs:					nDamage nElementType
	*
	* Returns:				void
	*
	* Mod. Date:		    02/04/2013
	* Mod. Initials:	    BRG
	*****************************************************************/
	void OnAttack(int nDamage,int nElementType);

	bool GetIsAlive(void) {return m_bIsAlive;}

	int GetWizardForm()	{return m_nWizardForm;}

	void SetWizardForm(int _nWizardForm)	{m_nWizardForm = _nWizardForm;}

	vec3f GetInitialPosition()	{return m_tInitialPosition;}

	void SetInitialPosition(vec3f _tInitialPosition)	{m_tInitialPosition = _tInitialPosition;}

	bool GetExhausted()	{return m_bExhausted;}

	void SetExhausted(bool _bExhausted)	{m_bExhausted = _bExhausted;}

	void SetWizardSubtype(int nSubType) {m_nWizardSubType = nSubType;};

	int GetWizardSubtype() {return m_nWizardSubType;}

	bool GetEscaped(void)	{return m_bEscaped;}
	void SetEscaped(bool _bEscaped) {m_bEscaped = _bEscaped;}
	
	bool GetIsDefeated(void) {return m_bDefeated;}
	void SetIsDefeated(bool _bDefeated) {m_bDefeated = _bDefeated;}
	CSceneryObject * GetBubble()		{return m_pcBubble;}
	
	void HideBubble()	{ CView::SetIsVisible(GetBubble()->GetRenderID(),false);}
	void ShowBubble()	{ CView::SetIsVisible(GetBubble()->GetRenderID(),true);}

	float GetWizardTime() {return m_tWizTimer.GetElapsedTime();}

	void SpawnEnemies();

	void CrystalDisabled();

	void ReactivateCrystals();

	void IncrementPhase();

	void PushBackWizardSpawner(CSpawner * pcSpawner);

	void ClearWizardSpawners();

	void SetTotalHealth(float fHealth) {m_fTotalHealth = fHealth; };
	float GetTotalHealth() {return m_fTotalHealth;};

	void SetBubbleBurstEmitterActive(bool bActive);
	void SetBubbleSheenEmitterActive(bool bActive);
	void SetCrashEmitterActive(bool bActive);
	void SetDeathBurstEmitterActive(bool bActive);
	void SetDeathFlowEmitterActive(bool bActive);
	void SetSmokeEmitterActive(bool bActive);
	void SetTakeoffEmitterActive(bool bActive);
	void SetSpawnEmitterActive(bool bActive);

};

#endif
