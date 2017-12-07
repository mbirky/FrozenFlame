#include "RenderInstanceAttached.h"
#include "AssetManager.h"
#include "RenderInstanceAnimated.h"
#include "Model.h"

CRenderInstanceAttached::CRenderInstanceAttached(int nModelID, const CRenderInstance* pOwner, const string& szAttachmentJoint)
	: CRenderInstance(nModelID, nullptr), m_pOwner(pOwner), m_bEnabled(true)
{
	const CModel* pAttModel = CAssetManager::GetModel(GetModelID());
	m_nAnchorJointID = pAttModel->GetJointID(szAttachmentJoint);
}

void CRenderInstanceAttached::Update(float fElapsedTime)
{
	// Call the parent's update
	CRenderInstance::Update(fElapsedTime);

	// If we don't have owner we shouldn't continue
	if(!m_pOwner)
		return;

	// Calculate this attachment's matrix based on it's owner
	// Initialize the attachment matrix to its owner's matrix
	D3DXMATRIX d3dAttachmentMatrix = m_pOwner->GetWorldTransform();

	// Get the owner's model
	const CModel* pOwnerModel = CAssetManager::GetModel(m_pOwner->GetModelID());

	// If our owner has a valid anchor joint, multiply the attachment matrix by the joint matrix
	if(m_pOwner->GetType() == ANIMATED)
	{
		const CRenderInstanceAnimated* pAnimOwner = dynamic_cast<const CRenderInstanceAnimated*>(m_pOwner);

		if(pAnimOwner)
		{
			int nOwnerAnchorJoint = pAnimOwner->GetAnchorJoint();
			const CAnimPlayer* pOwnerAnimPlayer = pAnimOwner->GetAnimPlayer();

			if(nOwnerAnchorJoint != -1 && pOwnerAnimPlayer)
			{
				const CKeyFrame* pKeyFrame;
				if(pOwnerAnimPlayer->IsPlaying())
					pKeyFrame = pOwnerAnimPlayer->GetCurFrameHandle(nOwnerAnchorJoint);
				else
					pKeyFrame = &pOwnerModel->GetBindPose(nOwnerAnchorJoint);

				d3dAttachmentMatrix = pKeyFrame->GetWorldTransform() * d3dAttachmentMatrix;

				CModel* pAttModel = CAssetManager::GetModel(GetModelID());
				if(m_nAnchorJointID != -1 && pAttModel)
				{
					const CKeyFrame* pAttKeyFrame = &pAttModel->GetBindPose(m_nAnchorJointID);

					D3DXMATRIX d3dJointInverse;
					D3DXMatrixInverse(&d3dJointInverse, nullptr, &pAttKeyFrame->GetWorldTransform());
					d3dAttachmentMatrix = d3dJointInverse * d3dAttachmentMatrix;
				}

			}
		}
	}


	SetWorldTransform(d3dAttachmentMatrix);
}

bool CRenderInstanceAttached::IsRenderable(bool bCollisionRenderer) const
{
	return CRenderInstance::IsRenderable(bCollisionRenderer) && m_bEnabled; 
}

D3DXVECTOR4 CRenderInstanceAttached::GetEffectColor(void) const
{
	if(m_pOwner)
		return m_pOwner->GetEffectColor();
	return CRenderInstance::GetEffectColor();
}