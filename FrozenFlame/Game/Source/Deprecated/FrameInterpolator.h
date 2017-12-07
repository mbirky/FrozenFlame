#if 0

// Frame Interpolator
// Class that is supposed to interpolate between the frames for a given animation
// For now this will be done in the CPU side but in the future we can change it to
// do the calculations on a shader, GPU side.

/***********************************************
 * Filename:  		FrameInterpolator.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/29/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		This class uses an animation
 *					and a vector of Keyframes to
 *					interpolate between frames for
 *					purposes of animation
 ************************************************/

#ifndef _FRAME_INTERPOLATOR_H_
#define _FRAME_INTERPOLATOR_H_


#define ANIMATOR AnimationManager::GetInstance()

#include "KeyFrame.h"
//class CKeyFrame;
class CAnimation;
class CFrameInterpolator
{
	//friend class CAnimation;
	friend class CAnimationPlayer;
	//////////////////////////////////////////////////////////////////////////
	//	------------------------------------------------------------------	//
	//							DATA	MEMBERS								//
	//	------------------------------------------------------------------	//
	//////////////////////////////////////////////////////////////////////////

	// Animation this interpolator is using
	CAnimation* m_pAnimation;

	// Current keyFrame
	vector<CKeyFrame> m_vKeyFrames;

	// Current time
	double m_dTime;

public:
	// Default Constructor - Set variables to safe values

	/*****************************************************************
	* CFrameInterpolator():	Default constructor for the Frame Interpolator
	*						that initializes all variables to safe values
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
	CFrameInterpolator(void);
	
	// Attaches an animation to the interpolator

	/*****************************************************************
	* SetAnimation():		Sets the Interpolator's animation pointer to
	*						the passed in value.
	*
	* Ins:					const CAnimation*
	*
	* Outs:					CAnimation*
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void SetAnimation(CAnimation* pAnimation);

	/*****************************************************************
	* GetAnimation():		Allows other objects to access
	*						the Interpolator's animation pointer
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				const CAnimation*
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/

	const CAnimation* GetAnimation(void)  { return m_pAnimation;}

	// Adds time to the interpolator

	/*****************************************************************
	* AddTime():			Increments the Interpolator's timer variable
	*						by the passed in value.
	*
	* Ins:					double
	*
	* Outs:					double
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void AddTime(double dTime);

	// Sets the interpolator's time

	/*****************************************************************
	* SetTime():			Sets the Interpolator's timer variable
	*						to the passed in value.
	*
	* Ins:					double
	*
	* Outs:					double
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void SetTime(double dTime);


	/*****************************************************************
	* GetCurFrame():		Returns the Interpolator's current KeyFrame
	*						by reference.
	*
	* Ins:					const int
	*
	* Outs:					void
	*
	* Returns:				CKeyFrame &
	*
	* Mod. Date:		    11/02/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CKeyFrame& GetCurFrame(const int index) {return m_vKeyFrames[index];}

	// Interpolates between the animations key-frames based
	// on the interpolator's current time

	/*****************************************************************
	* Process():			Interpolates the matrix values between 2 KeyFrames
	*						the frames used differ based on the Interpolator's
	*						current time value
	*
	* Ins:					double &
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void Process(float& fTime);
	
	// Destructor - Clean Up

	/*****************************************************************
	* ~CFrameInterpolator():	Default destructor for the Frame
	*							Interpolator
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
	~CFrameInterpolator(void);


};

#endif // _FRAME_INTERPOLATOR_H_

#endif