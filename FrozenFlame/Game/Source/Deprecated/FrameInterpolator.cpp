#if 0

#include "FrameInterpolator.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "../Util/Matrix4x4.h"
#include "../Util/Util.h"

// Default Constructor - Set variables to safe values
CFrameInterpolator::CFrameInterpolator(void)
{
	m_dTime = 0.0;
	m_pAnimation = nullptr;
}

// Attaches an animation to the interpolator
void CFrameInterpolator::SetAnimation(CAnimation* pAnimation)
{
	// Set the animation pointer
	m_pAnimation = pAnimation;
	// Reset the timer
	m_dTime = 0.0;
}

// Adds time to the interpolator
void CFrameInterpolator::AddTime(double dTime)
{
	m_dTime += dTime;
}

// Sets the interpolator's time
void CFrameInterpolator::SetTime(double dTime)
{
	m_dTime = dTime;
}

// Interpolates between theanimations key-frames based
// on the interpolator's current time
void CFrameInterpolator::Process(float& fTime)
{
	unsigned int nMyCounter = 0;
	unsigned int nOtherCounter = 0;
	float fInBetween = 0.0f;
	float fDiff = 0.0f;

	int nCurFrame = 0;
	int nNextFrame = 0;
	
	//double dTiming = dTime;

	if(fTime > m_pAnimation->m_fDuration)
	{
		fTime -= m_pAnimation->m_fDuration;
	}

	// For each joint
	unsigned int uiNumJoints = m_pAnimation->m_vJoints.size();
	if(m_vKeyFrames.size() < uiNumJoints)
		m_vKeyFrames.resize(uiNumJoints);

	for( unsigned int i = 0; i < uiNumJoints; ++i )
	{
		// For each keyframe
		unsigned int uiNumFrames = m_pAnimation->m_vJoints[i].m_vKeyFrames.size();
		for(unsigned int j = 0; j < uiNumFrames; ++j)
		{
			if(fTime < m_pAnimation->m_vJoints[i].m_vKeyFrames[j].GetTime())
			{
				nCurFrame = j;
				fDiff = (fTime - m_pAnimation->m_vJoints[i].m_vKeyFrames[nCurFrame].GetTime());
				nNextFrame = (j+1) % uiNumFrames;
				break;
			}
		}

		if(nCurFrame >= (int)uiNumFrames)
			continue;

		CKeyFrame keyFrame = m_pAnimation->m_vJoints[i].m_vKeyFrames[nCurFrame];
		keyFrame.SetWorldTransform(InterpolateD3DXMatrix(m_pAnimation->m_vJoints[i].m_vKeyFrames[nCurFrame].GetWorldTransform(),
			m_pAnimation->m_vJoints[i].m_vKeyFrames[nNextFrame].GetWorldTransform(), fDiff));
		m_vKeyFrames[i] = keyFrame;
	}
}

// Destructor - Clean Up
CFrameInterpolator::~CFrameInterpolator(void)
{
	m_vKeyFrames.clear();
}

#endif