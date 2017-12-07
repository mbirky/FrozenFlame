/********************************************************************
* Filename:			CMainMenuState.cpp
* Date:				10/31/2012
* Mod. Date:		10/31/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the main game state
********************************************************************/

#include "CMainMenuState.h"
#include "CCreditsState.h"
#include "CGameState.h"
#include "COptionsMenuState.h"
#include "CDifficultyState.h"
#include "CAchievementState.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include "../Util/CFontManager.h"
#include "../Core/CWorldManager.h"

#include "../Renderer/AssetManager.h"

#define SPLASHPATH "Splash Screens/"

CMainMenuState::CMainMenuState(void) : m_nSelected(SO_START_GAME), m_nFontID(-1), m_bIsSplashing(false), m_cSplash(-1)
{

}

void CMainMenuState::Enter(void)
{
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &m_tButtonHover.m_nImageWidth, &m_tButtonHover.m_nImageHeight);
	m_tBackground.m_nImageID = CView::LoadSprite("mainmenubg.png", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);

	// update the loading bar
	CGame::GetInstance()->DrawLoading(60);

	m_nFontID = CView::LoadFont("font example.txt");

	// update the loading bar
	CGame::GetInstance()->DrawLoading(70);

	m_bConfirmation = false;
	m_bExit = false;

	if(CGame::GetInstance()->GetSplashing() == false)
	{
		string toLoad = SPLASHPATH;
		m_tScreens[SPLASH_GPGAMES].m_nTextureID		= CView::LoadSprite((toLoad + "GPGlogo.png").c_str(),		&m_tScreens[SPLASH_GPGAMES].width,		&m_tScreens[SPLASH_GPGAMES].height);
		m_tScreens[SPLASH_BITBOSSES].m_nTextureID	= CView::LoadSprite((toLoad + "Bit Bosses.png").c_str(),	&m_tScreens[SPLASH_BITBOSSES].width,	&m_tScreens[SPLASH_BITBOSSES].height);
		m_tScreens[SPLASH_8BITBACON].m_nTextureID	= CView::LoadSprite((toLoad + "8BitBacon.png").c_str(),		&m_tScreens[SPLASH_8BITBACON].width,	&m_tScreens[SPLASH_8BITBACON].height);
		m_tScreens[SPLASH_BITSHIFT].m_nTextureID	= CView::LoadSprite((toLoad + "BitShift.png").c_str(),		&m_tScreens[SPLASH_BITSHIFT].width,		&m_tScreens[SPLASH_BITSHIFT].height);
		m_tScreens[SPLASH_DIRECTX].m_nTextureID		= CView::LoadSprite((toLoad + "DirectX.png").c_str(),		&m_tScreens[SPLASH_DIRECTX].width,		&m_tScreens[SPLASH_DIRECTX].height);
		m_tScreens[SPLASH_WWISE].m_nTextureID		= CView::LoadSprite((toLoad + "Wwise.png").c_str(),			&m_tScreens[SPLASH_WWISE].width,		&m_tScreens[SPLASH_WWISE].height);


		for(int i = 0; i < SPLASH_TOTAL; i++)
		{
			m_tScreens[i].m_fCurrentLife = 0.0f;
			m_tScreens[i].m_fLifeTime = 5.0f;

			m_tScreens[i].m_cColor = D3DXCOLOR(1,1,1,0);
			m_tScreens[i].m_cStartColor = D3DXCOLOR(1,1,1,0);
			m_tScreens[i].m_cEndColor = D3DXCOLOR(1,1,1,1);
			
		}

		m_bIsSplashing = true;
		m_cSplash = SPLASH_GPGAMES;
	}
	
	// update the loading bar
	CGame::GetInstance()->DrawLoading(80);

	CWorldManager::GetInstance()->Reset(true);

	CSoundManager::GetInstance()->StopAll();
	if(CGame::GetInstance()->GetSplashing() == true)
	{
		CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MX_GamePlay");
	}

	// set the minion kill count to 0 when entering the main menu
	CGame::GetInstance()->ResetMinionKillCount();
}

bool CMainMenuState::Input(void)
{
	if(m_bIsSplashing == false)
	{
		if(m_pInput->Select())
		{
			
			if(!m_bConfirmation)
			{
				if(SO_START_GAME == m_nSelected)
				{
					CGame::GetInstance()->ChangeState(CGameState::GetInstance());
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				}

				if(SO_DIFFICULTY == m_nSelected)
				{
					CGame::GetInstance()->PushState(CDifficultyState::GetInstance());
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				}

				if(SO_OPTIONS == m_nSelected)
				{
					CGame::GetInstance()->PushState(COptionsMenuState::GetInstance());
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				}

				if(SO_CREDITS == m_nSelected)
				{
					CGame::GetInstance()->PushState(CCreditsState::GetInstance());
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				}

				if (SO_ACHIEVEMENTS == m_nSelected)
				{
					CGame::GetInstance()->PushState(CAchievementState::GetInstance());
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				}

				else if(SO_EXIT == m_nSelected)
				{
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
					m_bConfirmation = true;
					
				}
			}

			else
			{
				if(m_bExit)
				{
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
					return false;
				}

				else
				{
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
					m_bConfirmation = false;
				}
			}
		}

		if(m_pInput->Menu_Up())
		{
			--m_nSelected;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			if(m_nSelected == SO_MIN)
			{
				m_nSelected = SO_MAX - 1;
			}
		}
		if(m_pInput->Menu_Down())
		{
			++m_nSelected;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			if(m_nSelected == SO_MAX)
			{
				m_nSelected = SO_MIN + 1;
			}
		}

		if(m_pInput->Menu_Left() || m_pInput->Menu_Right())
		{
			if(m_bConfirmation)
			{
				if(m_fChangeTimer >= 0.5f)
				{
					m_bExit = !m_bExit;
					m_fChangeTimer = 0.0f;
				}
			}
		}
	}

	return true;
}

void CMainMenuState::Update(float _fElapsedTime)
{
	CView::Update(_fElapsedTime);
	m_fChangeTimer += _fElapsedTime;

	if(m_bIsSplashing == true)
	{
		m_tScreens[m_cSplash].m_fCurrentLife += _fElapsedTime;

		D3DXColorLerp(&m_tScreens[m_cSplash].m_cColor,&m_tScreens[m_cSplash].m_cStartColor,
			&m_tScreens[m_cSplash].m_cEndColor,m_tScreens[m_cSplash].m_fCurrentLife / m_tScreens[m_cSplash].m_fLifeTime);

		if(abs(m_tScreens[m_cSplash].m_cColor.a - m_tScreens[m_cSplash].m_cEndColor.a) <= 0.1f)
			m_cSplash++;

		static bool buff = true;

		if(m_pInput->Right() && buff == true)
		{
			m_cSplash++;
			buff = false;
		}
		else
		{
			if(!m_pInput->Right())
			{
				buff = true;
			}
		}

		if(m_pInput->Select())
		{
			m_cSplash = SPLASH_TOTAL;
		}

		if(m_cSplash >= SPLASH_TOTAL)
		{
			m_bIsSplashing = false;
			CGame::GetInstance()->SetSplashing(true);
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MX_GamePlay");
		}
	}
}

void CMainMenuState::Render(void)
{
	if(m_bIsSplashing == true)
	{
		float fHalfScreenW = CGame::GetInstance()->GetScreenWidth() / 2.0f;
		float fHalfScreenH = CGame::GetInstance()->GetScreenHeight() / 2.0f;
	
		float fScale = 1.0f;
		int nDiffW = m_tScreens[m_cSplash].width - CGame::GetInstance()->GetScreenWidth();
		int nDiffH = m_tScreens[m_cSplash].height - CGame::GetInstance()->GetScreenHeight();

		if(nDiffW > 0 || nDiffH > 0)
		{
			if(nDiffW > nDiffH)
				fScale = 1.0f - float(nDiffW) / m_tScreens[m_cSplash].width;
			else
				fScale = 1.0f - float(nDiffH) / m_tScreens[m_cSplash].height;
		}
		
		float fHalfWidth = fScale * m_tScreens[m_cSplash].width / 2.0f;
		float fHalfHeight = fScale * m_tScreens[m_cSplash].height / 2.0f;
		
		CView::Draw(m_tScreens[m_cSplash].m_nTextureID, int(fHalfScreenW - fHalfWidth), int(fHalfScreenH - fHalfHeight),
			fScale, fScale, nullptr, 0.0f, 0.0f, 0.0f, m_tScreens[m_cSplash].m_cColor);
	}
	else
	{
		// Backgournd
		CView::Draw(m_tBackground.m_nImageID, 0, 0, float(CGame::GetInstance()->GetScreenWidth()) / m_tBackground.m_nImageWidth,
			float(CGame::GetInstance()->GetScreenHeight()) / m_tBackground.m_nImageHeight);

		int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
		int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

		int nButtonHoverPosX = nHalfScreenWidth - (m_tButtonHover.m_nImageWidth >> 1);
		int nButtonNormalPosX = nHalfScreenWidth - (m_tButtonNormal.m_nImageWidth >> 1);
		int nButtonHoverPosY = nHalfScreenHeight - (m_tButtonHover.m_nImageHeight >> 1);
		int nButtonNormalPosY = nHalfScreenHeight - (m_tButtonNormal.m_nImageHeight >> 1);
		int ButtonYOffset = 10;

		RECT tTextRect;

		if(m_bConfirmation)
		{
			float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth);
			float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight);

			tTextRect.left		=	0;
			tTextRect.top		=	LONG(nHalfScreenHeight - (nHalfScreenHeight >> 1));
			tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
			tTextRect.bottom	=	tTextRect.top + 50;
			CView::Write(m_nFontID, "Are you sure?", &tTextRect, true, 0.5f * fScaleX, 0.5f * fScaleY);


			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Yes",	m_bExit,	D3DXCOLOR(1.0, 1.0, 1.0, 1.0), -1, 0);
			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "No",	!m_bExit,	D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 1, 0);
		}
		else
		{

			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Start",		SO_START_GAME == m_nSelected,	D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, -2);
			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Difficulty",	SO_DIFFICULTY == m_nSelected,	D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, -1);
			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Options",		SO_OPTIONS == m_nSelected,		D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 0);
			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Challenges",	SO_ACHIEVEMENTS == m_nSelected,	D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 1);
			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Credits",		SO_CREDITS == m_nSelected,		D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 2);
			DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Exit",			SO_EXIT == m_nSelected,			D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 3);

			float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth);
			float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight);
			tTextRect.left		=	0;
			tTextRect.top		=	LONG((nHalfScreenHeight << 1) - (80 * fScaleY));
			tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
			tTextRect.bottom	=	tTextRect.top + 10;
			CView::Write(m_nFontID, "Up and Down Arrows change selection,", &tTextRect, true, 0.4f * fScaleX, 0.4f * fScaleY);

			tTextRect.top		=	LONG((nHalfScreenHeight << 1) - (40 * fScaleY));
			tTextRect.bottom	=	tTextRect.top + 10;
			CView::Write(m_nFontID, "Enter to select", &tTextRect, true, 0.4f * fScaleX, 0.4f * fScaleY);
		}
	}
}

void CMainMenuState::Exit(void)
{
	CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"MX_GamePlay");
}

CMainMenuState::~CMainMenuState()
{

}

void CMainMenuState::DrawButton(int _nMidX, int _nMidY, float _fScaleX, float _fScaleY, string _szText, bool _bHover, D3DXCOLOR d3dColor, float _fNumButtonWidthOffsets, float _fNumButtonHeightOffsets)
{
	// Fix the scale for the resolution

	_fScaleX *= (CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth));
	_fScaleY *= (CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight));

	if(_bHover)
	{
		int nHalfWidth =	static_cast<int>((m_tButtonHover.m_nImageWidth * _fScaleX)) >> 1;
		int nHalfHeight =	static_cast<int>((m_tButtonHover.m_nImageHeight * _fScaleY)) >> 1;
		int nButtonOffsetWidth =	static_cast<int>((m_tButtonHover.m_nImageWidth * _fNumButtonWidthOffsets) * _fScaleX);
		int nButtonOffsetHeight =	static_cast<int>((m_tButtonHover.m_nImageHeight * _fNumButtonHeightOffsets) * _fScaleY);

		CView::Draw(m_tButtonHover.m_nImageID, _nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _fScaleX, _fScaleY, nullptr, 0.0f, 0.0f, 0.0f, d3dColor);

		RECT  tTextRect = {_nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _nMidX + nHalfWidth + nButtonOffsetWidth, _nMidY + nHalfHeight + nButtonOffsetHeight};
		CView::Write(m_nFontID, _szText, &tTextRect, true, 0.6f * _fScaleX, 0.6f * _fScaleY, d3dColor);
	}
	else
	{
		int nHalfWidth = static_cast<int>((m_tButtonNormal.m_nImageWidth * _fScaleX)) >> 1;
		int nHalfHeight = static_cast<int>((m_tButtonNormal.m_nImageHeight * _fScaleY)) >> 1;
		int nButtonOffsetWidth = static_cast<int>((m_tButtonNormal.m_nImageWidth * _fNumButtonWidthOffsets) * _fScaleX);
		int nButtonOffsetHeight = static_cast<int>((m_tButtonNormal.m_nImageHeight * _fNumButtonHeightOffsets) * _fScaleY);

		CView::Draw(m_tButtonNormal.m_nImageID, _nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _fScaleX, _fScaleY, nullptr, 0.0f, 0.0f, 0.0f, d3dColor);
		
		RECT  tTextRect = {_nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight,  _nMidX + nHalfWidth + nButtonOffsetWidth, _nMidY + nHalfHeight + nButtonOffsetHeight};
		CView::Write(m_nFontID, _szText, &tTextRect, true, 0.6f * _fScaleX, 0.6f * _fScaleY, d3dColor);
	}
}