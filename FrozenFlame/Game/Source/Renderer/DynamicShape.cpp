#include "DynamicShape.h"

#include "RenderInstance.h"
#include "AssetManager.h"
#include "View.h"

// Default constructor
CDynamicShape::CDynamicShape(const CRenderInstance* pParent, bool bAttachedRotation)
	: m_pParent(pParent), m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr),
	m_nCurDiffuseTex(-1), m_nCurSpecularTex(-1), m_d3dColor(1, 1, 1, 1),
	m_uiNumVerts(0), m_uiNumPrimitives(0), m_uiVertSize(0), m_bAttachedRotation(bAttachedRotation)
{
	D3DXMatrixIdentity(&m_d3dLocalTransform);
}

// Make this into a plane segment (basically a square in 3D space)
void CDynamicShape::MakePlane(float fWidth, float fDepth)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	float rgQuadVerts[] = {
		/* POSITION */						/* NORMALS */		/* UVS */
		-fHalfWidth, 0.0f, fHalfDepth,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-fHalfWidth, 0.0f, -fHalfDepth,		0.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		fHalfWidth, 0.0f, -fHalfDepth,		0.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		fHalfWidth, 0.0f, fHalfDepth,		0.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	};
	UINT rgQuadIndices[] = {
		0, 2, 1,
		0, 3, 2
	};
	CView::sm_cRenderer.CreateGeometryBuffers(&m_pVertexBuffer, &m_pIndexBuffer,
		rgQuadVerts, sizeof(rgQuadVerts), rgQuadIndices, sizeof(rgQuadIndices));

	m_uiNumVerts = 4;
	m_uiNumPrimitives = 2;
	m_uiVertSize = 8 * sizeof(float);
}

void CDynamicShape::SetLocalScale(const vec3f& scale)
{
	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity(&tempMatrix);
	tempMatrix._11 = scale.x;
	tempMatrix._22 = scale.y;
	tempMatrix._33 = scale.z;
	m_LocalScale = tempMatrix;
}

// Make this into a triangle
void CDynamicShape::MakeTriangle(float fWidth, float fDepth)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	float rgTriangleVerts[] = {
		/* POSITION */						/* NORMALS */			/* UVS */
		-fHalfWidth, 0.0f, fHalfDepth,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		0.0f, 0.0f, -fHalfDepth,			0.0f, 0.0f, 0.0f,		0.5f, 1.0f,
		fHalfWidth, 0.0f, fHalfDepth,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	};
	UINT rgTriangleIndices[] = {
		0, 2, 1,
	};
	CView::sm_cRenderer.CreateGeometryBuffers(&m_pVertexBuffer, &m_pIndexBuffer,
		rgTriangleVerts, sizeof(rgTriangleVerts), rgTriangleIndices, sizeof(rgTriangleIndices));

	m_uiNumVerts = 3;
	m_uiNumPrimitives = 1;
	m_uiVertSize = 8 * sizeof(float);
}

void CDynamicShape::RecalculateBounds(void) const
{
	if(m_pParent)
	{
		vec3f vWorldMin, vWorldMax;
		m_pParent->GetCollisionBounds(vWorldMin, vWorldMax);
		SetWorldMin(vWorldMin);
		SetWorldMax(vWorldMax);
	}
}


// Updates this object's world transform based on the parent's matrix
void CDynamicShape::Update(float fElapsedTime)
{
	IRenderThing::Update(fElapsedTime);

	if(m_pParent)
	{
		if(m_bAttachedRotation)
		{
			SetWorldTransform(m_d3dLocalTransform * m_pParent->GetWorldTransform());
		}
		else
		{
			D3DXVECTOR3 d3dParentScale;
			D3DXQUATERNION d3dParentRotation;
			D3DXVECTOR3 d3dParentTranslation;

			D3DXMatrixDecompose(&d3dParentScale, &d3dParentRotation, &d3dParentTranslation, &m_pParent->GetWorldTransform());

			D3DXMATRIX d3dTranslation;
			D3DXMatrixTranslation(&d3dTranslation, d3dParentTranslation.x, d3dParentTranslation.y, d3dParentTranslation.z);

			D3DXMATRIX d3dScale;
			D3DXMatrixScaling(&d3dScale, d3dParentScale.x, d3dParentScale.y, d3dParentScale.z);

			// scale the shape by a local scale
			D3DXMatrixMultiply(&d3dScale, &d3dScale, &m_LocalScale);			

			SetWorldTransform(m_d3dLocalTransform * (d3dScale * d3dTranslation));
		}
	}
	else
		SetWorldTransform(m_d3dLocalTransform);
}

// Apply the current textures for the passed in shader
void CDynamicShape::ApplyTextures(int nShader)
{
	if(m_nCurDiffuseTex != -1)
		CAssetManager::ApplyTexture(nShader, m_nCurDiffuseTex);
	
	if(m_nCurSpecularTex != -1)
		CAssetManager::ApplyTexture(nShader, m_nCurSpecularTex);
}

// Draws the dynamic shape with the passed in shader
void CDynamicShape::Render(int nCurShader , UINT uiPass)
{
	if(!m_pVertexBuffer || !m_pIndexBuffer)
		return;

	CAssetManager::ShaderBeginPass(nCurShader, uiPass);

	// Apply the current textures for the passed in shader
	if(m_nCurDiffuseTex != -1)
		CAssetManager::ApplyTexture(nCurShader, m_nCurDiffuseTex);
		//CAssetManager::ApplyTexture(nCurShader, "diffuseMap", m_nCurDiffuseTex);
	
	if(m_nCurSpecularTex != -1)
		CAssetManager::ApplyTexture(nCurShader, "specularMap", m_nCurSpecularTex);

	CAssetManager::ShaderCommitChanges(nCurShader);

	// Set stream source and indices
	CView::sm_cRenderer.SetStreamSource(m_pVertexBuffer, m_uiVertSize);
	CView::sm_cRenderer.SetIndices(m_pIndexBuffer);

	// Draw the shape
	CView::sm_cRenderer.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, m_uiNumVerts, m_uiNumPrimitives);

	CAssetManager::ShaderEndPass(nCurShader);
}

// Modify the local matrix
void CDynamicShape::Translate(bool bPost, float fX, float fY, float fZ)
{
	D3DXMATRIX d3dTranslate;
	D3DXMatrixTranslation(&d3dTranslate, fX, fY, fZ);

	if(bPost)
		m_d3dLocalTransform = m_d3dLocalTransform * d3dTranslate;
	else
		m_d3dLocalTransform = d3dTranslate * m_d3dLocalTransform;
}

void CDynamicShape::Rotate(bool bPost, float fX, float fY, float fZ)
{
	D3DXMATRIX d3dRotate;
	D3DXMatrixRotationYawPitchRoll(&d3dRotate, fY, fX, fZ);

	if(bPost)
		m_d3dLocalTransform = m_d3dLocalTransform * d3dRotate;
	else
		m_d3dLocalTransform = d3dRotate * m_d3dLocalTransform;
}

void CDynamicShape::Scale(bool bPost, float fX, float fY, float fZ)
{
	D3DXMATRIX d3dScale;
	D3DXMatrixScaling(&d3dScale, fX, fY, fZ);

	if(bPost)
		m_d3dLocalTransform = m_d3dLocalTransform * d3dScale;
	else
		m_d3dLocalTransform = d3dScale * m_d3dLocalTransform;
}

bool CDynamicShape::IsRenderable(bool bCollisionRenderer) const
{
	if(m_pParent)
	{
		return m_pParent->IsRenderable(bCollisionRenderer);
	}
	return false;
}