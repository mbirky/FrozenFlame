/********************************************************************
* Filename:			CLossState.h
* Date:				11/6/2012
* Mod. Date:		11/6/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Loss state
********************************************************************/

#include "CLossState.h"
#include "CGameState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "CMainMenuState.h"
#include "../Core/CWorldManager.h"

void CLossState::Enter(void)
{
	m_nSelected = LSO_LAST_CKECKPOINT;

	m_nFontID = CView::LoadFont("font example.txt");

	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"DX_PlayerDie");

	m_nSelected = LSO_LAST_CKECKPOINT;
	m_fFade = 0.0f;
	m_cFadeState = FADE_IN;

	m_nImageID = CView::LoadSprite("Fade.png");

	if (CInput::GetInstance()->GetIfUsingController())
	{
		CInput::GetInstance()->SetLeftMotorSpeed(0);
		CInput::GetInstance()->SetRightMotorSpeed(0);
	}

	CSoundManager::GetInstance()->StopAll();
	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_MX_DEFEAT, GAME_OBJECT_ID_PLAYER);
}

bool CLossState::Input(void)
{
	if(FADE_COMPLETE != m_cFadeState)
	{
		return true;
	}

	if(m_pInput->Select())
	{
		CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
		m_cFadeState = FADE_OUT;
	}

	if(m_pInput->Up())
	{
		--m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == LSO_MIN)
		{
			m_nSelected = LSO_MAX - 1;
		}
	}
	if(m_pInput->Down())
	{
		++m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == LSO_MAX)
		{
			m_nSelected = LSO_MIN + 1;
		}
	}
	return true;
}

void CLossState::Update(float _fElapsedTime)
{
	CView::Update(_fElapsedTime);

	if(FADE_IN == m_cFadeState)
	{
		m_fFade += 0.5f * _fElapsedTime;
		
		if(m_fFade >= 1.0f)
		{
			m_fFade = 1.0f;
			m_cFadeState = FADE_COMPLETE;
		}
	}
	else if(FADE_OUT == m_cFadeState)
	{
		m_fFade -= 0.5f * _fElapsedTime;

		if(m_fFade <= 0.0f)
		{
			m_fFade = 0.0f;
			m_cFadeState = FADE_NOT;
		}
	}
	else if(FADE_NOT == m_cFadeState)
	{
		if(LSO_LAST_CKECKPOINT == m_nSelected)
		{
			CWorldManager::GetInstance()->Reset(false);
			CGame::GetInstance()->PopState();
			//if (CGame::GetInstance()->GetCurrentState() == CGameState::GetInstance() && CWorldManager::GetInstance()->GetCurrentSection() != FINAL_BOSS)
			//{	// restart the game music if we're going back to gameplay state
			//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MX_MainMenu");
			//}
			return;
		}
		else if(LSO_MAIN_MENU == m_nSelected)
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			return;
		}
	}
}

void CLossState::Render(void)
{
	if(IGameState* prev = GetPrevState())
	{
		prev->Render();
	}

	D3DXCOLOR color(1.0f, 1.0f, 1.0f, m_fFade);

	int nYOffset = 50;

	RECT  tTextRect = {0, 0, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight() >> 1};

	
	
	CView::Write(m_nFontID, "YOU LOST", &tTextRect, true, 2.0f, color);

	if(FADE_COMPLETE == m_cFadeState)
	{
		CView::DrawButton(CGame::GetInstance()->GetScreenWidth() >> 1, CGame::GetInstance()->GetScreenHeight() >> 1, 1.0f, 1.0f, "Retry", LSO_LAST_CKECKPOINT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
		CView::DrawButton(CGame::GetInstance()->GetScreenWidth() >> 1, CGame::GetInstance()->GetScreenHeight() >> 1, 1.0f, 1.0f, "Main Menu", LSO_MAIN_MENU == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 1);
	}


}

void CLossState::Exit(void)
{

}
