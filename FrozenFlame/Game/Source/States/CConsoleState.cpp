/********************************************************************
* Filename:			CConsoleState.h
* Date:				1/8/2012
* Mod. Date:		1/8/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Console state
********************************************************************/

#include "CConsoleState.h"
#include "../Renderer/View.h"
#include "../Util/CInput.h"
#include "../CGame.h"
#include "../Globals.h"
#include "../Core/CWorldManager.h"

void CConsoleState::Enter(void)
{
	m_nFontID = CView::LoadFont("font example.txt");
	
	m_tBlackBar.m_nImageID = CView::LoadSprite("BlackBar.png", &m_tBlackBar.m_nImageWidth, &m_tBlackBar.m_nImageHeight);

	ZeroMemory(m_ucKeysPressed, sizeof(m_ucKeysPressed));
	HRESULT hr = DirectInput8Create(CGame::GetInstance()->GetHINSTANCE(), 0x0800, IID_IDirectInput8, (void**)&m_pDInput, NULL);

	m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, 0);
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->SetCooperativeLevel(CGame::GetInstance()->GetHWND(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	m_pKeyboard->Acquire();

	CGame::GetInstance()->StopTimer();
	CInput::GetInstance()->Disable();
}

bool CConsoleState::Input(void)
{
	////////////////////////////////////////////////////////////////////////////
	// Store the previously pressed keys
	memcpy(m_ucPrevKeysPressed, m_ucKeysPressed, sizeof(m_ucKeysPressed));

	// Poll Keyboard
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_ucKeysPressed), (void**)&m_ucKeysPressed);

	// if failed try to reaquire keyboard
	if(FAILED(hr))
	{
		ZeroMemory(m_ucKeysPressed, sizeof(m_ucKeysPressed));

		hr = m_pKeyboard->Acquire();
		return true;
	}


	//	Get the Current Keyboard State.
	if (!(GetKeyboardState(m_ucAsciiVals)))
		return 0; // Could not GetKeyboardState.

	unsigned short usNum	= 0;
	unsigned int uiVKCode	= 0;

	if(m_ucKeysPressed[DIK_BACK] & 0x80)
	{
		if(!(m_ucPrevKeysPressed[DIK_BACK] & 0x80) && m_szCommand.size() > 0)
		{
			m_szCommand.pop_back();
		}
	}
	else if(m_ucKeysPressed[DIK_RETURN] & 0x80)
	{
		CWorldManager::GetInstance()->CheckStack(CWorldManager::GetInstance()->GetluaState());
		if(!luaL_loadbuffer(CWorldManager::GetInstance()->GetluaState(), m_szCommand.c_str(), m_szCommand.length(), NULL))
		{
			CWorldManager::GetInstance()->CheckStack(CWorldManager::GetInstance()->GetluaState());
			lua_pcall(CWorldManager::GetInstance()->GetluaState(), 0, LUA_MULTRET, 0);
		}

		m_szPrevCommand = m_szCommand;
		m_szCommand.clear();
		CGame::GetInstance()->PopState();
		return true;
	}
	else if(m_ucKeysPressed[DIK_UP] & 0x80)
	{
		m_szCommand = m_szPrevCommand;
	}
	else if(m_ucKeysPressed[DIK_GRAVE] & 0x80)
	{
		// Ignore this key
	}
	else
	{
		//	Go through all keys on the keyboard
		for (unsigned int j = 0; j < 256; j++)
		{
			//	If the offset matches the key we are looking for and the high bit is on, it is pressed.
			if ((m_ucKeysPressed[j] & 0x80) && !(m_ucPrevKeysPressed[j] & 0x80))
			{
				//	Map the Scan Code from DirectInput to a Virtual Key value...
				uiVKCode = MapVirtualKeyEx(j, 1, m_keyLayout);
				//	...and translate that Virtual Key into an Ascii Value.
				ToAsciiEx(uiVKCode, j, m_ucAsciiVals, &usNum, 0, m_keyLayout);
			}
		}

		if(usNum)
		{
			m_szCommand += static_cast<char>(usNum);
		}
	}

	return true;
}

void CConsoleState::Update(float _dElapsedTime)
{
	CInput::GetInstance()->Disable();
	CView::Update(_dElapsedTime);
	GetPrevState()->Update(_dElapsedTime);
}

void CConsoleState::Render(void)
{
	GetPrevState()->Render();

	CView::Draw(m_tBlackBar.m_nImageID, 0, 0, float(CGame::GetInstance()->GetScreenWidth()) / m_tBlackBar.m_nImageWidth,
		1.0f);

	RECT CommandRect;
	CommandRect.left = 10;
	CommandRect.top = 10;
	CommandRect.right = 20;
	CommandRect.bottom = 20;
	CView::Write(m_nFontID, m_szCommand, &CommandRect, false, 0.25f);
}

void CConsoleState::Exit(void)
{
	CGame::GetInstance()->StartTimer();
	CInput::GetInstance()->Enable();
}