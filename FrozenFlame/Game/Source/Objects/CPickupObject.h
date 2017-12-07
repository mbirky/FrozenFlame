/***********************************************
* Filename:  		CPickupObject.h
* Date:      		12/20/2012
* Mod. Date: 		12/20/2012
* Mod. Initials:	JM
* Author:    		Justin Maza
* Purpose:   		Function declarations for the pickup object
************************************************/

#ifndef CPICKUP_OBJECT_H
#define CPICKUP_OBJECT_H

#include "../Renderer/View.h"
#include "CollidableObject.h"

enum ePickupTypes {HEALTH_PICKUP, RAGE_PICKUP, NUM_PICKUP_TYPES};

class CPickupObject : public CCollidableObject
{
	int m_nPickupType;
	int m_nLightID;
	float m_fLifetime;
	int	  m_nHealthIncrease;			// increases the player's health by this much
	int	  m_nRageIncrease;				// increases the player's health by this much
	float m_fHomingDistance;			// if the player is within this distance of the object, it will home in on the player
	float m_fHomingSpeed;				// the speed factor for how fast the object homes in on the player
	float m_fBobbingSpeed;				// the speed factor for how fast the object bobs up and down
	float m_fBobbingDistance;			// the object will float this far up before coming back down (and vice versa)
	float m_fCurrentFloatDistance;		// the object has floated this far since it started floating
	bool  m_bBobbingUp;					// true if the object is floating up, false if the object is floating down	

	CCollidableObject* m_pSource;

public:

	/*****************************************************************
	*	CPickupObject():	Default constructor for the pickup object
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    12/20/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CPickupObject(void);

	/*****************************************************************
	*	~CPickupObject():	Default destructor for the pickup object
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    102/20/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	~CPickupObject();

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
	* Mod. Date:		    12/20/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	virtual void Initialize(void);

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
	* Mod. Date:		    12/20/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	virtual void Reinitialize();

	/*****************************************************************
	*	Update():			Updates all data for the class based off of time
	*
	* Ins:					fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    12/20/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	virtual void Update(float fElapsedTime);

	void Uninitialize(void);

	int GetPickupType(void)						{ return m_nPickupType;}
	CCollidableObject* GetSource(void)			{ return m_pSource;}
	int GetHealthIncrease() const				{ return m_nHealthIncrease; }
	int GetRageIncrease() const					{ return m_nRageIncrease; }

	void SetPickupType(int nType);
	void SetHealthIncrease(int amount)			{ m_nHealthIncrease = amount;}
	void SetRageIncrease(int amount)			{ m_nRageIncrease = amount; }
	//void SetSource

	
};

#endif