/***********************************************
 * Filename:  		SceneryObject.h
 * Date:      		1/18/2013
 * Mod. Date: 		1/18/2013
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		Function declarations for the scenery object
 ************************************************/

#ifndef SCENERY_OBJECT_H_
#define SCENERY_OBJECT_H_

#include "baseobject.h"
#include "../Renderer/View.h"
#include "../Animations/AnimPlayer.h"
#include "../Renderer/Emitter.h"

class CSceneryObject : public CBaseObject
{
	bool m_bIsUpdatable;
	string m_szName;

	bool m_bIsTransparent;
	CAnimPlayer* m_pAniPlayer;

	// This variable will be used to make the object delete itself after a certain time.
	// If it is zero, the object will delete itself
	// If it is greater than zero, the object will decrease elapsed time from it every Update
	// If it is -1, it will be ignored
	// All this awesome commented code was brought to you by your fellow handsome QA Lead, Daniel Lima
	float m_fLifetime;
	float m_fDuration;

public:

	/*****************************************************************
	* CSceneryObject():		Default constructor for the scenery object
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    1/18/2013
	* Mod. Initials:	    JM
	*****************************************************************/
	CSceneryObject(void);
	
	/*****************************************************************
	* ~CSceneryObject():	Default destructor for the scenery object
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    1/18/2013
	* Mod. Initials:	    JM
	*****************************************************************/
	~CSceneryObject(void);

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
	* Mod. Date:		    1/18/2013
	* Mod. Initials:	    JM
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
	* Mod. Date:		    1/18/2013
	* Mod. Initials:	    JM
	*****************************************************************/
	void Reinitialize();

	/*****************************************************************
	* Uninitialize():		returns the object to the starting state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/18/2013
	* Mod. Initials:	    JM
	*****************************************************************/
	void Uninitialize();

	/*****************************************************************
	* Update():				Updates all data for the class based off of time
	*
	* Ins:					fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/18/2013
	* Mod. Initials:	    JM
	*****************************************************************/
	void Update(float fElapsedTime);

	/***************
	*  Accessors
	***************/
	bool		 GetIsUpdatable	   (void) const	{return m_bIsUpdatable;}
	const char*  GetName		   (void) const {return m_szName.c_str();}
	float		 GetLifetime	   (void) const { return m_fLifetime; }
	CAnimPlayer* GetAnimationPlayer(void)	{return m_pAniPlayer;}

	/***************
	* Mutators
	***************/
	void SetIsUpdatable(bool _bIsUpdatable)		{ m_bIsUpdatable = _bIsUpdatable; }
	void SetName(const char* _szName)			{ m_szName = _szName; }
	void SetTransparent(bool bTrans)			{ m_bIsTransparent = bTrans; }
	void SetLifetime(float fLifetime)			{ m_fLifetime = fLifetime; m_fDuration = fLifetime; }
	void SetAnimPlayer(CAnimPlayer* _pAniPlayer)	{m_pAniPlayer = _pAniPlayer;}

};
#endif