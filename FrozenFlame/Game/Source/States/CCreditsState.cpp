/********************************************************************
* Filename:			CVideoOptionsState.cpp
* Date:				11/23/2012
* Mod. Date:		11/23/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the controls menu state
********************************************************************/

#include "CCreditsState.h"
#include "CMainMenuState.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include "../Util/CFontManager.h"
#include "../Globals.h"

#define SPLASHPATH "Splash Screens/"

CCreditsState::CCreditsState(void) : m_fResetPoint(-10700), m_fRiseRate(300)
{
	m_nFontID = -1;
	m_fPosition = 0.0f;

}

void CCreditsState::Enter(void)
{
	float nHalfScreenHeight = (float)(CGame::GetInstance()->GetScreenHeight()) *0.5f;
	string toLoad = SPLASHPATH;
	m_tBackground.m_nImageID = CView::LoadSprite("Fade.png", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);
	m_tTeamImage.m_nImageID = CView::LoadSprite((toLoad + "8 bit bacon logo.png").c_str(), &m_tTeamImage.m_nImageWidth, &m_tTeamImage.m_nImageHeight);
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tLogoImage.m_nImageID = CView::LoadSprite((toLoad + "GPGlogo.png").c_str(), &m_tLogoImage.m_nImageWidth, &m_tLogoImage.m_nImageHeight);
	m_nFontID = CView::LoadFont("font example.txt");
	m_fPosition = (float)nHalfScreenHeight * 2.0f;
	m_fResetTimer = 0.0f;
	m_bNeedReset = false;
	//m_fPosTrack = 0.0f;
	m_fFadeOut = 1.0f;

	// Just a temporary measure until we get Resolution implemented
#if _DEBUG
	m_fFontScaleX = (float)CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth)*0.7f;
	m_fFontScaleY = (float)CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight)*0.7f;

#else

	m_fFontScaleX = (float)CGame::GetInstance()->GetScreenWidth() / float(g_nDevelopmentResolutionWidth)*0.7f;
	m_fFontScaleY = (float)CGame::GetInstance()->GetScreenHeight() / float(g_nDevelopmentResolutionHeight)*0.7f;

#endif

	// Matt
	m_tTeamPics[0].m_nImageID = CView::LoadSprite("TeamImages/Birky.png", &m_tTeamPics[0].m_nImageWidth, &m_tTeamPics[0].m_nImageHeight);
	// Dan										   
	m_tTeamPics[1].m_nImageID = CView::LoadSprite("TeamImages/Dan.png", &m_tTeamPics[1].m_nImageWidth, &m_tTeamPics[1].m_nImageHeight);
	// Justin									  
	m_tTeamPics[2].m_nImageID = CView::LoadSprite("TeamImages/Justin.png", &m_tTeamPics[2].m_nImageWidth, &m_tTeamPics[2].m_nImageHeight);
	// CJ										  
	m_tTeamPics[3].m_nImageID = CView::LoadSprite("TeamImages/CJ.png", &m_tTeamPics[3].m_nImageWidth, &m_tTeamPics[3].m_nImageHeight);
	// Brett									
	m_tTeamPics[4].m_nImageID = CView::LoadSprite("TeamImages/Brett.png", &m_tTeamPics[4].m_nImageWidth, &m_tTeamPics[4].m_nImageHeight);
	// Sean										  
	m_tTeamPics[5].m_nImageID = CView::LoadSprite("TeamImages/Sean.png", &m_tTeamPics[5].m_nImageWidth, &m_tTeamPics[5].m_nImageHeight);
	// Steven									   
	m_tTeamPics[6].m_nImageID = CView::LoadSprite("TeamImages/Steven.png", &m_tTeamPics[6].m_nImageWidth, &m_tTeamPics[6].m_nImageHeight);
	// JC										  
	m_tTeamPics[7].m_nImageID = CView::LoadSprite("TeamImages/JC.png", &m_tTeamPics[7].m_nImageWidth, &m_tTeamPics[7].m_nImageHeight);

	
	//m_tLogoImage.m_nImageWidth *= ((float)CGame::GetInstance()->GetScreenWidth() / m_tLogoImage.m_nImageWidth)/2;
	//m_tLogoImage.m_nImageHeight *= ((float)CGame::GetInstance()->GetScreenHeight() / m_tLogoImage.m_nImageHeight)/2;

	//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MainMenu");
}

bool CCreditsState::Input(void)
{


	if(m_pInput->Back())
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}

	
	return true;
}

void CCreditsState::Update(float _fElapsedTime)
{
	CView::Update(_fElapsedTime);
	m_fPosition -= m_fRiseRate * _fElapsedTime * m_fFontScaleX;
	//m_fPosTrack += m_fRiseRate * _fElapsedTime;

	if(m_bNeedReset)
	{
		m_fResetTimer += _fElapsedTime;

		if(m_fResetTimer > 2.0f)
		{
			/*m_fResetTimer = 0.0f;
			m_fPosition = float((float)CGame::GetInstance()->GetScreenHeight() * 1.0f);
			m_fPosTrack = 0.0f;
			m_bNeedReset = false;*/
			m_fFadeOut -= 0.7f * _fElapsedTime;

			if(m_fFadeOut <= 0.0f)
			{
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			}
		}
	}
	//m_fIncrementTimer += _fElapsedTime;

}

void CCreditsState::Render(void)
{
	// Backgournd
	CView::Draw(m_tBackground.m_nImageID, 0, 0, float((float)CGame::GetInstance()->GetScreenWidth()) / m_tBackground.m_nImageWidth,
		float((float)CGame::GetInstance()->GetScreenHeight()) / m_tBackground.m_nImageHeight, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(m_fFadeOut, m_fFadeOut, m_fFadeOut, 1.0f));

	int nHalfScreenWidth = (int)(CGame::GetInstance()->GetScreenWidth() * 0.5f);
	int nHalfScreenHeight = (int)(CGame::GetInstance()->GetScreenHeight() * 0.5f);
	int nButtonNormalPosX = (int)(nHalfScreenWidth) - (m_tButtonNormal.m_nImageWidth >> 1);
	int ButtonYOffset = 10;
	float fImageYScale = float((float)CGame::GetInstance()->GetScreenHeight()) / m_tTeamImage.m_nImageHeight;
	float fAspect = float((float)CGame::GetInstance()->GetScreenWidth() * 1.0f/(float)CGame::GetInstance()->GetScreenHeight() * 1.0f);
	float fLogoWidth = float((float)CGame::GetInstance()->GetScreenWidth() * 1.0f/ m_tLogoImage.m_nImageWidth * 1.0f);
	float fLogoHeight = float((float)CGame::GetInstance()->GetScreenHeight() * 1.0f/ m_tLogoImage.m_nImageHeight * 1.0f);
	float fTeamWidth = float((float)CGame::GetInstance()->GetScreenWidth() * 1.0f / m_tTeamImage.m_nImageWidth * 1.0f);
	float fTeamHeight = float((float)CGame::GetInstance()->GetScreenHeight() * 1.0f/ m_tTeamImage.m_nImageHeight * 1.0f) ;

	RECT tTextRect;
	string szText;

	int m_nPos;
	szText = "Frozen Flame";

	tTextRect.left		=	0;
	tTextRect.top		=	(LONG)m_fPosition;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	m_nPos = int(tTextRect.bottom + nHalfScreenHeight/1.5f);

	szText = "An";

	tTextRect.left		=	0;
	tTextRect.top		=	m_nPos + nHalfScreenHeight / 2;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	m_nPos = tTextRect.bottom + nHalfScreenHeight/7;

	CView::Draw(m_tTeamImage.m_nImageID, int(nHalfScreenWidth - (m_tTeamImage.m_nImageWidth * fTeamWidth)/(4)) , /*(int)m_fPosition*/m_nPos + nHalfScreenHeight/2 - (int)(m_tTeamImage.m_nImageHeight * fTeamHeight / (4)), fTeamWidth / (2.0f),
		fTeamHeight / (4.0f));
	m_nPos += (nHalfScreenHeight/2 - (int)(m_tTeamImage.m_nImageHeight * fTeamHeight / (4))) + nHalfScreenHeight/4;;

	szText = "Game";

	tTextRect.left		=	0;
	tTextRect.top		=	m_nPos + nHalfScreenHeight / 2;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	m_nPos = tTextRect.bottom;

	szText = "A";

	tTextRect.left		=	0;LONG(nHalfScreenWidth - (m_tTeamImage.m_nImageWidth * fTeamWidth/2)/2);
	tTextRect.top		=	m_nPos + nHalfScreenHeight / 2;//(LONG)m_fPosition + nHalfScreenHeight + (m_tButtonNormal.m_nImageHeight + ButtonYOffset) * 2 + int(m_tTeamImage.m_nImageHeight * fImageYScale/2) + ButtonYOffset * 2;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();//tTextRect.left + nButtonNormalPosX;
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset; //m_tButtonNormal.m_nImageHeight;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	m_nPos = tTextRect.bottom;

	CView::Draw(m_tLogoImage.m_nImageID, int(nHalfScreenWidth - (m_tLogoImage.m_nImageWidth * fLogoWidth)/4 )  /*+ ButtonYOffset * 3*/ /** ((float)CGame::GetInstance()->GetScreenWidth()) / m_tLogoImage.m_nImageWidth*/, m_nPos + int(nHalfScreenHeight/2 - m_tLogoImage.m_nImageHeight * fLogoHeight / 4.0f) /*+ (m_tButtonNormal.m_nImageHeight + ButtonYOffset) * 2*/, fLogoWidth/2,
		fLogoHeight/2);
	m_nPos += int(nHalfScreenHeight/2 - m_tLogoImage.m_nImageHeight * fLogoHeight / 4.0f) + nHalfScreenHeight;


	szText = "Production";

	tTextRect.left		=	0;
	tTextRect.top		=	m_nPos;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	m_nPos = tTextRect.bottom + nHalfScreenHeight;

	szText = "8-bit Bacon Team Members";

	tTextRect.left		=	0;
	tTextRect.top		=	m_nPos;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	tTextRect.bottom += nHalfScreenHeight;

	szText = "Developers";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
			

	tTextRect.bottom += 100 + nHalfScreenHeight/2;
	szText = "Matthew Birky";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw Matt's Pic	
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[0].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[0].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom =tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[0].m_nImageHeight *m_fFontScaleY *0.6f)) ;


	szText = "Daniel Lima";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw Dan's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[1].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[1].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom = tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[1].m_nImageHeight *m_fFontScaleY *0.6f)) ;
		
	szText = "Justin Maza";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw Justin's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[2].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[2].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom =tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[2].m_nImageHeight *m_fFontScaleY *0.6f)) ;

	
	szText = "Charles Meade";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw CJ's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[3].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[3].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom = tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[3].m_nImageHeight *m_fFontScaleY *0.6f)) ;

	
	szText = "Brett Green";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw Brett's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[4].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[4].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom = tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[4].m_nImageHeight *m_fFontScaleY *0.6f)) ;

	
	szText = "Sean Welch";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw Sean's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[5].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[5].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom = tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[5].m_nImageHeight *m_fFontScaleY *0.6f)) ;


	szText = "Steven Seufert";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw Steven's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[6].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[6].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom = tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[6].m_nImageHeight *m_fFontScaleY *0.6f)) ;
	szText = "Joe Ricks";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	// Draw JC's Pic
	tTextRect.bottom += 25;

	CView::Draw(m_tTeamPics[7].m_nImageID, 
		(int)(nHalfScreenWidth - (m_tTeamPics[7].m_nImageWidth * m_fFontScaleX) / 4.0f) , 
		tTextRect.bottom +  ButtonYOffset, 
		 m_fFontScaleX *0.5f,
		 m_fFontScaleY*0.5f);

	tTextRect.bottom = tTextRect.bottom + ButtonYOffset + static_cast<LONG>((m_tTeamPics[7].m_nImageHeight *m_fFontScaleY *0.6f)) ;


	//tTextRect.bottom += nHalfScreenHeight/2;

	szText = "Concept Artists";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom + nHalfScreenHeight/2;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/4;
	szText = "Santiago Hoyos";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/4;
	szText = "Oscar Valera";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight;

	szText = "GP Staff";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight;

	szText = "External Producer";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Don Smith";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/3;

	szText = "Art Director";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Derek Marunowski";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	tTextRect.bottom += nHalfScreenHeight/3;

	szText = "Studio Head";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Liam Hislop";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/3;

	szText = "Course Directors - Month 1";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Rod Moye";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/4;

	szText = "Joel Carroll";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/3;

	szText = "Course Director - Month 2";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Jason Hinders";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/3;

	szText = "Course Director - Months 3 - 5";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Mike Lebo";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/3;

	szText = "Gameplay & Production Support";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/6;

	szText = "Patrick Kelly";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

	tTextRect.bottom += nHalfScreenHeight/3;

	// if the player comes here from the main menu, this won't show up
	// if the player comes here from beating the game, show how many 
	// minions were killed
	if (CGame::GetInstance()->GetMinionKillCount())
	{
		char buffer[128];
		sprintf_s(buffer, "%d Minions were harmed ", CGame::GetInstance()->GetMinionKillCount());
		szText = buffer;

		tTextRect.left		=	0;
		tTextRect.top		=	tTextRect.bottom;
		tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
		tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
		CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

		tTextRect.bottom += nHalfScreenHeight/6;

		szText = " in the playing of this game ";
		tTextRect.left		=	0;
		tTextRect.top		=	tTextRect.bottom;
		tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
		tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
		CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));

		tTextRect.bottom += nHalfScreenHeight/6;

		sprintf_s(buffer, "");	// clear buffer
		sprintf_s(buffer, "You took %d:%d to defeat the wizard ", CGame::GetInstance()->GetAccumulatedPlayMinutes(), CGame::GetInstance()->GetAccumulatedPlaySeconds());
		szText = buffer;

		tTextRect.left		=	0;
		tTextRect.top		=	tTextRect.bottom;
		tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
		tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
		CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
		
		sprintf_s(buffer, "");
		switch(CGame::GetInstance()->GetDifficultyLevel())
		{
		case DIFF_EASY: sprintf_s(buffer, " on Easy difficulty ");
			break;
		case DIFF_MEDIUM: sprintf_s(buffer, " on Medium difficulty ");
			break;
		case DIFF_HARD: sprintf_s(buffer, " on Hard difficulty ");
			break;
		}
		szText = buffer;

		tTextRect.bottom += nHalfScreenHeight/6;
		tTextRect.left		=	0;
		tTextRect.top		=	tTextRect.bottom;
		tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
		tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
		CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));
		
		tTextRect.bottom += nHalfScreenHeight/6;

		//tTextRect.bottom += nHalfScreenHeight/3;
	}
	tTextRect.bottom += nHalfScreenHeight;

	szText = "Special Thanks Roberto Rubiano";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));


	tTextRect.bottom += nHalfScreenHeight;
	szText = "Thanks for playing!";

	tTextRect.left		=	0;
	tTextRect.top		=	tTextRect.bottom;
	tTextRect.right		=	CGame::GetInstance()->GetScreenWidth();
	tTextRect.bottom	=	tTextRect.top + ButtonYOffset;
	CView::Write(m_nFontID, szText, &tTextRect, true, m_fFontScaleX,m_fFontScaleY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - abs(float(((tTextRect.top * 2.0f) / CGame::GetInstance()->GetScreenHeight()) - 1))));


	if(tTextRect.bottom < 0)
	{
		m_bNeedReset = true;
	}

	//CView::Draw(m_tButtonNormal.m_nImageID, nHalfScreenWidth - int(m_tButtonNormal.m_nImageWidth * 2.6f)/2, nHalfScreenHeight * 2 - (m_tButtonNormal.m_nImageHeight + ButtonYOffset), 2.6f);
	//tTextRect.left		=	0;//LONG(nHalfScreenWidth - (m_tTeamImage.m_nImageWidth * fTeamWidth/2)/2);//nButtonNormalPosX - m_tButtonNormal.m_nImageWidth - ButtonYOffset * 9/* - (m_tTeamImage.m_nImageWidth)/2*/;
	//tTextRect.top		=	nHalfScreenHeight * 2 - (m_tButtonNormal.m_nImageHeight)  + ButtonYOffset * 2;
	//tTextRect.right		=	(float)CGame::GetInstance()->GetScreenWidth();//tTextRect.left + nButtonNormalPosX;
	//tTextRect.bottom	=	tTextRect.top + ButtonYOffset; //m_tButtonNormal.m_nImageHeight;
	//CView::Write(m_nFontID, "Press Escape to skip", &tTextRect, true, m_fFontScaleX,m_fFontScaleY);

	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 1.0f, 1.0f, "Press Escape to Skip", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, 3.5f);
}


void CCreditsState::Exit(void)
{
	CGame::GetInstance()->ResetMinionKillCount();
	CGame::GetInstance()->ResetAccumulatedPlayTime();	
	//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"MainMenu");
}


CCreditsState::~CCreditsState()
{

}
