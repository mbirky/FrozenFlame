////////////////////////////////////////////////////////////////////////////
//	Renderer.h & Renderer.cpp
//		Singleton that initializes Direct3D and calls the render processes
//		for all objects that are passed to its Render method in a set.
//
//	Author:					Daniel Lima - August 2012
/////////////////////////////////////////////////////////////////////////////

#include "Renderer.h"
#include "RenderInstanceAnimated.h"
#include "RenderInstanceAttached.h"
#include "AssetManager.h"
#include "../Objects/PlayerObject.h"
#include "../Animations/Animation.h"
#include "DirLight.h"
#include "PointLight.h"
#include "../Core/CWorldManager.h"
#include "DynamicShape.h"
#include "PostProcess.h"

CRenderer::CRenderer(void)
	: m_VisibleSet(PriorityQueue<IRenderThing*>(CompareRenderThings)),
	m_pD3D(nullptr), m_pDevice(nullptr), m_nWindowWidth(0), m_nWindowHeight(0),
	m_pSpriteMng(nullptr), m_pEffectPool(nullptr), m_pDirLight(nullptr), m_fTimer(0.0f), m_bPostProcessActive(true)
{
}

int CRenderer::Initialize(HWND hWindow, bool bWindowed, bool bVSync, int nClientWidth, int nClientHeight)
{
	// Get the window dimensions
	m_nWindowWidth = nClientWidth;
	m_nWindowHeight = nClientHeight;

	// Create the Direct3D object
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_pD3D) return VIEW_INIT_FAILED;
	// Create the Device
	D3DPRESENT_PARAMETERS presentParams;

	ZeroMemory(&presentParams, sizeof(D3DPRESENT_PARAMETERS));
	presentParams.hDeviceWindow = hWindow;
	presentParams.Windowed = bWindowed;
	presentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	presentParams.EnableAutoDepthStencil = true;
	presentParams.PresentationInterval =
		bVSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT result = m_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, hWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParams, &m_pDevice);

	if(result != D3D_OK)
		return VIEW_INIT_FAILED;

	// Create sprite manager
	D3DXCreateSprite(m_pDevice, &m_pSpriteMng);
	D3DXCreateEffectPool(&m_pEffectPool);

	UpdateWindow(hWindow);

	// Initialize vertex declaration
	_D3DVERTEXELEMENT9 VertPosNorTex[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(VertPosNorTex, &m_pVertexDecl);

	_D3DVERTEXELEMENT9 VertAnimated[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(VertAnimated, &m_pAnimatedDecl);

	_D3DVERTEXELEMENT9 VertPos[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(VertPos, &m_pDebugDecl);

	_D3DVERTEXELEMENT9 VertPosTex[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(VertPosTex, &m_pQuadDecl);

	_D3DVERTEXELEMENT9 VertParticle[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(VertParticle, & m_pParticleDecl);

	// Initialize camera
	m_cCamera.Initialize(m_nWindowWidth, m_nWindowHeight);

	CreateGBuffers();

	return VIEW_SUCCEEDED;
}

void CRenderer::CreateGBuffers(void)
{
	IDirect3DSurface9* pOldRT;
	D3DSURFACE_DESC desc;
	m_pDevice->GetRenderTarget(0, &pOldRT);
	pOldRT->GetDesc(&desc);

	if(pOldRT)
	{
		pOldRT->Release();
		pOldRT = nullptr;
	}

	m_cDepthTarget.Create(desc.Width, desc.Height, D3DFMT_R32F);
	m_cDiffuseTarget.Create(desc.Width, desc.Height, D3DFMT_A8R8G8B8);
	m_cNormalTarget.Create(desc.Width, desc.Height);
	m_cSpecularTarget.Create(desc.Width, desc.Height);
	m_cSceneTarget.Create(desc.Width, desc.Height, D3DFMT_A8R8G8B8);
	m_cRageEffectTarget.Create(desc.Width, desc.Height, D3DFMT_A8R8G8B8);

	m_nGeometryShader	= CAssetManager::LoadShader("GBuffers.fx");
	m_nFinalShader		= CAssetManager::LoadShader("ForwardPass.fx");
	m_nDebugShader		= CAssetManager::LoadShader("Debug.fx");
	m_nParticleShader	= CAssetManager::LoadShader("Particles.fx");

	CAssetManager::ShaderSetTexture(m_nGeometryShader, "depthGBuffer",		m_cDepthTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nGeometryShader, "diffuseGBuffer",	m_cDiffuseTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nGeometryShader, "specularGBuffer",	m_cSpecularTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nGeometryShader, "normalGBuffer",		m_cNormalTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nGeometryShader, "lightBuffer",		m_cSceneTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nGeometryShader, "effectGBuffer",		m_cRageEffectTarget.GetTexture());

	CAssetManager::ShaderSetTexture(m_nParticleShader, "depthGBuffer",		m_cDepthTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nParticleShader, "diffuseGBuffer",	m_cDiffuseTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nParticleShader, "specularGBuffer",	m_cSpecularTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nParticleShader, "normalGBuffer",		m_cNormalTarget.GetTexture());
	CAssetManager::ShaderSetTexture(m_nParticleShader, "lightBuffer",		m_cSceneTarget.GetTexture());

	D3DXVECTOR2 vHalfPixel(0.5f / desc.Width, 0.5f / desc.Height);
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gHalfPixel", &vHalfPixel, sizeof(D3DXVECTOR2));
	CAssetManager::ShaderSetValue(m_nParticleShader, "gHalfPixel", &vHalfPixel, sizeof(D3DXVECTOR2));

	float rgScreenQuadVerts[] = {
		/* POSITION */			/* UVS */
		-1.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	};
	UINT rgScreenQuadIndices[] = {
		0, 2, 1,
		0, 3, 2
	};
	CreateGeometryBuffers(&m_pScreenQuadVertices, &m_pScreenQuadIndices,
		rgScreenQuadVerts, sizeof(rgScreenQuadVerts), rgScreenQuadIndices, sizeof(rgScreenQuadIndices));

	m_pDirLight = new CDirLight(true, D3DXVECTOR3(0.3f, -0.5f, 0.3f),
		D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 0.7f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 48.0f, 0.5f);
	m_pDirLight->CreateRenderTarget(desc.Width, desc.Height, D3DFMT_R32F);

	m_pDirLight->BuildInitialMatrices(m_cCamera.GetRadius(), m_cCamera.GetCenterPos());

	// Initialize the post process chain with the width and the height of the screen surface
	m_cPostProcessChain.Initialize(desc.Width, desc.Height);
}

CPostProcess* CRenderer::AddPostProcess(const char* szShader, const char* szTechnique, EPostTargetState ePTS)
{
	// Create the post process effect
	CPostProcess* pEffect = new CPostProcess(m_pQuadDecl, 5 * sizeof(float),
		m_pScreenQuadVertices, m_pScreenQuadIndices, szShader, szTechnique, ePTS);

	// Add it to the post process chain
	m_cPostProcessChain.AddPostProcess(pEffect);

	// Add it to the Renderer's vector of Post Processes (just so we can clean it later)
	m_vpPostProcesses.push_back(pEffect);

	return pEffect;
}

void CRenderer::RemovePostProcess(CPostProcess* pEffect)
{
	m_cPostProcessChain.RemovePostProcess(pEffect);
}


int CRenderer::AddRenderInstance(IRenderThing* pInst)
{
	m_listRenderInstances.push_back(pInst);
	return reinterpret_cast<int>(pInst);
}

void CRenderer::Update(float fElapsedTime)
{
	// Increase our timer
	m_fTimer += fElapsedTime;


	// Let's build our visible set while we update all render instances
	m_VisibleSet.clear();

	for(list<IRenderThing*>::iterator iter = m_listRenderInstances.begin();
		iter != m_listRenderInstances.end(); ++iter)
	{
		IRenderThing* pThing = *iter;
		if(pThing)
		{
			pThing->Update(fElapsedTime);

			if(m_cCamera.IsObjectInside(pThing) && pThing->IsRenderable(false))
				m_VisibleSet.push(pThing);
		}
	}

	// Update the camera
	m_cCamera.Update(fElapsedTime);
	
	// Update the directional light's matrices
#ifndef CHEAP_RENDERING
	m_pDirLight->BuildViewProjection(m_cCamera.GetRadius(), m_cCamera.GetCenterPos());
#endif
}

void CRenderer::Render(void)
{
	// Those uniforms are the same for a frame for all the shaders and passes that we're using so they can be set in here
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gCameraPos", m_cCamera.GetPosition(), sizeof(D3DXVECTOR3));
	CAssetManager::ShaderSetMatrix(m_nGeometryShader, "gInvViewProj", &m_cCamera.GetInvViewProjection());
	CAssetManager::ShaderSetMatrix(m_nGeometryShader, "gViewProj", &m_cCamera.GetViewProjection());
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gGammaValue", &CGame::GetInstance()->GetGammaValue(), sizeof(float));
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gTime", &m_fTimer, sizeof(float));

	// Render all our geometry buffers
	RenderGBuffers();

#ifndef CHEAP_RENDERING
	// Render the depths for our directional light so that we can render shadows later
	RenderDirLightDepths();
	// Renedr our directional light and all the point lights
	RenderLights();
#endif

	// Run post process and render our final pass to the screen
	RenderForwardPass();
}

void CRenderer::RenderMenuModels(int nBackgroundID)
{
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gCameraPos", m_cCamera.GetPosition(), sizeof(D3DXVECTOR3));
	CAssetManager::ShaderSetMatrix(m_nGeometryShader, "gInvViewProj", &m_cCamera.GetInvViewProjection());
	CAssetManager::ShaderSetMatrix(m_nGeometryShader, "gViewProj", &m_cCamera.GetViewProjection());
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gGammaValue", &CGame::GetInstance()->GetGammaValue(), sizeof(float));

	// Activate render targets
	m_cDiffuseTarget.ActivateTarget(0);
	m_cDepthTarget.ActivateTarget(3);

	// Clear and render geometry buffers
	CAssetManager::ShaderSetValue(m_nGeometryShader, "gTime", &m_fTimer, sizeof(float));
	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	RenderGameplay(RPASS_MENU);

	// Revert render targets
	m_cDepthTarget.RevertTarget();
	m_cDiffuseTarget.RevertTarget();

	// Run post process and render our final pass to the screen
	m_pDevice->SetVertexDeclaration(m_pQuadDecl);
	IDirect3DTexture9* hFinalSceneTexture = m_cDiffuseTarget.GetTexture();
	if(!hFinalSceneTexture)
		return;

	// Clear all the buffers
	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	CAssetManager::ShaderSetTechnique(m_nFinalShader, "Final");

	UINT numPasses = CAssetManager::ShaderBegin(m_nFinalShader);
	for(UINT uiPass = 0; uiPass < numPasses; ++uiPass)
	{
		CAssetManager::ShaderBeginPass(m_nFinalShader, uiPass);
		CAssetManager::ShaderSetValue(m_nFinalShader, "gPlayerPos", m_cCamera.GetPlayerPos(), sizeof(D3DXVECTOR3));
		CAssetManager::ApplyTexture(m_nFinalShader, "backgroundMap", nBackgroundID);
		CAssetManager::ShaderSetTexture(m_nFinalShader, "sceneMap", hFinalSceneTexture);
		CAssetManager::ShaderCommitChanges(m_nFinalShader);

		SetStreamSource(m_pScreenQuadVertices, sizeof(float) * 5);
		SetIndices(m_pScreenQuadIndices);
		DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 2);

		CAssetManager::ShaderEndPass(m_nFinalShader);
	}
	CAssetManager::ShaderEnd(m_nFinalShader);
}

void CRenderer::RenderGBuffers(void)
{
	// Activate render targets
	m_cDiffuseTarget.ActivateTarget(0);
	m_cNormalTarget.ActivateTarget(1);
	m_cSpecularTarget.ActivateTarget(2);
	m_cDepthTarget.ActivateTarget(3);
	
	// Clear and render geometry buffers
	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	RenderGameplay(RPASS_GBUFFERS);

	// Revert render targets
	m_cDepthTarget.RevertTarget();
	m_cDiffuseTarget.RevertTarget();
	m_cNormalTarget.RevertTarget();
	m_cSpecularTarget.RevertTarget();

	// Render rage effect
	m_cRageEffectTarget.ActivateTarget(0);
	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	RenderGameplay(RPASS_RAGE_EFFECT);
	m_cRageEffectTarget.RevertTarget();
}

void CRenderer::RenderDirLightDepths(void)
{
	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
	D3DCOLORWRITEENABLE_RED);

	m_pDirLight->ActivateRenderTarget(0);
	m_pDirLight->ApplyLight();

	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	RenderGameplay(RPASS_SHADOW);

	m_pDirLight->RevertRenderTarget();

	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

}

void CRenderer::RenderLights(void)
{
	m_cSceneTarget.ActivateTarget(0);
	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1, 0);

	CAssetManager::ShaderSetMatrix(m_pDirLight->GetShader(), "gInvViewProj", &m_cCamera.GetInvViewProjection());
	CAssetManager::ShaderSetMatrix(m_pDirLight->GetShader(), "gViewProj", &m_cCamera.GetViewProjection());
	
	m_pDirLight->ApplyLight();
	CAssetManager::ShaderSetTechnique(m_pDirLight->GetShader(), "DirectionalLight");
	CAssetManager::ShaderCommitChanges(m_pDirLight->GetShader());

	m_pDevice->SetVertexDeclaration(m_pQuadDecl);
	
	UINT numPasses = CAssetManager::ShaderBegin(m_pDirLight->GetShader());
	for(UINT uiPass = 0; uiPass < numPasses; ++uiPass)
	{
		CAssetManager::ShaderBeginPass(m_pDirLight->GetShader(), uiPass);
	
		SetStreamSource(m_pScreenQuadVertices, sizeof(float) * 5);
		SetIndices(m_pScreenQuadIndices);
		DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 2);
		
		CAssetManager::ShaderEndPass(m_pDirLight->GetShader());
	}
	CAssetManager::ShaderEnd(m_pDirLight->GetShader());


	numPasses = 0;
	int nShader = -1;
	for(std::vector<CPointLight*>::iterator iter = m_vpPointLights.begin(); iter != m_vpPointLights.end(); iter++)
	{
		CPointLight* pCurLight = *iter;
		if(!pCurLight || !pCurLight->GetEnabled())
			continue;

		if(!numPasses || nShader != pCurLight->GetShader())
		{
			nShader = pCurLight->GetShader();
			CAssetManager::ShaderSetTechnique(nShader, "PointLight");
			numPasses = CAssetManager::ShaderBegin(nShader);
		}

		if(!numPasses || nShader == -1)
			continue;

		pCurLight->ApplyLight();
		D3DXMATRIX d3dWorld = pCurLight->GetWorldMatrix();
		CAssetManager::ShaderSetMatrix(nShader, "gWorld", &d3dWorld);
		D3DXMATRIX mvp = d3dWorld * m_cCamera.GetViewProjection();
		CAssetManager::ShaderSetMatrix(nShader, "gMVP", &mvp);
		CAssetManager::ShaderCommitChanges(nShader);

		for(UINT uiPass = 0; uiPass < numPasses; ++uiPass)
		{
			CAssetManager::ShaderBeginPass(nShader, uiPass);
			pCurLight->GetBoundingMesh()->DrawSubset(0);
			CAssetManager::ShaderEndPass(nShader);
		}
	}
	CAssetManager::ShaderEnd(nShader);


	m_cSceneTarget.RevertTarget();
}

void CRenderer::RenderGameplay(ERenderPass ePass)
{
	if(m_VisibleSet.empty())
		return;

	deque<IRenderThing*>::const_iterator iter = m_VisibleSet.getDeque()->begin();
	unsigned short ucLastFlags = (*iter)->GetFlags();
	
	int nShader;
	const char* szTechnique;
	InterpretFlags(ePass, ucLastFlags, nShader, szTechnique);
	CAssetManager::ShaderSetTechnique(nShader, szTechnique);

	UINT numPasses = CAssetManager::ShaderBegin(nShader);
	bool bShaderOpen = numPasses ? true : false;

	while(iter != m_VisibleSet.getDeque()->end())
	{
		unsigned short ucCurFlags = (*iter)->GetFlags();
		if(ucLastFlags != ucCurFlags)
		{
			if(bShaderOpen)
				CAssetManager::ShaderEnd(nShader);
			InterpretFlags(ePass, ucCurFlags, nShader, szTechnique);
			ucLastFlags = ucCurFlags;
			CAssetManager::ShaderSetTechnique(nShader, szTechnique);
			numPasses = CAssetManager::ShaderBegin(nShader);
			bShaderOpen = numPasses ? true : false;
		}

		if(bShaderOpen)
			RenderOneThing(*iter, nShader, numPasses);

		iter++;
	}

	if(bShaderOpen)
		CAssetManager::ShaderEnd(nShader);
}

void CRenderer::RenderOneThing(IRenderThing* pThing, int nShader, unsigned nNumPasses)
{
	CAssetManager::ShaderSetMatrix(nShader, "gWorld", &pThing->GetWorldTransform());
	CAssetManager::ShaderSetValue(nShader, "gEffectColor", pThing->GetEffectColor(), sizeof(D3DXVECTOR4));
	D3DXMATRIX mvp = pThing->GetWorldTransform() * m_cCamera.GetViewProjection();
	CAssetManager::ShaderSetMatrix(nShader, "gMVP", &mvp);
	CAssetManager::ShaderSetValue(nShader, "gAddedColor", pThing->GetAddedColor(), sizeof(D3DXVECTOR4));
	
	for(UINT uiPass = 0; uiPass < nNumPasses; ++uiPass) {
		pThing->Render(nShader,uiPass);	}
}

void CRenderer::InterpretFlags(ERenderPass ePass, unsigned short ucFlags, int& nShader, const char*& szTechnique)
{
	bool bMenuModel = ucFlags & RENDER_MENU_MODEL ? true : false;
	if((ePass == RPASS_GBUFFERS && !bMenuModel) ||
		(ePass == RPASS_MENU && bMenuModel))
	{
		if(ucFlags & RENDER_TWOD_PARTICLE)
		{
			nShader = m_nParticleShader;
			m_pDevice->SetVertexDeclaration(m_pParticleDecl);
			szTechnique = "DiffuseParticles";
		}
		else
		{
			nShader = m_nGeometryShader;
			if(ucFlags & RENDER_ANIMATED)
			{
				m_pDevice->SetVertexDeclaration(m_pAnimatedDecl);

				if(ucFlags & RENDER_TRANSPARENT)
					szTechnique = "GBuffersAlphaAnimated";
				else if(ucFlags & RENDER_FULLY_TRANSLUCENT)
					szTechnique = "GBuffersFullyTranslucentAnimated";
				else if(ucFlags & RENDER_EFFECT)
					szTechnique = "GBuffersEffectsAnimated";
				else
					szTechnique = "GBuffersDefaultAnimated";
			}
			else
			{
				m_pDevice->SetVertexDeclaration(m_pVertexDecl);

				if(ucFlags & RENDER_TRANSPARENT)
				{
					if(ucFlags & RENDER_SCROLLING_UVS)
						szTechnique = "GBuffersScrollingUVsAlpha";
					else
						szTechnique = "GBuffersAlpha";
				}
				else if(ucFlags & RENDER_FULLY_TRANSLUCENT)
				{
					if(ucFlags & RENDER_SCROLLING_UVS)
						szTechnique = "GBuffersScrollingUVsFullyTranslucent";
					else
						szTechnique = "GBuffersFullyTranslucent";
				}
				else if(ucFlags & RENDER_EFFECT)
					szTechnique = "GBuffersEffects";
				else if(ucFlags & RENDER_SCROLLING_UVS)
					szTechnique = "GBuffersScrollingUVs";
				else
					szTechnique = "GBuffersDefault";
			}
		}
	}
	else if(ePass == RPASS_SHADOW && !bMenuModel)
	{
		// For the shadow pass, if this is a 2D particle or it's not a shadow caster, we shouldn't render it
		if((ucFlags & RENDER_TWOD_PARTICLE) || (ucFlags & RENDER_NOT_SHADOWCASTER))
		{
			nShader = -1;
			szTechnique = "";
		}
		// Otherwise, let's just use the directional light shader
		else
		{
			nShader = m_pDirLight->GetShader();

			if(ucFlags & RENDER_ANIMATED)
			{
				m_pDevice->SetVertexDeclaration(m_pAnimatedDecl);
				szTechnique = "DirectionalDepthAnimated";
			}
			else
			{
				m_pDevice->SetVertexDeclaration(m_pVertexDecl);
				szTechnique = "DirectionalDepth";
			}
		}
	}
	else if(ePass == RPASS_RAGE_EFFECT && !bMenuModel)
	{
		// We only want certain objects to get past the rage effect
		if(ucFlags & RENDER_TWOD_PARTICLE)
		{
			nShader = m_nParticleShader;
			m_pDevice->SetVertexDeclaration(m_pParticleDecl);
			szTechnique = "DiffuseParticles";
		}
		else if(ucFlags & RENDER_RAGE_AFFECTED)
		{
			nShader = m_nGeometryShader;

			if(ucFlags & RENDER_ANIMATED)
			{
				m_pDevice->SetVertexDeclaration(m_pAnimatedDecl);

				if(ucFlags & RENDER_RAGE_BRIGHTENED)
					szTechnique = "GBuffersRageAnimated";
				else
					szTechnique = "GBuffersDefaultAnimated";
			}
			else
			{
				m_pDevice->SetVertexDeclaration(m_pVertexDecl);

				if(ucFlags & RENDER_RAGE_BRIGHTENED)
					szTechnique = "GBuffersRage";
				else
					szTechnique = "GBuffersDefault";
			}
		}
		else
		{
			nShader = m_nGeometryShader;
			m_pDevice->SetVertexDeclaration(m_pVertexDecl);
			szTechnique = "GBuffersBlack";
		}
	}
	else
	{
		nShader = -1;
		szTechnique = "";
	}
}
	
void CRenderer::RenderForwardPass(void)
{
	m_pDevice->SetVertexDeclaration(m_pQuadDecl);

	// Run the post process chain and cast the final scene texture
	IDirect3DTexture9* hFinalSceneTexture = nullptr;
	if(m_bPostProcessActive)
		hFinalSceneTexture = m_cPostProcessChain.Run(&m_cSceneTarget);
	else
		hFinalSceneTexture = m_cSceneTarget.GetTexture();
	
	if(!hFinalSceneTexture)
		return;

	// Clear all the buffers
	Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1, 0);

#ifdef CHEAP_RENDERING
	CAssetManager::ShaderSetTechnique(m_nFinalShader, "Debug");
#else
	CAssetManager::ShaderSetTechnique(m_nFinalShader, "Final");
#endif

	UINT numPasses = CAssetManager::ShaderBegin(m_nFinalShader);
	for(UINT uiPass = 0; uiPass < numPasses; ++uiPass)
	{
		CAssetManager::ShaderBeginPass(m_nFinalShader, uiPass);

		CAssetManager::ShaderSetValue(m_nFinalShader, "gPlayerPos", m_cCamera.GetPlayerPos(), sizeof(D3DXVECTOR3));
		
		int nBackgroundID = CWorldManager::GetInstance()->GetCurBackground();
		if(nBackgroundID != -1)
			CAssetManager::ApplyTexture(m_nFinalShader, "backgroundMap", nBackgroundID);
		
		CAssetManager::ShaderSetTexture(m_nFinalShader, "sceneMap", hFinalSceneTexture);
		CAssetManager::ShaderCommitChanges(m_nFinalShader);

		SetStreamSource(m_pScreenQuadVertices, sizeof(float) * 5);
		SetIndices(m_pScreenQuadIndices);
		DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 2);

		CAssetManager::ShaderEndPass(m_nFinalShader);
	}
	CAssetManager::ShaderEnd(m_nFinalShader);
}

void CRenderer::RenderCollisionBounds(bool bAABB)
{
	m_pDevice->SetVertexDeclaration(m_pDebugDecl);

	for(list<IRenderThing*>::iterator iter = m_listRenderInstances.begin();
		iter != m_listRenderInstances.end(); ++iter)
	{
		CRenderInstance* pInst = dynamic_cast<CRenderInstance*>(*iter);

		if(!pInst || !m_cCamera.IsObjectInside(pInst) || !pInst->IsRenderable(true))
			continue;

		// Set technique for the shader
		CAssetManager::ShaderSetTechnique(m_nDebugShader, "myTechnique");

		vec3f vPos, vMax, vMin;
		float fRadius;
		CView::GetCollisionSphere((int)*iter, vPos, fRadius);
		CView::GetCollisionAabb((int)*iter, vMin, vMax);

		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, vPos.x, vPos.y, vPos.z);
		CAssetManager::ShaderSetMatrix(m_nDebugShader, "gWorld", &mWorld);
		CAssetManager::ShaderSetMatrix(m_nDebugShader, "gViewProj", &m_cCamera.GetViewProjection());

		D3DXVECTOR4 vColor;

		if(bAABB)
		{
			if(pInst->IsVisible())
				vColor = D3DXVECTOR4(0, 1, 0, 1);
			else
				vColor = D3DXVECTOR4(1, 1, 0, 1);
		}
		else
		{
			vColor = D3DXVECTOR4(1, 0, 1, 1);
		}
		
		CAssetManager::ShaderSetValue(m_nDebugShader, "gColor", vColor, sizeof(vColor));
		
		ID3DXMesh* collisionMesh = nullptr;
		if(bAABB)
			D3DXCreateBox(m_pDevice, (vMax.x - vMin.x), (vMax.y - vMin.y), (vMax.z - vMin.z), &collisionMesh, nullptr);
		else
			D3DXCreateSphere(m_pDevice, fRadius, 15, 15, &collisionMesh, nullptr);

		CAssetManager::ShaderCommitChanges(m_nDebugShader);

		// Begin the shader and get the number of passes
		UINT numPasses = CAssetManager::ShaderBegin(m_nDebugShader);

		// Loop through all the needed passes
		for(UINT uiPass = 0; uiPass < numPasses; ++uiPass)
		{
			// Begin the pass for the shader
			CAssetManager::ShaderBeginPass(m_nDebugShader, uiPass);
			// Render the mesh
			if(collisionMesh)
				collisionMesh->DrawSubset(0);
			// End the pass for the shader
			CAssetManager::ShaderEndPass(m_nDebugShader);
		}

		if(collisionMesh)
			collisionMesh->Release();

		// End the shader
		CAssetManager::ShaderEnd(m_nDebugShader);
	}
}

void CRenderer::RenderInvisibleWalls(vector<AABB>& vWalls)
{
	m_pDevice->SetVertexDeclaration(m_pDebugDecl);

	for(size_t i = 0; i < vWalls.size(); ++i)
	{
		// Set technique for the shader
		CAssetManager::ShaderSetTechnique(m_nDebugShader, "myTechnique");

		vec3f& vMin = vWalls[i].min;
		vec3f& vMax = vWalls[i].max;
		vec3f vPos = (vMax + vMin) / 2.0f;

		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, vPos.x, vPos.y, vPos.z);
		CAssetManager::ShaderSetMatrix(m_nDebugShader, "gWorld", &mWorld);
		CAssetManager::ShaderSetMatrix(m_nDebugShader, "gViewProj", &m_cCamera.GetViewProjection());

		D3DXVECTOR4 vColor;
		vColor = D3DXVECTOR4(1, 1, 0, 1);
		CAssetManager::ShaderSetValue(m_nDebugShader, "gColor", vColor, sizeof(vColor));
		
		ID3DXMesh* collisionMesh = nullptr;
		D3DXCreateBox(m_pDevice, (vMax.x - vMin.x), (vMax.y - vMin.y),
			(vMax.z - vMin.z), &collisionMesh, nullptr);

		CAssetManager::ShaderCommitChanges(m_nDebugShader);

		// Begin the shader and get the number of passes
		UINT numPasses = CAssetManager::ShaderBegin(m_nDebugShader);

		// Loop through all the needed passes
		for(UINT uiPass = 0; uiPass < numPasses; ++uiPass)
		{
			// Begin the pass for the shader
			CAssetManager::ShaderBeginPass(m_nDebugShader, uiPass);
			// Render the mesh
			if(collisionMesh)
			{
				collisionMesh->DrawSubset(0);
			}
			// End the pass for the shader
			CAssetManager::ShaderEndPass(m_nDebugShader);
		}

		if(collisionMesh)
		{
			collisionMesh->Release();
		}

		// End the shader
		CAssetManager::ShaderEnd(m_nDebugShader);
	}
}

CRenderer::~CRenderer(void)
{
	ClearAllInstances();

	m_cPostProcessChain.Clear();
	for(vector<CPostProcess*>::iterator iter = m_vpPostProcesses.begin();
		iter != m_vpPostProcesses.end(); ++iter)
	{
		if(*iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}

	for(size_t lightID = 0; lightID < m_vpPointLights.size(); ++lightID)
	{
		if(m_vpPointLights[lightID])
		{
			delete m_vpPointLights[lightID];
			m_vpPointLights[lightID] = nullptr;
		}
	}

	if(m_pDirLight)
	{
		delete m_pDirLight;
		m_pDirLight = nullptr;
	}

	if(m_pEffectPool)
	{
		m_pEffectPool->Release();
		m_pEffectPool = nullptr;
	}
	if(m_pSpriteMng)
	{
		m_pSpriteMng->Release();
		m_pSpriteMng = nullptr;
	}
	if(m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

void CRenderer::CreateGeometryBuffers(IDirect3DVertexBuffer9** pOutVertBuff, IDirect3DIndexBuffer9** pOutIndexBuff,
										 void* pVertSrc, UINT uiVertLen, void* pIndexSrc, UINT uiIndexLen)
{
	void* mem = 0;
	HRESULT hr;

	if(pOutVertBuff)
	{
		hr = m_pDevice->CreateVertexBuffer(uiVertLen, 0, 0, D3DPOOL_MANAGED, pOutVertBuff, 0);

		if(hr != D3D_OK)
			return;

		if(pVertSrc)
		{
			(*pOutVertBuff)->Lock(0, 0, &mem, 0);
			memcpy(mem, pVertSrc, uiVertLen);
			(*pOutVertBuff)->Unlock();
		}
	}

	if(pOutIndexBuff)
	{
		hr = m_pDevice->CreateIndexBuffer(uiIndexLen, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, pOutIndexBuff, 0);

		if(hr != D3D_OK)
			return;

		if(pOutIndexBuff)
		{
			(*pOutIndexBuff)->Lock(0, 0, &mem, 0);
			memcpy(mem, pIndexSrc, uiIndexLen);
			(*pOutIndexBuff)->Unlock();
		}
	}
}

void CRenderer::CreateDynamicBuffer(IDirect3DVertexBuffer9** pOutVertBuff, UINT uiVertLen)
{
	HRESULT hr;

	if(pOutVertBuff) {
		hr = m_pDevice->CreateVertexBuffer(uiVertLen, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, pOutVertBuff,0);
	}
}

void CRenderer::RemoveRenderInstance(int nRenderInstID)
{
	IRenderThing* pThing = reinterpret_cast<IRenderThing*>(nRenderInstID);

	for(list<IRenderThing*>::iterator iter = m_listRenderInstances.begin();
		iter != m_listRenderInstances.end(); ++iter)
	{
		if((*iter) == pThing)
		{
			RemoveInstanceLights(pThing);
			RemoveInstanceAttachments(pThing);
			delete (*iter);
			m_listRenderInstances.remove(*iter);
			break;
		}
	}

	m_VisibleSet.remove(pThing);
}

void CRenderer::ClearAllInstances(void)
{
	for(list<IRenderThing*>::iterator iter = m_listRenderInstances.begin();
		iter != m_listRenderInstances.end(); ++iter)
	{
		delete (*iter);
	}
	m_listRenderInstances.clear();

	m_VisibleSet.clear();
}

void CRenderer::ToScreenSpace(float fInX, float fInY, float fInZ, int& nOutX, int& nOutY)
{
	D3DXVECTOR3 d3dPos(fInX, fInY, fInZ);

	D3DXVECTOR4 clipSpacePos;
	D3DXVec3Transform(&clipSpacePos, &d3dPos, &m_cCamera.GetViewProjection());

	if(clipSpacePos.w == 0)
		return;

	D3DXVECTOR3 ndcSpacePos =
		D3DXVECTOR3(clipSpacePos.x, clipSpacePos.y, clipSpacePos.z) / clipSpacePos.w;
	
	nOutX = (int)(((ndcSpacePos.x + 1.0f) / 2.0f) * m_nWindowWidth);
	nOutY = (int)((1.0f - ((ndcSpacePos.y + 1.0f) / 2.0f)) * m_nWindowHeight);
}

int	 CRenderer::AddPointLight(CPointLight* pLight)
{
	if(!pLight) return -1;

	if(pLight->GetOwner())
	{
		for(size_t i = 0; i < m_vpPointLights.size(); ++i)
		{
			if(m_vpPointLights[i] && m_vpPointLights[i]->GetOwner() == pLight->GetOwner())
			{
				delete m_vpPointLights[i];
				m_vpPointLights[i] = pLight;
				return (int)i;
			}
		}
	}

	m_vpPointLights.push_back(pLight);
	return m_vpPointLights.size() - 1;
}

void CRenderer::RemovePointLight(int nLightID)
{
	if(m_vpPointLights[nLightID])
	{
		delete m_vpPointLights[nLightID];
		m_vpPointLights[nLightID] = nullptr;
	}
}

bool CRenderer::CompareRenderThings(IRenderThing* const& lhs, IRenderThing* const& rhs)
{
	return *lhs < *rhs;
}

CPointLight* CRenderer::GetPointLight(int nLightID)
{
	if(nLightID >= 0 && nLightID < (int)m_vpPointLights.size())
		return m_vpPointLights[nLightID];
	return nullptr;
}

void CRenderer::RemoveInstanceLights(const IRenderThing* pInst)
{
	for(size_t i = 0; i < m_vpPointLights.size(); ++i)
	{
		if(m_vpPointLights[i] && m_vpPointLights[i]->GetOwner() == pInst)
		{
			delete m_vpPointLights[i];
			m_vpPointLights[i] = nullptr;
		}
	}
}

void CRenderer::RemoveInstanceAttachments(const IRenderThing* pInst)
{
	list<IRenderThing*> listToDelete;

	for(list<IRenderThing*>::iterator iter = m_listRenderInstances.begin();
		iter != m_listRenderInstances.end(); ++iter)
	{
		if((*iter)->GetType() == IRenderThing::ATTACHED)
		{
			CRenderInstanceAttached* pAttInst = dynamic_cast<CRenderInstanceAttached*>(*iter);
			if(pAttInst->GetOwner() == pInst)
				listToDelete.push_back(pAttInst);
		}
	}

	for(list<IRenderThing*>::iterator iter = listToDelete.begin();
		iter != listToDelete.end(); ++iter)
	{
		delete *iter;
		m_listRenderInstances.remove(*iter);
		m_VisibleSet.remove(*iter);
	}
}
