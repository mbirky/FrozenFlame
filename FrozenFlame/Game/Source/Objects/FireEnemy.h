/***********************************************
 * Filename:  		FireEnemy.h
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the fire enemy object
 ************************************************/

#ifndef FIRE_ENEMY_H
#define FIRE_ENEMY_H

#include "enemy.h"
#include "../Renderer/Emitter.h"
#include "../Sound/CSoundManager.h"

class CFireEnemy :
	public CEnemy
{
private:
	bool		m_bIsRolling;
	CEmitter*	m_pcEmitter;
	CEmitter*	m_cEmit[3];
	bool		m_bIsRollUp;
	bool		m_bPlayRollUpSound;			// flag for if it's okay to play the roll up sound; only play sound once, not a zillion times
	vec3f		m_vOldPos;
	float		m_fStuckTimer;
	float		m_fOldSpeed;
	float		m_fWarmUpTimer;
	bool		m_bWarmingUp;

	int			m_nRollDamage;
	int			m_nPrevAttackedByElement;
	int			m_nNumContinuousSameElementAttacks;

	int			m_nColdToNeutralHitCount;
	int			m_nNeutralToColdHitCount;
	int			m_nNeutralToHotHitCount;
	int			m_nHotToNeutralHitCount;
	int			m_nHotToBurningHitCount;
	int			m_nBurningToHotHitCount;

	int			m_nBaseAttackDamage;

	int			m_nLightID;
	int			m_nShapeID;
	int			m_nCubeEmitID;
	int			m_nCubeEmitID2;
	CBaseObject* m_pcCube;
	CBaseObject* m_pcDest;
public:

	/*****************************************************************
	*	CFireEnemy():		Default constructor for the fire enemy object
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
	CFireEnemy(void);

	/*****************************************************************
	*	~CFireEnemy():		Default destructor for the fire enemy object
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
	~CFireEnemy(void);

	/*****************************************************************
	*	Initialize():		The default funciton that sets all starting values for
	*						this instance of the class
	*
	* Ins:					nType
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

	void ResetParticles(void);
	void RemoveEmitters(void);

	void CollisionReaction(CCollidableObject* pObj = nullptr);

	void Uninitialize(void);

	/*********************************************
	*
	*					Mutators
	*
	**********************************************/
	void SetIsRolling(bool _bIsRolling)					{m_bIsRolling = _bIsRolling;}
	void SetIsRollingUp(bool _bIsRollup)				{m_bIsRollUp = _bIsRollup;}
	void SetIsWarmingUp(bool _bIsWarmup)				{m_bWarmingUp = _bIsWarmup;}
	void SetPlayRollUpSound(bool okayToPlaySound)		{ m_bPlayRollUpSound = okayToPlaySound;}

	/*********************************************
	*
	*					Accessors
	*
	**********************************************/
	bool GetIsRolling()					{return m_bIsRolling;}
	bool GetIsRollingUp(void)			{return m_bIsRollUp; }
	CEmitter* GetEmitter(void)			{return m_pcEmitter;}
	int GetRollingDamage(void)			{return m_nRollDamage;}
	bool GetIsWarmingUp(void)			{return m_bWarmingUp;}
	bool CanPlayRollUpSound() const		{ return m_bPlayRollUpSound; }
};

#endif