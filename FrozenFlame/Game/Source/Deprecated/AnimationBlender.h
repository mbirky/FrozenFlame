#if 0


// Animation Blender
// Class that is supposed to interpolate between different animations, blending
// when necessary, and using the frame interpolator to interpolate between the frames
// afterwards.

/***********************************************
 * Filename:  		AnimationBlender.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		Uses two Frame Interpolators to
 *					enable a smooth transition between
 *					multiple animations without forcing
 *					the user to wait for one animation
 *					to complete before playing another
 ************************************************/

#ifndef _ANIMATION_BLENDER_H_
#define _ANIMATION_BLENDER_H_

#include "FrameInterpolator.h"

class CAnimationBlender
{
	friend class CAnimationPlayer;

	//CAnimation *m_pCurrentAnimation;  // The animation currently playing
	//CAnimation *m_pBlendingAnimation; // The animation that will be interpolated to in the blender
public:
	CFrameInterpolator m_pInterpolator;
	CFrameInterpolator m_pBlendingInterpolator; // The Interpolator that will be used to interpolate between animations


	// This is where we will be processing our animations and determining if we need to blend multiple animations

	/***************************************************************************
	* Process():			Updates the animations and then uses its Frame Interpolator
	*						to interpolate between the Animation's frames.  If there are
	*						multiple animations (pBlenAnim != nullptr), then it updates
	*						both animations before switching over to the second one.
	*
	* Ins:					double, CAnimation*, CAnimation*
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	//void Process(double dTime, CAnimation* pInterpAnim, CAnimation* pBlendAnim = nullptr);
	void Process(CAnimationPlayer* pAniPlayer);

	/***************************************************************************
	* GetInterpolator():	Returns the Blender's primary Frame Interpolator
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CFrameInterpolator
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	CFrameInterpolator GetInterpolator(void)	{return m_pInterpolator;}		  //Returns primary FrameInterpolator

	/***************************************************************************
	* GetInstance():		Returns the Blender's secondary (interpolation blending)
	*						Frame Interpolator
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CFrameInterpolatorS
	*
	* Mod. Date:		    10/28/2012
	* Mod. Initials:	    JM
	***************************************************************************/
	CFrameInterpolator GetBlendingInterpolator(void)  {return m_pBlendingInterpolator;} // Returns secondary (blending) FrameInterpolator


	/*****************************************************************
	* CAnimationBlender():	Default constructor for the Animation Blender
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
	CAnimationBlender(void);

	/*****************************************************************
	* ~CAnimationBlender():	Default destructor for the Animation Blender
	*						de-initializes all its variables
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	~CAnimationBlender(void);




};
#endif // _ANIMATION_BLENDER_H_

#endif