#ifndef _POSTPROCESS_CHAIN_H_
#define _POSTPROCESS_CHAIN_H_

#include "RenderTarget.h"
#include "PostProcess.h"
#include <list>
using std::list;

class CPostProcessChain
{
	// The list of post process effects to be run
	list<CPostProcess*> m_ProcessChain;

	// This class will create two RenderTarget instances.
	// When processing the effects, it will swap between these two
	// RenderTargets as the source and destination objects for the effects.
	CRenderTarget m_cFirstTarget;
	CRenderTarget m_cSecondTarget;

	// The render targets to use when down/up sampling
	CRenderTarget m_cFirstDownTarget;
	CRenderTarget m_cSecondDownTarget;

	// The scale to sample up/down for
	UINT m_uiScale;

	// The class keeps a source and destination pointers to make
	// the swapping lesst costly.
	CRenderTarget* m_pDestTarget;
	CRenderTarget* m_pSrcTarget;

public:
	// Constructor
	CPostProcessChain(void);
	
	// Create the RenderTargets and any other needed initialization
	void Initialize(UINT uiWidth, UINT uiHeight);

	// Iterare through the effects using the source and destination target
	// pointers and swapping
	IDirect3DTexture9* Run(CRenderTarget* pMainTarget);

	// Adds a post-process effect to the swap chain
	void AddPostProcess(CPostProcess* pEffect);

	// Removes a post-process effect from the swap chain
	void RemovePostProcess(CPostProcess* pEffect);

	// Clears the list of post-process pointers
	void Clear(void);
};

#endif