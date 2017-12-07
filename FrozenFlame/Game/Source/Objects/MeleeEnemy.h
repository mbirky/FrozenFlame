/***********************************************
 * Filename:  		MeleeEnemy.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/29/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the melee enemy object
 ************************************************/
#ifndef MELEE_ENEMY_H
#define MELEE_ENEMY_H

#include "enemy.h"
#include "../Renderer/Emitter.h"
#include "../Objects/EnvironmentObject.h"
#include "../Objects/ProjectileObject.h"
#include "../Sound/CSoundManager.h"

class CView;

class CMeleeEnemy :
	public CEnemy
{
	//enum MeleeEnemyAnimations { MENEMY_IDLE = 3, MENEMY_WALK, MENEMY_ATTACK, MENEMY_DEATH};

	bool m_bAfterEffectActive;
	float m_fEffectTimer;
	CEmitter* m_pcEffectEmitter1;	// Fire Effect
	CEmitter* m_pcEffectEmitter2;	// Ice Shatter Effect
	CEmitter* m_pcEffectEmitter3;	// Ash Effect
	CEmitter* m_pcAmbientColdEmitter;
	CEmitter* m_pcAmbientFireEmitter;
	CEnvironmentObject* m_pcIceBlock;
	int m_nIceBlockID;

	int m_nLightID;

	// sound buffers, so sounds are played once instead of every frame
	bool  m_bCanPlayHurtSound;		
	bool  m_bCanPlayDeathSound;
	float m_fHurtSoundTimer;		// how long it's been since playing a hurt sound
	float m_fHurtSoundResetTime;	// how long before reseting the hurt sound buffer

public:
	/*****************************************************************
	*	CMeleeEnemy():		Default constructor for the Melee enemy object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CMeleeEnemy(void);

	/*****************************************************************
	*	~CMeleeEnemy():		Default destructor for the melee enemy object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	~CMeleeEnemy(void);

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
	* Mod. Date:		    10/29/2012
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
	* Mod. Date:		    10/29/2012
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
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Update(float fElapsedTime);

	/*****************************************************************
	*	OnAttack():			Calls the CCollidable's OnAttack, then checks to see if 
	*						the agent needs to change to Panic Behavior.
	*
	* Ins:					void
	*
	* Outs:					nDamage nElementType
	*
	* Returns:				void
	*
	* Mod. Date:		    11/13/2012
	* Mod. Initials:	    MB
	*****************************************************************/
	void OnAttack(int nDamage,int nElementType);

	void Uninitialize();

	/*****************************************************************
	*	IceDeath():			Shatters the Ice Block around the Minion, and
	*						Uninitializes it.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/19/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void IceDeath();

	/*****************************************************************
	*	FireDeath():		Creates and AshCloud effect, and
	*						Uninitializes the minion.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/19/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void FireDeath();

	/*****************************************************************
	*	NormalDeath():		Plays the Minion's death animation before it is uninitialized.
	*
	* Ins:					void
	*
	* Outs:					nDamage nElementType
	*
	* Returns:				void
	*
	* Mod. Date:		    1/19/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void NormalDeath();

	/*****************************************************************
	* LoadLuaValues():		Loads and sets the enemy's variables from a lua file
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:		    01/22/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void LoadLuaValues();

	//////////////////////////////////////////////////////////////
	// ACCESSORS
	//////////////////////////////////////////////////////////////
	bool GetAfterEffectActive()	{return m_bAfterEffectActive;}

	float GetEffectTimer()	{return m_fEffectTimer;}

	CEmitter* GetEffectEmitter1()	{return m_pcEffectEmitter1;}

	CEmitter* GetEffectEmitter2()	{return m_pcEffectEmitter2;}

	//////////////////////////////////////////////////////////////
	// MUTATORS
	//////////////////////////////////////////////////////////////
	void SetAfterEffectActive(bool _bAfterEffectActive)	{m_bAfterEffectActive = _bAfterEffectActive;}

	void SetEffectTimer(float _fEffectTimer)	{m_fEffectTimer = _fEffectTimer;}

	void SetEffectEmitter1(CEmitter* _pcEffectEmitter)	{m_pcEffectEmitter1 = _pcEffectEmitter;}

	void SetEffectEmitter2(CEmitter* _pcEffectEmitter)	{m_pcEffectEmitter2 = _pcEffectEmitter;}
};

#endif