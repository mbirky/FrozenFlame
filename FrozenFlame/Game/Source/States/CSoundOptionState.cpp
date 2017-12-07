/********************************************************************
* Filename:			CVideoOptionsState.cpp
* Date:				11/23/2012
* Mod. Date:		11/23/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the controls menu state
********************************************************************/

#include "CSoundOptionState.h"
#include "COptionsMenuState.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include "../Util/CFontManager.h"
#include "../Globals.h"

static const char* g_szOptionsLoc = "Resource/Options/";
const short g_usMaxSound = 100;
const short g_usMinSound = 0;

CSoundOptionState::CSoundOptionState(void) : m_nSelected(SND_MASTER), m_bIsMuteChangeable(true)
{
	m_nFontID = -1;
	m_fIncrementTimer = 0.0f;
	m_usMusicVolume = 0;
	m_usSoundVolume = 0;
	m_bPrevLeft = false;
	m_bPrevRight = false;
}

void CSoundOptionState::Enter(void)
{
	m_tBackground.m_nImageID = CView::LoadSprite("fire-and-ice.jpg", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);

	m_nFontID = CView::LoadFont("font example.txt");

	m_nSelected = SND_MASTER;

	m_usMasterVolume = 100;
	m_usMusicVolume = 100;
	m_usSoundVolume = 100;
	isMuteOn = false;

	m_bPrevLeft = false;
	m_bPrevRight = false;

	LoadSettings("Sound");

	if(isMuteOn == false)
	{
		CSoundManager::GetInstance()->AdjustMaster((float)m_usMasterVolume);
	}
	else
	{
		CSoundManager::GetInstance()->AdjustMaster(0);
	}

	CSoundManager::GetInstance()->AdjustMusic((float)m_usMusicVolume);
	CSoundManager::GetInstance()->AdjustSFX((float)m_usSoundVolume);

	m_fFlashTimer = 0.0f;
	m_fPlayTimer = 0.0f;
	m_bDisplayControls = true;
	m_tTimer.Start();

}

bool CSoundOptionState::Input(void)
{
	switch(m_nSelected)
	{
	case SND_MASTER:
		{
			if(m_pInput->Menu_Right())
			{
				if( m_usMasterVolume < g_usMaxSound)
				{
					if(m_fIncrementTimer >= 0.1f)
					{
						m_usMasterVolume += 1;
						m_fIncrementTimer = 0.0f;
						if(isMuteOn == false)
							CSoundManager::GetInstance()->AdjustMaster((float)m_usMasterVolume);
					}
				}

				else if( m_usMasterVolume > g_usMaxSound )
				{
					m_usMasterVolume = g_usMaxSound;
				}
			}

			if(m_pInput->Menu_Left())
			{
				if( m_usMasterVolume < g_usMinSound)
				{
					m_usMasterVolume = g_usMinSound;
				}

				else if( m_usMasterVolume > g_usMinSound )
				{
					if(m_fIncrementTimer >= 0.1f)
					{
						m_usMasterVolume -= 1;
						m_fIncrementTimer = 0.0f;
						if(isMuteOn == false)
							CSoundManager::GetInstance()->AdjustMaster((float)m_usMasterVolume);
					}
				}
			}
		}
		break;
	case SND_MUSIC:
		{
			if(m_pInput->Menu_Right())
			{
				if( m_usMusicVolume < g_usMaxSound)
				{
					if(m_fIncrementTimer >= 0.1f)
					{
						m_usMusicVolume += 1;
						m_fIncrementTimer = 0.0f;
						if(isMuteOn == false)
							CSoundManager::GetInstance()->AdjustMusic((float)m_usMusicVolume);
					}	
				}

				else if( m_usMusicVolume > g_usMaxSound )
				{
					m_usMusicVolume = g_usMaxSound;
				}
			}

			if(m_pInput->Menu_Left())
			{
				if( m_usMusicVolume < g_usMinSound)
				{
					m_usMusicVolume = g_usMinSound;
					
				}

				else if( m_usMusicVolume > g_usMinSound )
				{
					if(m_fIncrementTimer >= 0.1f)
					{
						m_usMusicVolume -= 1;
						m_fIncrementTimer = 0.0f;
						if(isMuteOn == false)
							CSoundManager::GetInstance()->AdjustMusic((float)m_usMusicVolume);
					}
				}
			}
			break;
		}

	case SND_SOUND:
		{

			if(m_pInput->Menu_Right())
			{
				if( m_usSoundVolume < g_usMaxSound)
				{
					if(m_fIncrementTimer >= 0.1f)
					{
						m_usSoundVolume += 1;
						m_fIncrementTimer = 0.0f;
						if(isMuteOn == false)
						{
							CSoundManager::GetInstance()->AdjustSFX((float)m_usSoundVolume);

							if(m_fPlayTimer >= 0.3f)
							{
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDHIT_CRYSTAL,GAME_OBJECT_ID_PLAYER);
								m_fPlayTimer = 0.0f;
							}

						}

						
					}
				}

				else if( m_usSoundVolume > g_usMaxSound )
				{
					m_usSoundVolume = g_usMaxSound;
				}
			}

			if(m_pInput->Menu_Left())
			{
				if( m_usSoundVolume < g_usMinSound)
				{
					m_usSoundVolume = g_usMinSound;
				}

				else if( m_usSoundVolume > g_usMinSound )
				{
					if(m_fIncrementTimer >= 0.1f)
					{
						m_usSoundVolume -= 1;
						m_fIncrementTimer = 0.0f;
						if(isMuteOn == false)
						{
							CSoundManager::GetInstance()->AdjustSFX((float)m_usSoundVolume);

							if(m_fPlayTimer >= 0.3f)
							{
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDHIT_CRYSTAL,GAME_OBJECT_ID_PLAYER);
								m_fPlayTimer = 0.0f;
							}
						}
					}
				}
			}
			break;
		}
	case SND_MUTE:
		{
			
			if(m_pInput->Menu_Right() || m_pInput->Menu_Left())
			{
				if(m_bIsMuteChangeable)
				{
					isMuteOn = !isMuteOn;
					m_bIsMuteChangeable = false;
				}
			}
			else 
			{
				m_bIsMuteChangeable = true;
			}

			
			if(isMuteOn)
			{
				CSoundManager::GetInstance()->AdjustMaster(0);
			}
			else
			{
				CSoundManager::GetInstance()->AdjustMaster(m_usMasterVolume);
			}
		}
		break;
	case SND_DEFAULT:
		{
			if (m_pInput->Menu_Right())
			{
				if (!m_bPrevRight)
				{
					m_bPrevRight = true;
					m_nSelected = SND_EXIT;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				m_bPrevRight = false;
			}

			if (m_pInput->Menu_Left())
			{
				if (!m_bPrevLeft)
				{
					m_bPrevLeft = true;
					m_nSelected = SND_SAVE;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				m_bPrevLeft = false;
			}
		}
		break;

	case SND_SAVE:
		{
			if (m_pInput->Menu_Right())
			{
				if (!m_bPrevRight)
				{
					m_bPrevRight = true;
					m_nSelected = SND_DEFAULT;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				m_bPrevRight = false;
			}

			if (m_pInput->Menu_Left())
			{
				if (!m_bPrevLeft)
				{
					m_bPrevLeft = true;
					m_nSelected = SND_EXIT;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				m_bPrevLeft = false;
			}
		}
		break;

	case SND_EXIT:
		{
			if (m_pInput->Menu_Right())
			{
				if (!m_bPrevRight)
				{
					m_bPrevRight = true;
					m_nSelected = SND_SAVE;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				m_bPrevRight = false;
			}

			if (m_pInput->Menu_Left())
			{
				if (!m_bPrevLeft)
				{
					m_bPrevLeft = true;
					m_nSelected = SND_DEFAULT;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				m_bPrevLeft = false;
			}
		}
		break;
	}

	if(m_pInput->Select())
	{
		
		if(SND_EXIT == m_nSelected)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
			CGame::GetInstance()->PopState();
		}

		if (SND_DEFAULT == m_nSelected)
		{
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			RestoreDefaults();
			if(isMuteOn == false)
				CSoundManager::GetInstance()->AdjustMaster((float)m_usMasterVolume);
		}

		if (SND_SAVE == m_nSelected)
		{
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			SaveSettings("Sound");
		}
	}

	if(m_pInput->Menu_Up())
	{
		--m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == SND_MIN)
		{
			m_nSelected = SND_MAX - 1;
		}
	}
	if(m_pInput->Menu_Down())
	{
		++m_nSelected;
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		if(m_nSelected == SND_MAX)
		{
			m_nSelected = SND_MIN + 1;
		}
	}

	return true;
}

void CSoundOptionState::Update(float _fElapsedTime)
{
	_fElapsedTime = m_tTimer.GetElapsedTime();

	CView::Update(_fElapsedTime);
	m_fIncrementTimer += _fElapsedTime;
	m_fFlashTimer += _fElapsedTime;
	m_fPlayTimer += _fElapsedTime;
}

void CSoundOptionState::Render(void)
{
	// Backgournd
	if(CGame::GetInstance()->GetFromPause() == false)
	{
		CView::Draw(m_tBackground.m_nImageID, 0, 0, float(CGame::GetInstance()->GetScreenWidth()) / m_tBackground.m_nImageWidth,
			float(CGame::GetInstance()->GetScreenHeight()) / m_tBackground.m_nImageHeight);
	}
	else
	{
		GetPrevState()->Render();
	}

	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Sound", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0, -2.25);

	// Buffer then draw the Master volume buttons
	char buffer[32] = {0};
	sprintf_s(buffer, "%d", m_usMasterVolume);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Mstr Vol.", SND_MASTER == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, -1);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, buffer, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, -1);

	// Buffer then draw the music volume buttons
	sprintf_s(buffer, "%d", m_usMusicVolume);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Music Vol.", SND_MUSIC == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 0);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, buffer, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 0);

	// Buffer then draw the sound volume buttons
	sprintf_s(buffer, "%d", m_usSoundVolume);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Sound Vol.", SND_SOUND == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 1);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, buffer, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 1);

	// Buffer then draw if mute is on/off
	isMuteOn ? sprintf_s(buffer,"ON") : sprintf_s(buffer,"OFF");
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Mute", SND_MUTE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 2);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, buffer, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 2);

	// Draw the Back button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Back", SND_EXIT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0, 4);

	// Draw the Default Button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Default", SND_DEFAULT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1.25, 4);

	// Draw the save Button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Save", SND_SAVE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.25, 4);

	//if(m_bDisplayControls)
	//{
	//	tTextRect.left		=	nButtonHoverPosX /*- nHalfScreenWidth/2*/;
	//	tTextRect.top		=	nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 3;
	//	tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
	//	tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
	//	CView::Write(m_nFontID, "Up and Down Arrows to change selection,", &tTextRect, true, 0.4f);
	//
	//	tTextRect.left		=	nButtonHoverPosX /*- nHalfScreenWidth/2*/;
	//	tTextRect.top		=	nHalfScreenHeight + (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 4;
	//	tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
	//	tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
	//	CView::Write(m_nFontID, "Right and Left Arrows to change values", &tTextRect, true, 0.4f);
	//}
}

void CSoundOptionState::Exit(void)
{
	LoadSettings("Sound");

	if(isMuteOn == false)
	{
		CSoundManager::GetInstance()->AdjustMaster((float)m_usMasterVolume);
	}
	else
	{
		CSoundManager::GetInstance()->AdjustMaster(0);
	}
}

void CSoundOptionState::LoadSettings(string szFilename)
{
	if(szFilename.empty())
		return;
	
	string szFilePath = g_szOptionsLoc;
	szFilePath.append(szFilename);
	szFilePath.append(".txt");
	fstream in;
	in.open(szFilePath.c_str(), ios_base::in | ios_base::binary);
	
	if(!in.is_open())
	{
		return;
	}

	while(!in.eof())
	{
		in.read((char*)&m_usMasterVolume,sizeof(unsigned short));
		in.read((char*)&m_usMusicVolume, sizeof(unsigned short));
		in.read((char*)&m_usSoundVolume, sizeof(unsigned short));
		in.read((char*)&isMuteOn, sizeof(bool));
	}
	
	
	in.close();
	in.clear();

}

void CSoundOptionState::SaveSettings(string szFilename)
{
	ofstream out;
	
	string szFilePath = g_szOptionsLoc;
	szFilePath.append(szFilename);
	szFilePath.append(".txt");
	out.open(szFilePath.c_str(), out.out | out.trunc | out.binary);
	
	if(!out.is_open())
	{
		return;
	}
	
	out.write((char*)&m_usMasterVolume,sizeof(unsigned short));
	out.write((char*)&m_usMusicVolume, sizeof(unsigned short));
	out.write((char*)&m_usSoundVolume, sizeof(unsigned short));
	out.write((char*)&isMuteOn,sizeof(bool));

	out.close();
	out.clear();
}

CSoundOptionState::~CSoundOptionState()
{

}

void CSoundOptionState::RestoreDefaults()
{
	m_usMasterVolume = 100;
	m_usMusicVolume = 100;
	m_usSoundVolume = 100;
	isMuteOn = false;

	SaveSettings("Sound");
}
