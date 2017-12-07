/********************************************************************
* Filename:			CLossState.h
* Date:				11/6/2012
* Mod. Date:		11/6/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Loss state
********************************************************************/

#ifndef CLossSTATE_H
#define CLossSTATE_H

#include "../Globals.h"
#include "IGameState.h"

enum eLostStateOptions{ LSO_MIN, LSO_LAST_CKECKPOINT, LSO_MAIN_MENU, LSO_MAX };

class CLossState : public IGameState
{
private:

	char m_cFadeState;

	int m_nSelected;
	int m_nImageID;
	int m_nFontID;
	float m_fFade;

	/********************************************************************
	* CLossState():			The constructor for the Loss state
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
	explicit CLossState(void)	{}

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Loss state
	*
	* Ins:					CLossState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CLossState(const CLossState&);

	/********************************************************************
	* operator=():			The assignment operator for the Loss state
	*
	* Ins:					CLossState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CLossState& operator=(const CLossState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Loss state
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
	~CLossState(void) {}

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CLossState*
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CLossState* GetInstance()
	{
		static CLossState instance;
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

#endif	// CLossSTATE_H