/********************************************************************
* Filename:			COptionsMenuState.h
* Date:				11/17/2012
* Mod. Date:		11/17/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the main option menu state
********************************************************************/
#ifndef _COPTIONSMENUSTATE_H_
#define _COPTIONSMENUSTATE_H_

#include "../Globals.h"
#include "IGameState.h"
#include "../Util/CInput.h"

enum eOptions{OPTION_MIN, OPTION_SOUND, OPTION_VIDEO, OPTION_CONTROLS, OPTION_EXIT, OPTION_MAX};

class COptionsMenuState : public IGameState
{
	int m_nSelected;
	int m_nFontID;

	TImage m_tBackground;
	TImage m_tButtonNormal;
	TImage m_tButtonHover;

	/********************************************************************
	* COptionsMenuState():		The constructor for the Option menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit COptionsMenuState(void);

	/********************************************************************
	* COptionsMenuState():		The copy constructor for the Option menu state
	*
	* Ins:					COptionsMenuState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit COptionsMenuState(const COptionsMenuState&);

	/********************************************************************
	* operator=():			The assignment operator for the Main menu state
	*
	* Ins:					COptionsMenuState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	COptionsMenuState& operator=(const COptionsMenuState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Option menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	~COptionsMenuState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static COptionsMenuState*
	*
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	static COptionsMenuState* GetInstance()
	{
		static COptionsMenuState instance;
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
	* Mod. Date:			11/17/2012
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
	* Mod. Date:			11/17/2012
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
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Update(float _fElapsedTime);

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
	* Mod. Date:			11/17/2012
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
	* Mod. Date:			11/17/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Exit(void);
};

#endif