/********************************************************************
* Filename:			CPauseState.h
* Date:				11/4/2012
* Mod. Date:		12/8/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the pause Game state
********************************************************************/

#ifndef CPAUSESTATE_H
#define CPAUSESTATE_H

#include "../Globals.h"
#include "IGameState.h"

class CPauseState : public IGameState
{
private:
	enum ePauseSelectedOptions{PSO_MIN, PSO_RESUME_GAME, PSO_LAST_CHECKPOINT, PSO_OPTIONS, PSO_MAIN_MENUE, PSO_MAX };

	int m_nSelected;

	bool m_bConfirmation;
	bool m_bExit;
	CTimer m_tUpdateTime;
	float m_fChangeTimer;

	int m_nFontID;

	/********************************************************************
	* CPauseState():		The constructor for the Pause state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/4/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CPauseState(void);

	/********************************************************************
	* CPauseState():		The copy constructor for the Pause state
	*
	* Ins:					CPauseState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/4/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CPauseState(const CPauseState&);

	/********************************************************************
	* operator=():			The assignment operator for the Pause state
	*
	* Ins:					CPauseState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/4/2012
	* Mod. Initials:		MB
	********************************************************************/
	CPauseState& operator=(const CPauseState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Pause state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/4/2012
	* Mod. Initials:		MB
	********************************************************************/
	~CPauseState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CPauseState*
	*
	* Mod. Date:			11/4/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CPauseState* GetInstance()
	{
		static CPauseState instance;
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
	* Mod. Date:			11/14/2012
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
	* Mod. Date:			11/4/2012
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
	* Mod. Date:			11/4/2012
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
	* Mod. Date:			11/5/2012
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
	* Mod. Date:			11/14/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Exit(void);
};

#endif		// CPAUSESTATE_H