/***************************************************************
* Filename:			CInput.cpp
* Date:				10/27/2012
* Mod. Date:		11/20/2012
* Mod. Initials:	JM
* Author:			Matthew G. Birky
* Purpose:			This is a wrapper for all input
***************************************************************/

#include "CInput.h"


static const char* g_szOptionsLoc = "Resource/Options/";
static const int PlayerOne = 0;

CInput::CInput() : m_cMenuUp(0x57), m_cMenuDown(0x53), m_cMenuRight(0x44), m_cMenuLeft(0x41)
{
	m_cUp = m_cMenuUp;
	m_cDown = m_cMenuDown;
	m_cRight = m_cMenuRight;
	m_cLeft = m_cMenuLeft;
	m_cFireAttack = 0x4A;
	m_cIceAttack = 0x4B;
	m_cRage = 0x4C;
	m_cDash = VK_SPACE;
	m_bUsingController = false;
	m_bInputEnabled = true;
	m_bPlayerInputEnabled = true;

	m_sButtons[COMMAND_FIRE_ATTACK] = XINPUT_GAMEPAD_B;
	m_sButtons[COMMAND_ICE_ATTACK] = XINPUT_GAMEPAD_X;
	m_sButtons[COMMAND_RAGE] = XINPUT_GAMEPAD_Y;
	m_sButtons[COMMAND_DASH] = XINPUT_GAMEPAD_A;

	LoadInput("Controls");
	ZeroMemory(&m_xsControllerState, sizeof(XINPUT_STATE));

	ZeroMemory(&m_xvControllerVibration, sizeof(XINPUT_VIBRATION));
}

void CInput::Update()
{
	DWORD dwControllerResult;
	DWORD dwKeyResult;
	
	XINPUT_KEYSTROKE xiKeystroke;

	
	ZeroMemory(&xiKeystroke, sizeof(XINPUT_KEYSTROKE));
	ZeroMemory((void*)&m_sBitField, sizeof(m_sBitField));
	ZeroMemory((void*)&m_chCharacterInput, sizeof(m_chCharacterInput));

	// if input is not enabled, do nothing
	if(!m_bInputEnabled)
	{
		return;
	}

	// SMS: this has been upgraded to an int to accomodate additional commands
	static int sPrevPressed;
	//static bool bNoMove = false;

	dwControllerResult = XInputGetState(PlayerOne, &m_xsControllerState);
	dwKeyResult = XInputGetKeystroke(PlayerOne, XINPUT_FLAG_GAMEPAD, &xiKeystroke);
	if(m_bUsingController && dwControllerResult == ERROR_SUCCESS)
	{
		//if( dwControllerResult == ERROR_SUCCESS )
		{
			// Controller is connected.
			
			// -----------INSERT BUTTON CHECKS HERE----------

			if( (m_xsControllerState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_xsControllerState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(m_xsControllerState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_xsControllerState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
			{    
				m_xsControllerState.Gamepad.sThumbLX = 0;
				m_xsControllerState.Gamepad.sThumbLY = 0;
				//bNoMove = true;
			}

			float fLThumbXVal = m_xsControllerState.Gamepad.sThumbLX; // / 500000.0f;
			float fLThumbYVal = m_xsControllerState.Gamepad.sThumbLY; // / 500000.0f;

			// Move Left
			if(fLThumbXVal < -20000.0f || GetAsyncKeyState(m_cLeft) || GetAsyncKeyState(VK_LEFT))
				//if(xiKeystroke.VirtualKey & VK_PAD_LTHUMB_LEFT)
			{
				m_chCharacterInput |= IN_LEFT;
				// m_chCharacterInput |= IN_MENU_LEFT;

				if(!(sPrevPressed & IN_LEFT))
				{
					m_sBitField |= IN_LEFT;
					sPrevPressed |= IN_LEFT;
					//sPrevPressed &= ~IN_RIGHT;
				}

				if(!(sPrevPressed & IN_MENU_LEFT))
				{
					m_sBitField |= IN_MENU_LEFT;
					sPrevPressed |= IN_MENU_LEFT;

				}
			}
			else
			{
				sPrevPressed &= ~IN_LEFT;
				//m_sBitField &= ~IN_LEFT;
				sPrevPressed &= ~IN_MENU_LEFT;
			}

			// Move Right
			if(fLThumbXVal > 20000.0f || GetAsyncKeyState(m_cRight) || GetAsyncKeyState(VK_RIGHT))
				//if(xiKeystroke.VirtualKey & VK_PAD_LTHUMB_RIGHT)
			{
				m_chCharacterInput |= IN_RIGHT;
				//m_chCharacterInput |= IN_MENU_RIGHT;

				if(!(sPrevPressed & IN_RIGHT))
				{
					m_sBitField |= IN_RIGHT;
					sPrevPressed |= IN_RIGHT;

					//sPrevPressed &= ~IN_LEFT;
				}

				if(!(sPrevPressed & IN_MENU_RIGHT))
				{
					m_sBitField |= IN_MENU_RIGHT;
					sPrevPressed |= IN_MENU_RIGHT;
				}
			}
			else
			{
				sPrevPressed &= ~IN_RIGHT;
				//m_sBitField &= ~IN_RIGHT;
				sPrevPressed &= ~IN_MENU_RIGHT;
			}

			// if( (xiKeystroke.VirtualKey & VK_PAD_LTHUMB_RIGHT) != 0 && (xiKeystroke.Flags & XINPUT_KEYSTROKE_REPEAT) != 0)
			// {
			//	  m_chCharacterInput |= IN_RIGHT;
			//
			//	  m_sBitField |= IN_RIGHT;
			//	  sPrevPressed |= IN_RIGHT;
			// }

			// Move Down
			if(fLThumbYVal < -20000.0f || GetAsyncKeyState(m_cDown) || GetAsyncKeyState(VK_DOWN))
			{
				m_chCharacterInput |= IN_DOWN;

				if(!(sPrevPressed & IN_DOWN))
				{
					m_sBitField |= IN_DOWN;
					sPrevPressed |= IN_DOWN;

					//sPrevPressed &= ~IN_UP;
				}

				if(!(sPrevPressed & IN_MENU_DOWN))
				{
					m_sBitField |= IN_MENU_DOWN;
					sPrevPressed |= IN_MENU_DOWN;
				}
			}
			else
			{
				sPrevPressed &= ~IN_DOWN;
				//m_sBitField &= ~IN_DOWN;
				sPrevPressed &= ~IN_MENU_DOWN;
			}

			// Move Up
			if(fLThumbYVal > 20000.0f || GetAsyncKeyState(m_cUp) || GetAsyncKeyState(VK_UP))
			{
				m_chCharacterInput |= IN_UP;

				if(!(sPrevPressed & IN_UP))
				{
					m_sBitField |= IN_UP;
					sPrevPressed |= IN_UP;					

					//sPrevPressed &= ~IN_DOWN;
				}

				if(!(sPrevPressed & IN_MENU_UP))
				{
					m_sBitField |= IN_MENU_UP;
					sPrevPressed |= IN_MENU_UP;
				}
			}
			else
			{
				sPrevPressed &= ~IN_UP;
				//m_sBitField &= ~IN_UP;
				sPrevPressed &= ~IN_MENU_UP;					
			}

			// Check to make sure we are not moving during the dead zone

			//********************************** Controller-based Menu Navigation ******************************************************
			//if( (xiKeystroke.VirtualKey & VK_PAD_LTHUMB_UP) == VK_PAD_LTHUMB_UP && (xiKeystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN) )
			//{
			//	// m_chCharacterInput |= IN_MENU_UP;
			//	
			//	if(!(sPrevPressed & IN_MENU_UP))
			//	{
			//
			//		m_sBitField |= IN_MENU_UP;
			//		sPrevPressed |= IN_MENU_UP;
			//	}
			//}
			//
			//else
			//{
			//	sPrevPressed &= ~IN_MENU_UP;
			//}
			//
			//if( (xiKeystroke.VirtualKey & VK_PAD_LTHUMB_DOWN) == VK_PAD_LTHUMB_DOWN && (xiKeystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN) )
			//{
			//	// m_chCharacterInput |= IN_MENU_DOWN;
			//	
			//	if(!(sPrevPressed & IN_MENU_DOWN))
			//	{
			//
			//		m_sBitField |= IN_MENU_DOWN;
			//		sPrevPressed |= IN_MENU_DOWN;
			//	}
			//}
			//
			//else
			//{
			//	sPrevPressed &= ~IN_MENU_DOWN;
			//}

			//if( (xiKeystroke.VirtualKey & VK_PAD_LTHUMB_LEFT) == VK_PAD_LTHUMB_LEFT && (xiKeystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN) )
			//{
			//	 m_chCharacterInput |= IN_MENU_LEFT;
			//	
			//	if(!(sPrevPressed & IN_MENU_LEFT))
			//	{
			//
			//		m_sBitField |= IN_MENU_LEFT;
			//		sPrevPressed |= IN_MENU_LEFT;
			//	}
			//}
			//
			//else
			//{
			//	sPrevPressed &= ~IN_MENU_LEFT;
			//}
			//
			//if( (xiKeystroke.VirtualKey & VK_PAD_LTHUMB_RIGHT) == VK_PAD_LTHUMB_RIGHT && (xiKeystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN) )
			//{
			//	 m_chCharacterInput |= IN_MENU_RIGHT;
			//	
			//	if(!(sPrevPressed & IN_MENU_RIGHT))
			//	{
			//
			//		m_sBitField |= IN_MENU_RIGHT;
			//		sPrevPressed |= IN_MENU_RIGHT;
			//	}
			//}
			//
			//else
			//{
			//	sPrevPressed &= ~IN_MENU_RIGHT;
			//}
			//********************************** End Controller-based Menu Navigation ******************************************************

			if( (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) || (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) || GetAsyncKeyState(VK_RETURN))
			{
				if(!(sPrevPressed & IN_SELECT) )
				{
					m_sBitField |= IN_SELECT;
					sPrevPressed |= IN_SELECT;
				}
			}
			else
			{
				sPrevPressed &= ~IN_SELECT;

			}

			// Back
			if( (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) || GetAsyncKeyState(VK_ESCAPE))
			{
				if(!(sPrevPressed & IN_BACK))
				{
					m_sBitField |= IN_BACK;
					sPrevPressed |= IN_BACK;
				}
			}
			else
			{
				sPrevPressed &= ~IN_BACK;
			}

			if( (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) || GetAsyncKeyState(VK_ESCAPE))
			{
				if(!(sPrevPressed & IN_PAUSE))
				{
					m_sBitField |= IN_PAUSE;
					sPrevPressed |= IN_PAUSE;
				}
			}  
			else
			{
				sPrevPressed &= ~IN_PAUSE;
			}

			// Open the help screen
			if ( (m_xsControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) || GetAsyncKeyState(VK_F1))
			{
				if(!(sPrevPressed & IN_HELP))
				{
					m_sBitField |= IN_HELP;
					sPrevPressed |= IN_HELP;
				}
			}
			else
			{
				sPrevPressed &= ~IN_HELP;
			}


			if(m_xsControllerState.Gamepad.wButtons & m_sButtons[COMMAND_FIRE_ATTACK] || GetAsyncKeyState(m_cFireAttack))
			{
				m_chCharacterInput |= IN_FIRE_ATK;

				if(!(sPrevPressed & IN_FIRE_ATK))
				{
					m_sBitField |= IN_FIRE_ATK;
					sPrevPressed |= IN_FIRE_ATK;
				}
			}
			else
			{
				sPrevPressed &= ~IN_FIRE_ATK;
			}


			if(m_xsControllerState.Gamepad.wButtons & m_sButtons[COMMAND_ICE_ATTACK] || GetAsyncKeyState(m_cIceAttack))
			{
				m_chCharacterInput |= IN_ICE_ATK;

				if(!(sPrevPressed & IN_ICE_ATK))
				{
					m_sBitField |= IN_ICE_ATK;
					sPrevPressed |= IN_ICE_ATK;
				}
			} 
			else
			{
				sPrevPressed &= ~IN_ICE_ATK;
			}

			if(m_xsControllerState.Gamepad.wButtons & m_sButtons[COMMAND_RAGE] || (m_xsControllerState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) || GetAsyncKeyState(m_cRage))
			{
				m_chCharacterInput |= IN_RAGE;

				if(!(sPrevPressed & IN_RAGE))
				{
					m_sBitField |= IN_RAGE;
					sPrevPressed |= IN_RAGE;
				}
			}
			else
			{
				sPrevPressed &= ~IN_RAGE;
			}

			if(m_xsControllerState.Gamepad.wButtons & m_sButtons[COMMAND_DASH] || (m_xsControllerState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) || GetAsyncKeyState(m_cDash))
			{
				m_chCharacterInput |= IN_DASH;

				if(!(sPrevPressed & IN_DASH))
				{
					m_sBitField |= IN_DASH;
					sPrevPressed |= IN_DASH;
				}
				else
				{
					sPrevPressed &= ~IN_DASH;
				}
			} // end dash command
		} // end //if( dwControllerResult == ERROR_SUCCESS )
	} // end if(m_bUsingController && dwControllerResult == ERROR_SUCCESS)

// If the controller is not being used...
	else
	{
		// Up
		if(GetAsyncKeyState(m_cUp) || GetAsyncKeyState(VK_UP))
		{
			m_chCharacterInput |= IN_UP;

			if(!(sPrevPressed & IN_UP))
			{
				m_sBitField |= IN_UP;
				sPrevPressed |= IN_UP;
			}
		}
		else
		{
			sPrevPressed &= ~IN_UP;
		}

		// Down
		if(GetAsyncKeyState(m_cDown) || GetAsyncKeyState(VK_DOWN))
		{
			m_chCharacterInput |= IN_DOWN;

			if(!(sPrevPressed & IN_DOWN))
			{
				m_sBitField |= IN_DOWN;
				sPrevPressed |= IN_DOWN;
			}
		}
		else
		{
			sPrevPressed &= ~IN_DOWN;
		}

		// Left
		if(GetAsyncKeyState(m_cLeft) || GetAsyncKeyState(VK_LEFT))
		{
			m_chCharacterInput |= IN_LEFT;

			if(!(sPrevPressed & IN_LEFT))
			{
				m_sBitField |= IN_LEFT;
				sPrevPressed |= IN_LEFT;
			}
		}
		else
		{
			sPrevPressed &= ~IN_LEFT;
		}

		//Right
		if(GetAsyncKeyState(m_cRight) || GetAsyncKeyState(VK_RIGHT))
		{
			m_chCharacterInput |= IN_RIGHT;

			if(!(sPrevPressed & IN_RIGHT))
			{
				m_sBitField |= IN_RIGHT;
				sPrevPressed |= IN_RIGHT;
			}
		}
		else
		{
			sPrevPressed &= ~IN_RIGHT;
		}

		// Dash
		if(GetAsyncKeyState(m_cDash))
		{
			m_chCharacterInput |= IN_DASH;

			if(!(sPrevPressed & IN_DASH))
			{
				m_sBitField |= IN_DASH;
				sPrevPressed |= IN_DASH;
			}
		}
		else
		{
			sPrevPressed &= ~IN_DASH;
		}

		// Fire Attack
		if(GetAsyncKeyState(m_cFireAttack))
		{
			m_chCharacterInput |= IN_FIRE_ATK;

			if(!(sPrevPressed & IN_FIRE_ATK))
			{
				m_sBitField |= IN_FIRE_ATK;
				sPrevPressed |= IN_FIRE_ATK;
			}
		}
		else
		{
			sPrevPressed &= ~IN_FIRE_ATK;
		}

		// Ice Attack
		if(GetAsyncKeyState(m_cIceAttack))
		{
			m_chCharacterInput|= IN_ICE_ATK;
				
			if(!(sPrevPressed & IN_ICE_ATK))
			{
				m_sBitField |= IN_ICE_ATK;
				sPrevPressed |= IN_ICE_ATK;
			}
		}
		else
		{
			sPrevPressed &= ~IN_ICE_ATK;
		}

		// Rage
		if(GetAsyncKeyState(m_cRage))
		{
			m_chCharacterInput |= IN_RAGE;

			if(!(sPrevPressed & IN_RAGE))
			{
				m_sBitField |= IN_RAGE;
				sPrevPressed |= IN_RAGE;
			}
		}
		else
		{
			sPrevPressed &= ~IN_RAGE;
		}

		// Select
		if(GetAsyncKeyState(VK_RETURN))
		{
			if(!(sPrevPressed & IN_SELECT))
			{
				m_sBitField |= IN_SELECT;
				sPrevPressed |= IN_SELECT;
			}
		}
		else
		{
			sPrevPressed &= ~IN_SELECT;
		}

		// Back
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			if(!(sPrevPressed & IN_BACK))
			{
				m_sBitField |= IN_BACK;
				sPrevPressed |= IN_BACK;
			}
		}
		else
		{
			sPrevPressed &= ~IN_BACK;
		}

		// Pause
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			if(!(sPrevPressed & IN_PAUSE))
			{
				m_sBitField |= IN_PAUSE;
				sPrevPressed |= IN_PAUSE;
			}
		}
		else
		{
			sPrevPressed &= ~IN_PAUSE;
		}

		// Help screen
		if(GetAsyncKeyState(VK_F1))
		{
			if(!(sPrevPressed & IN_HELP))
			{
				m_sBitField |= IN_HELP;
				sPrevPressed |= IN_HELP;
			}
		}
		else
		{
			sPrevPressed &= ~IN_HELP;
		}

		// Alt + Q
		if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(0x51))
		{
			if(!(sPrevPressed & IN_QUIT))
			{
				m_sBitField |= IN_QUIT;
				sPrevPressed |= IN_QUIT;
			}
		}
		else
		{
			sPrevPressed &= ~IN_QUIT;
		}

		// Alt
		if(GetAsyncKeyState(VK_MENU))
		{
			m_sBitField |= IN_ALT;
			sPrevPressed |= IN_ALT;
		}
		else
		{
			sPrevPressed &= ~IN_ALT;
		}
		
//*********************************** Menu Navigation Controls **********************************************
		// Menu Up
		if(GetAsyncKeyState(m_cMenuUp) || GetAsyncKeyState(VK_UP))
		{
			//m_chCharacterInput |= IN_MENU_UP;

			if(!(sPrevPressed & IN_MENU_UP))
			{
				m_sBitField |= IN_MENU_UP;
				sPrevPressed |= IN_MENU_UP;
			}
		}
		else
		{
			sPrevPressed &= ~IN_MENU_UP;
		}

		// Menu Down
		if(GetAsyncKeyState(m_cMenuDown) || GetAsyncKeyState(VK_DOWN))
		{
			//m_chCharacterInput |= IN_MENU_DOWN;

			if(!(sPrevPressed & IN_MENU_DOWN))
			{
				m_sBitField |= IN_MENU_DOWN;
				sPrevPressed |= IN_MENU_DOWN;
			}
		}
		else
		{
			sPrevPressed &= ~IN_MENU_DOWN;
		}

		// Menu Left
		if(GetAsyncKeyState(m_cMenuLeft) || GetAsyncKeyState(VK_LEFT))
		{
			//m_chCharacterInput |= IN_MENU_LEFT;

			//if(!(sPrevPressed & IN_MENU_LEFT))
			//{
				m_sBitField |= IN_MENU_LEFT;
				sPrevPressed |= IN_MENU_LEFT;
			//}
		}
		else
		{
			sPrevPressed &= ~IN_MENU_LEFT;
		}

		// Menu Right
		if(GetAsyncKeyState(m_cMenuRight) || GetAsyncKeyState(VK_RIGHT))
		{
			//m_chCharacterInput |= IN_MENU_RIGHT;

			//if(!(sPrevPressed & IN_MENU_RIGHT))
			//{
				m_sBitField |= IN_MENU_RIGHT;
				sPrevPressed |= IN_MENU_RIGHT;
			//}
		}
		else
		{
			sPrevPressed &= ~IN_MENU_RIGHT;
		}
	}
//*********************************** End Menu Navigation Controls ******************************************
}

bool CInput::Up(void)
{
	if(m_sBitField & IN_UP)
		return true;
	return false;
}

bool CInput::Menu_Up(void)
{
	if(m_sBitField & IN_MENU_UP)
		return true;
	return false;
}

bool CInput::Down(void)
{
	if(m_sBitField & IN_DOWN)
		return true;
	return false;
}

bool CInput::Menu_Down(void)
{
	if(m_sBitField & IN_MENU_DOWN)
		return true;
	return false;
}

bool CInput::Left(void)
{
	if(m_sBitField & IN_LEFT)
		return true;
	return false;
}

bool CInput::Menu_Left(void)
{
	if(m_sBitField & IN_MENU_LEFT)
		return true;
	return false;
}

bool CInput::Right(void)
{
	if(m_sBitField & IN_RIGHT)
		return true;
	return false;
}

bool CInput::Menu_Right(void)
{
	if(m_sBitField & IN_MENU_RIGHT)
		return true;
	return false;
}

bool CInput::Select(void)
{
	if((m_sBitField & IN_SELECT) && !(m_sBitField & IN_ALT))
		return true;
	return false;
}

bool CInput::Back(void)
{
	if(m_sBitField & IN_BACK)
		return true;
	return false;
}

bool CInput::Pause(void)
{
	if(m_sBitField & IN_PAUSE)
		return true;
	return false;
}

bool CInput::Help(void)
{
	if(m_sBitField & IN_HELP)
		return true;
	return false;
}

bool CInput::Quit(void)
{
	if(m_sBitField & IN_QUIT)
		return true;
	return false;
}

bool CInput::AltSelect(void)
{
	if((m_sBitField & IN_SELECT) && (m_sBitField & IN_ALT))
		return true;
	return false;
}

char CInput::GetCharacterKeys(void)
{
	if(m_bPlayerInputEnabled)
		return m_chCharacterInput;
	else
		return 0;
}

void CInput::LoadInput(string szFilename)
{
	char cCommands[8];

	
	if(szFilename.empty())
		return;

	string szFilePath = g_szOptionsLoc;//szFilename;

	szFilePath.append(szFilename);
	//szFilePath.append(".option");
	szFilePath.append(".txt");

	string szOpenPath = "Resource/Options/Test.txt";

	fstream in;

	// TODO: Open the file for loading
	in.open(szOpenPath.c_str(), ios_base::in/* | ios_base::binary*/);

	if(!in.is_open())
	{
		return;
	}

	//string szLoadout = m_cCommands;

	//while(!in.eof())
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
	
	in.read((char*)&(m_bUsingController),1);


	// TODO: Set the gameplay input variables to the new values
	for(int i = 0; i < 4; ++i)
	{
		//string szLoadout = m_cCommands;
		in.read((char*)&(m_sButtons[i]), sizeof(WORD));
		//in >> m_sButtons[i];
		//in.ignore(INT_MAX, '\n');
	}
	


	// TODO: Close the file
	in.close();
	in.clear();

	//for(size_t i = 0; i < strlen(szLoadout.c_str()); ++i)
	//{
	//	m_cCommands[i] = szLoadout[i];
	//}

	//if(!szLoadout.empty())
	{
		m_cUp = cCommands[0];
		m_cDown = cCommands[1];
		m_cLeft = cCommands[2];
		m_cRight = cCommands[3];
		m_cFireAttack = cCommands[4];
		m_cIceAttack = cCommands[5];
		m_cRage = cCommands[6];
		m_cDash = cCommands[7];
	}
}

/********************************************************************
* SetLeftMotorSpeed():	Sets the motor speed levels for the vibration 
*						function of a controller.
*
* Ins:					motorSpeed 0 to 65,535; 0 is no vibration,
*						65,535 is max vibration
*
* Outs:					none
*
* Returns:				none
*
* Mod. Date:			01/18/2013
* Mod. Initials:		SMS
*
* Notes:				The left motor is the low-frequency rumble motor. 
*						The right motor is the high-frequency rumble motor. 
*						The two motors are not the same, and they create 
*						different vibration effects.
********************************************************************/
void CInput::SetLeftMotorSpeed(unsigned int motorSpeed)
{
	if (motorSpeed < 0)
		motorSpeed = 0;
	else if (motorSpeed > 65535)
		motorSpeed = 65535;

	m_nLeftMotorSpeed = motorSpeed;

	m_xvControllerVibration.wLeftMotorSpeed = motorSpeed;
	XInputSetState(0, &m_xvControllerVibration);
}

/* SetRightMotorSpeed():	Sets the motor speed levels for the vibration 
*						function of a controller.
*
* Ins:					motorSpeed 0 to 65,535; 0 is no vibration,
*						65,535 is max vibration
*
* Outs:					none
*
* Returns:				none
*
* Mod. Date:			01/18/2013
* Mod. Initials:		SMS
*
* Notes:				The left motor is the low-frequency rumble motor. 
*						The right motor is the high-frequency rumble motor. 
*						The two motors are not the same, and they create 
*						different vibration effects.
********************************************************************/
void CInput::SetRightMotorSpeed(unsigned int motorSpeed)
{
	if (motorSpeed < 0)
		motorSpeed = 0;
	else if (motorSpeed > 65535)
		motorSpeed = 65535;

	m_nRightMotorSpeed = motorSpeed;

	m_xvControllerVibration.wRightMotorSpeed = motorSpeed;
	XInputSetState(0, &m_xvControllerVibration);
}