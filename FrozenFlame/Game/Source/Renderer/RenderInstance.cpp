#include "RenderInstance.h"
#include "../Objects/CollidableObject.h"
#include "../Util/Util.h"
#include "AssetManager.h"
#include "../Animations/Interpolator.h"
#include "Model.h"
#include "../Animations/Animation.h"
#include "Emitter.h"
#include "PointLight.h"
#include "../Objects/EnvironmentObject.h"
#include "../Objects/Enemy.h"
#include "../AI/Header/CFSM.h"
#include "../States/CVideoOptionsState.h"

#include <iostream>
using std::cout;

CRenderInstance::CRenderInstance(int nModelID, CBaseObject* pObject)
{
	// Set the model ID and the object
	m_nModelID = nModelID;
	m_pObject = pObject;

	// If object is a 2D effect, we don't have a model. - JC
	if(m_pObject && m_pObject->GetType() == TWO_D)
		return;

	// Recalculate our bounds
	CAssetManager::GetModelBounds(m_nModelID, m_vCenter, m_vExtent);
	RecalculateBounds();

	// First time object's bounds are calculated, take into account scaling. - JC
	if(m_pObject)
	{
		m_vExtent.x = m_vExtent.x * pObject->GetWorldMatrix()->xx;
		m_vExtent.y = m_vExtent.y * pObject->GetWorldMatrix()->yy;
		m_vExtent.z = m_vExtent.z * pObject->GetWorldMatrix()->zz;
	}
}

bool CRenderInstance::IsRenderable(bool bCollisionRenderer) const
{
	// if this render instance is not visible and we're not on the collision renderer it shouldn't be rendered
	// (if we were rendering collisions we would want to render it regardless)
	if(!IsVisible() && !bCollisionRenderer)
	{
		return false;
	}

	// If this object is a 3D particle, we should check if it's alive
	if(m_pObject && m_pObject->GetType() == THREE_D)
	{
		CEmitter::CParticle* pParticleObj = dynamic_cast<CEmitter::CParticle*>(m_pObject);
		if(!pParticleObj->GetAlive() || !pParticleObj->GetActive())
			return false;
	}

	// If we got here it should be rendered
	return true;
}

CRenderInstance::~CRenderInstance(void)
{
	m_pObject = nullptr;
}

void CRenderInstance::Update(float fElapsedTime)
{
	// Call the parent's update
	IRenderThing::Update(fElapsedTime);
	
	if(!m_pObject)
		return;

	// Calculate the d3d matrix
	D3DXMATRIX rot;
	D3DXMatrixRotationY(&rot, D3DXToRadian(180));

	D3DXMATRIX mat(m_pObject->GetWorldMatrix()->ma);

	if(m_pObject->GetType() == MELEE_OBJECT || m_pObject->GetType() == EXPLOSIVE_OBJECT || m_pObject->GetType() == RANGED_OBJECT || m_pObject->GetType() == FIRE_OBJECT || m_pObject->GetType() == ICE_OBJECT
		|| m_pObject->GetType() == WIZARD_OBJECT)
		mat = rot * mat;
	else if(m_pObject->GetType() == TWO_D)
		D3DXMatrixIdentity(&mat);
	SetWorldTransform(mat);
}

void CRenderInstance::Render(int nCurShader, UINT uiPass)
{
	CAssetManager::ShaderBeginPass(nCurShader, uiPass);

	// Removed anything having to do with 2D particles from here.  Don't need it. - JC
	if((GetObjectType() == THREE_D && !(CVideoOptionsState::GetInstance()->GetRenderFlags() & RF_PARTICLES)))
		return; // Early out if particles are off and it's a 3d emitter - JC

	// Grab the model of this object
	CModel* pModel = CAssetManager::GetModel(m_nModelID);

	// Get its array of meshes
	const vector<TMesh>& vtMeshes = pModel->GetMeshArray();
	size_t numMeshes = vtMeshes.size();

	// Loop through its meshes
	for(size_t idMesh = 0; idMesh < numMeshes; ++idMesh)
	{
		// For each mesh, apply textures and draw it
		const TMesh& curMesh = vtMeshes[idMesh];

		pModel->ApplyTextures(nCurShader, idMesh);

		CView::sm_cRenderer.SetStreamSource(curMesh.m_pVertexBuff, pModel->GetVertSize());
		CView::sm_cRenderer.SetIndices(curMesh.m_pIndexBuff);
		CAssetManager::ShaderCommitChanges(nCurShader);

		CView::sm_cRenderer.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, curMesh.m_uiNumVerts, curMesh.m_uiNumTris);
	}

	CAssetManager::ShaderEndPass(nCurShader);
}

void CRenderInstance::RecalculateBounds(void) const
{
	if(!m_pObject || !m_pObject->GetWorldMatrix())
		return;

	vec3f vCenterPoint, vMaxPoint;
	
	matrix4f worldMatrix = *m_pObject->GetWorldMatrix();
	if(GetFlags() & RENDER_FIXED_COLLISION)
	{
		worldMatrix.axis_x.normalize();
		worldMatrix.axis_y.normalize();
		worldMatrix.axis_z.normalize();
	}

	VectorByMatrixMult(vCenterPoint, m_vCenter, worldMatrix);
	VectorByMatrixMult(vMaxPoint, m_vCenter + m_vExtent, worldMatrix);

	vec3f vNewExtent( abs(vMaxPoint.x - vCenterPoint.x), abs(vMaxPoint.y - vCenterPoint.y), abs(vMaxPoint.z - vCenterPoint.z));
	
	SetWorldMin(vCenterPoint - vNewExtent);
	SetWorldMax(vCenterPoint + vNewExtent);
}


D3DXVECTOR4 CRenderInstance::GetEffectColor(void) const
{
	if(!m_pObject)
		return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	vec3f color = m_pObject->GetColor();

	return D3DXVECTOR4(color.x, color.y, color.z, m_pObject->GetTransparency());
}

const vec3f CRenderInstance::GetPosition(void) const
{
	vec3f vPos;

	if(m_pObject)
		vPos = m_pObject->GetWorldMatrix()->axis_pos;
	else
		vPos = vec3f(0.0f, 0.0f, 0.0f);

	return vPos;
}
