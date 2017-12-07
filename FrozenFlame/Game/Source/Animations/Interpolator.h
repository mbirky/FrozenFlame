// Interpolator Class
// by Justin Maza & Daniel Lima
// - This is a static class responsible for interpolating through keyframes based on time
// updated 11/24/2012

#ifndef _INTERPOLATOR_H_
#define _INTERPOLATOR_H_

#include "KeyFrame.h"

class CAnimation;
class CInterpolator
{
	// the current interpolated pose
	static vector<CKeyFrame> sm_vCurPose;

	// helper function to interpolate through one animation
	static void Process(CAnimation* pAnim, float fTime, float fBlend);

public:
	// the update function fills in the curPose vector with interpolated keyframes
	// based on the passed in parameters, should be called on AnimPlayer
	static void Update(int nCurAnim, float fCurTime, int nNextAnim, float fNextTime, bool& bFinishedBlending);

	// this returns the current keyframe for the passed in joint ID
	static CKeyFrame* GetCurFrame(int nJointID);
	static vector<CKeyFrame>& GetCurPose(void) { return sm_vCurPose; }

	static int GetKeyframeNumber(int nCurAnim, int nJointID, float fCurTime);
};

#endif