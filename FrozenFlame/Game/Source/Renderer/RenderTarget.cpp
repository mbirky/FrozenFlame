#include "RenderTarget.h"
#include "View.h"

// Constructor
CRenderTarget::CRenderTarget(void)
{
	m_pTargetTexture = nullptr;
	m_pOldSurface = nullptr;
	m_pSurface = nullptr;
	m_dwTargetIndex = -1;
}

// Destructor
CRenderTarget::~CRenderTarget(void)
{
	if(m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = nullptr;
	}
	delete m_pSurface;
}

// Initializes this render target
void CRenderTarget::Create(UINT uiWidth, UINT uiHeight, D3DFORMAT d3dFormat)
{
	m_pTargetTexture = CView::sm_cRenderer.CreateTargetTexture(uiWidth, uiHeight, d3dFormat);

	// Get the top level surface of the texture we just created
	m_pTargetTexture->GetSurfaceLevel(0, &m_pSurface);
}

// Sets this as the current render target
void CRenderTarget::ActivateTarget(DWORD dwTargetIndex)
{
	// Save the target index and the old render target
	m_pOldSurface = CView::sm_cRenderer.GetRenderTarget(dwTargetIndex);

	// Set the target to the surface we stored in the create method
	CView::sm_cRenderer.SetRenderTarget(dwTargetIndex, m_pSurface);

	m_dwTargetIndex = dwTargetIndex;
}

// Reverts back to whatever target was valid before this one
void CRenderTarget::RevertTarget(void)
{
	CView::sm_cRenderer.SetRenderTarget(m_dwTargetIndex, m_pOldSurface);
	m_dwTargetIndex = 0;
}
