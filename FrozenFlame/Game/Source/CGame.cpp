/********************************************************************
* Filename:			CGame.cpp
* Date:				10/30/2012
* Mod. Date:		11/14/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This class is used to manage the game calling the
*					current states functions.
********************************************************************/

#include "CGame.h"
#include "States/CMainMenuState.h"
#include <ctime>
#include "Core/CWorldManager.h"
#include "States/CGameState.h"
#include "States/CPauseState.h"
#include "States/CWinState.h"
#include "Renderer\CHUD.h"
#include "States\CVideoOptionsState.h"

CGame::CGame(void) : m_fElapsedTime(0.0), m_nDifficultyLevel(DIFF_MEDIUM)
{
	m_pInput = CInput::GetInstance();
	m_pSound = CSoundManager::GetInstance();
	m_pcAchievementManager = CAchievementManager::GetInstance();
	m_bFromPause = false;
	m_Timer.Reset();
	m_fGamma = 0.0f;
	m_bWaiting = false;
	m_bFullScreen = false;
	ZeroMemory(&m_WindowPlacement, sizeof(WINDOWPLACEMENT));
}

CGame::~CGame(void)
{

}

void CGame::DrawLoading(float _fPercentage)
{
	_fPercentage /= 100;

	CView::Start();

	float fScaleX = m_nScreenWidth / float(g_nDevelopmentResolutionWidth);
	float fScaleY = m_nScreenHeight / float(g_nDevelopmentResolutionHeight);

	RECT area = {0,0,m_tLoadingBackground.m_nImageWidth,m_tLoadingBackground.m_nImageHeight};

	CView::Draw(m_tLoadingBackground.m_nImageID, 0, 0, float(m_nScreenWidth) / m_tLoadingBackground.m_nImageWidth, float(m_nScreenHeight) / m_tLoadingBackground.m_nImageHeight, &area, 0, 0, 0);

	RECT tTextRect;
	tTextRect.left = 0;
	tTextRect.right = CGame::GetInstance()->GetScreenWidth();

	if(!m_bWaiting)
	{
		CView::Draw(m_tLoadingSwordEmpty.m_nImageID, m_nScreenWidth / 2 - int((m_tLoadingSwordEmpty.m_nImageWidth >> 1) * fScaleX),  
			m_nScreenHeight / 2 - int((m_tLoadingSwordEmpty.m_nImageHeight >> 1) * fScaleY), fScaleX, fScaleY);

		RECT tLoadingImageRect;
		tLoadingImageRect.left = 0;
		tLoadingImageRect.top = 0;
		tLoadingImageRect.right = LONG(m_tLoadingSwordFull.m_nImageWidth * _fPercentage);
		tLoadingImageRect.bottom = m_tLoadingSwordFull.m_nImageHeight;

		CView::Draw(m_tLoadingSwordFull.m_nImageID, m_nScreenWidth / 2 - int((m_tLoadingSwordFull.m_nImageWidth >> 1) * fScaleX),
			m_nScreenHeight / 2 - int((m_tLoadingSwordFull.m_nImageHeight >> 1) * fScaleX), fScaleX, fScaleY, &tLoadingImageRect);


		RECT tTextRect;
		tTextRect.left = 0;
		tTextRect.right = m_nScreenWidth;
		tTextRect.top = LONG(m_nScreenHeight / 2 + (100 * fScaleY));
		tTextRect.bottom = tTextRect.top;
		CView::Write(m_nFontID, "Loading", &tTextRect, true, 1.0f * fScaleX, 1.0f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		tTextRect.top = CGame::GetInstance()->GetScreenHeight() - (CGame::GetInstance()->GetScreenHeight() >> 2);
		tTextRect.bottom = CGame::GetInstance()->GetScreenHeight();
		CView::Write(m_nFontID, "Press Enter to continue", &tTextRect, true, 0.6f);
	}
	CView::Finish();
}

void CGame::Init(HWND hWnd, HINSTANCE _hInstance, int _nScreenWidth, int _nScreenHeight, bool _bFullScreen)
{

	m_hWnd = hWnd;
	m_hInstance = _hInstance;
	
	m_nScreenWidth = _nScreenWidth;
	m_nScreenHeight = _nScreenHeight;
	
	CView::Initialize(m_hWnd, _nScreenWidth, _nScreenHeight);

	_bFullScreen ? SetFullScreenOn() : SetFullScreenOff();

	m_tLoadingBackground.m_nImageID = CView::LoadSprite("loadingbg.png", &m_tLoadingBackground.m_nImageWidth, &m_tLoadingBackground.m_nImageHeight);
	m_tLoadingSwordEmpty.m_nImageID = CView::LoadSprite("load0.png", &m_tLoadingSwordEmpty.m_nImageWidth, &m_tLoadingSwordEmpty.m_nImageHeight);
	m_tLoadingSwordFull.m_nImageID = CView::LoadSprite("load100.png", &m_tLoadingSwordFull.m_nImageWidth, &m_tLoadingSwordFull.m_nImageHeight);
	m_nFontID = CView::LoadFont("font example.txt");


	DrawLoading(0);

	m_pcAchievementManager->Init();

	DrawLoading(10);

	CWorldManager::GetInstance()->Init();

	DrawLoading(20);

	m_pSound->Init();
	m_nSoundBank = m_pSound->Loadbank("SoundBank.bnk","Final Sound Names.txt");
	m_nSoundBank2 = m_pSound->Loadbank("SoundBank2.bnk","Test SoundBank.txt");
	m_pSound->SetUpSoundObjects();

	DrawLoading(33);

	CHud::GetInstance()->LoadHUDObjects();

	m_Timer.Start();
	srand(unsigned int(time(0)));

	m_fMusicVolume = 50;
	m_fSFXVolume = 50;

	DrawLoading(50);

	fstream in;

	in.open("Resource/Options/Sound.txt", ios_base::in | ios_base::binary);
	if(in.is_open())
	{
		in.read((char*)&m_fMasterVolume, sizeof(unsigned short));
		in.read((char*)&m_fMusicVolume, sizeof(unsigned short));
		in.read((char*)&m_fSFXVolume, sizeof(unsigned short));
		in.read((char*)&m_bIsMuteOn, sizeof(bool));
	}
	in.close();

	unsigned char ucRFlags;
	unsigned int  unTemp;
	in.open("Resource/Options/Video.txt", ios_base::in | ios_base::binary);
	if(in.is_open())
	{
		in.read((char*)&ucRFlags, sizeof(unsigned char));
		in.read((char*)&unTemp, sizeof(unsigned int));
		in.read((char*)&m_fGamma, sizeof(float));
		in.close();
	}
	else
	{
		m_fGamma = 1.0f;
	}
	
	if(m_bIsMuteOn == false)
	{
		CSoundManager::GetInstance()->AdjustMaster((float)m_fMasterVolume);
	}
	else
	{
		CSoundManager::GetInstance()->AdjustMaster(0);
	}

	CSoundManager::GetInstance()->AdjustMusic((float)m_fMusicVolume);
	CSoundManager::GetInstance()->AdjustSFX((float)m_fSFXVolume);

	m_bDoneSplashing = false;

	ChangeState(CMainMenuState::GetInstance());

	DrawLoading(100);
}

eGameReturn CGame::Main(void)
{
	if(Input() == false)
	{
		return GAMERETURN_QUIT;
	}

	Update();
	Render();

	return GAMERETURN_RUN;
}

void CGame::Shutdown(void)
{
	// Shutdown all the states
	while(m_pCurrentState)
	{
		IGameState *tmp = m_pCurrentState;

		m_pCurrentState = m_pCurrentState->GetPrevState();

		tmp->Exit();
	}

	m_pSound->Unloadbank(m_nSoundBank);
	m_pSound->Shutdown();
	m_pcAchievementManager->SaveAchievements();
	m_pcAchievementManager->Shutdown();

	CWorldManager::GetInstance()->Shutdown();

	CView::Shutdown();
}

bool CGame::Input(void)
{
	m_pInput->Update();

	if(m_pInput->Quit())
	{
		return false;
	}

	if(m_pInput->AltSelect())
	{
		if(m_bFullScreen)
			SetFullScreenOff();
		else
			SetFullScreenOn();

		CVideoOptionsState::GetInstance()->UpdateFlags();
	}

	if(m_pCurrentState)
	{
		return m_pCurrentState->Input();
	}

	return false;
}

void CGame::Update(void)
{
	// Check if the victory condition is already set and reset it
	/* This snippet is being moved to a condition for the zoom in GameState's Update()
	/*if(m_bVictory)
	{
		ChangeState(CWinState::GetInstance());
		m_bVictory = false;
	}*/

	m_fElapsedTime = m_Timer.GetElapsedTime();

	m_pCurrentState->Update(m_fElapsedTime);
	m_pSound->Update();
	m_pcAchievementManager->UpdateAchievements(m_fElapsedTime);
}

void CGame::Render(void)
{
	CView::Start();
	{
		m_pCurrentState->Render();
	}
	m_pcAchievementManager->Render();
	CView::Finish();
}

void CGame::ChangeState(IGameState* _pNewState)
{
	while(m_pCurrentState)
	{
		IGameState *tmp = m_pCurrentState;

		m_pCurrentState = m_pCurrentState->GetPrevState();

		tmp->Exit();
	}

	m_pCurrentState = _pNewState;

	if(m_pCurrentState)
	{
		m_pCurrentState->Enter();
	}
}

void CGame::PushState(IGameState* _pNewState)
{
	if(_pNewState)
	{
		_pNewState->SetPrevState(m_pCurrentState);
		m_pCurrentState = _pNewState;
		m_pCurrentState->Enter();
	}
}

void CGame::PopState(void)
{
	if(m_pCurrentState)
	{
		IGameState* tmp = m_pCurrentState;

		m_pCurrentState = m_pCurrentState->GetPrevState();

		if(tmp)
		{
			tmp->Exit();
		}
	}
}

void CGame::StartTimer(void)
{
	m_Timer.Reset();
	m_Timer.Start();
}

void CGame::StopTimer(void)
{
	m_Timer.Stop();
}

float CGame::GetRunningGameTime()
{
	return m_Timer.GetTotalTime();
}

void CGame::GainFocus(void)
{
	m_pInput->Enable();
}

void CGame::LoseFocus(void)
{
	// if our current state is a game state, let's push a pause
	if(dynamic_cast<CGameState*>(m_pCurrentState))
	{
		PushState(CPauseState::GetInstance());
	}
	m_pInput->Disable();
}

void CGame::SetFullScreenOn(void)
{
	if(m_bFullScreen) return;

	// Remember the window position.
	m_WindowPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hWnd, &m_WindowPlacement);
	// Remove the window's title bar.
	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
	// Put the changes to the window into effect.
	SetWindowPos(m_hWnd, 0, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	// Position the window to cover the entire screen.
	SetWindowPos(m_hWnd, nullptr,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_SHOWWINDOW);

	m_bFullScreen = true;
}

void CGame::SetFullScreenOff(void)
{
	if(!m_bFullScreen) return;

	// Restore the window's title bar.
	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	// Put the changes to the window into effect.
	SetWindowPos(m_hWnd, 0, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	// Restore the window's original position.
	SetWindowPlacement(m_hWnd, &m_WindowPlacement);

	m_bFullScreen = false;
}

void CGame::SetRenderFlags(unsigned char ucRenderFlags)
{
	CView::SetViewFlags(ucRenderFlags);

	if(ucRenderFlags & RF_FULLSCREEN)
		SetFullScreenOn();
	else
		SetFullScreenOff();
}