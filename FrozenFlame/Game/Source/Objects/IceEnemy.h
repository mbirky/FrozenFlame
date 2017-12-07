/***********************************************
 * Filename:  		IceEnemy.h
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the ice enemy object
 ************************************************/

#ifndef ICE_ENEMY_H
#define ICE_ENEMY_H

#include "enemy.h"
#include "../Renderer/Emitter.h"
class CIceEnemy :
	public CEnemy
{
	// Ice beast emitters
	CEmitter* m_pIceEmitter;
	CEmitter* m_pSmokeEmitter;
	int m_nIceBreathAttID;

	// Time a snowball takes to fall
	float m_fSnowballFallTime;

	// Are we ice breathinginginginging? - JC
	bool m_bIceBreathEnabled;

	int			m_nPrevAttackedByElement;
	int			m_nNumContinuousSameElementAttacks;

	int			m_nBaseAttackDamage;

	int	m_nHotToNeutralHitCount;
	int	m_nNeutralToHotHitCount;
	int	m_nNeutralToColdHitCount;
	int	m_nColdToNeutralHitCount;
	int	m_nColdToFrozenHitCount;
	int	m_nFrozenToColdHitCount;

	int m_nShapeID;
	int m_nLightID;

public:

	/*****************************************************************
	*	CIceEnemy():		Default constructor for the ice enemy object
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
	CIceEnemy(void);

	/*****************************************************************
	*	~CIceEnemy():		Default destructor for the ice enemy object
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
	~CIceEnemy(void);

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
	*	OnAttack():			runs any logic when this object gets attacked by something
	*
	* Ins:					void
	*
	* Outs:					nDamage nElementType
	*
	* Returns:				void
	*
	* Mod. Date:		    11/05/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void OnAttack(int nDamage,int nElementType);

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

	void Uninitialize(void);
	void UpdateAnimations(float fElapsedTime);

	int GetIceBreathAttackID() const					{ return m_nIceBreathAttID; }
	/*********************************************
	*
	*					Mutators
	*
	*********************************************/

	void PrepareToThrow(void);
	void PrepareToBlow(void);
	void StartRunning(void);
	void SetIceBreathEnabled(bool bEnabled);
	void Punch(void);
	bool GetIceBreathEnabled() {return m_bIceBreathEnabled;}

};

#endif