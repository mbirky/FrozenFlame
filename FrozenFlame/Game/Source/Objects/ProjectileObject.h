/***********************************************
 * Filename:  		ProjectileObject.h
 * Date:      		10/30/2012
 * Mod. Date: 		10/31/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the projectile object
 ************************************************/

#ifndef PROJECTILE_OBJECT_H
#define PROJECTILE_OBJECT_H

#include "CollidableObject.h"
#include "Enemy.h"


class CProjectileObject :
	public CCollidableObject
{
private:
	CEnemy * m_pcOwner;
	CEmitter* m_pEmitterTrail;

	float m_fSpeed;
	float m_fLifeTimer;

	bool m_bDeflected;

public:
	/*****************************************************************
	*	CProjectileObject():	Default constructor for the projectile object
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
	CProjectileObject(void);

	/*****************************************************************
	*	~CProjectileObject():	Default destructor for the projectile object
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
	~CProjectileObject(void);

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

	void Uninitialize();

	void OnAttack(int nDamage,int nElementType);
	
	// When the attack element for the projectile is set, we wanna change its render instance as well
	void SetAttackElement(int _nAttackElement);

	/***************
	*  Accessors
	***************/
	CEnemy * GetOwner()	{return m_pcOwner;}
	bool GetDeflected()	{return m_bDeflected;}
	float GetLifeTime() {return m_fLifeTimer;}

	/***************
	* Mutators
	***************/
	void SetOwner(CEnemy * pcOwner)		{m_pcOwner = pcOwner;}
	void SetDeflected(bool bDeflected)	{m_bDeflected = bDeflected;}
	void SetLifeTime(float fTime)		{m_fLifeTimer = fTime;}
	void SetSpeed(float fSpeed)			{m_fSpeed = fSpeed;}
};

#endif