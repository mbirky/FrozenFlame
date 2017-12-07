/********************************************************************
* Filename:			CHelpScreenState.cpp
* Date:				01/18/2013
* Mod. Date:		01/18/2013
* Mod. Initials:	SMS
* Author:			Steven Seufert
* Purpose:			Defines all behavior for the help screen
********************************************************************/

#include "CHelpScreenState.h"
#include "CControlsState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "../Core/CWorldManager.h"
#include "../Objects/PlayerObject.h"

#include "CGameState.h"

CHelpScreenState::CHelpScreenState(void) : m_nSelected(HSO_RESUME_GAME)
{
	m_nFontID = -1;
	m_nCurrentPage = -1;
	m_bPageHasBeenTurned = false;
	m_fPageBufferTimer = -1.0f;
	m_fPageBufferDuration = 0.2f;
}

void CHelpScreenState::Enter(void)
{
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &m_tButtonHover.m_nImageWidth, &m_tButtonHover.m_nImageHeight);
	m_tPageLeft.m_nImageID = CView::LoadSprite("left arrow.png", &m_tPageLeft.m_nImageWidth, &m_tPageLeft.m_nImageHeight);
	m_tPageRight.m_nImageID = CView::LoadSprite("right arrow.png", &m_tPageRight.m_nImageWidth, &m_tPageRight.m_nImageHeight);

	m_nFontID = CView::LoadFont("font example.txt");

	m_nSelected = HSO_RESUME_GAME;
	m_nCurrentPage = HELP_PAGE_ONE;

	m_bPageHasBeenTurned = false;
	m_fPageBufferTimer = 0.0f;
	m_fPageBufferDuration = 0.2f;

	//CSoundManager::GetInstance()->PauseSound(CGame::GetInstance()->GetSoundBank(),"Game_play_music_03_Loop");

	// pause the game's timer
	CGame::GetInstance()->StopTimer();
	// start up help screen's personal timer since the game's is now paused
	m_helpScreenTimer.Start();
}

bool CHelpScreenState::Input(void)
{
	if(m_pInput->Back() || m_pInput->Help())
	{
		// Will need to be pop state
		CGame::GetInstance()->PopState();
	}

	if(m_pInput->Select())
	{
		switch(m_nSelected)
		{
		case HSO_RESUME_GAME:
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				CGame::GetInstance()->PopState();				
			}
			break;

		case HSO_CONTROLS:
			{
				CGame::GetInstance()->PushState(CControlsState::GetInstance());
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			}
			break;

		default:
			{
				CGame::GetInstance()->PopState();
			}
			break;
		}
	}

	if(m_pInput->Menu_Up())
	{
		--m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == HSO_MIN)
		{
			m_nSelected = HSO_MAX - 1;
		}
	}
	if(m_pInput->Menu_Down())
	{
		++m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == HSO_MAX)
		{
			m_nSelected = HSO_MIN + 1;
		}
	}
	if (false == m_bPageHasBeenTurned)
	{
		// turn page left
		if(m_pInput->Menu_Left())
		{
			--m_nCurrentPage;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			m_bPageHasBeenTurned = true;
			if(m_nCurrentPage < 0)
			{
				m_nCurrentPage = NUM_HELP_PAGES - 1;
			}
		}
		// turn page right
		if (m_pInput->Menu_Right())
		{
			++m_nCurrentPage;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			m_bPageHasBeenTurned = true;
			if(m_nCurrentPage >= NUM_HELP_PAGES)
			{
				m_nCurrentPage = 0;
			}
		}
	}

	return true;
}

void CHelpScreenState::Update(float _fElapsedTime)
{
	_fElapsedTime = m_helpScreenTimer.GetElapsedTime();

	CView::Update(_fElapsedTime);

	if (m_bPageHasBeenTurned)
	{
		m_fPageBufferTimer += _fElapsedTime;
		// once you've fulfilled the bufer time
		if (m_fPageBufferTimer >= m_fPageBufferDuration)
		{
			m_bPageHasBeenTurned = false; // you can turn the page again
			m_fPageBufferTimer = 0;
		}
	}
}

void CHelpScreenState::Render(void)
{
	// render the game in the background
	GetPrevState()->Render();

	RenderStatusBarHelp();

	switch(m_nCurrentPage)
	{
	case HELP_PAGE_ONE:
		{
			RenderPageOne();
		}
		break;

	case HELP_PAGE_TWO:
		{
			RenderPageTwo();
		}
		break;

	case HELP_PAGE_THREE:
		{
			RenderPageThree();
		}
		break;
	}
	
	RenderButtons();	
}

/********************************************************************
* RenderPageOne():		displays everything that needs to be rendered 
*						on the first page
*
* Ins:					none
*
* Outs:					none
*
* Returns:				none
*
* Mod. Date:			01/18/2013
* Mod. Initials:		SMS
********************************************************************/
void CHelpScreenState::RenderPageOne(void)
{
	RECT tTextRect;
	int screenHeight = CGame::GetInstance()->GetScreenHeight();
	int screenWidth =  CGame::GetInstance()->GetScreenWidth();

	tTextRect.left		=	0;
	tTextRect.top		=	(LONG)(screenHeight * 0.17f);
	tTextRect.right		=	screenWidth;
	tTextRect.bottom	=	(LONG)(screenHeight * 0.27f);
	CView::Write(m_nFontID, "OBJECTIVE", &tTextRect, true, 0.7f);

	// if you're on the boss level
	/*if (CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->GetInitialized())
	{
		// defeat the dastardly wizard 
	}
	else*/
	{
		tTextRect.left		=	(LONG)(screenWidth * 0.17f);
		tTextRect.top		=	(LONG)(screenHeight * 0.28f);
		tTextRect.right		=	(LONG)(screenWidth * 0.83f);
		tTextRect.bottom	=	(LONG)(screenHeight * 0.36f);
		CView::Write(m_nFontID, "Perform feats of courage ", &tTextRect, true, 0.5f);
		
		tTextRect.top		=	(LONG)(screenHeight * 0.33f);
		tTextRect.bottom	=	(LONG)(screenHeight * 0.41f);
		CView::Write(m_nFontID, "and strength as you fight", &tTextRect, true, 0.5f); 

		tTextRect.top		=	(LONG)(screenHeight * 0.38f);
		tTextRect.bottom	=	(LONG)(screenHeight * 0.46f);
		CView::Write(m_nFontID, "your way through waves of ", &tTextRect, true, 0.5f);

		tTextRect.top		=	(LONG)(screenHeight * 0.43f);
		tTextRect.bottom	=	(LONG)(screenHeight * 0.51f);
		CView::Write(m_nFontID, "minions and deadly traps.", &tTextRect, true, 0.5f);

		tTextRect.top		=	(LONG)(screenHeight * 0.51f);
		tTextRect.bottom	=	(LONG)(screenHeight * 0.59f);
		CView::Write(m_nFontID, "Your greatest foe lies ", &tTextRect, true, 0.5f);

		tTextRect.top		=	(LONG)(screenHeight * 0.56f);
		tTextRect.bottom	=	(LONG)(screenHeight * 0.64f);
		CView::Write(m_nFontID, "nearby and is up to no good.", &tTextRect, true, 0.5f);
	}
}

/********************************************************************
* RenderPageTwo():		displays everything that needs to be rendered 
*						on the second page
*
* Ins:					none
*
* Outs:					none
*
* Returns:				none
*
* Mod. Date:			01/18/2013
* Mod. Initials:		SMS
********************************************************************/
void CHelpScreenState::RenderPageTwo(void)
{
	RECT tTextRect;
	int screenHeight = CGame::GetInstance()->GetScreenHeight();
	int screenWidth =  CGame::GetInstance()->GetScreenWidth();

	tTextRect.left		=	0;
	tTextRect.top		=	(LONG)(screenHeight * 0.17f);
	tTextRect.right		=	screenWidth;
	tTextRect.bottom	=	(LONG)(screenHeight * 0.27f);
	CView::Write(m_nFontID, "HELPFUL HINTS", &tTextRect, true, 0.7f);

	tTextRect.left		=	(LONG)(screenWidth * 0.1f);
	tTextRect.top		=	(LONG)(screenHeight * 0.28f);
	tTextRect.right		=	(LONG)(screenWidth * 0.83f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.36f);
	CView::Write(m_nFontID, "Unleashing your rage when your rage", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.31f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.36f);
	CView::Write(m_nFontID, "meter is full will send you into your", &tTextRect, false, 0.38f); 

	tTextRect.top		=	(LONG)(screenHeight * 0.34f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.39f);
	CView::Write(m_nFontID, "avatar state. Use this state to deal", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.37f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.42f);
	CView::Write(m_nFontID, "extra damage and regenerate health.", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.42f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.47f);
	CView::Write(m_nFontID, "Use fire attacks to set your foes on", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.45f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.50f);
	CView::Write(m_nFontID, "fire. Use ice attacks to chill and ", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.48f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.53f);
	CView::Write(m_nFontID, "freeze enemies in place.", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.53f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.58f);
	CView::Write(m_nFontID, "Keep an eye out for ranged minions.", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.56f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.61f);
	CView::Write(m_nFontID, "Using the wrong element on their", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.59f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.64f);
	CView::Write(m_nFontID, "magics will get you stunned for ", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.62f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.67f);
	CView::Write(m_nFontID, "a moment.", &tTextRect, false, 0.38f);
	
}

void CHelpScreenState::RenderPageThree()
{
	RECT tTextRect;
	int screenHeight = CGame::GetInstance()->GetScreenHeight();
	int screenWidth =  CGame::GetInstance()->GetScreenWidth();

	tTextRect.left		=	0;
	tTextRect.top		=	(LONG)(screenHeight * 0.17f);
	tTextRect.right		=	screenWidth;
	tTextRect.bottom	=	(LONG)(screenHeight * 0.27f);
	CView::Write(m_nFontID, "HELPFUL HINTS", &tTextRect, true, 0.7f);

	tTextRect.left		=	(LONG)(screenWidth * 0.1f);
	tTextRect.top		=	(LONG)(screenHeight * 0.28f);
	tTextRect.right		=	(LONG)(screenWidth * 0.83f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.36f);
	CView::Write(m_nFontID, "Most standard minions are summoned", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.31f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.36f);
	CView::Write(m_nFontID, "from portals from the netherealm. If", &tTextRect, false, 0.38f); 

	tTextRect.top		=	(LONG)(screenHeight * 0.34f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.39f);
	CView::Write(m_nFontID, "you find yourself getting overwhelmed,", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.37f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.42f);
	CView::Write(m_nFontID, "some portals can be destroyed.", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.42f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.47f);
	CView::Write(m_nFontID, "You can dash to escape harm,", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.45f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.50f);
	CView::Write(m_nFontID, "but you can also ram enemies ", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.48f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.53f);
	CView::Write(m_nFontID, "out of the way.", &tTextRect, false, 0.38f);

	/*tTextRect.top		=	(LONG)(screenHeight * 0.53f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.58f);
	CView::Write(m_nFontID, "Keep an eye out for ranged minions.", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.56f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.61f);
	CView::Write(m_nFontID, "Using the wrong element on their", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.59f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.64f);
	CView::Write(m_nFontID, "magics will get you stunned for ", &tTextRect, false, 0.38f);

	tTextRect.top		=	(LONG)(screenHeight * 0.62f);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.67f);
	CView::Write(m_nFontID, "a moment.", &tTextRect, false, 0.38f);*/
}

// displays an explanation to the health and rage meters
void CHelpScreenState::RenderStatusBarHelp(void)
{
	RECT tTextRect;
	int screenWidth = CGame::GetInstance()->GetScreenWidth();
	int screenHeight = CGame::GetInstance()->GetScreenHeight();
	
	// Health bar arrow
	CView::Draw(m_tPageLeft.m_nImageID, (int)(screenWidth * 0.36f), (int)(screenHeight * 0.015f), 0.26f, 0.26f);
	// rage bar arrow
	CView::Draw(m_tPageLeft.m_nImageID, (int)(screenWidth * 0.36f), (int)(screenHeight * 0.07f), 0.26f, 0.26f);

	// health info
	tTextRect.left		=	(LONG)(screenWidth * 0.40f);
	tTextRect.top		=	(LONG)(screenHeight * 0.025f);
	tTextRect.right		=	(LONG)(screenWidth);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.07f);
	CView::Write(m_nFontID, "HEALTH ", &tTextRect, false, 0.2f);
	
	tTextRect.left		=	(LONG)(screenWidth * 0.47f);
	tTextRect.top		=	(LONG)(screenHeight * 0.02f);
	CView::Write(m_nFontID, "This meter shows how much more damage you", &tTextRect, false, 0.21f);
	
	tTextRect.top		=	(LONG)(screenHeight * 0.035f);	
	CView::Write(m_nFontID, "can sustain.", &tTextRect, false, 0.21f);

	// rage info
	tTextRect.left		=	(LONG)(screenWidth * 0.40f);
	tTextRect.top		=	(LONG)(screenHeight * 0.085f);
	tTextRect.right		=	(LONG)(screenWidth);
	tTextRect.bottom	=	(LONG)(screenHeight * 0.14f);
	CView::Write(m_nFontID, "RAGE ", &tTextRect, false, 0.2f);
	
	tTextRect.left		=	(LONG)(screenWidth * 0.47f);
	tTextRect.top		=	(LONG)(screenHeight * 0.075f);
	CView::Write(m_nFontID, "This meter shows the rage you've built", &tTextRect, false, 0.21f);
	
	tTextRect.top		=	(LONG)(screenHeight * 0.09f);	
	CView::Write(m_nFontID, "up. Once full, you can unleash your rage.", &tTextRect, false, 0.21f);
}

/********************************************************************
* RenderButtons():		displays the buttons on the help screen
*
* Ins:					none
*
* Outs:					none
*
* Returns:				none
*
* Mod. Date:			01/18/2013
* Mod. Initials:		SMS
********************************************************************/
void CHelpScreenState::RenderButtons(void)
{
	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

	int nButtonHoverPosX = nHalfScreenWidth - (m_tButtonHover.m_nImageWidth >> 1);
	int nButtonNormalPosX = nHalfScreenWidth - (m_tButtonNormal.m_nImageWidth >> 1);
	int ButtonYOffset = 10;

	RECT tTextRect;

	if(HSO_RESUME_GAME == m_nSelected)
	{
		CView::Draw(m_tButtonHover.m_nImageID, nButtonHoverPosX, nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2);
		tTextRect.left		=	nButtonHoverPosX;
		tTextRect.top		=	nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "Resume Game", &tTextRect, true, 0.35f);
	}
	else
	{
		CView::Draw(m_tButtonNormal.m_nImageID, nButtonNormalPosX, nHalfScreenHeight + (m_tButtonNormal.m_nImageHeight + ButtonYOffset) * 2);
		tTextRect.left		=	nButtonNormalPosX;
		tTextRect.top		=	nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "Resume Game", &tTextRect, true, 0.35f);
	}

	if(HSO_CONTROLS == m_nSelected)
	{
		CView::Draw(m_tButtonHover.m_nImageID, nButtonNormalPosX, nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 3);
		tTextRect.left		=	nButtonHoverPosX;
		tTextRect.top		=	nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 3;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "Controls", &tTextRect, true, 0.4f);
	}
	else
	{
		CView::Draw(m_tButtonNormal.m_nImageID, nButtonNormalPosX, nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 3);
		tTextRect.left		=	nButtonNormalPosX;
		tTextRect.top		=	nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 3;
		tTextRect.right		=	tTextRect.left + m_tButtonNormal.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonNormal.m_nImageHeight;
		CView::Write(m_nFontID, "Controls", &tTextRect, true, 0.4f);
	}

	
	// Page Left/Left Arrow
	CView::Draw(m_tPageLeft.m_nImageID, m_tPageLeft.m_nImageWidth, (int)(nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2));
	tTextRect.left		= (LONG)(m_tPageLeft.m_nImageWidth * 1.38f);
	tTextRect.top		= (LONG)((nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2) + m_tPageLeft.m_nImageHeight * 0.25f);
	tTextRect.right		= m_tPageLeft.m_nImageWidth * 2;
	tTextRect.bottom	= (LONG)((nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2) + (m_tPageLeft.m_nImageHeight * 0.5f));
	CView::Write(m_nFontID, "Turn", &tTextRect, true, 0.32f);
	tTextRect.top		= (LONG)((nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2) + m_tPageLeft.m_nImageHeight * 0.65f);
	CView::Write(m_nFontID, "Page", &tTextRect, true, 0.32f);

	// Page Right/Right Arrow
	CView::Draw(m_tPageRight.m_nImageID, CGame::GetInstance()->GetScreenWidth() - m_tPageRight.m_nImageWidth * 2, (int)(nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2));
	tTextRect.left		= (LONG)(CGame::GetInstance()->GetScreenWidth() - m_tPageLeft.m_nImageWidth * 2.15f);
	tTextRect.top		= (LONG)((nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2) + m_tPageLeft.m_nImageHeight * 0.25f);
	tTextRect.right		= (LONG)(CGame::GetInstance()->GetScreenWidth() - m_tPageLeft.m_nImageWidth * 1.2f);
	tTextRect.bottom	= (LONG)((nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2) + (m_tPageLeft.m_nImageHeight * 0.5f));
	CView::Write(m_nFontID, "Turn", &tTextRect, true, 0.32f);
	tTextRect.top		= (LONG)((nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 2) + m_tPageLeft.m_nImageHeight * 0.65f);
	CView::Write(m_nFontID, "Page", &tTextRect, true, 0.32f);

}

void CHelpScreenState::Exit(void)
{
	//CSoundManager::GetInstance()->ResumeSound(CGame::GetInstance()->GetSoundBank(),"Gameplay_Music");

	// unpause the game's timer
	CGame::GetInstance()->StartTimer();
	// the help screen's timer doesn't need to be running when we leave
	m_helpScreenTimer.Stop();
}

CHelpScreenState::~CHelpScreenState()
{

}