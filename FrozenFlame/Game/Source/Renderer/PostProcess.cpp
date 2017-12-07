#include "PostProcess.h"
#include "AssetManager.h"
#include "RenderTarget.h"
#include "View.h"

// Constructor
CPostProcess::CPostProcess(IDirect3DVertexDeclaration9* pScreenQuadDecl, UINT uiVertSize,
		IDirect3DVertexBuffer9* pVertices, IDirect3DIndexBuffer9* pIndices,
		const char* szEffectFile, const char* szTechnique, EPostTargetState ePTS)
		: m_ePostTargetState(ePTS)
{
	m_pScreenQuadVertices = pVertices;
	m_pScreenQuadIndices = pIndices;

	m_pScreenQuadDecl = pScreenQuadDecl;
	m_uiScreenQuadVertSize = uiVertSize;

	m_nShaderID = CAssetManager::LoadShader(szEffectFile);
	m_szTechnique = szTechnique;

	m_hTargetTexture = CAssetManager::GetShaderParameter(m_nShaderID, nullptr, "sourceTexture");
}


// Executes the post process effect
void CPostProcess::Process(CRenderTarget* pDestTarget, IDirect3DTexture9* pSrcTex)
{
	// Set the render target
	if(pDestTarget)
		pDestTarget->ActivateTarget(0);

	CAssetManager::ShaderSetTechnique(m_nShaderID, m_szTechnique.c_str());

	// Clear the render target
	CView::sm_cRenderer.Clear(D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1, 0);

	// Set the vertex information (vertex decl, stream source)
	CView::sm_cRenderer.SetVertexDeclaration(m_pScreenQuadDecl);
	CView::sm_cRenderer.SetStreamSource(m_pScreenQuadVertices, m_uiScreenQuadVertSize);

	// Set the index stream
	CView::sm_cRenderer.SetIndices(m_pScreenQuadIndices);

	// Set the shader variables (source texture)
	CAssetManager::ShaderSetTexture(m_nShaderID, m_hTargetTexture, pSrcTex);
	CAssetManager::ShaderCommitChanges(m_nShaderID);

	// Start the effect
	UINT uiNumPasses = CAssetManager::ShaderBegin(m_nShaderID);
	for(UINT uiPass = 0; uiPass < uiNumPasses; ++uiPass)
	{
		CAssetManager::ShaderBeginPass(m_nShaderID, uiPass);
		// Draw indexed primitive (a quad)
		CView::sm_cRenderer.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			4, 2);
		CAssetManager::ShaderEndPass(m_nShaderID);
	}
	// End the effect
	CAssetManager::ShaderEnd(m_nShaderID);

	// Revert target
	if(pDestTarget)
		pDestTarget->RevertTarget();
}
