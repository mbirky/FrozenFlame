#include "CAchievementState.h"
#include "CGameState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "CMainMenuState.h"
#include "../Achievements/Header/CAchievementManager.h"

void CAchievementState::Enter()
{
	m_nFontID = CView::LoadFont("font example.txt");

	m_tBackground.m_nImageID = CView::LoadSprite("fire-and-ice.jpg", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &m_tButtonHover.m_nImageWidth, &m_tButtonHover.m_nImageHeight);

	m_nSelected = ACV_NEXT;
	m_nCurrentPage = 1;
	m_nMaxPages = 0;

	m_bPrevLeft = false;
	m_bPrevRight = false;

	m_nMaxPages = CAchievementManager::GetInstance()->GetAchievementSize() / 3;
	if (CAchievementManager::GetInstance()->GetAchievementSize() % 3 != 0)
		m_nMaxPages++;
}

bool CAchievementState::Input()
{
	switch (m_nSelected)
	{
	case ACV_BACK:
		{
			if (m_pInput->Select())
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				CGame::GetInstance()->PopState();
			}
		}
		break;

	case ACV_PREV:
		{
			if (m_pInput->Select())
			{
				m_nCurrentPage--;
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				if (m_nCurrentPage <= 0)
					m_nCurrentPage = m_nMaxPages;
			}
		}
		break;

	case ACV_NEXT:
		{
			if (m_pInput->Select())
			{
				m_nCurrentPage++;
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				if (m_nCurrentPage > m_nMaxPages)
					m_nCurrentPage = 1;
			}
		}
		break;

	case ACV_DELETE:
		{
			if (m_pInput->Select())
			{
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				CAchievementManager::GetInstance()->DeleteAchievements();
			}
		}
		break;
	}

	if (m_pInput->Menu_Left())
	{
		if (!m_bPrevLeft)
		{
			m_bPrevLeft = true;
			m_nSelected--;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			if (m_nSelected < 0)
				m_nSelected = ACV_MAX - 1;
		}
	}
	else
	{
		m_bPrevLeft = false;
	}

	if (m_pInput->Menu_Right())
	{
		if (!m_bPrevRight)
		{
			m_bPrevRight = true;
			m_nSelected++;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			if (m_nSelected >= ACV_MAX)
				m_nSelected = 0;
		}
	}
	else
	{
		m_bPrevRight = false;
	}
	return true;
}

void CAchievementState::Update(float _fElapsedTime)
{
	//CView::Update(_fElapsedTime);
}

void CAchievementState::Render()
{
	CView::Draw(m_tBackground.m_nImageID, 0, 0, float(CGame::GetInstance()->GetScreenWidth()) / m_tBackground.m_nImageWidth,
		float(CGame::GetInstance()->GetScreenHeight()) / m_tBackground.m_nImageHeight);

	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;
	int fAspect = /*(float)*/(nHalfScreenWidth << 1 / nHalfScreenHeight << 1);
	float fScaleAmountButton = (float)(((float)CGame::GetInstance()->GetScreenHeight() / (float)CGame::GetInstance()->GetScreenWidth()) * 0.7f);
	float fScaleAmountText = (float)(((float)CGame::GetInstance()->GetScreenHeight() / (float)CGame::GetInstance()->GetScreenWidth()) * 0.5f);
	int ButtonXOffset = int(CGame::GetInstance()->GetScreenWidth() * 0.065f);
	int ButtonClearOffset = int(CGame::GetInstance()->GetScreenWidth() * 0.055f);
	int nTextX = (ButtonXOffset << 1);
	int nTextY = (ButtonXOffset << 1);
	int nButtonMidY = (CGame::GetInstance()->GetScreenHeight() - 20) - m_tButtonHover.m_nImageHeight;

	CView::DrawButton(nHalfScreenWidth, nTextY + m_tButtonNormal.m_nImageHeight, 0.7f, 0.7f, "Challenges", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 0.0f);

	// Draw the buttons
	CView::DrawButton(nHalfScreenWidth, nButtonMidY - (m_tButtonNormal.m_nImageHeight >> 1), fScaleAmountButton, fScaleAmountButton, "Back", ACV_BACK == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), -1.5f, 0);
	CView::DrawButton(nHalfScreenWidth, nButtonMidY - (m_tButtonNormal.m_nImageHeight >> 1), fScaleAmountButton, fScaleAmountButton, "Prev", ACV_PREV == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), -0.5f, 0);
	CView::DrawButton(nHalfScreenWidth, nButtonMidY - (m_tButtonNormal.m_nImageHeight >> 1), fScaleAmountButton, fScaleAmountButton, "Next", ACV_NEXT == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0.5f, 0);
	CView::DrawButton(nHalfScreenWidth, nButtonMidY - (m_tButtonNormal.m_nImageHeight >> 1), fScaleAmountButton, fScaleAmountButton, "Delete", ACV_DELETE == m_nSelected, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 1.5f, 0);


	RECT tTextRect;
	tTextRect.left = nTextX;
	tTextRect.top = (nTextY + m_tButtonNormal.m_nImageHeight);
	tTextRect.right = nTextX + (ButtonXOffset << 1);
	tTextRect.bottom = nTextY + ButtonXOffset;

	if (CAchievementManager::GetInstance()->GetAchievementSize() > 0)
	{
		// Now to write out the achievements, 5 per page!
		int nMaxRange = m_nCurrentPage * 3 - 1;
		//if (CAchievementManager::GetInstance()->GetAchievementSize()-1 >= (unsigned int)nMaxRange-4)
		//{
		//	CView::Draw(m_tButtonNormal.m_nImageID, tTextRect.left - (m_tButtonNormal.m_nImageWidth >> 3), tTextRect.top - (m_tButtonNormal.m_nImageHeight >> 3), 2.7f, 0.7f);
		//	CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-4]->m_szAchievementName, &tTextRect, false, fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//	//DrawButton((tTextRect.left + (tTextRect.right - tTextRect.left)), (tTextRect.top + (tTextRect.bottom-tTextRect.top)), fScaleAmountButton * 3.0f, "", false, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//	if (CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-4]->m_bIsAchieved)
		//	{
		//		// Decrease the X and write Clear!
		//		tTextRect.left -= (ButtonClearOffset << 1);
		//		tTextRect.right -= (ButtonClearOffset << 1);
		//
		//		CView::Write(m_nFontID, "Clear!", &tTextRect, false, fScaleAmountText);
		//
		//		// Set the rect back for regular writing :)
		//		tTextRect.left += (ButtonClearOffset << 1);
		//		tTextRect.right += (ButtonClearOffset << 1);
		//	}
		//
		//	tTextRect.top += ButtonXOffset >> 2;
		//	tTextRect.bottom += ButtonXOffset >> 2;
		//	CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-4]->m_szDescription, &tTextRect, false, fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//}
		//
		//if (CAchievementManager::GetInstance()->GetAchievementSize()-1 >= (unsigned int)nMaxRange-3)
		//{
		//	// Second achievement of the page.
		//	tTextRect.top += (ButtonXOffset);
		//	tTextRect.bottom += (ButtonXOffset);
		//	CView::Draw(m_tButtonNormal.m_nImageID, tTextRect.left - (m_tButtonNormal.m_nImageWidth >> 3), tTextRect.top - (m_tButtonNormal.m_nImageHeight >> 3), 2.7f, 0.7f);
		//	CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-3]->m_szAchievementName, &tTextRect, false, fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//
		//	if (CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-3]->m_bIsAchieved)
		//	{
		//		// Decrease the X and write Clear!
		//		tTextRect.left -= (ButtonClearOffset << 1);
		//		tTextRect.right -= (ButtonClearOffset << 1);
		//
		//		CView::Write(m_nFontID, "Clear!", &tTextRect, false, fScaleAmountText);
		//
		//		// Set the rect back for regular writing :)
		//		tTextRect.left += (ButtonClearOffset << 1);
		//		tTextRect.right += (ButtonClearOffset << 1);
		//	}
		//	
		//	tTextRect.top += ButtonXOffset >> 2;
		//	tTextRect.bottom += ButtonXOffset >> 2;
		//	CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-3]->m_szDescription, &tTextRect, false, fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//}

		if (CAchievementManager::GetInstance()->GetAchievementSize()-1 >= (unsigned int)nMaxRange-2)
		{
			// Third achievement of the page.
			tTextRect.top += (ButtonXOffset);
			tTextRect.bottom += (ButtonXOffset);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-2]->m_szAchievementName, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, -1.2f);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-2]->m_szDescription, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, -0.8f);
			//CView::Draw(m_tButtonNormal.m_nImageID, tTextRect.left - (m_tButtonNormal.m_nImageWidth >> 3), tTextRect.top - (m_tButtonNormal.m_nImageHeight >> 3), (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*2.7f, (CGame::GetInstance()->GetScreenHeight()/float(g_nDevelopmentResolutionHeight))*0.7f);
			tTextRect.left += m_tButtonNormal.m_nImageWidth / 8;
			tTextRect.right += m_tButtonNormal.m_nImageWidth / 8;
			//CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-2]->m_szAchievementName, &tTextRect, false, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

			if (CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-2]->m_bIsAchieved)
			{
				tTextRect.left -= m_tButtonNormal.m_nImageWidth / 8;
				tTextRect.right -= m_tButtonNormal.m_nImageWidth / 8;

				// Decrease the X and write Clear!
				tTextRect.left -= (ButtonClearOffset << 1);
				tTextRect.right -= (ButtonClearOffset << 1);

				//CView::Write(m_nFontID, "Clear!", &tTextRect, false, fScaleAmountText);
				CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Clear!", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -2.3f, -1.0f);

				// Set the rect back for regular writing :)
				tTextRect.left += (ButtonClearOffset << 1);
				tTextRect.right += (ButtonClearOffset << 1);

				tTextRect.left += m_tButtonNormal.m_nImageWidth / 8;
				tTextRect.right += m_tButtonNormal.m_nImageWidth / 8;
			}
			
			tTextRect.top += ButtonXOffset >> 2;
			tTextRect.bottom += ButtonXOffset >> 2;
			//CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-2]->m_szDescription, &tTextRect, false, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

			tTextRect.left -= m_tButtonNormal.m_nImageWidth / 8;
			tTextRect.right -= m_tButtonNormal.m_nImageWidth / 8;
		}

		if (CAchievementManager::GetInstance()->GetAchievementSize()-1 >= (unsigned int)nMaxRange-1)
		{
			// Fourth achievement of the page.
			tTextRect.top += (ButtonXOffset);
			tTextRect.bottom += (ButtonXOffset);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-1]->m_szAchievementName, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, 0.8f);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-1]->m_szDescription, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, 1.2f);
			//CView::Draw(m_tButtonNormal.m_nImageID, tTextRect.left - (m_tButtonNormal.m_nImageWidth >> 3), tTextRect.top - (m_tButtonNormal.m_nImageHeight >> 3), (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*2.7f, (CGame::GetInstance()->GetScreenHeight()/float(g_nDevelopmentResolutionHeight))*0.7f);
			tTextRect.left += m_tButtonNormal.m_nImageWidth / 8;
			tTextRect.right += m_tButtonNormal.m_nImageWidth / 8;
			//CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-1]->m_szAchievementName, &tTextRect, false, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

			if (CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-1]->m_bIsAchieved)
			{
				tTextRect.left -= m_tButtonNormal.m_nImageWidth / 8;
				tTextRect.right -= m_tButtonNormal.m_nImageWidth / 8;

				// Decrease the X and write Clear!
				tTextRect.left -= (ButtonClearOffset << 1);
				tTextRect.right -= (ButtonClearOffset << 1);

				//CView::Write(m_nFontID, "Clear!", &tTextRect, false, fScaleAmountText);
				CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Clear!", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -2.3f, 1.0f);

				// Set the rect back for regular writing :)
				tTextRect.left += (ButtonClearOffset << 1);
				tTextRect.right += (ButtonClearOffset << 1);

				tTextRect.left += m_tButtonNormal.m_nImageWidth / 8;
				tTextRect.right += m_tButtonNormal.m_nImageWidth / 8;
			}
			
			tTextRect.top += ButtonXOffset >> 2;
			tTextRect.bottom += ButtonXOffset >> 2;
			//CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange-1]->m_szDescription, &tTextRect, false, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

			tTextRect.left -= m_tButtonNormal.m_nImageWidth / 8;
			tTextRect.right -= m_tButtonNormal.m_nImageWidth / 8;
		}

		if (CAchievementManager::GetInstance()->GetAchievementSize()-1 >= (unsigned int)nMaxRange)
		{
			// Fifth achievement of the page.
			tTextRect.top += (ButtonXOffset);
			tTextRect.bottom += (ButtonXOffset);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange]->m_szAchievementName, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, 2.8f);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange]->m_szDescription, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, 3.2f);
			//CView::Draw(m_tButtonNormal.m_nImageID, tTextRect.left - (m_tButtonNormal.m_nImageWidth >> 3), tTextRect.top - (m_tButtonNormal.m_nImageHeight >> 3), (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*2.7f, (CGame::GetInstance()->GetScreenHeight()/float(g_nDevelopmentResolutionHeight))*0.7f);
			tTextRect.left += m_tButtonNormal.m_nImageWidth / 8;
			tTextRect.right += m_tButtonNormal.m_nImageWidth / 8;
			//CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange]->m_szAchievementName, &tTextRect, false, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

			if (CAchievementManager::GetInstance()->GetAchievements()[nMaxRange]->m_bIsAchieved)
			{
				tTextRect.left -= m_tButtonNormal.m_nImageWidth / 8;
				tTextRect.right -= m_tButtonNormal.m_nImageWidth / 8;

				// Decrease the X and write Clear!
				tTextRect.left -= (ButtonClearOffset << 1);
				tTextRect.right -= (ButtonClearOffset << 1);

				//CView::Write(m_nFontID, "Clear!", &tTextRect, false, fScaleAmountText);
				CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Clear!", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -2.3f, 3.0f);

				// Set the rect back for regular writing :)
				tTextRect.left += (ButtonClearOffset << 1);
				tTextRect.right += (ButtonClearOffset << 1);

				tTextRect.left += m_tButtonNormal.m_nImageWidth / 8;
				tTextRect.right += m_tButtonNormal.m_nImageWidth / 8;
			}
			
			tTextRect.top += ButtonXOffset >> 2;
			tTextRect.bottom += ButtonXOffset >> 2;
			//CView::Write(m_nFontID, CAchievementManager::GetInstance()->GetAchievements()[nMaxRange]->m_szDescription, &tTextRect, false, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*fScaleAmountText, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

			tTextRect.left -= m_tButtonNormal.m_nImageWidth / 8;
			tTextRect.right -= m_tButtonNormal.m_nImageWidth / 8;
		}
	}
}

void CAchievementState::Exit()
{

}
