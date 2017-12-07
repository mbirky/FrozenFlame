/********************************************************************
* Filename:			CPauseState.cpp
* Date:				11/4/2012
* Mod. Date:		12/8/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the pause state
********************************************************************/

#include "CPauseState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "CMainMenuState.h"
#include "../Core/CWorldManager.h"
#include "COptionsMenuState.h"

#include "CGameState.h"

CPauseState::CPauseState(void) : m_nSelected(PSO_MAIN_MENUE)
{
	m_nFontID = -1;
	m_fChangeTimer = 0.0f;
}

void CPauseState::Enter(void)
{
	m_nFontID = CView::LoadFont("font example.txt");

	m_nSelected = PSO_RESUME_GAME;
	m_bConfirmation = false;
	m_bExit = false;
	m_tUpdateTime.Start();
	

	//CSoundManager::GetInstance()->PauseSound(CGame::GetInstance()->GetSoundBank(),"Game_play_music_03_Loop");

	CGame::GetInstance()->StopTimer();

	if (CInput::GetInstance()->GetIfUsingController())
	{
		CInput::GetInstance()->SetLeftMotorSpeed(0);
		CInput::GetInstance()->SetRightMotorSpeed(0);
	}
}

bool CPauseState::Input(void)
{
	if(m_pInput->Back())
	{
		// Will need to be pop state
		CGame::GetInstance()->PopState();
	}

	if(m_pInput->Select())
	{
		if(!m_bConfirmation)
		{
			if(PSO_MAIN_MENUE == m_nSelected)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				/*if(GetPrevState())
				{*/
					m_bConfirmation = true;
				//}

				//else
				//{
				//	CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
				//}
			}
			else if(PSO_OPTIONS == m_nSelected)
			{
				CGame::GetInstance()->SetFromPause(true);
				CGame::GetInstance()->PushState(COptionsMenuState::GetInstance());
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			}
			else if(PSO_LAST_CHECKPOINT == m_nSelected)
			{				
				CGameState::GetInstance()->Reset();
				CGame::GetInstance()->PopState();
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			}
			else if(PSO_RESUME_GAME == m_nSelected)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				CGame::GetInstance()->PopState();				
			}
		}

		else
		{
			if(m_bExit)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
				m_bConfirmation = false;
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
		if(m_nSelected == PSO_MIN)
		{
			m_nSelected = PSO_MAX - 1;
		}
	}
	if(m_pInput->Menu_Down())
	{
		++m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == PSO_MAX)
		{
			m_nSelected = PSO_MIN + 1;
		}
	}

	if(m_pInput->Menu_Left() || m_pInput->Menu_Right())
	{
		if(m_fChangeTimer >= 0.5f)
		{
			if(m_bConfirmation)
			{
				m_bExit = !m_bExit;
			}
			m_fChangeTimer = 0.0f;
		}
	}

	return true;
}

void CPauseState::Update(float _fElapsedTime)
{
	CView::Update(_fElapsedTime);
	_fElapsedTime = m_tUpdateTime.GetElapsedTime();
	m_fChangeTimer += _fElapsedTime;
}

void CPauseState::Render(void)
{
	GetPrevState()->Render();

	float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth);
	float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight);

	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

	if(this == CGame::GetInstance()->GetCurrentState())
	{
		if(m_bConfirmation)
		{
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 1.0f, 1.0f, "Are you sure", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0, -2);

			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "Yes", m_bExit, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, "No", !m_bExit, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);
		}

		else
		{
			// Buttons
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Main Menu", PSO_MAIN_MENUE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Options", PSO_OPTIONS == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Retry", PSO_LAST_CHECKPOINT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, -1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Resume", PSO_RESUME_GAME == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, -2);
		}
	}
}

void CPauseState::Exit(void)
{
	//CSoundManager::GetInstance()->ResumeSound(CGame::GetInstance()->GetSoundBank(),"Gameplay_Music");

	CGame::GetInstance()->StartTimer();
}

CPauseState::~CPauseState()
{

}