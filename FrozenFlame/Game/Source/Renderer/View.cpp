#include "View.h"
#include "AssetManager.h"
#include "RenderInstanceAttached.h"
#include "RenderInstanceParticles.h"
#include "RenderInstanceAnimated.h"
#include "CHUD.h"
#include "PointLight.h"
#include "DynamicShape.h"
#include "Model.h"
#include "CEmitterManager.h"

#include "../Util/CInput.h"
#include "../Util/CFontManager.h"

#include "../Objects/BaseObject.h"
#include "../Objects/CollidableObject.h"
#include "../Objects/EnvironmentObject.h"
#include "../Objects/MeleeEnemy.h"
#include "../Objects/PlayerObject.h"

#include "../Core/CWorldManager.h"
#include "../Animations/AnimPlayer.h"
#include "../States/CVideoOptionsState.h"
#include "../Objects/MenuModelObject.h"

// Defining static variables
CRenderer			CView::sm_cRenderer;
CFontManager		CView::sm_cFontManager;
CEmitterManager		CView::sm_cEmitterManager;
bool				CView::sm_bInitialized = false;
bool				CView::sm_bSceneStarted = false;
bool				CView::sm_bSpriteStarted = false;

bool				CView::sm_bRenderCollisionBoxes = false;
bool				CView::sm_bRenderCollisionSpheres = false;
bool				CView::sm_bRender2dParticles = true;
bool				CView::sm_bRenderDamage = false;

char				CView::sm_cFadeState = FADE_NOT;
float				CView::sm_fFade = 0.0f;
int					CView::sm_nFontID = -1;
TImage				CView::sm_tFade;
CPostProcess*		CView::sm_pRageEffect = nullptr;

const CPostProcess*	CView::sm_pHeatDistortion = nullptr;
float				CView::sm_fHeatOffset;

TImage				CView::sm_tButtonNormal;
TImage				CView::sm_tButtonHover;

///////////////////////////////////////////////////////////////////////////////////
//	BASIC FUNCTIONALITY
///////////////////////////////////////////////////////////////////////////////////
int		CView::Initialize(HWND hWnd, int nClientWidth, int nClientHeight)
{
	// Check if it wasn't already initialized
	if(sm_bInitialized)
	{
		return VIEW_INIT_FAILED;
	}
	// Initialize the Renderer
	if(sm_cRenderer.Initialize(hWnd, true, true, nClientWidth, nClientHeight) != VIEW_SUCCEEDED)
	{
		return VIEW_INIT_FAILED;
	}

	sm_cEmitterManager.Init();

	// Now we're initialized
	sm_bInitialized = true;

	// Load Image for fade - Roberto
	sm_tFade.m_nImageID = LoadSprite("Fade.png", &sm_tFade.m_nImageWidth, &sm_tFade.m_nImageHeight);

	// Load button images
	sm_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &sm_tButtonNormal.m_nImageWidth, &sm_tButtonNormal.m_nImageHeight);
	sm_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &sm_tButtonHover.m_nImageWidth, &sm_tButtonHover.m_nImageHeight);

	sm_nFontID = CView::LoadFont("font example.txt");

	// All this is just for adding bloom =)
	sm_cRenderer.AddPostProcess("PostProcess.fx", "FrozenFlame");
	sm_cRenderer.AddPostProcess("BrightPass.fx", "BrightPass");
	sm_cRenderer.AddPostProcess("GaussianBlur.fx", "GaussianBlurH", PTS_DOWNSCALE);
	sm_cRenderer.AddPostProcess("GaussianBlur.fx", "BloomH");
	sm_cRenderer.AddPostProcess("GaussianBlur.fx", "BloomV");	
	sm_cRenderer.AddPostProcess("GaussianBlur.fx", "GaussianBlurV", PTS_UPSCALE);
	sm_cRenderer.AddPostProcess("PostProcess.fx", "CombineBloom");

	sm_pHeatDistortion = sm_cRenderer.AddPostProcess("PPHeatDistortion.fx", "WarmColorHeatDistortion");
	CAssetManager::ShaderSetTexture(sm_pHeatDistortion->GetShaderID(), "gHeatNormalTexture", sm_cRenderer.LoadTexture("Resource/Textures/HeatHazeNormalMap.bmp"));
	CAssetManager::ShaderSetTexture(sm_pHeatDistortion->GetShaderID(), "gHeatMaskTexture", sm_cRenderer.LoadTexture("Resource/Textures/HeatHazeMask.bmp"));
	sm_fHeatOffset = 0.0f;	

	return VIEW_SUCCEEDED;
}
void	CView::Update(float fElapsedTime)
{
	sm_fHeatOffset -= 0.1f;
	CAssetManager::ShaderSetValue(sm_pHeatDistortion->GetShaderID(), "heatOffsetY", &sm_fHeatOffset, sizeof(float));

	sm_cEmitterManager.UpdateEmitters(fElapsedTime);
	sm_cRenderer.Update(fElapsedTime);
	UpdateFade(fElapsedTime);
}
void	CView::RenderMenuModels(int nBackgroundID)
{
	sm_cRenderer.RenderMenuModels(nBackgroundID);
}
void	CView::Render(void)
{
	sm_cRenderer.Render();

	if(sm_bRenderCollisionBoxes)
	{
		sm_cRenderer.RenderCollisionBounds(true);
		sm_cRenderer.RenderInvisibleWalls(CWorldManager::GetInstance()->GetWallAABBs());
	}
	if(sm_bRenderCollisionSpheres)
	{
		sm_cRenderer.RenderCollisionBounds(false);
	}

	CHud::GetInstance()->DrawDamageSplash();
	CHud::GetInstance()->DrawEffectSplash(CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetElementalState());
	CHud::GetInstance()->DrawHealthAndRage(CWorldManager::GetInstance()->GetObjectManager()->GetPlayer(), 0 , 0);

	Fade();
}
void	CView::Shutdown(void)
{
	Clear();
	CAssetManager::Shutdown();
	sm_cEmitterManager.Shutdown();
}
void	CView::Clear(void)
{
	// This should clear all the render instances but if we uncomment this line the game
	// crashes when we go back to Main Menu.
	sm_cRenderer.ClearAllInstances();

	// This will make us remove the rage effect if it's still up
	StopRaging();
}
int		CView::Push(CBaseObject* pObject, string szModel, unsigned short ucFlags)
{
	if(!pObject || szModel.empty())
		return -1;

	int nModelID = CAssetManager::LoadModel(szModel, true);
	const CModel* pModel = CAssetManager::GetModel(nModelID);
	CRenderInstance* pInstance;
	if(ucFlags & RENDER_TWOD_PARTICLE)
	{
		pInstance = new CRenderInstanceParticles(pObject);
	}
	else if(pModel->GetNumJoints())
	{
		pInstance = new CRenderInstanceAnimated(nModelID, pObject);
	}
	else
	{
		pInstance = new CRenderInstance(nModelID, pObject);
	}

	pInstance->SetFlags(pInstance->GetFlags() | ucFlags);
	int nInstID = sm_cRenderer.AddRenderInstance(pInstance);
	return nInstID;
}
int		CView::PushAttachment(int nRenderInstID, string szAttachmentModel, string szJointToAttach, string szAttachmentJoint, unsigned short ucFlags)
{
	if(nRenderInstID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nRenderInstID);

		if(pRenderThing)
		{
			CRenderInstance* pInstance = dynamic_cast<CRenderInstance*>(pRenderThing);

			int nModelID = CAssetManager::LoadModel(szAttachmentModel, true);
			CRenderInstanceAttached* pAttachment = new CRenderInstanceAttached(nModelID, pInstance, szAttachmentJoint);
			pAttachment->SetFlags(pAttachment->GetFlags() | ucFlags);
			int nAttachmentID = sm_cRenderer.AddRenderInstance(pAttachment);

			if(pInstance->GetType() == IRenderThing::ANIMATED)
			{
				const CModel* pModel = CAssetManager::GetModel(pInstance->GetModelID());
				int nOwnerAnchorJoint = pModel->GetJointID(szJointToAttach);
				dynamic_cast<CRenderInstanceAnimated*>(pInstance)->SetAnchorJoint(nOwnerAnchorJoint);
			}

			return nAttachmentID;
		}
	}
	return -1;
}
void	CView::Pop(int nRenderInstID)
{
	if(nRenderInstID != -1)
	{
		sm_cRenderer.RemoveRenderInstance(nRenderInstID);
	}
}
void	CView::Start(D3DCOLOR d3dColor)
{
	// If we already started go back
	if(sm_bSceneStarted)
		return;

	// Else start the scene
	sm_cRenderer.BeginScene();
	sm_bSceneStarted = true;

	// Clear the screen
	sm_cRenderer.Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3dColor, 1.0f, 0);
}
void	CView::Finish(void)
{
	// If we haven't started go back
	if(!sm_bSceneStarted)
		return;

	// If sprite has been started, let's end it
	if(sm_bSpriteStarted)
	{
		sm_cRenderer.EndSprite();
		sm_bSpriteStarted = false;
	}

	// End the scene
	sm_cRenderer.EndScene();
	sm_bSceneStarted = false;

	// Present
	sm_cRenderer.Present();
}
void	CView::SetViewFlags(unsigned char ucFlags)
{
	sm_cRenderer.SetPostProcessActive(ucFlags & RF_POST_PROCESS ? true : false);
}

///////////////////////////////////////////////////////////////////////////////////
//	2D RENDERING
///////////////////////////////////////////////////////////////////////////////////
int		CView::LoadSprite(const char* szFilename, int* pnWidth, int* pnHeight)
{
	return CAssetManager::LoadTexture(szFilename, true, pnWidth, pnHeight);
}
void	CView::Draw(int nSpriteID, int nPosX, int nPosY, float fScaleX, float fScaleY, RECT* pSourceRect, float fRotCenterX, float fRotCenterY, float fRotation, DWORD dwColor)
{
	// If the sprite manager has not been started, do it
	if(!sm_bSpriteStarted && sm_bSceneStarted)
	{
		sm_cRenderer.BeginSprite();
		sm_bSpriteStarted = true;
	}

	CAssetManager::Draw(nSpriteID, (float)nPosX, (float)nPosY, fScaleX, fScaleY, pSourceRect, fRotCenterX, fRotCenterY, fRotation, dwColor);			
}
int		CView::LoadFont(const char* szFilename)
{
	return sm_cFontManager.LoadFont(szFilename);
}
void	CView::Write(int nFontID, string szText, RECT* pArea, bool bCentered, float fScale, D3DCOLOR d3dColor)
{
	if(bCentered)
	{
		sm_cFontManager.WriteCentered(nFontID, szText, pArea, fScale, d3dColor);
	}
	else
	{
		sm_cFontManager.Write(nFontID, szText, pArea, fScale, d3dColor);
	}
}
void	CView::Write(int nFontID, string szText, RECT* pArea, bool bCentered, float fScaleX, float fScaleY, D3DCOLOR d3dColor)
{
	if(bCentered)
	{
		sm_cFontManager.WriteCentered(nFontID, szText, pArea, fScaleX, fScaleY, d3dColor);
	}
	else
	{
		sm_cFontManager.Write(nFontID, szText, pArea, fScaleX, fScaleY, d3dColor);
	}
}
void	CView::DrawButton(int _nMidX, int _nMidY, float _fScaleX, float _fScaleY, string _szText, bool _bHover, D3DXCOLOR d3dButtonColor, float _fNumButtonWidthOffsets, float _fNumButtonHeightOffsets, D3DXCOLOR d3dTextColor)
{
	// Fix the scale for the resolution
	_fScaleX *= (CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth));
	_fScaleY *= (CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight));

	if(_bHover)
	{
		int nHalfWidth =	static_cast<int>((sm_tButtonHover.m_nImageWidth * _fScaleX)) >> 1;
		int nHalfHeight =	static_cast<int>((sm_tButtonHover.m_nImageHeight * _fScaleY)) >> 1;
		int nButtonOffsetWidth =	static_cast<int>((sm_tButtonHover.m_nImageWidth * _fNumButtonWidthOffsets) * _fScaleX);
		int nButtonOffsetHeight =	static_cast<int>((sm_tButtonHover.m_nImageHeight * _fNumButtonHeightOffsets) * _fScaleY);

		CView::Draw(sm_tButtonHover.m_nImageID, _nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _fScaleX, _fScaleY, nullptr, 0.0f, 0.0f, 0.0f, d3dButtonColor);

		RECT  tTextRect = {_nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _nMidX + nHalfWidth + nButtonOffsetWidth, _nMidY + nHalfHeight + nButtonOffsetHeight};
		CView::Write(sm_nFontID, _szText, &tTextRect, true, 0.6f * _fScaleX, 0.6f * _fScaleY, d3dTextColor);
	}
	else
	{
		int nHalfWidth = static_cast<int>((sm_tButtonNormal.m_nImageWidth * _fScaleX)) >> 1;
		int nHalfHeight = static_cast<int>((sm_tButtonNormal.m_nImageHeight * _fScaleY)) >> 1;
		int nButtonOffsetWidth = static_cast<int>((sm_tButtonNormal.m_nImageWidth * _fNumButtonWidthOffsets) * _fScaleX);
		int nButtonOffsetHeight = static_cast<int>((sm_tButtonNormal.m_nImageHeight * _fNumButtonHeightOffsets) * _fScaleY);

		CView::Draw(sm_tButtonNormal.m_nImageID, _nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _fScaleX, _fScaleY, nullptr, 0.0f, 0.0f, 0.0f, d3dButtonColor);
		
		RECT  tTextRect = {_nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight,  _nMidX + nHalfWidth + nButtonOffsetWidth, _nMidY + nHalfHeight + nButtonOffsetHeight};
		CView::Write(sm_nFontID, _szText, &tTextRect, true, 0.6f * _fScaleX, 0.6f * _fScaleY, d3dTextColor);
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	RENDER INSTANCES/SHAPES CONTROL
///////////////////////////////////////////////////////////////////////////////////
int		CView::DynamicShapeCreate(int nRenderInstID, bool bAttachedRotation, vec3f scale)
{
	if(nRenderInstID != -1)
	{
		CDynamicShape* pShape = new CDynamicShape(GetRenderInstance(nRenderInstID), bAttachedRotation);
		pShape->SetFlags(RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED);

		// this is very hacky because I'm only specifically setting a scale here instead of a matrix,
		// but we don't have need for anything other transform besides scale on this object
		pShape->SetLocalScale(scale);
		return sm_cRenderer.AddRenderInstance(pShape);
	}
	return -1;
}
void	CView::DynamicShapeMakePlane(int nShapeID, float fWidth, float fDepth)
{
	if(nShapeID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nShapeID);

		if(pRenderThing && pRenderThing->GetType() == IRenderThing::SHAPE)
		{
			dynamic_cast<CDynamicShape*>(pRenderThing)->MakePlane(fWidth, fDepth);
		}
	}
}
void	CView::DynamicShapeSetLocalMatrix(int nShapeID, const matrix4f& tMatrix)
{
	if(nShapeID != -1)
	{
		CDynamicShape* pShape = dynamic_cast<CDynamicShape*>(sm_cRenderer.GetRenderThing(nShapeID));
		if(pShape)
		{
			pShape->SetLocalMatrix(D3DXMATRIX(tMatrix.ma));
		}
	}
}
void	CView::DynamicShapeSetTextures(int nShapeID, int nDiffuseID, int nSpecularID, const vec4f& color)
{
	if(nShapeID != -1)
	{
		CDynamicShape* pShape = dynamic_cast<CDynamicShape*>(sm_cRenderer.GetRenderThing(nShapeID));
		if(pShape)
		{
			pShape->SetCurDiffuseTex(nDiffuseID);
			pShape->SetCurSpecularTex(nSpecularID);
			pShape->SetColor(D3DXVECTOR4(color.array()));
		}
	}
}
void	CView::DynamicShapeRemove(int nShapeID)
{
	if(nShapeID != -1)
	{
		sm_cRenderer.RemoveRenderInstance(nShapeID);
	}
}
void	CView::GetCollisionAabb(int nRenderInstID, vec3f& vMin, vec3f& vMax)
{
	if(nRenderInstID != -1)
	{
		if(IRenderThing* pThing = sm_cRenderer.GetRenderThing(nRenderInstID))
			pThing->GetCollisionBounds(vMin, vMax);
	}
}
void	CView::GetCollisionSphere(int nRenderInstID, vec3f& vPos, float& fRadius)
{
	if(nRenderInstID != -1)
	{
		vec3f vMin, vMax;
		if(IRenderThing* pThing = sm_cRenderer.GetRenderThing(nRenderInstID))
			pThing->GetCollisionBounds(vMin, vMax);

		vPos = (vMin + vMax) / 2;
		fRadius = (vMax - vPos).magnitude();
	}
}
void	CView::FlashRenderInstance(int nRenderInstID, vec4f vColor, float fDuration)
{
	if(nRenderInstID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nRenderInstID);

		if(pRenderThing)
			pRenderThing->Flash(D3DXVECTOR4(vColor.array()), fDuration);
	}
}
void	CView::SetIsVisible(int nRenderInstID, bool bIsVisible)
{
	if(nRenderInstID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nRenderInstID);

		if(pRenderThing)
		{
			unsigned short curFlags = pRenderThing->GetFlags();

			if(bIsVisible)
				// turn off the invisible flag
				curFlags &= (~RENDER_INVISIBLE);
			else
				// turn on the invisible flag
				curFlags |= RENDER_INVISIBLE;

			pRenderThing->SetFlags(curFlags);
		}
	}
}
void	CView::SetAttachmentEnabled(int nAttachmentID, bool bEnabled)
{
	if(nAttachmentID != -1)
	{
		reinterpret_cast<CRenderInstanceAttached*>(nAttachmentID)->SetEnabled(bEnabled);
	}
}
void	CView::SetRenderInstanceFlags(int nRenderInstID, unsigned short ucFlags)
{
	if(nRenderInstID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nRenderInstID);

		if(pRenderThing)
			pRenderThing->SetFlags(ucFlags);
	}
}
const CRenderInstance*	CView::GetRenderInstance(int nRenderInstID)
{
	IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nRenderInstID);
	return dynamic_cast<const CRenderInstance*>(pRenderThing);
}
CAnimPlayer*			CView::GetAnimPlayer(int nRenderInstID)
{
	if(nRenderInstID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nRenderInstID);

		if(pRenderThing && pRenderThing->GetType() == IRenderThing::ANIMATED)
		{
			return dynamic_cast<CRenderInstanceAnimated*>(pRenderThing)->GetAnimPlayer();
		}
	}
	return nullptr;
}
CRenderInstance*		CView::GetAttachmentInstance(int nRenderInstID, int nAttachmentID)
{
	if(nAttachmentID != -1)
	{
		IRenderThing* pRenderThing = sm_cRenderer.GetRenderThing(nAttachmentID);

		if(pRenderThing && pRenderThing->GetType() == IRenderThing::ATTACHED)
			return dynamic_cast<CRenderInstance*>(pRenderThing);
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////
//	LIGHTING
///////////////////////////////////////////////////////////////////////////////////
int		CView::MakeLight(int nOwnerRenderID, bool bStartEnabled, const vec3f& vOffset, const vec4f& vDiffuseColor, float fRange, const vec4f& vAmbientColor, const vec4f& vSpecularColor, float fSpecPower, float fSpecIntensity, const vec3f& vAttenuation)
{
	CPointLight* pLight = new CPointLight(
		bStartEnabled,
		D3DXVECTOR3(vOffset.v),
		fRange,
		D3DXVECTOR3(vAttenuation.v),
		D3DXVECTOR4(vAmbientColor.v),
		D3DXVECTOR4(vDiffuseColor.v),
		D3DXVECTOR4(vSpecularColor.v),
		fSpecPower,
		fSpecIntensity);

	if(nOwnerRenderID != -1)
		pLight->SetOwner(sm_cRenderer.GetRenderThing(nOwnerRenderID));

	return sm_cRenderer.AddPointLight(pLight);
}
int		CView::MakeLight(const vec3f& vPosition, bool bStartEnabled, const vec4f& vDiffuseColor, float fRange, const vec4f& vAmbientColor, const vec4f& vSpecularColor, float fSpecPower, float fSpecIntensity, const vec3f& vAttenuation)
{
	return MakeLight(-1, bStartEnabled, vPosition, vDiffuseColor, fRange,
		vAmbientColor, vSpecularColor, fSpecPower, fSpecIntensity, vAttenuation);
}
void	CView::DestroyLight(int nLightID)
{
	if(nLightID != -1)
		sm_cRenderer.RemovePointLight(nLightID);
}
void	CView::SetLightEnabled(int nLightID, bool bEnabled)
{
	if(nLightID != -1)
	{
		CPointLight* pLight = sm_cRenderer.GetPointLight(nLightID);
		if(pLight)
			pLight->SetEnabled(bEnabled);
	}
}
void	CView::SetLightDiffuseColor(int nLightID, const vec4f& vDiffuseColor)
{
	if(nLightID != -1)
	{
		CPointLight* pLight = sm_cRenderer.GetPointLight(nLightID);
		if(pLight)
			pLight->SetDiffuseColor(D3DXVECTOR4(vDiffuseColor.v));
	}
}
void	CView::SetLightOffset(int nLightID, const vec3f& vOffset)
{
	if(nLightID != -1)
	{
		CPointLight* pLight = sm_cRenderer.GetPointLight(nLightID);
		if(pLight)
			pLight->SetPosition(D3DXVECTOR3(vOffset.v));
	}
}
const CPointLight*		CView::GetLight(int nLightID)
{
	if(nLightID != -1)
		return sm_cRenderer.GetPointLight(nLightID);
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////
//	FADING
///////////////////////////////////////////////////////////////////////////////////
void	CView::Fade(void)
{
	D3DXCOLOR color(1, 1, 1, sm_fFade);

	RECT area = {0,0,sm_tFade.m_nImageWidth,sm_tFade.m_nImageHeight};

	CView::Draw(sm_tFade.m_nImageID,0,0,float(CGame::GetInstance()->GetScreenWidth()) / sm_tFade.m_nImageWidth,
		float(CGame::GetInstance()->GetScreenHeight()) / sm_tFade.m_nImageHeight,&area,0,0,0,color);
}
void	CView::ResetFade(void)
{
	sm_fFade = 0.0;
	sm_cFadeState = FADE_NOT;
}
void	CView::UpdateFade(float _fElapsedTime)
{
	// We begin fading in
	if(FADE_OUT == sm_cFadeState)
	{
		sm_fFade += 0.4f * _fElapsedTime;

		// When we are at or over 1 we will complete fading in and begin to fade out
		if(sm_fFade >= 1.0f)
		{
			// cap at 1
			sm_fFade = 1.0f;
			sm_cFadeState = FADE_COMPLETE;
		}
	}
	else if(FADE_IN == sm_cFadeState) // Begin to fade out when fading in is complete
	{
		sm_fFade -= 0.4f * _fElapsedTime;

		// When we are at or less than 0 we will be done fading
		if(sm_fFade <= 0)
		{
			// cap at 0
			sm_fFade = 0.0f;
			sm_cFadeState = FADE_NOT;

			SetCameraToPan();
			GetCamera().StartPanning();
			CInput::GetInstance()->DisablePlayerInput();

		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
//	MISC
///////////////////////////////////////////////////////////////////////////////////
void	CView::StartRaging(void)
{
	if(!sm_pRageEffect)
	{
		sm_pRageEffect = sm_cRenderer.AddPostProcess("PostProcess.fx", "RageEffect");
	}
}
void	CView::StopRaging(void)
{
	if(sm_pRageEffect)
	{
		sm_cRenderer.RemovePostProcess(sm_pRageEffect);
		sm_pRageEffect = nullptr;
	}
}
void	CView::SetCameraToPan(void)
{
	// Uncomment this and comment out new waypoint code below to get old cutscene
	// functionality back. (Out to end of level and back) - JC
	/*const CLevel& cFinalLevel = (CWorldManager::GetInstance()->GetLevel(CWorldManager::GetInstance()->GetTotalCurrentLevels - 1));
	GetCamera().Clear();
	GetCamera().AddWayPoint(cFinalLevel.GetCenterX(), 0, cFinalLevel.GetCenterZ());*/

	// Sets camera's position to high over top of last level section, and pans/zooms to player. -JC
	GetCamera().Clear();
	const CLevel& cFinalLevel = *CWorldManager::GetInstance()->GetLevel(int(CWorldManager::GetInstance()->GetTotalCurrentLevels()) - 1);
	GetCamera().SetCurPos(vec3f(cFinalLevel.GetCenterX() - 40.0f, 65.0f,cFinalLevel.GetCenterZ()));
	GetCamera().AddWayPointToPlayer();
}
