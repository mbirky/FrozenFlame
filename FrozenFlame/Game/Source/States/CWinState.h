/********************************************************************
* Filename:			CWinState.h
* Date:				11/6/2012
* Mod. Date:		11/6/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Win state
********************************************************************/

#ifndef CWINSTATE_H
#define CWINSTATE_H

#include "../Globals.h"
#include "IGameState.h"

class CMenuModelObject;
class CWinState : public IGameState
{
	CMenuModelObject*	m_pMenuPortal;
	CMenuModelObject*	m_pMenuPlayer;
	int					m_nFireSwordID;
	int					m_nIceSwordID;
	float				m_fTimer;

private:
	int m_nFontID;
	TImage m_tBackGround;
	TImage m_tDarken;

	/********************************************************************
	* CWinState():			The constructor for the Win state
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
	explicit CWinState(void)	{}

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Win state
	*
	* Ins:					CWinState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CWinState(const CWinState&);

	/********************************************************************
	* operator=():			The assignment operator for the Win state
	*
	* Ins:					CWinState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CWinState& operator=(const CWinState&);

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
	~CWinState(void) {}

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CWinState*
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CWinState* GetInstance()
	{
		static CWinState instance;
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

#endif	// CWINSTATE_H