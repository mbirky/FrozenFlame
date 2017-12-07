/********************************************************************
* Filename:			CInput.h
* Date:				10/27/2012
* Mod. Date:		11/20/2012
* Mod. Initials:	JM
* Author:			Matthew G. Birky
* Purpose:			This is a wrapper for all input
********************************************************************/

#ifndef CINPUT_H
#define CINPUT_H

#include <Windows.h>
#include <fstream>
#include "../Globals.h"
#include <XInput.h>

#pragma comment(lib, "XInput.lib")

enum eInputBitMask		{IN_UP = 1, IN_DOWN = 2, IN_LEFT = 4, IN_RIGHT = 8, IN_DASH = 16, IN_FIRE_ATK = 32, IN_ICE_ATK = 64, IN_RAGE = 128, IN_SELECT = 256, IN_BACK = 512, IN_PAUSE = 1024, IN_QUIT = 2048, IN_MENU_UP = 4096 \
						, IN_MENU_DOWN = 8192, IN_MENU_LEFT = 16384, IN_MENU_RIGHT = 32768, IN_HELP = 65536, IN_ALT = 131072 };

class CInput
{
private:
	int m_sBitField;
	char m_chCharacterInput;

	XINPUT_STATE m_xsControllerState;
	XINPUT_VIBRATION m_xvControllerVibration;

	// Menu Navigation Commands - These will NEVER be able to be changed
	const char m_cMenuUp;
	const char m_cMenuDown;
	const char m_cMenuRight;
	const char m_cMenuLeft;

	// Gameplay Controls - These will be able to be changed via the controls options menu
	char m_cUp;
	char m_cDown;
	char m_cRight;
	char m_cLeft;
	char m_cFireAttack;
	char m_cIceAttack;
	char m_cRage;
	char m_cDash;
	bool m_bUsingController;

	bool m_bInputEnabled;
	bool m_bPlayerInputEnabled;

	WORD m_sButtons[4];

	// rumble motors
	unsigned int m_nLeftMotorSpeed;
	unsigned int m_nRightMotorSpeed;

	/********************************************************************
	* CInput():			The constructor of the Input Class
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/1/2012
	* Mod. Initials:	MB
	********************************************************************/
	CInput();

	/********************************************************************
	* CInput():			The Copy constructor of the CInput class
	*
	* Ins:				const CInput&
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/1/2012
	* Mod. Initials:	MB
	********************************************************************/
	CInput(const CInput&);

	/********************************************************************
	* operator=:		The assignement operator for CInput
	*
	* Ins:				const CInput&
	*
	* Outs:				void
	*
	* Returns:			CInput&
	*
	* Mod. Date:		11/1/2012
	* Mod. Initials:	MB
	********************************************************************/
	CInput& operator=(const CInput&);

	/********************************************************************
	* GetInstance():	The destructor for the CInput class
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/1/2012
	* Mod. Initials:	MB
	********************************************************************/
	~CInput()	{}

public:
	/********************************************************************
	* GetInstance():	Gets an instance of the class
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			CInput*
	*
	* Mod. Date:		11/1/2012
	* Mod. Initials:	MB
	********************************************************************/
	static CInput* GetInstance(void)
	{
		static CInput instance;
		return &instance;
	}

	/********************************************************************
	* Update():			Sets the bit field with all the currently pressed
	*					keys
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	void Update(void);

	/********************************************************************
	* Up():				Checks to see if there is an up signal for
						gameplay navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Up(void);

	/********************************************************************
	* Menu_Up():		Checks to see if there is an up signal for menu
	*					navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		11/20/2012
	* Mod. Initials:	JM
	********************************************************************/
	bool Menu_Up(void);

	/********************************************************************
	* Down():			Checks to see if there is a down signal for
	*					gameplay navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Down(void);

	/********************************************************************
	* Menu_Down():		Checks to see if there is a down signal for menu
	*					navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		11/20/2012
	* Mod. Initials:	JM
	********************************************************************/
	bool Menu_Down(void);

	/********************************************************************
	* Left():			Checks to see if there is a left signal for
	*					gameplay navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Left(void);

	/********************************************************************
	* Menu_Left():		Checks to see if there is a left signal for menu
	*					navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		11/20/2012
	* Mod. Initials:	JM
	********************************************************************/
	bool Menu_Left(void);

	/********************************************************************
	* Right():			Checks to see if there is a right signal for
	*					gameplay navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Right(void);

	/********************************************************************
	* Menu_Right():		Checks to see if there is a right signal for menu
	*					navigation currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		11/20/2012
	* Mod. Initials:	JM
	********************************************************************/
	bool Menu_Right(void);

	/********************************************************************
	* Select():			Checks to see if there is a select signal
	*					currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Select(void);

	/********************************************************************
	* Back():			Checks to see if there is a back signal currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Back(void);

	/********************************************************************
	* Pause():			Checks to see if there is a pause signal currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		10/29/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Pause(void);

	/********************************************************************
	* Help():			Checks to see if there is a help signal currently
	*
	* Ins:				none
	*
	* Outs:				none
	*
	* Returns:			true if the input for showing the help screen has
	*					been toggled, false if not
	*
	* Mod. Date:		01/18/2013
	* Mod. Initials:	SMS
	********************************************************************/
	bool Help(void);

	/********************************************************************
	* Quit():			Checks to see if there is a quit signal currently
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		11/1/2012
	* Mod. Initials:	MB
	********************************************************************/
	bool Quit(void);

	/********************************************************************
	* Quit():			Checks to see if the ALT and the Select keys are down
	*
	* Ins:				void
	*
	* Outs:				void
	*
	* Returns:			bool
	*
	* Mod. Date:		2/4/2013
	* Mod. Initials:	DL
	********************************************************************/
	bool AltSelect(void);

	/********************************************************************
	* LoadInput():		Checks to see if there is a pre-set input file and
	*					then changes the gameplay input to these presets
	*					if they exist.
	*
	* Ins:				std::string
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/20/2012
	* Mod. Initials:	JM
	********************************************************************/
	void LoadInput(string szFilename);

	/********************************************************************
	* GetCharacterKeys():	Checks all the keys that affect the character
	*						and turns on the appropriate bit in the 
	*						return Char
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:			10/29/2012
	* Mod. Initials:		MB
	********************************************************************/
	char GetCharacterKeys(void);
	
	/********************************************************************
	* GetButton():			Returns a button value depending on the passed
	*						in value
	*
	* Ins:					int
	*
	* Outs:					void
	*
	* Returns:				WORD
	*
	* Mod. Date:			11/29/2012
	* Mod. Initials:		JM
	********************************************************************/
	WORD GetButton(int nIndex) { return m_sButtons[nIndex];}

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
	void SetLeftMotorSpeed(unsigned int motorSpeed);
	int  GetLeftMotorSpeed() const					{ return m_nLeftMotorSpeed; }

	/********************************************************************
	* SetRightMotorSpeed():	Sets the motor speed levels for the vibration 
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
	void SetRightMotorSpeed(unsigned int motorSpeed);
	int  GetRightMotorSpeed() const					{ return m_nRightMotorSpeed; }

	void Enable(void) { m_bInputEnabled = true; }
	void Disable(void) { m_bInputEnabled = false; }

	void EnablePlayerInput(void) { m_bPlayerInputEnabled = true; }
	void DisablePlayerInput(void) { m_bPlayerInputEnabled = false; }

	bool GetIfUsingController() const		{ return m_bUsingController; }
	inline bool GetIsInputEnabled()const	{ return m_bPlayerInputEnabled == true; }

	char GetMoveUpKey(void)		{return m_cUp;}
	char GetMoveDownKey(void)	{return m_cDown;}
	char GetMoveLeftKey(void)	{return m_cLeft;}
	char GetMoveRightKey(void)	{return m_cRight;}
	char GetFireAtkKey(void)	{return m_cFireAttack;}
	char GetIceAtkKey(void)		{return m_cIceAttack;}
	char GetRageKey(void)		{return m_cRage;}
	char GetDashKey(void)		{return m_cDash;}

	void Clear(void) {ZeroMemory((void*)&m_sBitField,sizeof(m_sBitField)); ZeroMemory((void*)&m_chCharacterInput, sizeof(m_chCharacterInput));}
};

#endif