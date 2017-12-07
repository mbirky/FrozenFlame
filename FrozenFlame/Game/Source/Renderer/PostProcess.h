#ifndef _POST_PROCESS_H_
#define _POST_PROCESS_H_

#include <d3dx9.h>

#include <string>
using std::string;

// Forward declarations
class CRenderTarget;

enum EPostTargetState {
		PTS_DEFAULT = 0,
		PTS_DOWNSCALE,
		PTS_UPSCALE
	};

class CPostProcess
{
	// Id of the shader effect used by this post process effect (from asset manager)
	int m_nShaderID;
	string m_szTechnique;

	// Handle to the texture defining the image to run the post-process
	// effect on
	D3DXHANDLE m_hTargetTexture;

	// The mesh to run this post-process effect in (a quad)
	IDirect3DVertexBuffer9*			m_pScreenQuadVertices;
	IDirect3DIndexBuffer9*			m_pScreenQuadIndices;
	IDirect3DVertexDeclaration9*	m_pScreenQuadDecl;
	UINT m_uiScreenQuadVertSize;

	EPostTargetState m_ePostTargetState;

public:
	// Constructor
	CPostProcess(IDirect3DVertexDeclaration9* pScreenQuadDecl, UINT uiVertSize,
		IDirect3DVertexBuffer9* pVertices, IDirect3DIndexBuffer9* pIndices,
		const char* szEffectFile, const char* szTechnique, EPostTargetState ePTS = PTS_DEFAULT);

	// Executes the post process effect
	void Process(CRenderTarget* pDestTarget, IDirect3DTexture9* pSrcTex);

	// Accessors
	inline int GetShaderID(void) const { return m_nShaderID; }
	inline const D3DXHANDLE GetTargetTexture(void) const { return m_hTargetTexture; }
	inline EPostTargetState GetPostTargetState(void) const { return m_ePostTargetState; }
};


#endif