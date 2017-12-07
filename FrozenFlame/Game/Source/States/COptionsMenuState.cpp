/********************************************************************
* Filename:			COptionsMenuState.cpp
* Date:				11/17/2012
* Mod. Date:		11/17/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the options menu state
********************************************************************/

#include "CSoundOptionState.h"
#include "COptionsMenuState.h"
#include "CVideoOptionsState.h"
#include "CControlsState.h"
#include "CMainMenuState.h"
#include "CGameState.h"
#include "../Renderer/View.h"
#include "../Util/CFontManager.h"
#include "../CGame.h"


COptionsMenuState::COptionsMenuState(void) : m_nSelected(OPTION_SOUND), m_nFontID(-1)
{

}

void COptionsMenuState::Enter(void)
{
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &m_tButtonHover.m_nImageWidth, &m_tButtonHover.m_nImageHeight);
	m_tBackground.m_nImageID = CView::LoadSprite("fire-and-ice.jpg", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);

	m_nFontID = CView::LoadFont("font example.txt");

	m_nSelected = OPTION_MIN + 1;
}

bool COptionsMenuState::Input(void)
{
	if(m_pInput->Select())
	{

		if(OPTION_SOUND == m_nSelected)
		{
			CGame::GetInstance()->PushState(CSoundOptionState::GetInstance());
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
		}
		
		else if(OPTION_VIDEO == m_nSelected)
		{
			CGame::GetInstance()->PushState(CVideoOptionsState::GetInstance());
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
		}

		else if(OPTION_CONTROLS == m_nSelected)
		{
			CGame::GetInstance()->PushState(CControlsState::GetInstance());
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
		}

		else if(OPTION_EXIT == m_nSelected)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
			CGame::GetInstance()->PopState();			
		}
	}

	if(m_pInput->Menu_Up())
	{
		--m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == OPTION_MIN)
		{
			m_nSelected = OPTION_MAX - 1;
		}
	}
	if(m_pInput->Menu_Down())
	{
		++m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == OPTION_MAX)
		{
			m_nSelected = OPTION_MIN + 1;
		}
	}
	return true;
}

void COptionsMenuState::Update(float _fElapsedTime)
{
	CView::Update(_fElapsedTime);
}

void COptionsMenuState::Render(void)
{
	if(CGame::GetInstance()->GetFromPause() == false)
	{
		// Backgournd
		CView::Draw(m_tBackground.m_nImageID, 0, 0, float(CGame::GetInstance()->GetScreenWidth()) / m_tBackground.m_nImageWidth,
			float(CGame::GetInstance()->GetScreenHeight()) / m_tBackground.m_nImageHeight);
	}
	else if(CGame::GetInstance()->GetFromPause() == true)
	{
		GetPrevState()->Render();
	}

	if(this == CGame::GetInstance()->GetCurrentState())
	{
		int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
		int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Sound", OPTION_SOUND == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, -1);
		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Video", OPTION_VIDEO == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 0);
		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Controls", OPTION_CONTROLS == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 1);
		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Back", OPTION_EXIT == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 2);
	}
}

void COptionsMenuState::Exit(void)
{
	if(CGame::GetInstance()->GetFromPause() == true)
	{
		CGame::GetInstance()->SetFromPause(false);
	}
}

COptionsMenuState::~COptionsMenuState()
{

}
