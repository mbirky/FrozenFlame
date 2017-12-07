#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <d3d9.h>
#include <d3dx9.h>

class CRenderTarget
{
	// The texture to render to
	IDirect3DTexture9*	m_pTargetTexture;
	// The surface of the render texture to use
	IDirect3DSurface9*	m_pSurface;
	// The surface that was valid before thid one
	IDirect3DSurface9*	m_pOldSurface;
	// The surface index this render target is using
	DWORD				m_dwTargetIndex;

public:
	// Constructor
	CRenderTarget(void);

	// Destructor
	~CRenderTarget(void);

	// Initializes this render target
	void Create(UINT uiWidth, UINT uiHeight, D3DFORMAT d3dFormat = D3DFMT_A2R10G10B10);

	// Sets this as the current render target
	void ActivateTarget(DWORD dwTargetIndex);

	// Reverts back to whatever target was valid before this one
	void RevertTarget(void);


	inline IDirect3DTexture9* GetTexture(void) const { return m_pTargetTexture; }
};

#endif