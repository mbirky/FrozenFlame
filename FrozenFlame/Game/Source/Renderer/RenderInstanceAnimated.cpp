#include "RenderInstanceAnimated.h"
#include "AssetManager.h"
#include "Model.h"

CRenderInstanceAnimated::CRenderInstanceAnimated(int nModelID, CBaseObject* pObject)
	: CRenderInstance(nModelID, pObject), m_nWeaponAnchorJoint(-1)
{
	// This ia an animated render instance so let's set the proper flag
	SetFlags(GetFlags() | RENDER_ANIMATED);

	const CModel* pModel = CAssetManager::GetModel(GetModelID());
	ZeroMemory(m_rgPreCalculatedPose, sizeof(D3DXMATRIX) * pModel->GetNumJoints());
}

void CRenderInstanceAnimated::Update(float fElapsedTime)
{
	// Call the parent's update
	CRenderInstance::Update(fElapsedTime);

	// Update the animation player
	m_cAnimPlayer.Update(fElapsedTime);

	if(m_cAnimPlayer.IsPlaying())
		SetFlags(GetFlags() | RENDER_ANIMATED);
	else
		SetFlags(GetFlags() & ~RENDER_ANIMATED);

	// Pre-calculate all the animation matrices to send to the shader
	const CModel* pModel = CAssetManager::GetModel(GetModelID());
	for(size_t uiJoint = 0; uiJoint < pModel->GetNumJoints() && uiJoint < sm_nMaxJoints; ++uiJoint)
	{
		const CKeyFrame* pCurKeyFrame = m_cAnimPlayer.GetCurFrameHandle(uiJoint);
		if(!pCurKeyFrame)
			continue;
		const CKeyFrame& bindPose = pModel->GetBindPose(uiJoint);

		// Get the transform for the current key frame and the bind pose
		const D3DXMATRIX& curPoseMat = pCurKeyFrame->GetWorldTransform();
		const D3DXMATRIX& bindMat = bindPose.GetWorldTransform();

		// Get the inverse of the bind pose transform
		D3DXMATRIX bindInverse;
		D3DXMatrixInverse(&bindInverse, nullptr, &bindMat);

		m_rgPreCalculatedPose[uiJoint] = bindInverse * curPoseMat;
	}
}

void CRenderInstanceAnimated::Render(int nCurShader, UINT uiPass)
{
	// Set the animation matrices
	const CModel* pModel = CAssetManager::GetModel(GetModelID());
	CAssetManager::ShaderSetMatrixArray(nCurShader, "gCurPoseMats", m_rgPreCalculatedPose, pModel->GetNumJoints());

	// Call the parent's Render method
	CRenderInstance::Render(nCurShader,uiPass);
}