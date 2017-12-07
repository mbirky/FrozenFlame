/********************************************************************
* Filename:			CConsoleState.h
* Date:				1/8/2012
* Mod. Date:		1/8/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Console state
********************************************************************/

#ifndef CConsoleState_H
#define CConsoleState_H

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
};

#include "../Globals.h"
#include "IGameState.h"
#include <dinput.h>
#pragma comment (lib, "dinput8")


class CConsoleState : public IGameState
{
private:
	int m_nFontID;
	string m_szCommand;
	string m_szPrevCommand;
	TImage m_tBlackBar;

	IDirectInput8*			m_pDInput;
	IDirectInputDevice8*	m_pKeyboard;

	unsigned char		m_ucKeysPressed[256];			//	Holds Keyboard Data.
	unsigned char		m_ucPrevKeysPressed[256];		//	Used for BufferedKeyEx.

	unsigned char		m_ucAsciiVals[256];		//	For CheckKeys and CheckBufferedKeys.
	HKL					m_keyLayout;			//	For CheckKeys and CheckBufferedKeys.

	/********************************************************************
	* CConsoleState():		The constructor for the Win state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CConsoleState(void) {};

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Win state
	*
	* Ins:					CConsoleState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CConsoleState(const CConsoleState&);

	/********************************************************************
	* operator=():			The assignment operator for the Win state
	*
	* Ins:					CConsoleState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CConsoleState& operator=(const CConsoleState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Win state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	~CConsoleState(void) {};

	bool NextIndicie(unsigned int& _nIndex, string _szInput);
	bool GetString(string& _szOut, unsigned int& _nIndex, string _szInput);
	bool GetDouble(double& _dOut, unsigned int& _nIndex, string _szInput);
	void ParseString(string _szInput);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CConsoleState*
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CConsoleState* GetInstance()
	{
		static CConsoleState instance;
		return &instance;
	}

	/********************************************************************
	* Enter():				This is a function that enters into the
	*						state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Enter(void);

	/********************************************************************
	* Input():				This is the function that handels input for
	*						this state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	bool Input(void);

	/********************************************************************
	* Update():				This is the function that handels update for
	*						this state
	*
	* Ins:					double
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Update(float _dElapsedTime);

	/********************************************************************
	* Render():				This is the function that handels render for
	*						this state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Render(void);

	/********************************************************************
	* Exit():				This exits the state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Exit(void);
};

#endif	// CConsoleState_H