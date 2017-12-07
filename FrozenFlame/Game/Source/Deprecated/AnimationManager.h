#if 0


// Animation Manager
// Class that is supposed to load and store all the animation data for the game

/***********************************************
 * Filename:  		AnimationManager.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		Stores each loaded animation
 *					within a container that is
 *					accessed using enumerated values
 ************************************************/

#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include <vector>
using std::vector;

#include "Animation.h"
#include "AnimationPlayer.h"
class CAnimation;

class CAnimationManager
{
	//friend class CAnimationPlayer;
	enum Animations {e_IDLE, e_WALK, e_ATTACK};

	//////////////////////////////////////////////////////////////////////////
	//	------------------------------------------------------------------	//
	//							DATA	MEMBERS								//
	//	------------------------------------------------------------------	//
	//////////////////////////////////////////////////////////////////////////

	// Singleton's static instance
	static CAnimationManager* sm_pInstance;
	
	vector<CAnimation*> m_vpAnimations;

	//////////////////////////////////////////////////////////////////////////
	//	------------------------------------------------------------------	//
	//							PRIVATE	FUNCTIONS							//
	//	------------------------------------------------------------------	//
	//////////////////////////////////////////////////////////////////////////

	/***************************************************************************
	* CAnimationManager():		Default constructor for the Animation
	*							Manager that holds all the game's animations
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	CAnimationManager(void);

	// For a proper singleton, the trilogy needs to be defined
	/***************************************************************************
	* CAnimationManager():		Copy constructor for the Animation
	*							Manager that holds all the game's animations
	*
	* Ins:					const CAnimationManager&
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	CAnimationManager(const CAnimationManager&) {}				// Copy Constructor

	/***************************************************************************
	* = operator():				Overload for the Manager's assignment operator
	*
	* Ins:					const CAnimationManager&
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	CAnimationManager& operator=(const CAnimationManager&) {}		// Assignment Operator

	/***************************************************************************
	* ~CAnimationManager():		Destructor for the Animation Manager
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	~CAnimationManager(void) {}								// Destructor


	//////////////////////////////////////////////////////////////////////////
	//	------------------------------------------------------------------	//
	//							PUBLIC	FUNCTIONS							//
	//	------------------------------------------------------------------	//
	//////////////////////////////////////////////////////////////////////////

	// Used to access the singleton's instance

	/***************************************************************************
	* GetInstance():		Returns a static instance pointer of the Animation Manager
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static CAnimationManager*
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	static CAnimationManager* GetInstance(void);

	// Used to delete the singleton's instance

	/***************************************************************************
	* DeleteInstance():		Deletes the static instance pointer of the Animation Manager
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static void
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	static void DeleteInstance(void);


	/***************************************************************************
	* Initialize():			Initializes all the variables of the Animation Player
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	void Initialize(void);

	////////////////////////////////////////////////////////////////////////
	// Name: LoadAnimation												  //
	//																	  //
	// Parameters: filename - path of animation file being loaded in	  //
	//																	  //
	// Return: The index of the animation loaded in						  //
	//																	  //
	// Description: Calls the Animation's load function then adds it to	  //
	//              the manager's container of Animations				  //
	////////////////////////////////////////////////////////////////////////

	/***************************************************************************
	* LoadAnimation():		Creates a new instance of the Animation class and
	*						then uses its LoadAnimation function to	load an animation 
	*						and then add the new instance to the Manager's container
	*						of Animations
	*
	* Ins:					const char*
	*
	* Outs:					void
	*
	* Returns:				int
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	int LoadAnimation(const char* filename);

	////////////////////////////////////////////////////////////////////////
	// Name: Shutdown													  //
	//																	  //
	// Parameters: void													  //
	//																	  //
	// Return: void														  //
	//																	  //
	// Description: Goes through the container of Animations and deletes  //
	//              each Animation										  //
	////////////////////////////////////////////////////////////////////////

	/***************************************************************************
	* Shutdown():			De-initializes and deletes the Manager's variables
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	void Shutdown(void);

};


#endif // _ANIMATION_MANAGER_H_

#endif