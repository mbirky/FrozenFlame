#include "PostProcessChain.h"

// Constructor
CPostProcessChain::CPostProcessChain(void)
	: m_pSrcTarget(nullptr), m_pDestTarget(nullptr)
{
	// We default the down sampling scale to 2
	m_uiScale = 2;
}

// Create the RenderTargets and any other needed initialization
void CPostProcessChain::Initialize(UINT uiWidth, UINT uiHeight)
{
	m_cFirstTarget.Create(uiWidth, uiHeight);
	m_cSecondTarget.Create(uiWidth, uiHeight);

	m_cFirstDownTarget.Create(uiWidth / m_uiScale, uiHeight / m_uiScale);
	m_cSecondDownTarget.Create(uiWidth / m_uiScale, uiHeight / m_uiScale);

	m_pSrcTarget = &m_cFirstTarget;
	m_pDestTarget = &m_cSecondTarget;
}

// Iterare through the effects using the source and destination target
// pointers and swapping
IDirect3DTexture9* CPostProcessChain::Run(CRenderTarget* pMainTarget)
{
	if(m_ProcessChain.empty())
		return pMainTarget->GetTexture();

	bool downScale = false;
	bool upScale = false;
	for(list<CPostProcess*>::iterator iter = m_ProcessChain.begin();
		iter != m_ProcessChain.end(); ++iter)
	{
		CPostProcess* curEffect = *iter;
		if(!curEffect) continue;

		// We should use the right render targets if we're down/up scaling
		if(curEffect->GetPostTargetState() == PTS_DOWNSCALE)
		{
			m_pDestTarget = &m_cFirstDownTarget;
			downScale = true;
		}
		else if(curEffect->GetPostTargetState() == PTS_UPSCALE)
		{
			m_pDestTarget = &m_cFirstTarget;
			upScale = true;
		}

		// If this is the first effect we're applying
		if(iter == m_ProcessChain.begin())
			curEffect->Process(m_pDestTarget, pMainTarget->GetTexture());
		else
			curEffect->Process(m_pDestTarget, m_pSrcTarget->GetTexture());

		// If we down/up scaled, we need to make we set the source targets
		// so they get swapped correctly after
		if(downScale)
		{
			m_pSrcTarget = &m_cSecondDownTarget;
			downScale = false;
		}
		else if(upScale)
		{
			m_pSrcTarget = &m_cSecondTarget;
			upScale = false;
		}

		// Swap source and destination target pointers
		std::swap(m_pDestTarget, m_pSrcTarget);
	}

	return m_pSrcTarget->GetTexture();
}

// Adds a post-process effect to the swap chain
void CPostProcessChain::AddPostProcess(CPostProcess* pEffect)
{
	m_ProcessChain.push_back(pEffect);
}

// Removes a post-process effect from the swap chain
void CPostProcessChain::RemovePostProcess(CPostProcess* pEffect)
{
	m_ProcessChain.remove(pEffect);
}

// Clears the list of post-process pointers
void CPostProcessChain::Clear(void)
{
	m_ProcessChain.clear();
}