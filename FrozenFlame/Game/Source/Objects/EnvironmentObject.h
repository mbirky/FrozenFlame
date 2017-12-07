/***********************************************
 * Filename:  		EnvironmentObject.h
 * Date:      		10/30/2012
 * Mod. Date: 		11/06/2012
 * Mod. Initials:	DL
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the environment object
 ************************************************/

#ifndef ENVIRONMENT_OBJECT_H
#define ENVIRONMENT_OBJECT_H

#include "CollidableObject.h"
#include "Enemy.h"
class CEnvironmentObject :
	public CCollidableObject
{
private:
	int m_nSubType;
	// HACK - this is for dashing to shatter ice cubes. - Sean
	CEnemy* m_pcOwner;

public:
	enum ESubEnvironment {ENVOBJ_NULL = -1, ENVOBJ_GROUND, ENVOBJ_WIZARDGROUND, ENVOBJ_BRIDGE, ENVOBJ_CLIFF, ENVOBJ_WALL, ENVOBJ_ROCK1, ENVOBJ_ROCK2, ENVOBJ_ROCK3, ENVOBJ_DEADTREE1, ENVOBJ_DEADTREE2,
		ENVOBJ_LIVINGTREE, ENVOBJ_PINETREE, ENVOBJ_SHRUB, ENVOBJ_SIGN, ENVOBJ_ICECUBE, ENVOBJ_LAVA,ENVOBJ_COUNT};


	/*****************************************************************
	*	CEnvironmentObject():	Default constructor for the environment object
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
	CEnvironmentObject(void);

	/*****************************************************************
	*	~CEnvironmentObject():	Default destructor for the envirnoment object
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
	~CEnvironmentObject(void);

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

	void Uninitialize();
	
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

	/***************
	*  Accessors
	***************/
	int GetSubType(void)	{ return m_nSubType; }
	CEnemy* GetOwner()	{return m_pcOwner;}

	/***************
	* Mutators
	***************/
	void SetSubType(int nSubType);
	void SetOwner(CEnemy* pcOwner)	{m_pcOwner = pcOwner;}
};

#endif
