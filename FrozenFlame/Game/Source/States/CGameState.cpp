/********************************************************************
* Filename:			CGameState.cpp
* Date:				10/31/2012
* Mod. Date:		11/13/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the main game state
********************************************************************/

#include "CGameState.h"
#include "CPauseState.h"
#include "CHelpScreenState.h"
#include "CWinState.h"
#include "CLossState.h"
#include "CConsoleState.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include "../Core/CWorldManager.h"
#include "../States/CVideoOptionsState.h"
//#include "../Globals.h"
#include "../Renderer/CHUD.h"
#include "CFadeState.h"
#include "../Objects/Signpost.h"
#include "CPopupState.h"
#include "CLoadState.h"

CGameState::CGameState(void)
{
	m_tEmptyBar.m_nImageID = -1;
	m_tHealthBar.m_nImageID = -1;
	m_tRageBar.m_nImageID = -1;
	m_nBackground = -1;
	m_nBorderID = -1;
	m_nFontID = -1;
	m_fFlashTimer = 0.0f;
	m_fFlashRate = 0.0f;
	m_nRageButtonID = -1;
	m_wRageButton = 0;
	m_fDisplayTime = 0.0f;
	m_bWaiting = false;
	m_bDisplayMessage = true;
	m_bZoomCamera = true;
	m_bCameraIsZooming = false;
	m_fZoomTimer = 0.0f;
	m_fZoomDistance = 15.0f;
	m_fZoomDuration = 0.9f;

	//m_bRenderDamage = false;

	m_pInput = CInput::GetInstance();
	m_nFontID = CView::LoadFont("font example.txt");
	m_nLoadScreenID = CView::LoadSprite("loadingbg.png");
}

void CGameState::Enter(void)
{	
	// push us to the loading state where we can show some text and splash
	CGame::GetInstance()->PushState(CLoadState::GetInstance());

	// Load video settings to make sure player actually has their options take effect. -JC
	CVideoOptionsState::GetInstance()->LoadSettings("Video");
	
	CWorldManager::GetInstance()->Reset(false);
	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MX_MainMenu");

	m_tEmptyBar.m_nImageID = CView::LoadSprite("HealthBar1.png", &m_tEmptyBar.m_nImageWidth, &m_tEmptyBar.m_nImageHeight);
	m_tHealthBar.m_nImageID = CView::LoadSprite("HealthBar3.png", &m_tHealthBar.m_nImageWidth, &m_tHealthBar.m_nImageHeight);
	m_tRageBar.m_nImageID = CView::LoadSprite("RageBar2.png", &m_tRageBar.m_nImageWidth, &m_tRageBar.m_nImageHeight);
	m_nBackground = CView::LoadSprite("Volcano.jpg");
	m_tGolieDot.m_nImageID = CView::LoadSprite("GolieDot.png", &m_tGolieDot.m_nImageWidth, &m_tGolieDot.m_nImageHeight);
	m_fFlashTimer = 0.9f;
	m_fFlashRate = 0.3f;

	m_tBorder.m_nImageID = CView::LoadSprite("DamageForegroundImage.png", &m_tBorder.m_nImageWidth, &m_tBorder.m_nImageHeight);
	m_tHitBorder.m_nImageID = CView::LoadSprite("TakenHitForegroundImage.png",&m_tHitBorder.m_nImageWidth,&m_tHitBorder.m_nImageHeight);
	CInput::GetInstance()->LoadInput("Controls");
	m_wRageButton = CInput::GetInstance()->GetButton(COMMAND_RAGE);

	m_tPressEnter.m_nImageID = CView::LoadSprite("PressEnterIcon.png", &m_tPressEnter.m_nImageHeight, &m_tBorder.m_nImageHeight);
}

bool CGameState::Input(void)
{
	if(m_pInput->Pause())
	{
		CGame::GetInstance()->PushState(CPauseState::GetInstance());
	}
	if(m_pInput->Help())
	{
		CGame::GetInstance()->PushState(CHelpScreenState::GetInstance());
	}

	// Buffered check of the tilda key if hit switch to the console state
	static bool bTildaBuffer = false;
	if(GetAsyncKeyState(VK_OEM_3))
	{
		if(!bTildaBuffer)
		{
			CGame::GetInstance()->PushState(CConsoleState::GetInstance());
		}
		bTildaBuffer = true;
	}
	else
	{
		bTildaBuffer = false;
	}

	if(m_pInput->Select())
	{
		if(CView::GetCamera().IsTransitioning() && !CGame::GetInstance()->GetWaiting())
		{
			CView::GetCamera().Clear();
			CView::GetCamera().StopTransitions();
			m_pInput->EnablePlayerInput();
		}

		else if(CWorldManager::GetInstance()->GetObjectManager()->GetCurrentSign())
		{
			CGame::GetInstance()->PushState(CPopupState::GetInstance());
			m_bDisplayMessage = false;
		}
	}

	return true;
}

void CGameState::Update(float _fElapsedTime)
{
	if(!CGame::GetInstance()->GetWaiting())
	{
		// adds to accumulated play time and will show how long you played the game at the end of the credits
		CGame::GetInstance()->AddToPlayTime(_fElapsedTime);

		// Flipped the order of these.  It was drawing THEN updating, BAAAAAD. - JC

		if(!m_bDisplayMessage)
		{
			m_bDisplayMessage = true;
		}

		CWorldManager::GetInstance()->Update(_fElapsedTime);
		CView::Update(_fElapsedTime);
		CHud::GetInstance()->Update(_fElapsedTime);
		if(CView::GetDisplayDamage() == true)
		{
			m_fDisplayTime += (1.0f * _fElapsedTime);
		}

		if(CWorldManager::GetInstance()->Lost())
		{
			//CGame::GetInstance()->ChangeState(CLossState::GetInstance());
			CGame::GetInstance()->PushState(CLossState::GetInstance());
			return;
		}

		if(CWorldManager::GetInstance()->GetRageScale() >= 1.0f)
		{
			m_fFlashTimer += m_fFlashRate * _fElapsedTime;
		}

		if( m_fFlashTimer > 1.2f || m_fFlashTimer < 0.9f )
		{
			m_fFlashRate = -m_fFlashRate;
		}

		if(CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->GetIsDefeated() && CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->GetInitialized())
		{
			m_pInput->DisablePlayerInput();
			CGame::GetInstance()->StopTimer();
		}
		if(!m_pInput->GetIsInputEnabled() && !CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->GetInitialized())
		{
			m_pInput->EnablePlayerInput();
			CGame::GetInstance()->StartTimer();
		}
	}

	// zoom when you enter the portal at the end of a stage
	// or zoom when you defeat the wizard
	if(CGame::GetInstance()->GetVictory() ||
		((m_bZoomCamera || CWorldManager::GetInstance()->GetCurrentSection() == FINAL_BOSS) && CWorldManager::GetInstance()->GetSectionComplete()) )
	{	
		// set to start the zoom
		if (m_bZoomCamera)
		{
			m_bZoomCamera = false;
			m_bCameraIsZooming = true;
			CView::GetCamera().SetOffset(vec3f(0, 4.0f, 0));
			CView::GetCamera().Update(_fElapsedTime);	// apply the offset changes before going further
			CView::GetCamera().ZoomCamera(m_fZoomDistance, m_fZoomDuration + 1.0f, false);
		}
	}
	// increase the buffer timer
	if (m_bCameraIsZooming)
	{
		m_fZoomTimer += _fElapsedTime;
	}
	// if the camera is zooming and the time is complete, fade out
	if (m_bCameraIsZooming && m_fZoomTimer >= m_fZoomDuration)
	{
		m_fZoomTimer = 0.0f;
		m_bZoomCamera = false;	// reset for next level
		m_bCameraIsZooming = false;
		CGame::GetInstance()->PushState(CFadeState::GetInstance());
		return;
	}

}

void CGameState::Render(void)
{
	
	
	if(!CGame::GetInstance()->GetWaiting())
	{
		float fAspect = float(CGame::GetInstance()->GetScreenWidth()) * 1.0f / float(CGame::GetInstance()->GetScreenHeight()) * 1.0f;
		CView::Render();


	// Moved this to the CView::Render() function so that the fade will render over the hud - Roberto
	//CView::RenderDamageImages(m_nFontID);


	CEnemy* pGuardian = CWorldManager::GetInstance()->GetObjectManager()->GetCrystalGuardian();
	if(pGuardian != NULL)
	{
		// DEPRECATED FUNCTION	
		// I didn't think we would need this function anymore so I got rid of it -Dan
		// CView::DrawGuardianDot(pGuardian->GetRenderID(), m_tGolieDot);
	}

	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;
	

	

	CSignpost* pSign = CWorldManager::GetInstance()->GetObjectManager()->GetCurrentSign();
	if(pSign)
	{
		float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth) * pSign->GetPopupScale();
		float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight) * 0.3f;

		int nHalfWidth =	static_cast<int>((pSign->GetPopup().m_nImageWidth * fScaleX )) >> 1;
		int nHalfHeight =	static_cast<int>((pSign->GetPopup().m_nImageHeight * fScaleY)) >> 1;
		int nButtonOffsetWidth =	static_cast<int>((pSign->GetPopup().m_nImageWidth) * fScaleX);
		int nButtonOffsetHeight =	static_cast<int>((pSign->GetPopup().m_nImageHeight) * fScaleY);
		if(!CView::GetCamera().IsTransitioning())
		{
			if(m_bDisplayMessage)
			{
				float fFontSize = 0.0f;
				RECT rText;
				RECT  tTextRect; //{nHalfScreenWidth - nHalfWidth, nHalfScreenHeight - nHalfHeight, nHalfScreenWidth + nButtonOffsetWidth, nHalfScreenHeight + nButtonOffsetHeight};
			

				CGame* pInst = CGame::GetInstance();

				// We won't be needing these variables anymore - Justin
				//float fWidth = float(pInst->GetScreenWidth()) / (pSign->GetPopup().m_nImageWidth * pSign->GetPopupScale())/2.0f;
				//float fHeight = float(pInst->GetScreenHeight()) / (pSign->GetPopup().m_nImageHeight * 1.7f) / 2.0f;
				//
				//float scale = pSign->GetPopupScale();

				int xPos; //= int(pInst->GetScreenWidth() / 2) - int((pSign->GetPopup().m_nImageWidth * scale/ (2.0f * fAspect)));
				int yPos;
				

				fFontSize = 1.5f;
				
				xPos = int(pInst->GetScreenWidth() / 2) - nHalfWidth;
				yPos = int(pInst->GetScreenHeight() * 0.9f) - nHalfHeight;
				
				tTextRect.top = yPos;
				tTextRect.left = xPos;
				tTextRect.right = tTextRect.left + nButtonOffsetWidth;
				tTextRect.bottom = tTextRect.top + nButtonOffsetHeight;
				
				CView::Draw(pSign->GetPopup().m_nImageID,  xPos, yPos, fScaleX, fScaleY);

				rText.left = 0;
				rText.right = pInst->GetScreenWidth();
				rText.top = long(tTextRect.bottom - 105 * fScaleY);
				rText.bottom = long(rText.top + 5 * fScaleY);
				
				CView::Write(pSign->GetFontID(), pSign->GetDescription(), &tTextRect, true, fFontSize * fScaleX / 1.3f, fFontSize * fScaleY);
				
				fFontSize -= 0.3f;
				CView::Write(pSign->GetFontID(), "Hit Select/Confirm to read more", &rText, true, fFontSize * fScaleX / 1.5f, fFontSize * fScaleY);

			}
		}
		

	}

	if(CView::GetCamera().IsTransitioning())
	{
		// HACK
		// Brought to you today by Dan, or Mr. Hack Lead for you.
		//static float counter = 0;
		//if(counter >= 4.0f) counter = 0.0f;

		//RECT sourceRect = { 0, 128 * int(counter), 256, 128 * int(counter + 1) };
		//CView::Draw(m_tPressEnter.m_nImageID, CGame::GetInstance()->GetScreenWidth() - 256, 0, 1.0f, 1.0f, &sourceRect);
		//counter += 0.25f;

		RECT tTextRect;
	
		int nScreenWidth	= CGame::GetInstance()->GetScreenWidth();
		int nScreenHeight	= CGame::GetInstance()->GetScreenHeight();

		float fScaleX = nScreenWidth / float(g_nDevelopmentResolutionWidth);
		float fScaleY = nScreenHeight / float(g_nDevelopmentResolutionHeight);
	
		string szText = "Press Enter to continue";
		tTextRect.left = 0;
		tTextRect.right = CGame::GetInstance()->GetScreenWidth();

		tTextRect.top = LONG(nScreenHeight - (150 * fScaleY));
		tTextRect.bottom = tTextRect.top;
		CView::Write(m_nFontID, szText, &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}


	// Moved this to the CView::Render() function so that the fade will render over the hud - Roberto
	//CHud::GetInstance()->DrawDamageSplash();
	//CHud::GetInstance()->DrawHealthAndRage(CWorldManager::GetInstance()->GetObjectManager()->GetPlayer(), 0 , 0);

	}
}

void CGameState::Exit(void)
{
	//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"MX_MainMenu");
	//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank2(), "PS_LevelBoss");
	//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"SFX_Heartbeat");

	CSoundManager::GetInstance()->StopAll();

	CWorldManager::GetInstance()->GetObjectManager()->UninitializeAllObjects();
	CView::Clear();
}

void CGameState::Reset()
{
	Exit();	
	CWorldManager::GetInstance()->SetBossLevelLoaded(false);
	CView::GetEmitterManager().Shutdown();
	// Load video settings to make sure player actually has their options take effect. -JC
	CVideoOptionsState::GetInstance()->LoadSettings("Video");
	
	CWorldManager::GetInstance()->Reset(false);
	if (CWorldManager::GetInstance()->GetCurrentSection() != FINAL_BOSS){
		CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MX_MainMenu");
	}

	m_tEmptyBar.m_nImageID = CView::LoadSprite("HealthBar1.png", &m_tEmptyBar.m_nImageWidth, &m_tEmptyBar.m_nImageHeight);
	m_tHealthBar.m_nImageID = CView::LoadSprite("HealthBar3.png", &m_tHealthBar.m_nImageWidth, &m_tHealthBar.m_nImageHeight);
	m_tRageBar.m_nImageID = CView::LoadSprite("RageBar2.png", &m_tRageBar.m_nImageWidth, &m_tRageBar.m_nImageHeight);
	m_nBackground = CView::LoadSprite("Volcano.jpg");
	m_tGolieDot.m_nImageID = CView::LoadSprite("GolieDot.png", &m_tGolieDot.m_nImageWidth, &m_tGolieDot.m_nImageHeight);
	m_fFlashTimer = 0.9f;
	m_fFlashRate = 0.3f;

	m_tBorder.m_nImageID = CView::LoadSprite("DamageForegroundImage.png", &m_tBorder.m_nImageWidth, &m_tBorder.m_nImageHeight);
	m_tHitBorder.m_nImageID = CView::LoadSprite("TakenHitForegroundImage.png",&m_tHitBorder.m_nImageWidth,&m_tHitBorder.m_nImageHeight);
	CInput::GetInstance()->LoadInput("Controls");
	m_wRageButton = CInput::GetInstance()->GetButton(COMMAND_RAGE);

	m_tPressEnter.m_nImageID = CView::LoadSprite("PressEnterIcon.png", &m_tPressEnter.m_nImageHeight, &m_tBorder.m_nImageHeight);
		
}

CGameState::~CGameState(void)
{

}
