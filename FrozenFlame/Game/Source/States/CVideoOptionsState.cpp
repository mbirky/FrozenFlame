/********************************************************************
* Filename:			CVideoOptionsState.cpp
* Date:				11/23/2012
* Mod. Date:		11/23/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the controls menu state
********************************************************************/

#include "CVideoOptionsState.h"
#include "COptionsMenuState.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include "../Util/CFontManager.h"
#include "../Globals.h"

static const char* g_szOptionsLoc = "Resource/Options/";
const float g_nMaxGamma = 100;
const float g_nMinGamma = 1;

CVideoOptionsState::CVideoOptionsState(void) : m_nSelected(VID_FULLSCREEN)
{
	m_nFontID = -1;
	m_nGammaPower = 0;
	m_fActualGamma = 0.0f;
	m_ucRenderFlags = 0;
	m_fIncrementTimer = 0.0f;
	m_fFlashTimer = 0.0f;
	m_bPrevLeft = false;
	m_bPrevRight = false;

}

void CVideoOptionsState::Enter(void)
{
	m_tBackground.m_nImageID = CView::LoadSprite("fire-and-ice.jpg", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);

	m_nFontID = CView::LoadFont("font example.txt");

	m_nSelected = VID_FULLSCREEN;

	m_nGammaPower = 65;
	m_fActualGamma = 1.3f;
	m_fIncrementTimer = 0.5f;
	m_fFlashTimer = 0.0f;
	m_bDisplayControls = false;
	m_bPrevLeft = false;
	m_bPrevRight = false;

	m_ucRenderFlags = RF_RES_1024x768 | RF_POST_PROCESS | RF_PARTICLES;

	LoadSettings("Video");
	m_ucOldResolutionFlag = (m_ucRenderFlags & RF_RES_ALL_ON);
	m_bChangedResolution = false;

	m_tTimer.Start();
}

bool CVideoOptionsState::Input(void)
{
	switch(m_nSelected)
	{
	case VID_FULLSCREEN:
		{
			if(m_pInput->Menu_Right())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					if( (m_ucRenderFlags & RF_FULLSCREEN) != 0 )
					{
						m_ucRenderFlags &= ~RF_FULLSCREEN;
					}

					else
					{
						m_ucRenderFlags |= RF_FULLSCREEN;
					}
					m_fIncrementTimer = 0.0f;
				}
			}

			if(m_pInput->Menu_Left())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					if( (m_ucRenderFlags & RF_FULLSCREEN) != 0 )
					{
						m_ucRenderFlags &= ~RF_FULLSCREEN;
					}

					else
					{
						m_ucRenderFlags |= RF_FULLSCREEN;
					}
					m_fIncrementTimer = 0.0f;
				}
			}
			break;
		}

	case VID_RESOLUTION:
		{

			if(m_pInput->Menu_Right())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					if(m_ucRenderFlags & RF_RES_640x480)
					{
						m_ucRenderFlags &= ~RF_RES_640x480;
						m_ucRenderFlags |= RF_RES_1024x768;
					}
					else if(m_ucRenderFlags & RF_RES_1024x768)
					{
						m_ucRenderFlags &= ~RF_RES_1024x768;
						m_ucRenderFlags |= RF_RES_1280x720;
					}
					else if(m_ucRenderFlags & RF_RES_1280x720)
					{
						m_ucRenderFlags &= ~RF_RES_1280x720;
						m_ucRenderFlags |= RF_RES_1680x1050;
					}
					else if(m_ucRenderFlags & RF_RES_1680x1050)
					{
						m_ucRenderFlags &= ~RF_RES_1680x1050;
						m_ucRenderFlags |= RF_RES_1920x1080;
					}
					else if(m_ucRenderFlags & RF_RES_1920x1080)
					{
						m_ucRenderFlags &= ~RF_RES_1920x1080;
						m_ucRenderFlags &= ~RF_RES_ALL_ON;	// we're now moving to auto so turn off all resolution bits
					}
					else
					{
						// we're on auto so there's nothing to turn off
						m_ucRenderFlags |= RF_RES_640x480; // but as we move we're going to the lowest resolution
					}
					m_fIncrementTimer = 0.0f;
				}
			}

			if(m_pInput->Menu_Left())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					if(m_ucRenderFlags & RF_RES_640x480)
					{
						m_ucRenderFlags &= ~RF_RES_640x480;
						m_ucRenderFlags &= ~RF_RES_ALL_ON;	// in the lower resolution, if we move left we should go to auto - all off
					}
					else if(m_ucRenderFlags & RF_RES_1024x768)
					{
						m_ucRenderFlags &= ~RF_RES_1024x768;
						m_ucRenderFlags |= RF_RES_640x480;
					}
					else if(m_ucRenderFlags & RF_RES_1280x720)
					{
						m_ucRenderFlags &= ~RF_RES_1280x720;
						m_ucRenderFlags |= RF_RES_1024x768;
					}
					else if(m_ucRenderFlags & RF_RES_1680x1050)
					{
						m_ucRenderFlags &= ~RF_RES_1680x1050;
						m_ucRenderFlags |= RF_RES_1280x720;
					}
					else if(m_ucRenderFlags & RF_RES_1920x1080)
					{
						m_ucRenderFlags &= ~RF_RES_1920x1080;
						m_ucRenderFlags |= RF_RES_1680x1050;
					}
					else
					{
						// if we're at auto and we're moving left, go to the highest one
						m_ucRenderFlags |= RF_RES_1920x1080;
					}
					m_fIncrementTimer = 0.0f;
				}
			}
			break;
		}

		case VID_GAMMA:
		{

			if(m_pInput->Menu_Right())
			{
				if( m_nGammaPower <= g_nMaxGamma)
				{
					if(m_fIncrementTimer >= 0.15f && m_nGammaPower < 101)
					{
						m_nGammaPower += 1;
						//m_nActualGamma -= 1;
						m_fIncrementTimer = 0.0f;
						CGame::GetInstance()->SetGammaValue(float(m_nGammaPower/100.0f) * 2.0f);
					}
				}

				if( m_nGammaPower > 100 )
				{
					m_nGammaPower = 100;
				}
			}

			if(m_pInput->Menu_Left())
			{

				if( m_nGammaPower > g_nMinGamma )
				{
					if(m_fIncrementTimer >= 0.15f)
					{
						m_nGammaPower -= 1;
						//m_nActualGamma += 1;
						m_fIncrementTimer = 0.0f;
						CGame::GetInstance()->SetGammaValue(float(m_nGammaPower/100.0f) * 2.0f);
					}
				}

				if( m_nGammaPower <= g_nMinGamma && m_nGammaPower)
				{
					m_nGammaPower = 1;
					
				}
			}
			break;
		}

		case VID_POSTPROCESS:
		{

			if(m_pInput->Menu_Right() || m_pInput->Menu_Left())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					if( (m_ucRenderFlags & RF_POST_PROCESS) != 0 )
					{
						m_ucRenderFlags &= ~RF_POST_PROCESS;
					}

					else
					{
						m_ucRenderFlags |= RF_POST_PROCESS;
					}
					m_fIncrementTimer = 0.0f;
					CGame::GetInstance()->SetRenderFlags(m_ucRenderFlags);
				}
			}
			break;
		}

		case VID_PARTICLES:
		{

			if(m_pInput->Menu_Right() || m_pInput->Menu_Left())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					if( (m_ucRenderFlags & RF_PARTICLES) != 0 )
					{
						m_ucRenderFlags &= ~RF_PARTICLES;
					}

					else
					{
						m_ucRenderFlags |= RF_PARTICLES;
					}
					m_fIncrementTimer = 0.0f;
				}
			}

			break;
		}

		case VID_EXIT:
			{
				if (m_pInput->Menu_Left())
				{
					if (!m_bPrevLeft)
					{
						m_bPrevLeft = true;
						m_nSelected = VID_DEFAULT;
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
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
						m_nSelected = VID_SAVE;
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
					}
				}
				else
				{
					m_bPrevRight = false;
				}
			}
			break;

		case VID_SAVE:
			{
				if(!m_bChangedResolution)
				{
					if (m_pInput->Menu_Left())
					{
						if (!m_bPrevLeft)
						{
							m_bPrevLeft = true;
							m_nSelected = VID_EXIT;
							CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
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
							m_nSelected = VID_DEFAULT;
							CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
						}
					}
					else
					{
						m_bPrevRight = false;
					}
				}
			}
			break;

		case VID_DEFAULT:
			{
				if (m_pInput->Menu_Left())
				{
					if (!m_bPrevLeft)
					{
						m_bPrevLeft = true;
						m_nSelected = VID_SAVE;
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
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
						m_nSelected = VID_EXIT;
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
					}
				}
				else
				{
					m_bPrevRight = false;
				}
			}
			break;
	}

	if(m_pInput->Select())
	{
		
		if(VID_EXIT == m_nSelected)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
			CGame::GetInstance()->PopState();
		}
		if (m_nSelected == VID_DEFAULT)
		{
			CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
			RestoreDefaults();
		}
		if (m_nSelected == VID_SAVE)
		{
			if(!m_bChangedResolution)
			{
				if( (m_ucOldResolutionFlag ^ (m_ucRenderFlags & RF_RES_ALL_ON) ) != 0 )
				{
					m_bChangedResolution = true;
				}

				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
				SaveSettings("Video");
			}
			else if(m_bChangedResolution)
			{
				m_bChangedResolution = false;
			}
		}
	}

	if(m_pInput->Menu_Up())
	{
		if(!m_bChangedResolution){
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		//if(!m_bChanging)
		//{
		if (m_nSelected == VID_DEFAULT || m_nSelected == VID_SAVE)
		{
			m_nSelected = VID_PARTICLES;
		}
		else
		{
			--m_nSelected;
			if(m_nSelected == VID_MIN)
			{
				m_nSelected = VID_MAX - 1;
			}
		}
		}
	}
	if(m_pInput->Menu_Down())
	{
		if(!m_bChangedResolution){

		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
		//if(!m_bChanging)
		//{
		if (m_nSelected == VID_DEFAULT || m_nSelected == VID_SAVE || m_nSelected == VID_EXIT)
		{
			m_nSelected = VID_MIN + 1;
		}
		else
		{
			++m_nSelected;
			if(m_nSelected == VID_MAX)
			{
				m_nSelected = VID_MIN + 1;
			}
		}
		}
	}

	//RenderBox();
	return true;
}

void CVideoOptionsState::Update(float _fElapsedTime)
{
	_fElapsedTime = m_tTimer.GetElapsedTime();

	CView::Update(_fElapsedTime);
	m_fIncrementTimer += _fElapsedTime;

	//m_fFlashTimer += _fElapsedTime;

	//if(m_fFlashTimer >= 1.25f)
	//{
	//	m_fFlashTimer = 0.0f;
	//	m_bDisplayControls = !m_bDisplayControls;
	//}
}

void CVideoOptionsState::Render(void)
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

	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.8f, 0.8f, "Video", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0, -2.25);

	// full Screen
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "FullScreen",  VID_FULLSCREEN == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, -1);

	string szbuffer;
	if( (m_ucRenderFlags & RF_FULLSCREEN) != 0 )
	{
		szbuffer = "Yes";
	}

	else
	{
		szbuffer = "No";
	}
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, szbuffer.c_str(),  false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, -1);

	// draw Resolution button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Resolution",  VID_RESOLUTION == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 0);

	// Set the text buffer to the currently selected resolution then draw the button with the buffer
	if(m_ucRenderFlags & RF_RES_640x480)
		szbuffer = "640x480";
	else if(m_ucRenderFlags & RF_RES_1024x768)
		szbuffer = "1024x768";
	else if(m_ucRenderFlags & RF_RES_1280x720)
		szbuffer = "1280x720";
	else if(m_ucRenderFlags & RF_RES_1680x1050)
		szbuffer = "1680x1050";
	else if(m_ucRenderFlags & RF_RES_1920x1080)
		szbuffer = "1920x1080";
	else
		szbuffer = "Auto";
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, szbuffer.c_str(),  false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 0);

	// Draw the Gama button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Gamma",  VID_GAMMA == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 1);
	// Set the buffer to the current gamma then show it
	char buffer[32];
	sprintf_s(buffer, "%d", m_nGammaPower);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, buffer,  false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 1);

	// Draw the post process button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Post FX",  VID_POSTPROCESS == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 2);
	// Draw the button telling if post processing is on or off
	if( (m_ucRenderFlags & RF_POST_PROCESS) != 0 )
	{
		szbuffer = "On";
	}

	else
	{
		szbuffer = "Off";
	}
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, szbuffer.c_str(),  false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 2);

	// Draw the particles button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Particles",  VID_PARTICLES == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 3);
	// Draw on or off depending of if we have particles
	if( (m_ucRenderFlags & RF_PARTICLES) != 0 )
	{
		szbuffer = "On";
	}

	else
	{
		szbuffer = "Off";
	}
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, szbuffer.c_str(),  false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1, 3);

	// Draw the exit button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Back",  VID_EXIT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 5);

	// Draw the Default button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Default",  VID_DEFAULT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1.5, 5);

	// Draw the Save button
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5, 0.5, "Save",  VID_SAVE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.5, 5);

	if(m_bChangedResolution)
	{
		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 2.0f, 2.0f, " ",  true, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.5f, 0.7f, "Resolution will not be changed until game restart",  true, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
}

void CVideoOptionsState::Exit(void)
{
	//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"MainMenu");
	//SaveSettings("Video");
}

void CVideoOptionsState::LoadSettings(string szFilename)
{
	if(szFilename.empty())
		return;
	
	string szFilePath = g_szOptionsLoc;
	//
	szFilePath.append(szFilename);
	////szFilePath.append(".option");
	szFilePath.append(".txt");
	//
	fstream in;
	in.open(szFilePath.c_str(), ios_base::in | ios_base::binary);
	
	if(!in.is_open())
	{
		return;
	}

	while(!in.eof())
	{
		m_fActualGamma = float(m_nGammaPower/100.0f) * 2.0f;

		in.read((char*)&m_ucRenderFlags, sizeof(unsigned char));
		in.read((char*)&m_nGammaPower, sizeof(unsigned int));
		in.read((char*)&m_fActualGamma, sizeof(float));
	}
	
	CGame::GetInstance()->SetGammaValue(m_fActualGamma);

	UpdateFlags();

	CGame::GetInstance()->SetRenderFlags(m_ucRenderFlags);

	in.close();
	in.clear();
}

void CVideoOptionsState::UpdateFlags()
{
	if(CGame::GetInstance()->GetIsFullScreen() && !(m_ucRenderFlags & RF_FULLSCREEN))
		m_ucRenderFlags |= RF_FULLSCREEN;
	else if(!CGame::GetInstance()->GetIsFullScreen() && (m_ucRenderFlags & RF_FULLSCREEN))
		m_ucRenderFlags &= ~RF_FULLSCREEN;
}

void CVideoOptionsState::SaveSettings(string szFilename)
{
	ofstream out;
	
	string szFilePath = g_szOptionsLoc;
	//string szFilePath = szFilename;
	szFilePath.append(szFilename);
	szFilePath.append(".txt");
	////szFilePath.append(".option");
	//
	out.open(szFilePath.c_str(), out.out | out.trunc | out.binary);
	
	if(!out.is_open())
	{
		return;
	}
	
	//string szLoadout = m_ucRenderFlags;
	//}
	m_fActualGamma = float(m_nGammaPower/100.0f) * 2.0f;
	out.write((char*)&m_ucRenderFlags, sizeof(unsigned char));
	out.write((char*)&m_nGammaPower, sizeof(unsigned int));
	out.write((char*)&m_fActualGamma, sizeof(float));

	out.close();
	out.clear();
	CGame::GetInstance()->SetGammaValue(m_fActualGamma);
	CGame::GetInstance()->SetRenderFlags(m_ucRenderFlags);
}

CVideoOptionsState::~CVideoOptionsState()
{

}

void CVideoOptionsState::RestoreDefaults()
{
	m_ucRenderFlags = RF_RES_1024x768 | RF_POST_PROCESS | RF_PARTICLES;
	m_nGammaPower = 65;
	m_fActualGamma = 1.3f;

	SaveSettings("Video");
}