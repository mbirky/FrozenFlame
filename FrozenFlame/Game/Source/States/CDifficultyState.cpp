/********************************************************************
* Filename:			CDifficultyState.h
* Date:				1/19/2012
* Mod. Date:		1/19/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Difficulty state
********************************************************************/

#include "CDifficultyState.h"
#include "CGameState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "CMainMenuState.h"
#include "../Core/CWorldManager.h"

void CDifficultyState::Enter(void)
{
	m_nFontID = CView::LoadFont("font example.txt");

	m_tBackground.m_nImageID = CView::LoadSprite("fire-and-ice.jpg", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &m_tButtonHover.m_nImageWidth, &m_tButtonHover.m_nImageHeight);

	// Set the currently selected to the current difficulty
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		m_nSelected = DSO_EASY;
		break;
	case DIFF_MEDIUM:
		m_nSelected = DSO_MEDIUM;
		break;
	case DIFF_HARD:
		m_nSelected = DSO_HARD;
		break;
	}
}

bool CDifficultyState::Input(void)
{
	if(m_pInput->Select())
	{
		switch(m_nSelected)
		{
		case DSO_EASY:
			CGame::GetInstance()->SetDifficultyLevel(DIFF_EASY);
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			break;
		case DSO_MEDIUM:
			CGame::GetInstance()->SetDifficultyLevel(DIFF_MEDIUM);
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			break;
		case DSO_HARD:
			CGame::GetInstance()->SetDifficultyLevel(DIFF_HARD);
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			break;
		case DSO_BACK:
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
			CGame::GetInstance()->PopState();			
			break;
		}
	}

	if(m_pInput->Up())
	{
		--m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == DSO_MIN)
		{
			m_nSelected = DSO_MAX - 1;
		}
	}
	if(m_pInput->Down())
	{
		++m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == DSO_MAX)
		{
			m_nSelected = DSO_MIN + 1;
		}
	}
	return true;
}

void CDifficultyState::Update(float _fElapsedTime)
{
}

void CDifficultyState::Render(void)
{
	CView::Draw(m_tBackground.m_nImageID, 0, 0,  float(CGame::GetInstance()->GetScreenWidth()) / m_tBackground.m_nImageWidth,
			float(CGame::GetInstance()->GetScreenHeight()) / m_tBackground.m_nImageHeight);

	float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth);
	float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight);

	// Draw the current difficulty above the buttons
	RECT tCurrentDifficulty = {CGame::GetInstance()->GetScreenWidth() >> 1, LONG((CGame::GetInstance()->GetScreenHeight() >> 2) + (20 * fScaleY)), CGame::GetInstance()->GetScreenWidth() >> 1, LONG((CGame::GetInstance()->GetScreenHeight() >> 2) + (20 * fScaleY))};
	CView::Write(m_nFontID, "Current Difficulty:", &tCurrentDifficulty, true, 0.6f * fScaleX, 0.6f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// Move down some then write the difficulty
	tCurrentDifficulty.top += LONG(50.0f * fScaleY);	// The 50 is the hight that i need to move down so that i do not overlap text
	tCurrentDifficulty.bottom = tCurrentDifficulty.top;
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		CView::Write(m_nFontID, "Easy", &tCurrentDifficulty, true, 1.0f * fScaleX, 1.0f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case DIFF_MEDIUM:
		CView::Write(m_nFontID, "Medium", &tCurrentDifficulty, true, 1.0f * fScaleX, 1.0f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case DIFF_HARD:
		CView::Write(m_nFontID, "Hard", &tCurrentDifficulty, true, 1.0f * fScaleX, 1.0f * fScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	}

	// Set up variable for drawing buttons
	int nButtonOffset = 10;	// This is the distance between the buttons
	int nButtonMidX = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nButtonMidY = (CGame::GetInstance()->GetScreenHeight() >> 1) - m_tButtonHover.m_nImageHeight - (nButtonOffset << 1);

	// Draw the Easy button
	DrawButton(nButtonMidX, nButtonMidY, 0.8f, 0.8f, "Easy", DSO_EASY == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 1);

	// Move the button Draw down then draw Medium
	DrawButton(nButtonMidX, nButtonMidY, 0.8f, 0.8f, "Medium", DSO_MEDIUM == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 2);

	// Move the button Draw down then draw Hard
	DrawButton(nButtonMidX, nButtonMidY, 0.8f, 0.8f, "Hard", DSO_HARD == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 3);

	// Move the button Draw down then draw Back
	DrawButton(nButtonMidX, nButtonMidY, 0.8f, 0.8f, "Back", DSO_BACK == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0, 4);
}

void CDifficultyState::Exit(void)
{

}

void CDifficultyState::DrawButton(int _nMidX, int _nMidY, float _fScaleX, float _fScaleY, string _szText, bool _bHover, D3DXCOLOR d3dColor, float _fNumButtonWidthOffsets, float _fNumButtonHeightOffsets)
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
		CView::Write(m_nFontID, _szText, &tTextRect, true, 0.75f * _fScaleX, 0.6f * _fScaleY, d3dColor);
	}
	else
	{
		int nHalfWidth = static_cast<int>((m_tButtonNormal.m_nImageWidth * _fScaleX)) >> 1;
		int nHalfHeight = static_cast<int>((m_tButtonNormal.m_nImageHeight * _fScaleY)) >> 1;
		int nButtonOffsetWidth = static_cast<int>((m_tButtonNormal.m_nImageWidth * _fNumButtonWidthOffsets) * _fScaleX);
		int nButtonOffsetHeight = static_cast<int>((m_tButtonNormal.m_nImageHeight * _fNumButtonHeightOffsets) * _fScaleY);

		CView::Draw(m_tButtonNormal.m_nImageID, _nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight, _fScaleX, _fScaleY, nullptr, 0.0f, 0.0f, 0.0f, d3dColor);
		
		RECT  tTextRect = {_nMidX - nHalfWidth + nButtonOffsetWidth, _nMidY - nHalfHeight + nButtonOffsetHeight,  _nMidX + nHalfWidth + nButtonOffsetWidth, _nMidY + nHalfHeight + nButtonOffsetHeight};
		CView::Write(m_nFontID, _szText, &tTextRect, true, 0.75f * _fScaleX, 0.6f * _fScaleY, d3dColor);
	}
}