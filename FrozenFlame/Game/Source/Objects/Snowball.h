/***********************************************
 * Filename:  		Snowball.h
 * Date:      		11/15/2012
 * Mod. Date: 		11/17/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the snowball object
 ************************************************/
#ifndef SNOWBALL_OBJECT_H
#define SNOWBALL_OBJECT_H

#include "ProjectileObject.h"
class CSnowball :
	public CProjectileObject
{
	bool m_bWasThrown;
	bool m_bHitByFire;

	TImage m_tShadow;
	int m_nShadowRenderID;
	int m_nMoltenRenderID;			// the render instance ID for the meteor's molten ground texture
	CEmitter* m_pcFireEffect;
	CEmitter* m_pcIceExplosionSmall;
	CEmitter* m_pIceExplosionLarge;

	void ExplodeSnowball();
	void ExplodeMeteor();
public:

	/*****************************************************************
	*	CSnowball():	Default constructor for the snowball object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    11/17/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CSnowball(void);

	/*****************************************************************
	*	~CSnowball():	Default destructor for the snowball object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    11/17/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	~CSnowball(void);

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
	* Mod. Date:		    11/17/2012
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
	* Mod. Date:		    11/17/2012
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
	* Mod. Date:		    11/17/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Update(float fElapsedTime);

	void Uninitialize();

	void MakeMeteor();

	void SetAttackElement(int _nAttackElement);

	// Mutators
	inline void SetWasThrown(bool bWasThrown) { m_bWasThrown = bWasThrown; }
	inline void SetHitByFire(bool bHit) { m_bHitByFire = bHit; }

	// Accessors
	inline bool GetHitByFire(void)const {return m_bHitByFire;}

};

#endif