/********************************************************************
* Filename:			CControlsState.cpp
* Date:				11/21/2012
* Mod. Date:		11/21/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the controls menu state
********************************************************************/

#include "CControlsState.h"
#include "COptionsMenuState.h"
#include "../Renderer/View.h"
#include "../Util/CFontManager.h"
#include "../CGame.h"


static const char* g_szOptionsLoc = "Resource/Options/";

CControlsState::CControlsState(void) : m_nSelected(CONTROL_SELECT)
{
	m_nFontID = -1;
	m_cMoveUp = 'W';
	m_cMoveDown = 'S';
	m_cMoveLeft = 'A';
	m_cMoveRight = 'D';
	m_cFireAttack = 'J';
	m_cIceAttack = 'K';
	m_cRage ='L';
	m_cDash = VK_SPACE;

	m_bChanging = false;

	for(int i = 0; i < 8; ++i)
	{
		m_cCommands[i] = 0;
	}

	for(int i = 0; i < 4; ++i)
	{
		m_sButtons[i] = 0;
	}
	
	m_nIndexCheck = -1;
	m_cTempControl = -1;
	m_sTempButton = 0;

	m_bUseController = false;
	m_dwControllerResult = -1;
	m_bKeyboardMenu = true;
}

void CControlsState::Enter(void)
{
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);
	m_tButtonHover.m_nImageID = CView::LoadSprite("menu_button_hover.png", &m_tButtonHover.m_nImageWidth, &m_tButtonHover.m_nImageHeight);
	m_tBackground.m_nImageID = CView::LoadSprite("fire-and-ice.jpg", &m_tBackground.m_nImageWidth, &m_tBackground.m_nImageHeight);

	m_nFontID = CView::LoadFont("font example.txt");

	//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"MainMenu");

	m_nSelected = CONTROL_SELECT;

	m_cCommands[0] = 'W';
	m_cCommands[1] = 'S';
	m_cCommands[2] = 'A';
	m_cCommands[3] = 'D';
	m_cCommands[4] = 'K';
	m_cCommands[5] = 'J';
	m_cCommands[6] = 'L';
	m_cCommands[7] = VK_SPACE;

	m_sButtons[0] = XINPUT_GAMEPAD_B;
	m_sButtons[1] = XINPUT_GAMEPAD_X;
	m_sButtons[2] = XINPUT_GAMEPAD_Y;
	m_sButtons[3] = XINPUT_GAMEPAD_A;
	m_fIncrementTimer = 0.0f;

	
	//DWORD dwKeyResult;
	
//	XINPUT_KEYSTROKE xiKeystroke;
	

	ZeroMemory(&m_xsControllerState, sizeof(XINPUT_STATE));
	//ZeroMemory(&xiKeystroke, sizeof(XINPUT_KEYSTROKE));

	static short sPrevPressed;
	//static bool bNoMove = false;

	m_dwControllerResult = XInputGetState(0, &m_xsControllerState);

	if( m_dwControllerResult == ERROR_SUCCESS )
    {
		m_bUseController = true;
	}

	//else
	//{
	//	m_bUseController = false;
	//}
	m_bKeyboardMenu = true;
	LoadInput("Controls");
	m_tTimer.Start();
}

bool CControlsState::Input(void)
{
	if(m_bKeyboardMenu)
	{
		if(m_nSelected == CONTROL_SELECT)
		{
			if(m_pInput->Menu_Left() || m_pInput->Menu_Right())
			{
				if(m_fIncrementTimer >= 0.5f)
				{
					m_fIncrementTimer = 0.0f;
					m_bKeyboardMenu = false;
					m_nSelected = CONTROLLER_SELECT;
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
		}

	}
	else
	{

		switch(m_nSelected)
		{
		case CONTROLLER_SELECT:
			{
				if(m_pInput->Menu_Left() || m_pInput->Menu_Right())
				{
					if(m_fIncrementTimer >= 0.5f)
					{
						m_bKeyboardMenu = true;
						m_nSelected = CONTROL_SELECT;
						m_fIncrementTimer = 0.0f;
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
					}
				}

				break;
			}

		case CONTROLLER_ACTIVE:
			{
				if(m_pInput->Menu_Left() || m_pInput->Menu_Right())
				{
					if(m_fIncrementTimer >= 0.5f)
					{
						m_fIncrementTimer = 0.0f;
						m_bUseController = !m_bUseController;
					}
				}

				break;
			}
		}
	}

	if(m_pInput->Select())
	{
	//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
		if(m_bKeyboardMenu)
		{
			switch(m_nSelected)
			{
			case CONTROL_EXIT:
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				CGame::GetInstance()->PopState();
				break;
			case CONTROL_SAVE:
				{
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
					SaveInput("Controls");
					m_pInput->LoadInput("Controls");
				}
				break;
				
			case CONTROL_DEFAULT:
				{
					CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuAccept");
					RestoreDefaults();
				}
				break;

			default:
				m_bChanging = true;

			}
		}
		else
		{
			switch(m_nSelected)
			{
			case CONTROLLER_EXIT:
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_EXIT, GAME_OBJECT_ID_PLAYER);
				CGame::GetInstance()->PopState();
				break;

			default:
				m_bChanging = true;

			}
		}
		
	}

	if(m_pInput->Menu_Up())
	{
		if(!m_bChanging)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			if(m_bKeyboardMenu)
			{
				if(m_nSelected == CONTROL_MIN)
				{
					m_nSelected = CONTROL_MAX - 1;
				}
				else if (m_nSelected == CONTROL_FIREATTACK)
				{
					m_nSelected = CONTROL_SELECT;
				}
				else
				{
					--m_nSelected;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
					if(m_nSelected == CONTROLLER_MIN)
					{
						m_nSelected = CONTROLLER_MAX - 1;
					}
				}
			}
			else
			{
				--m_nSelected;
			//	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				if(m_nSelected == CONTROLLER_MIN)
				{
					m_nSelected = CONTROLLER_MAX - 1;
				}
			}
		}
	}
	if(m_pInput->Menu_Down())
	{
		if(!m_bChanging)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
			if(m_bKeyboardMenu)
			{
				if(m_nSelected == CONTROL_MAX)
				{
					m_nSelected = CONTROL_MIN + 1;
				}
				//else if(m_nSelected == CONTROL_EXIT || m_nSelected == CONTROL_DEFAULT || m_nSelected == CONTROL_SAVE)
				//{
				//	m_nSelected = CONTROL_SELECT;
				//}
				else if (m_nSelected == CONTROL_RIGHT)
				{
					m_nSelected = CONTROL_EXIT;
				}
				//else if (m_nSelected == CONTROL_DASH)
				//{
				//	m_nSelected = CONTROL_SAVE;
				//}
				else
				{
					++m_nSelected;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
			}
			else
			{
				++m_nSelected;
			//	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				if(m_nSelected == CONTROLLER_MAX)
				{
					m_nSelected = CONTROLLER_MIN + 1;
				}
			}
		}
	}

	if(m_pInput->Menu_Right())
	{
		if(!m_bChanging)
		{
			if(m_fIncrementTimer >= 0.5f)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				if (m_nSelected == CONTROL_EXIT)
				{
					m_nSelected = CONTROL_SAVE;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
				else if (m_nSelected == CONTROL_SAVE)
				{
					m_nSelected = CONTROL_DEFAULT;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
				else if (m_nSelected == CONTROL_DEFAULT)
				{
					m_nSelected = CONTROL_EXIT;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
				else if(m_bKeyboardMenu)
				{
					if(m_nSelected != CONTROL_SELECT)
					{
						if(m_nSelected < 6)
						{
							m_nSelected += 4;
			//				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
						}
						
						/*else if (m_nSelected == CONTROL_DASH)
						{
							m_nSelected = CONTROL_UP;
						}*/

						else
						{
							m_nSelected -= 4;
			//				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
						}
					}

					//if(m_nSelected >= CONTROL_MAX)
					//{
					//	m_nSelected = CONTROL_UP;
					//}
				}
				else
				{
					if(m_nSelected != CONTROLLER_SELECT)
					{
						++m_nSelected;
			//			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
					}

					if(m_nSelected == CONTROLLER_MAX)
					{
						m_nSelected = CONTROLLER_MIN + 1;
					}
				}

				m_fIncrementTimer = 0.0f;
			}
		}
	}

	if(m_pInput->Menu_Left())
	{
		if(!m_bChanging)
		{
			if(m_fIncrementTimer >= 0.5f)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				if (m_nSelected == CONTROL_EXIT)
				{
					m_nSelected = CONTROL_DEFAULT;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
				else if (m_nSelected == CONTROL_SAVE)
				{
					m_nSelected = CONTROL_EXIT;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
				else if (m_nSelected == CONTROL_DEFAULT)
				{
					m_nSelected = CONTROL_SAVE;
			//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
				}
				else if(m_bKeyboardMenu)
				{
					if(m_nSelected != CONTROL_SELECT)
					{
						if(m_nSelected > 5)
						{
							m_nSelected -= 4;
			//				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
						}
						else
						{
							m_nSelected += 4;
				//			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
						}
					}
				}
				else
				{
					if(m_nSelected != CONTROLLER_SELECT)
					{
						--m_nSelected;
				//		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MENU_MOVEMENT, GAME_OBJECT_ID_PLAYER);
					}

					if(m_nSelected == CONTROLLER_MIN)
					{
						m_nSelected = CONTROLLER_MAX - 1;
					}
				}
				m_fIncrementTimer = 0.0f;
			}
		}
	}

	//RenderBox();
	return true;
}

void CControlsState::Update(float _fElapsedTime)
{
	_fElapsedTime = m_tTimer.GetElapsedTime();
	CView::Update(_fElapsedTime);
	XInputGetState(0, &m_xsControllerState);
	m_fIncrementTimer += _fElapsedTime;

	if(-1 != m_nIndexCheck) 
	{
		bool bSwitched = false;

		if(m_bKeyboardMenu)
		{
			int nTemp = -1;
			

			if(-1 != m_cTempControl)
			{
				for(int i = 0; i < 8; ++i)
				{
					// If the key the user pressed was the same as the key set to another command,
					// we switch the commands the keys are set to

					if(m_cTempControl == m_cCommands[i] && i != m_nIndexCheck)
					{
						m_cCommands[i] = m_cCommands[m_nIndexCheck];
						m_cCommands[m_nIndexCheck] = m_cTempControl;
						//m_nIndexCheck = -1;
						//m_cTempControl = -1;
						//nTemp = i;
						bSwitched = true;
						break;
					}

					
					
				}
				// ...Otherwise we just set the key normally
				if(false == bSwitched)
				{
					m_cCommands[m_nIndexCheck] = m_cTempControl;
					//m_nIndexCheck = -1;
					//m_cTempControl = -1;
					//nTemp = m_nIndexCheck;
					//break;
				}
				//m_cCommands[nTemp] = m_cCommands[m_nIndexCheck];
				//m_cCommands[m_nIndexCheck] = m_cTempControl;
				m_cTempControl = -1;
			}
		}
		else
		{
			if(0 != m_sTempButton)
			{
				for(int i = 0; i < 4; ++i)
				{
					// If the key the user pressed was the same as the key set to another command,
					// we switch the commands the keys are set to

					if(m_sTempButton == m_sButtons[i] && i != m_nIndexCheck)
					{
						m_sButtons[i] = 0;
						m_sButtons[i] = m_sButtons[m_nIndexCheck];
						m_sButtons[m_nIndexCheck] = 0;
						m_sButtons[m_nIndexCheck] = m_sTempButton;
						//m_nIndexCheck = -1;
						//m_cTempControl = -1;
						//nTemp = i;
						bSwitched = true;
						break;
					}

					
					
				}
				// ...Otherwise we just set the key normally
				if(false == bSwitched)
				{
					m_sButtons[m_nIndexCheck] = 0;
					m_sButtons[m_nIndexCheck] = m_sTempButton;
					//m_nIndexCheck = -1;
					//m_cTempControl = -1;
					//nTemp = m_nIndexCheck;
					//break;
				}
				//m_cCommands[nTemp] = m_cCommands[m_nIndexCheck];
				//m_cCommands[m_nIndexCheck] = m_cTempControl;
				m_sTempButton = 0;
			}
		}

		m_nIndexCheck = -1;
		//bSwitched = false;
	}

	m_cMoveUp = m_cCommands[0];
	m_cMoveDown = m_cCommands[1];
	m_cMoveLeft = m_cCommands[2];
	m_cMoveRight = m_cCommands[3];
	m_cFireAttack = m_cCommands[4];
	m_cIceAttack = m_cCommands[5];
	m_cRage = m_cCommands[6];
	m_cDash = m_cCommands[7];
}

void CControlsState::Render(void)
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

	int nButtonHoverPosX = nHalfScreenWidth - (m_tButtonHover.m_nImageWidth >> 1);
	int nButtonNormalPosX = nHalfScreenWidth - (m_tButtonNormal.m_nImageWidth >> 1);
	int ButtonYOffset = 10;

	RECT tTextRect;

	if(!m_bChanging)
	{
		std::string szCharBlock;

		tTextRect.left		=	nButtonHoverPosX;
		tTextRect.top		=	nHalfScreenHeight - (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 6;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "\"Up\", \"Down\", \"Enter\" to choose the button to change.", &tTextRect, true, 0.5f);

		tTextRect.left		=	nButtonHoverPosX;
		tTextRect.top		=	nHalfScreenHeight - (m_tButtonHover.m_nImageHeight + ButtonYOffset) * 5;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "-> or <- to switch schemes.", &tTextRect, true, 0.5f);

		std::string szScheme;

		// Draw the current config
		if(m_bKeyboardMenu)
		{
			szScheme = "Keyboard";	
		}
		else
		{
			szScheme = "Controller";
		}
		CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.7f, 0.7f, szScheme, CONTROL_SELECT == m_nSelected || CONTROLLER_SELECT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, -2.25);

		if(m_bKeyboardMenu)
		{
			// Draw the move up button and text
			szCharBlock = m_cMoveUp;
			if(m_cMoveUp == VK_SPACE)
			{
				szCharBlock = "Space";
			}

			if(m_cMoveUp == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_UP == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -0.5, -1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Move Up", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1.5, -1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			
			// Draw the Move down Button and text
			szCharBlock = m_cMoveDown;
			if(m_cMoveDown == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cMoveDown == VK_TAB)
			{
				szCharBlock = "Tab";
			}			
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_DOWN == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -0.5, 0);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Move Down", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1.5, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw the move left button
			szCharBlock = m_cMoveLeft;
			if(m_cMoveLeft == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cMoveLeft == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_LEFT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -0.5, 1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Move Left", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1.5, 1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw move right Button
			szCharBlock = m_cMoveRight;
			if(m_cMoveRight == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cMoveRight == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_RIGHT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -0.5, 2);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Move Right", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1.5, 2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw the fire attack button
			szCharBlock = m_cFireAttack;
			if(m_cFireAttack == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cFireAttack == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_FIREATTACK == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.5, -1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Fire Attack", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.5, -1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw the ice attack button
			szCharBlock = m_cIceAttack;
			if(m_cIceAttack == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cIceAttack == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_ICEATTACK == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.5, 0);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Ice Attack", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.5, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// draw the rage button
			szCharBlock = m_cRage;
			if(m_cRage == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cRage == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_RAGE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.5, 1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Rage", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.5, 1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Write the dash key
			szCharBlock = m_cDash;
			if(m_cDash == VK_SPACE)
			{
				szCharBlock = "Space";
			}
			if(m_cDash == VK_TAB)
			{
				szCharBlock = "Tab";
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROL_DASH == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.5, 2);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Dash", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.5, 2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// draw the exit button
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Back", CONTROL_EXIT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 3.5);
			
			// draw the defaul button
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Default", CONTROL_DEFAULT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -1, 3.5);

			// Draw the controler save button
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Save", CONTROL_SAVE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 3.5);
		}

		else
		{
			// Draw the Fire attack button
			switch(m_sButtons[COMMAND_FIRE_ATTACK])
			{
			case XINPUT_GAMEPAD_A:
				szCharBlock = "A";
				break;

			case XINPUT_GAMEPAD_B:
				szCharBlock = "B";
				break;

			case XINPUT_GAMEPAD_X:
				szCharBlock = "X";
				break;

			case XINPUT_GAMEPAD_Y:
				szCharBlock = "Y";
				break;

			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROLLER_FIRE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -0.5, -1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Fire Attack", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1.5, -1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw the ice attack button
			switch(m_sButtons[COMMAND_ICE_ATTACK])
			{
			case XINPUT_GAMEPAD_A:
				szCharBlock = "A";
				break;

			case XINPUT_GAMEPAD_B:
				szCharBlock = "B";
				break;

			case XINPUT_GAMEPAD_X:
				szCharBlock = "X";
				break;

			case XINPUT_GAMEPAD_Y:
				szCharBlock = "Y";
				break;

			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROLLER_ICE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.5, -1);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Ice Attack", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.5, -1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


			// Draw the rage button
			switch(m_sButtons[COMMAND_RAGE])
			{
			case XINPUT_GAMEPAD_A:
				szCharBlock = "A";
				break;

			case XINPUT_GAMEPAD_B:
				szCharBlock = "B";
				break;

			case XINPUT_GAMEPAD_X:
				szCharBlock = "X";
				break;

			case XINPUT_GAMEPAD_Y:
				szCharBlock = "Y";
				break;

			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROLLER_RAGE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), -0.5, 0);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Rage", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1.5, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw the dash button
			switch(m_sButtons[COMMAND_DASH])
			{
			case XINPUT_GAMEPAD_A:
				szCharBlock = "A";
				break;

			case XINPUT_GAMEPAD_B:
				szCharBlock = "B";
				break;

			case XINPUT_GAMEPAD_X:
				szCharBlock = "X";
				break;

			case XINPUT_GAMEPAD_Y:
				szCharBlock = "Y";
				break;

			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, szCharBlock, CONTROLLER_DASH == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.5, 0);
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Dash", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.5, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


			// Writing the "Controller" Label for the button
			if(m_bUseController)
			{
				CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Yes", CONTROLLER_ACTIVE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 1);
			}

			else
			{
				CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "No", CONTROLLER_ACTIVE == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 1);
			}
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Controller", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), -1, 1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// Draw the back button
			CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.6f, 0.6f, "Back", CONTROLLER_EXIT == m_nSelected, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 3);
		}
	}

	else
	{
	RenderBox();
	}
}

void CControlsState::Exit(void)
{
	//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"MainMenu");
	//SaveInput("Controls");
	m_pInput->LoadInput("Controls");
}

void CControlsState::ChangeKey(char& input)
{

	if( (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) || m_pInput->Back())
	{
		m_bChanging = false;
		return;
	}

	for(char i = 'A'; i < ']'; ++i)
	{
		if(GetAsyncKeyState(i) & 0x8000)
		{
			input = i;
			m_bChanging = false;
			//i += '\0';
			
			//strcpy_s(input, sizeof(char), &i);
			//break;
			return;// input;
		}
	}

	for(char i = ','; i < '9'; ++i)
	{
		if(GetAsyncKeyState(i) & 0x8000)
		{
			//input = i;
			m_bChanging = false;
			//strcpy_s(input, sizeof(char), &i);
			//break;
			input = i;
			return;//input;
		}
	}

	if(GetAsyncKeyState(';') & 0x8000)
	{
		input = ';';
		m_bChanging = false;
		//return ";";
	}


	else if(GetAsyncKeyState('`') & 0x8000)
	{
		input = '`';
		m_bChanging = false;
		return;
	}

	else if(GetAsyncKeyState(VK_SPACE))
	{
		input = VK_SPACE;
		m_bChanging = false;
		return;
	}

	else if(GetAsyncKeyState('=') & 0x8000)
	{
		input = '=';
		m_bChanging = false;
		return;
	}
}

void CControlsState::ChangeButton(WORD& input)
{
	if( (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) || m_pInput->Back())
	{
		m_bChanging = false;
		return;
	}

	if(m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		input = XINPUT_GAMEPAD_A;
		m_bChanging = false;
		return;
	}

	else if(m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		input = XINPUT_GAMEPAD_B;
		m_bChanging = false;
		return;
	}

	else if(m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		input = XINPUT_GAMEPAD_X;
		m_bChanging = false;
		return;
	}

	else if(m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		input = XINPUT_GAMEPAD_Y;
		m_bChanging = false;
		return;
	}
	
	
}

void CControlsState::RenderBox(void)
{			
	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

	int nButtonHoverPosX = nHalfScreenWidth - (m_tButtonHover.m_nImageWidth >> 1);
	int nButtonNormalPosX = nHalfScreenWidth - (m_tButtonNormal.m_nImageWidth >> 1);
	int ButtonYOffset = 10;

	RECT tTextRect;

	CView::Draw(m_tButtonNormal.m_nImageID, nButtonHoverPosX - nHalfScreenWidth/2 + ButtonYOffset * 6 , nHalfScreenHeight - ButtonYOffset * 2, 2.5f, 2.0f);
	tTextRect.left		=	nButtonHoverPosX ;
	tTextRect.top		=	nHalfScreenHeight - ButtonYOffset * 2;
	tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
	tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;

	if(CONTROL_UP == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Up\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_UP - CONTROL_UP;

	}

	else if(CONTROL_DOWN == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Down\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_DOWN - CONTROL_UP;

	}

	else if(CONTROL_LEFT == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Left\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_LEFT - CONTROL_UP;

	}

	else if(CONTROL_RIGHT == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Right\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_RIGHT - CONTROL_UP;
	}

	else if(CONTROL_FIREATTACK == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Fire Attack\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_FIREATTACK - CONTROL_UP;

	}

	else if(CONTROL_ICEATTACK == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Ice Attack\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_ICEATTACK - CONTROL_UP;

	}

	else if(CONTROL_RAGE == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Rage\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_RAGE - CONTROL_UP;

	}

	else if(CONTROL_DASH == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the key you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Dash\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = CONTROL_DASH - CONTROL_UP;
	}

	else if(CONTROLLER_FIRE == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the button you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Fire Attack\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = COMMAND_FIRE_ATTACK;

	}

	else if(CONTROLLER_ICE == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the button you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Ice Attack\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = COMMAND_ICE_ATTACK;

	}

	else if(CONTROLLER_RAGE == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the button you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Rage\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = COMMAND_RAGE;

	}

	else if(CONTROLLER_DASH == m_nSelected)
	{
		CView::Write(m_nFontID, "Press the button you wish to", &tTextRect, true, 0.35f);
		tTextRect.left		=	nButtonHoverPosX ;
		tTextRect.top		=	tTextRect.bottom + ButtonYOffset/2;
		tTextRect.right		=	tTextRect.left + m_tButtonHover.m_nImageWidth;
		tTextRect.bottom	=	tTextRect.top + m_tButtonHover.m_nImageHeight;
		CView::Write(m_nFontID, "set the \"Dash\" command to", &tTextRect, true, 0.35f);
		m_nIndexCheck = COMMAND_DASH;
	}

	if(m_bKeyboardMenu)
	{
		ChangeKey(m_cTempControl);
	}

	else
	{
		ChangeButton(m_sTempButton);
	}
	//}while (m_bChanging);
}

void CControlsState::LoadInput(string szFilename)
{
	if(szFilename.empty())
		return;

	string szFilePath = g_szOptionsLoc;//szFilename;

	szFilePath.append(szFilename);
	//szFilePath.append(".option");
	szFilePath.append(".txt");
	string openName = "Resource/Options/Test.txt";

	fstream in;
	in.open(openName.c_str(), ios_base::in | ios_base::binary);

	if(!in.is_open())
	{
		return;
	}
	char cCommands[8];
	string szLoadout = m_cCommands;
	//int counter = 0;
	for(unsigned int i = 0; i < 8;i++)
	{
		//char* buffer = 0;

		////Read in the "Up" value
		//in.read(buffer, sizeof(char));
		//m_cMoveUp = *buffer;
		//
		////Read in the "Down" value
		//in.read(buffer, sizeof(char));
		//m_cMoveDown = *buffer;
		//
		////Read in the "Left" value
		//in.read(buffer, sizeof(char));
		//m_cMoveLeft = *buffer;
		//
		////Read in the "Right" value
		//in.read(buffer, sizeof(char));
		//m_cMoveRight = *buffer;

		//Read in the "Up" value
		//if(!in.good())
		//{
		//	break;
		//}

		// TODO: Parse through the file for each gameplay input

		//in.read((char*)szLoadout.c_str(), strlen(szLoadout.c_str()));
		//m_cComm = *buffer;

		in.read(&(cCommands[i]),1);
		//in >> cCommands[i];
		//in.ignore(INT_MAX, '\n');

		//counter++;
	}
	in.read((char*)&(m_bUseController),1);

	for(int i = 0; i < 4; ++i)
	{
		//string szLoadout = m_cCommands;
		//in.read((char*)&(m_sButtons[i]), sizeof(WORD));
		in >> m_sButtons[i];
		//in.ignore(INT_MAX, '\n');
	}
	
	in.close();
	in.clear();

	for(size_t i = 0; i < 8; ++i)
	{
		m_cCommands[i] = cCommands[i];
	}



	m_cMoveUp = cCommands[0];
	m_cMoveDown =  cCommands[1];
	m_cMoveLeft =  cCommands[2];
	m_cMoveRight =  cCommands[3];
	m_cFireAttack =  cCommands[4];
	m_cIceAttack =  cCommands[5];
	m_cRage =  cCommands[6];
	m_cDash =  cCommands[7];

}

void CControlsState::SaveInput(string szFilename)
{
	ofstream out;
	
	string szFilePath = g_szOptionsLoc;
	szFilePath.append(szFilename);
	szFilePath.append(".txt");
	string openName = "Resource/Options/Test.txt";

	out.open(openName.c_str(), ios_base::out | ios_base::trunc);

	if(!out.is_open())
	{
		return;
	}

	for(int i = 0; i < 8; ++i)
	{
		out.write(&(m_cCommands[i]),1);
	}

	out.write((char*)&(m_bUseController),1);

	for(int i = 0; i < 4; ++i)
	{
		out.write((char*)&(m_sButtons[i]), sizeof(WORD));
	}

	out.close();
	out.clear();
}

CControlsState::~CControlsState()
{

}

void CControlsState::RestoreDefaults()
{
	m_cMoveUp = 'W';
	m_cMoveDown = 'S';
	m_cMoveLeft = 'A';
	m_cMoveRight = 'D';
	m_cFireAttack = 'J';
	m_cIceAttack = 'K';
	m_cRage ='L';
	m_cDash = VK_SPACE;

	m_cCommands[0] = 'W';
	m_cCommands[1] = 'S';
	m_cCommands[2] = 'A';
	m_cCommands[3] = 'D';
	m_cCommands[4] = 'J';
	m_cCommands[5] = 'K';
	m_cCommands[6] = 'L';
	m_cCommands[7] = VK_SPACE;

	SaveInput("Controls");
}