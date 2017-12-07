/***********************************************
 * Filename:  		RangedEnemy.h
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the ranged enemy object
 ************************************************/

#ifndef RANGED_ENEMY_H
#define RANGED_ENEMY_H

#include "enemy.h"
#include "../Sound/CSoundManager.h"



class CRangedEnemy :
	public CEnemy
{
private:

	enum RangedEnemyAnimations {RENEMY_IDLE = 3, RENEMY_WALK, RENEMY_ATTACK, RENEMY_DEATH};

	struct tBobbingData
	{
		float fBobbingSpeed;				// the speed factor for how fast the object bobs up and down
		float fBobbingDistance;			// the object will float this far up before coming back down (and vice versa)
		float fCurrentFloatDistance;		// the object has floated this far since it started floating
		bool  bBobbingUp;					// true if the object is floating up, false if the object is floating down	
	};

	tBobbingData m_bobbingData;	// contains the data for causing the spawner to float up and down in the air
public:
	/*****************************************************************
	*	CRangedEnemy():		Default constructor for the ranged enemy object
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
	CRangedEnemy(void);

	/*****************************************************************
	*	~CRangedEnemy():		Default destructor for the ranged enemy object
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
	~CRangedEnemy(void);

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
	void Reinitialize(void);

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

	void Uninitialize(void);

	// This will not only set the attack element for the ranged minion but will
	// also load his staff based on the element.
	void SetAttackElement(int _nAttackElement);

};

#endif