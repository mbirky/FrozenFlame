/********************************************************************
* Filename:			CFadeState.h
* Date:				2/5/2013
* Mod. Date:		2/5/2013
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Console state
********************************************************************/

#include "CFadeState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "../Globals.h"
#include "CWinState.h"
#include "CWinState.h"
#include <process.h>
#include "CGameState.h"

void CFadeState::Enter(void)
{
	m_bSectionFade = false;
	m_pWorldManager = CWorldManager::GetInstance();
	m_tImage.m_nImageID = CView::LoadSprite("portal.png", &m_tImage.m_nImageWidth, &m_tImage.m_nImageHeight);
	fScale = (CGame::GetInstance()->GetScreenWidth() * 1.4f) / m_tImage.m_nImageWidth;
	m_fRot = 0.0;
	m_bIsWizardLevel = CWorldManager::GetInstance()->GetBoss();
}

bool CFadeState::Input(void)
{
	return true;
}

void CFadeState::Update(float _dElapsedTime)
{
	// Need to update the view so that the fade works
	CView::Update(_dElapsedTime);

	// Update the portals rotation.
	m_fRot -= 1.0f * _dElapsedTime;

	// If the fade is complete need to load the level
	if(CView::GetFadeState() == FADE_COMPLETE && m_pWorldManager->GetSectionComplete())
	{
		m_pWorldManager->SetSectionComplete(false);
		CView::GetCamera().ClearOffset();
		m_pWorldManager->SetCurrentSection(m_pWorldManager->GetCurrentSection() + 1);

		// start zooming the camera back out
		CView::GetCamera().ZoomCamera(-(CGameState::GetInstance()->GetPortalZoomDistance()), CGameState::GetInstance()->GetPortalZoomDuration(), false);
		CGameState::GetInstance()->SetPortalZoomOn(false);

		if(!m_pWorldManager->LoadCurrentSection())
		{
			// win the game if there is nothing else to load
			CAchievementManager::GetInstance()->ActivateAchievement("Champion");
			CGame::GetInstance()->PushState(CWinState::GetInstance());
			return;
		}
		else
		{
			// If the next section is loaded reset some varialbes
			if (!CAchievementManager::GetInstance()->GetHasPlayerRaged())
				CAchievementManager::GetInstance()->ActivateAchievement("Cool-headed");
			if (!CAchievementManager::GetInstance()->GetIsSpawnerDestroyed())
			{
				CAchievementManager::GetInstance()->ActivateAchievement("Speedrunner");
			}
			CAchievementManager::GetInstance()->SetHasPlayerRaged(false);
			CAchievementManager::GetInstance()->SetIsSpawnerDestroyed(false);
			CGame::GetInstance()->StartTimer();
		}

		// Set it so that we are fading in
		CView::SetFadeState(FADE_IN);
	}
	// No fade and section is complete
	else if(FADE_NOT == CView::GetFadeState() && m_pWorldManager->GetSectionComplete())
	{
		CView::SetFadeState(FADE_OUT);
	}
	// No fade and section not complete
	else if(FADE_NOT == CView::GetFadeState() && !m_pWorldManager->GetSectionComplete())
	{
		CGameState::GetInstance()->SetPortalZoomOn(false);
		CView::GetCamera().ClearOffset();
		CGame::GetInstance()->PopState();
	}
}

void CFadeState::Render(void)
{
	GetPrevState()->Render();

	// as long as not currently on the boss level play the portal
	if(!m_bIsWizardLevel)
	{
		CView::Draw(m_tImage.m_nImageID, int(CGame::GetInstance()->GetScreenWidth() - m_tImage.m_nImageWidth * fScale) >> 1, int(CGame::GetInstance()->GetScreenHeight() - m_tImage.m_nImageHeight * fScale) >> 1,
			fScale, fScale, nullptr, static_cast<float>(m_tImage.m_nImageWidth >> 1), static_cast<float>(m_tImage.m_nImageHeight >> 1), m_fRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, CView::GetFade()));
	}
}

void CFadeState::Exit(void)
{

}