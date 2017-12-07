#include "Interpolator.h"
#include "../Renderer/AssetManager.h"
#include "Animation.h"
#include "../Util/Util.h"

vector<CKeyFrame> CInterpolator::sm_vCurPose;

void CInterpolator::Update(int nCurAnim, float fCurTime, int nNextAnim, float fNextTime, bool& bFinishedBlending)
{
	bFinishedBlending = false;
	CAnimation* pCurAnim = CAssetManager::GetAnimation(nCurAnim);
	CAnimation* pNextAnim = CAssetManager::GetAnimation(nNextAnim);

	if(!pCurAnim) return;

	Process(pCurAnim, fCurTime, 0.0f);


	// if there's nothing to blend to we're done
	if(!pNextAnim) 
	{
		//for(unsigned int f = 0; f < pCurAnim->GetJoints()[0].m_vKeyFrames.size(); ++f)
		//{
		//	if(&GetCurFrame(0) == &pCurAnim->GetJoints()[0].m_vKeyFrames[f])
		//	{
		//		nFrameID = f;
		//	}
		//}
		return;
	}

	// otherwise...
	// this is the delta that will be applied to the blend, as it gets closer
	// to 1.0 we'll see more of the next animation and less of the current
	float deltaBlend = fNextTime / 1.0f;
	if(deltaBlend >= 1.0)
	{
		bFinishedBlending = true;
		return;
	}

	Process(pNextAnim, fNextTime, deltaBlend);

	//for(unsigned int f = 0; f < pCurAnim->GetJoints()[0].m_vKeyFrames.size(); ++f)
	//{
	//	if(&GetCurFrame(0) == &pCurAnim->GetJoints()[0].m_vKeyFrames[f])
	//	{
	//		nFrameID = f;
	//	}
	//}
	

}

void CInterpolator::Process(CAnimation* pAnim, float fTime, float fBlend)
{
	// get the number of joints of the current anim
	size_t numJoints = pAnim->GetJoints().size();

	// if it's greater than our vector size we should resize it
	if(numJoints != sm_vCurPose.size())
		sm_vCurPose.resize(numJoints);

	// loop through all the joints
	for(size_t i = 0; i < numJoints; ++i)
	{
		// get the current joint
		TJoint& curJoint = pAnim->GetJoints()[i];

		// find the current keyframe id for that joint
		int curFrameID = 0, nextFrameID = 0;
		size_t numFrames = curJoint.m_vKeyFrames.size();
		for(size_t j = 0; j < numFrames; ++j)
		{
			const CKeyFrame& keyFrame = curJoint.m_vKeyFrames[j];
			if(fTime < (keyFrame.GetTime() + keyFrame.GetTweenTime()))
			{
				curFrameID = j;
				
				nextFrameID = (curFrameID + 1) % numFrames;
				break;
			}
		}

		// this is the actual keyframe we're at
		CKeyFrame& curKeyFrame = curJoint.m_vKeyFrames[curFrameID];
		// this is the next actual keyframe
		CKeyFrame& nextKeyFrame = curJoint.m_vKeyFrames[nextFrameID];

		// let's find our delta
		float delta = (fTime - curKeyFrame.GetTime()) / curKeyFrame.GetTweenTime();

		if(fBlend)
		{
			D3DXMATRIX nextMatrix = InterpolateD3DXMatrix(
				curKeyFrame.GetWorldTransform(), nextKeyFrame.GetWorldTransform(), delta);
			sm_vCurPose[i].SetWorldTransform(InterpolateD3DXMatrix(
				sm_vCurPose[i].GetWorldTransform(), nextMatrix, fBlend));
		}
		else
		{
			sm_vCurPose[i].SetWorldTransform(InterpolateD3DXMatrix(
				curKeyFrame.GetWorldTransform(), nextKeyFrame.GetWorldTransform(), delta));
		}

	}
}

CKeyFrame* CInterpolator::GetCurFrame(int nJointID)
{
	if(nJointID >= 0 && nJointID < (int)sm_vCurPose.size())
	{
		return &sm_vCurPose[nJointID];
	}
	return nullptr;
}

int CInterpolator::GetKeyframeNumber(int nCurAnim, int nJointID, float fCurTime)
{
	int nReturn = -1;

	CAnimation* pAnim = CAssetManager::GetAnimation(nCurAnim);
	if(pAnim && nJointID >= 0 && nJointID <= (int)pAnim->GetJoints().size())
	{
		TJoint& curJoint = pAnim->GetJoints()[nJointID];
			
		// find the current keyframe id for that joint
		size_t numFrames = curJoint.m_vKeyFrames.size();
		for(size_t j = 0; j < numFrames; ++j)
		{
			const CKeyFrame& keyFrame = curJoint.m_vKeyFrames[j];
			if(fCurTime < (keyFrame.GetTime() + keyFrame.GetTweenTime()))
			{
				nReturn = j;
				break;
			}
		}
	}

	return nReturn;
}