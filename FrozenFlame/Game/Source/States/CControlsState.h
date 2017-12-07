/********************************************************************
* Filename:			CControlsState.h
* Date:				11/22/2012
* Mod. Date:		11/22/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the option menu state for game controls
********************************************************************/

#ifndef _CCONTROLSSTATE_H_
#define _CCONTROLSSTATE_H_

#include "../Globals.h"
#include "IGameState.h"
#include "../Util/CInput.h"
#include <fstream>

enum eControlOptions{CONTROL_MIN, CONTROL_SELECT, CONTROL_UP, CONTROL_DOWN, CONTROL_LEFT, CONTROL_RIGHT, CONTROL_FIREATTACK, CONTROL_ICEATTACK, CONTROL_RAGE, CONTROL_DASH, CONTROL_EXIT, CONTROL_DEFAULT, CONTROL_SAVE, CONTROL_MAX, CONTROLLER_MIN, \
					 CONTROLLER_SELECT, CONTROLLER_FIRE, CONTROLLER_ICE, CONTROLLER_RAGE, CONTROLLER_DASH, CONTROLLER_ACTIVE, CONTROLLER_EXIT, CONTROLLER_MAX};

class CControlsState : public IGameState
{
	int m_nSelected;

	TImage m_tBackground;
	TImage m_tButtonNormal;
	TImage m_tButtonHover;

	int m_nFontID;

	char m_cMoveUp;
	char m_cMoveDown;
	char m_cMoveLeft;
	char m_cMoveRight;
	char m_cFireAttack;
	char m_cIceAttack;
	char m_cRage;
	char m_cDash;

	char m_cCommands[8];
	WORD m_sButtons[4];
	char m_cTempControl;
	WORD m_sTempButton;
	int m_nIndexCheck;
	float m_fIncrementTimer;

	bool m_bChanging;
	bool m_bUseController;
	bool m_bKeyboardMenu;

	DWORD m_dwControllerResult;
	XINPUT_STATE m_xsControllerState;
	CTimer m_tTimer;			// the in-games' timer needs to be paused, so each state needs its own timer for fElapsedTime


	/********************************************************************
	* CControlsMenuState():		The constructor for the Control menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CControlsState(void);

	/********************************************************************
	* CControlsMenuState():		The copy constructor for the Control menu state
	*
	* Ins:					CControlsState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CControlsState(const CControlsState&);

	/********************************************************************
	* operator=():			The assignment operator for the Control menu state
	*
	* Ins:					CControlsState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	CControlsState& operator=(const CControlsState&);

	/********************************************************************
	* ~CControlsState():	The deconstructor for the Contol menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	~CControlsState(void);

	// Takes in a character by reference in order to change the key value on the keyboard
	void ChangeKey(char& input);

	// This function takes in a short by reference in order to change the button value on the controller
	void ChangeButton(WORD& input);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static CControlsMenuState*
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	static CControlsState* GetInstance()
	{
		static CControlsState instance;
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
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
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
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	bool Input(void);

	/********************************************************************
	* Update():				This is the function that handles update for
	*						this state
	*
	* Ins:					float
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Update(float _fElapsedTime);

	/********************************************************************
	* Render():				This is the function that handles render for
	*						this state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
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
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Exit(void);

	/********************************************************************
	* LoadInput():		Checks to see if there is a pre-set input file and
	*					then changes the gameplay input to these presets
	*					if they exist.
	*
	* Ins:				const char*
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
	* SaveInput():		Checks to see if there is an input file and
	*					then opens it if it exists, otherwise it creates
	*					a new one and saves out all current input data.
	*
	* Ins:				const char*
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/22/2012
	* Mod. Initials:	JM
	********************************************************************/
	void SaveInput(string szFilename);

	/********************************************************************
	* Render():				This is the function that handles render for
	*						this state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/20/2012
	* Mod. Initials:		JM
	********************************************************************/
	void RenderBox(void);

	void RestoreDefaults();

	
};
#endif // CCONTROLSSTATE_H_