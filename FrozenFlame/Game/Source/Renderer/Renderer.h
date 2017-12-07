/********************************************************************
* Filename:			Renderer.h
* Date:				10/15/2012
* Mod. Date:		11/12/2012
* Mod. Initials:	DL
* Author:			Daniel Lima
* Purpose:			Class that initializes Direct3D and works as an
*					interface to most API specific rendering code.
*					Renders all the instances in the game, making use
*					of the Shader, Texture and Animation Managers.
********************************************************************/

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Globals.h"

// Direct3D 9 Libraries
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Camera.h"
#include "RenderDefinitions.h"
#include "RenderTarget.h"
#include "PostProcessChain.h"

#include "../Util/PriorityQueue.h"
using namespace fullsail_ai;

// Forward declarations
class IRenderThing;
class CDynamicShape;
class CPointLight;
class CDirLight;

// Renderer class
class CRenderer
{
	// Direct3D Object and Device
	IDirect3D9*		 				m_pD3D;
	IDirect3DDevice9*				m_pDevice;

	// Direct3DX Sprite Manager and Effect Pool
	ID3DXSprite*					m_pSpriteMng;
	ID3DXEffectPool*				m_pEffectPool;

	// Vertex declarations
	IDirect3DVertexDeclaration9*	m_pVertexDecl;		// Static objects
	IDirect3DVertexDeclaration9*	m_pAnimatedDecl;	// Animated objects
	IDirect3DVertexDeclaration9*	m_pDebugDecl;		// Debug info
	IDirect3DVertexDeclaration9*	m_pParticleDecl;	// 2d Particles

	// Window dimensions
	LONG							m_nWindowWidth;
	LONG							m_nWindowHeight;

	// Camera that holds our View and Projection matrices
	CCamera							m_cCamera;

	// List of render instances that exist in the game
	list<IRenderThing*>				m_listRenderInstances;

	// Sorted queue of visible render instances
	PriorityQueue<IRenderThing*>	m_VisibleSet;

	// Shaders
	int								m_nDebugShader;			// Debug info
	int								m_nGeometryShader;		// Deferred GBuffers
	int								m_nFinalShader;			// Forward pass to screen quad
	int								m_nParticleShader;		// Point sprites

	// Screen quad for final, forward rendering of the scene
	IDirect3DVertexDeclaration9*	m_pQuadDecl;
	IDirect3DVertexBuffer9*			m_pScreenQuadVertices;
	IDirect3DIndexBuffer9*			m_pScreenQuadIndices;

	// All our deferred render targets
	CRenderTarget					m_cDepthTarget;			// Depth buffer
	CRenderTarget					m_cDiffuseTarget;		// Diffuse texture color
	CRenderTarget					m_cNormalTarget;		// Vertex normals
	CRenderTarget					m_cSpecularTarget;		// Specular texture color
	CRenderTarget					m_cSceneTarget;			// Final, lit scene for forward pass
	CRenderTarget					m_cRageEffectTarget;	// Bright coloring for rage effect

	// Deferred lights
	CDirLight*						m_pDirLight;			// World directional light
	vector<CPointLight*>			m_vpPointLights;		// Array of point lights

	// Post process effects
	CPostProcessChain				m_cPostProcessChain;	// Chain of post process effects
	vector<CPostProcess*>			m_vpPostProcesses;		// Keeps track of created post process effects
	bool							m_bPostProcessActive;	// Determines if post process is active or not

	// Timer used for scrolling UVs
	float m_fTimer;

	// Enum passed to render calls representing which render pass the Renderer is currently executing
	enum ERenderPass { RPASS_GBUFFERS, RPASS_SHADOW, RPASS_RAGE_EFFECT, RPASS_MENU };

	// Private functions
	void CreateGBuffers(void);								// Initializes all the Geometry Buffers
	void RenderGBuffers(void);								// Renders scene to geometry buffers (depth, normal, diffuse, specular)
	void RenderDirLightDepths(void);						// Renders depths for the directional light (shadows)
	void RenderLights(void);								// Renders scene to light buffer
	void RenderForwardPass(void);							// Renders final pass to the screen
	void RenderGameplay(ERenderPass ePass);					// Iterates through the list of render things and renders all of them
	void RenderOneThing(IRenderThing* pThing, int nShader, unsigned nNumPasses); 	// Renders one thing with the passed in shader
	
	// Interprets the passed in flags for the current render pass
	// Returns a shader ID and a technique name as output parameters
	void InterpretFlags(ERenderPass ePass, unsigned short ucFlags, int& nShader, const char*& szTechnique);

	// Function used to sort the visible set
	static bool CompareRenderThings(IRenderThing* const& lhs, IRenderThing* const& rhs);

	void RemoveInstanceLights(const IRenderThing* pInst);	// Removes all the lights attached to the passed in RenderThing
	void RemoveInstanceAttachments(const IRenderThing* pInst);	// Removes all the attachments attached to the passed in RenderThing

public:
	int AddPointLight(CPointLight* pLight);
	void RemovePointLight(int nLightID);
	CPointLight* GetPointLight(int nLightID);
	void RenderInvisibleWalls(vector<AABB>& vWalls);
	CPostProcess* AddPostProcess(const char* szShader, const char* szTechnique, EPostTargetState ePTS = PTS_DEFAULT);
	void RemovePostProcess(CPostProcess* pEffect);
	IRenderThing* GetRenderThing(int nID) { return reinterpret_cast<IRenderThing*>(nID); }

	/********************************************************************
	* CRenderer():			Default constructor - sets all the member variables
	*						to default safe values
	* Ins:					void
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	CRenderer(void);

	/********************************************************************
	* Initialize():			Initializes the D3D Object, Device, Sprite Manager
	*						and Camera based on the passed in parameters
	* Ins:					hWindow - handle to the window we're rendering to
	*						bWindowed - true for windowed, false for fullscreen
	*						bVSync - should vsync be on
	* Returns:				A value from EViewCodes that informs if the function
	*						succeeded or failed on initializing the Renderer
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	int Initialize(HWND hWindow, bool bWindowed, bool bVSync, int nClientWidth, int nClientHeight);

	/********************************************************************
	* Update():				Updates all the Render instances and the Camera
	* Ins:					fElapsedTime - time elapsed since last frame
	* Returns:				void
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	void Update(float fElapsedTime);

	/********************************************************************
	* Render():		Loops through all the render instances and calls
	*						Render() on all of them
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	void Render(void);
	void RenderMenuModels(int nBackgroundID);

	/********************************************************************
	* RenderCollisionBounds(): Renders all the collision AABBs and spheres
	*						for the render instances
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	void RenderCollisionBounds(bool bAABB);


	/********************************************************************
	* CreateGeometryBuffers(): Creates a vertex and an index buffer and fills
							them with the passed in data
	* Ins:					pVertSrc - data for vertices
	*						uiVertLen - number of bytes to copy from VertSrc
	*						pIndexSrc - data for indices
	*						uiIndexLen - number of bytes to copy from IndexSrc
	* Outs:					pOutVertBuff - the created vertex buffer
	*						pOutIndexBuff - the created index buffer
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	void CreateGeometryBuffers(IDirect3DVertexBuffer9** pOutVertBuff, IDirect3DIndexBuffer9** pOutIndexBuff,
		void* pVertSrc, UINT uiVertLen, void* pIndexSrc, UINT uiIndexLen);

	/********************************************************************
	* CreateDynamicBuffer(): Creates a dynamic buffer and returns it
							
	* Ins:					
	*						
	*						
	*						
	* Outs:					
	*						
	* Returns:				void
	* Mod. Date:			2/26/2013
	* Mod. Initials:		JC
	********************************************************************/
	void CreateDynamicBuffer(IDirect3DVertexBuffer9** pOutVertBuff, UINT uiVertLen);

	/********************************************************************
	* AddRenderInstance():	Adds a render instance to the vector to be rendered
	* Ins:					pInst - the instance to be added
	* Returns:				The ID of the instance on the vector or -1 if failed
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	int AddRenderInstance(IRenderThing* pInst);

	/********************************************************************
	* RemoveRenderInstance(): Deletes the render instance of the passed ID
	* Ins:					nRenderInstID - the ID of the instance
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	void RemoveRenderInstance(int nRenderInstID);

	/********************************************************************
	* ClearAllInstances():	Clears the vector of render instances
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	void ClearAllInstances(void);

	/********************************************************************
	* ~CRenderer():			Cleans up any allocated memory
	* Ins:					void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	~CRenderer(void);
	
	/********************************************************************
	* ToScreenSpace():		Gets 2d screen space position given a 3d
	*						world space position.
	* Ins:					fInX, fInY, fInZ - position to convert
	*						(in world space)
	* Outs:					nOutX, nOutY - converted position
	*						(in screen space)
	* Mod. Date:			1/1/2013
	* Mod. Initials:		DL
	********************************************************************/
	void ToScreenSpace(float fInX, float fInY, float fInZ, int& nOutX, int& nOutY);

	/********************************************************************
						DIRECT3D9 INTERFACE
		Those functions wrap all the Direct3d9 functionality that
		depends on the device, so that we don't need to let other
		rendering-related modules access the D3D Device.
	********************************************************************/
	void Clear(DWORD dwFlags, D3DCOLOR d3dColor, float fDepth, DWORD dwStencil)
	{ m_pDevice->Clear(0, nullptr, dwFlags, d3dColor, fDepth, dwStencil); }
	
	void BeginScene(void)
	{ m_pDevice->BeginScene(); }

	void BeginSprite(void)
	{ m_pSpriteMng->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT); }
	
	void EndSprite(void)
	{ m_pSpriteMng->End(); }
	
	void EndScene(void)
	{ m_pDevice->EndScene(); }
	
	void Present(void)
	{ m_pDevice->Present(0, 0, 0, 0); }
	
	void SetSpriteTransform(D3DXMATRIX* pMatrix)
	{ m_pSpriteMng->SetTransform(pMatrix); }
	
	void Draw(IDirect3DTexture9* pTexture, const RECT* pSrcRect, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition, D3DXCOLOR d3dColor)
	{ m_pSpriteMng->Draw(pTexture, pSrcRect, pCenter, pPosition, d3dColor); }
	
	void SetStreamSource(IDirect3DVertexBuffer9* pBuff, UINT uiVertSize)
	{ m_pDevice->SetStreamSource(0, pBuff, 0, uiVertSize); }
	
	void SetIndices(IDirect3DIndexBuffer9* pBuff)
	{ m_pDevice->SetIndices(pBuff); }

	void DrawIndexedPrimitive(D3DPRIMITIVETYPE d3dType, UINT uiNumVerts, UINT uiPrimCount)
	{ m_pDevice->DrawIndexedPrimitive(d3dType, 0, 0, uiNumVerts, 0, uiPrimCount); }

	void DrawPrimitive(D3DPRIMITIVETYPE d3dType, UINT uiPrimCount)
	{ m_pDevice->DrawPrimitive(d3dType,0,uiPrimCount); }

	void SetParticleDecl()
	{m_pDevice->SetVertexDeclaration(m_pDebugDecl);}

	void SetGameplayDecl()
	{m_pDevice->SetVertexDeclaration(m_pVertexDecl);}

	void SetVertexDeclaration(IDirect3DVertexDeclaration9* pVertDecl)
	{ m_pDevice->SetVertexDeclaration(pVertDecl); }

	IDirect3DTexture9* LoadTexture(const char* szFilename)
	{
		IDirect3DTexture9* texture;
		if(D3D_OK == D3DXCreateTextureFromFile(m_pDevice, szFilename, &texture))
			return texture;
		else
			return nullptr;
	}

	ID3DXEffect* LoadEffect(const char* szFilename)
	{
		ID3DXEffect* effect;

		ID3DXBuffer *pBuffer = NULL;

		D3DXCreateEffectFromFile(
			m_pDevice, szFilename, 0, 0,
			0, m_pEffectPool, &effect, &pBuffer);
		
		if(pBuffer)
		{
			LOG((char*)pBuffer->GetBufferPointer());
		}
		
		return effect;
	}

	ID3DXMesh* CreateSphere(float fRadius, UINT unSlices = 25, UINT unStacks = 25)
	{
		ID3DXMesh* pSphere;
		D3DXCreateSphere(m_pDevice, fRadius, unSlices, unStacks, &pSphere, nullptr);
		return pSphere;
	}

	ID3DXMesh* CreateBox(float fWidth, float fHeight, float fDepth)
	{
		ID3DXMesh* pBox;
		D3DXCreateBox(m_pDevice, fWidth, fHeight, fDepth, &pBox, nullptr);
		return pBox;
	}

	IDirect3DTexture9* CreateTargetTexture(UINT uiWidth, UINT uiHeight,
		D3DFORMAT d3dFormat)
	{
		IDirect3DTexture9* texture = nullptr;
		D3DXCreateTexture(m_pDevice, uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET,
			d3dFormat, D3DPOOL_DEFAULT, &texture);
		return texture;
	}

	void SetRenderTarget(DWORD dwTargetNo, IDirect3DSurface9* pSurface)
	{
		m_pDevice->SetRenderTarget(dwTargetNo, pSurface);
	}
	IDirect3DSurface9* GetRenderTarget(DWORD dwTargetNo)
	{
		IDirect3DSurface9* target = nullptr;
		m_pDevice->GetRenderTarget(dwTargetNo, &target);
		return target;
	}

	void FlushSprites(void)
	{
		m_pSpriteMng->Flush();
	}

	DWORD GetRenderState(D3DRENDERSTATETYPE d3dType)
	{
		DWORD dwRenderState;
		m_pDevice->GetRenderState(d3dType, &dwRenderState);
		return dwRenderState;
	}

	void SetRenderState(D3DRENDERSTATETYPE d3dType, DWORD dwValue)
	{
		m_pDevice->SetRenderState(d3dType, dwValue);
	}

	inline void SetPostProcessActive(bool bActive) { m_bPostProcessActive = bActive; }

	/***************
	*	Accessors
	***************/
	inline CCamera& GetCamera(void)				{ return m_cCamera; }
	inline LONG GetScreenWidth(void) const		{ return m_nWindowWidth; }
	inline LONG GetScreenHeight(void) const		{ return m_nWindowHeight; }
};


#endif // _RENDERER_H_