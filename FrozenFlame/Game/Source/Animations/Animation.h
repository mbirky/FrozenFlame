/***********************************************
 * Filename:  		Animation.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		Encapsulates all the necessary
 *					information needed to create
 *					an animation by manipulating
 *					the vertices of a model
 *					using a vector of joints to store
 *					the matrix information
 ************************************************/

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "../Renderer/RenderDefinitions.h"

class IAnimEvent;

class CAnimation
{
	friend class CAssetManager;

	// Duration of this animation
	float			m_fDuration;

	// Duration of the animation accounting for the events
	float			m_fActualDuration;
	
	// Vector of joints and their keyframes
	vector<TJoint>	m_vJoints;

	// Name of the animation
	string			m_szName;

	// Vector of animation events that should affect
	// whatever is playing this animation
	vector<const IAnimEvent*> m_vpAnimEvents;
	
public:
	inline float GetDuration(void) const { return m_fDuration; }

	// Default Constructor - Initialize data members to safe values

	/*****************************************************************
	* CAnimation():			Default constructor for the Animation class
	*						that initializes all variables to safe values
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CAnimation(void);
	
	// Load - Load animation data from a file

	/***************************************************************************
	* LoadAnimation():		Load an animation using the passed in file name
	*						and initializes the variables to the correct values
	*
	* Ins:					const char*
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	bool Load(const char* szFilename);
	
	// Destructor - Clean up

	/*****************************************************************
	* ~CAnimation():		Default destructor for the Animation class
	*						de-initializes all its variables
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	~CAnimation(void);

	/*****************************************************************
	* GetFrameDuration():	Returns the duration value of the Animation
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				double
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	float GetFrameDuration(void)	{return m_fDuration;}

	/*****************************************************************
	* GetFrameJoints():		Returns the Animation's container of joints
	*						by reference
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				vector<tJoint>&
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	vector<TJoint>& GetJoints(void) {return m_vJoints;}

	// Loads events for this animation from the passed in file
	bool LoadEvents(const char* szFilename);

	// Accessor to the vector of animation events
	const vector<const IAnimEvent*>& GetAnimEvents(void) const { return m_vpAnimEvents; }
};

#endif // _ANIMATION_H_