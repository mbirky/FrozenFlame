/***********************************************
 * Filename:  		Enemy.h
 * Date:      		10/29/2012
 * Mod. Date: 		11/02/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the enemy object
 ************************************************/

#ifndef ENEMY_OBJECT_H
#define ENEMY_OBJECT_H

#include "CollidableObject.h"

class CBaseBehavior;
class CEnemy :
	public CCollidableObject
{
private:
	friend class CBaseBehavior;
	friend class CFSM;

	CFSM* m_pcBehavior;
	
	int m_nAttackingElement;
	int m_nPreviousElement;

	float m_fMoveSpeed;
	float m_fKnockbackTimer;

	bool m_bIsCrystalGuardian;

	CBaseObject * m_pcCrystal;
	bool m_bHit;
	float m_fHitCooldown;

	float m_fStunTimer;				// the amount of time the enemy has been stunned
	float m_fStunDuration;			// when stunned, the enemy will be stunned for this long
	bool  m_bIsStunned;				

	float m_fSeparationRadius;		// personal space; this is the closest it will want to be to another enemy if flocking

	float m_fTimerOffset;			// A random float that will be used to offset each enemy's attack timer
	//float m_fAnimationSpeed;

public:

	/*****************************************************************
	*	CEnemy():		Default constructor for the enemy base object
	*						that all other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    11/02/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CEnemy(void);

	/*****************************************************************
	*	~CEnemy():		Default destructor for the enemy base object
	*						that all other objects must overload
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
	virtual ~CEnemy(void);

	/*****************************************************************
	*	Initialize():		The default funciton that sets all starting values for
	*						this instance of the class that other objects must overload
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
	virtual void Initialize(void);

	/*****************************************************************
	*	Reinitialize():		The default funciton that resets all starting values for
	*						this instance of the class that other objects must overload
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
	virtual void Reinitialize(void);

	/*****************************************************************
	*	Update():			Updates all data for the class based off of time and must be overloaded
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
	virtual void Update(float fElapsedTime) = 0;

	/*****************************************************************
	* UpdateStunTimer():	updates the stun for an enemy, if it is stunned
	*
	* Ins:					fElapsedTime - the time since the last frame
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    01/24/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void UpdateStunTimer(float fElapsedTime);
	
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
	virtual void Uninitialize();

	/*********************************************
	*
	*					Mutators
	*
	**********************************************/
	inline void SetFSM(CFSM* pfsm)							{ m_pcBehavior = pfsm;}
	inline void SetPreviousElement(int nElement)			{ m_nPreviousElement = nElement;}
	inline void SetMoveSpeed(float fSpeed)					{ m_fMoveSpeed = fSpeed;}
	inline void SetKnockbackTimer(float fKnockTime)			{ m_fKnockbackTimer = fKnockTime;}
	inline void SetGuardian(bool bGuardian)					{ m_bIsCrystalGuardian = bGuardian;}
	inline void SetCrystal(CBaseObject * pCrystal)			{ m_pcCrystal = pCrystal;}
	inline void SetIsHit(bool _bHit)						{ m_bHit = _bHit;}
	inline void SetHitCooldown(float _fHCooldown)			{ m_fHitCooldown = _fHCooldown;}
	inline void SetIsStunned(bool stunned)					{ m_bIsStunned = stunned; }
	inline void SetStunDuration(float time)					{ m_fStunDuration = time; }
	inline void SetTimerOffset(float fOffset)				{ m_fTimerOffset = fOffset; }
	//inline void SetAnimationSpeed(float fSpeed)				{ m_fAnimationSpeed = fSpeed; }
	/*********************************************
	*
	*					Accessors
	*
	**********************************************/
	inline CFSM* GetFSM(void)					{ return m_pcBehavior;}
	inline int GetPreviousElement(void)			{ return m_nPreviousElement;}
	inline float GetMoveSpeed(void)				{ return m_fMoveSpeed;}
	inline float GetKnockback(void)				{ return m_fKnockbackTimer;}
	inline bool GetIsGuardian()					{ return m_bIsCrystalGuardian;}
	inline CBaseObject * GetCrystal()			{ return m_pcCrystal;} 
	float GetSeparationRadius() const			{ return m_fSeparationRadius; }
	bool GetIsHit()const						{ return m_bHit; }
	inline float GetHitCooldown()const			{ return m_fHitCooldown; }
	inline bool GetIsStunned() const			{ return m_bIsStunned; }
	inline float GetTimerOffset()const			{ return m_fTimerOffset; }
	//inline float GetAnimationSpeed()const		{ return m_fAnimationSpeed; }
};

#endif