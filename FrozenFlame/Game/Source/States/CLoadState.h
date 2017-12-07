/********************************************************************
* Filename:			CLoadState.h
* Date:				1/24/2013
* Mod. Date:		1/24/2013
* Mod. Initials:	SW
* Author:			Sean Welch
* Purpose:			This is the Achievement state
********************************************************************/
#ifndef CLOADSTATE_H_
#define CLOADSTATE_H_
#include "../Globals.h"
#include "IGameState.h"
//enum eAchievementOptions {ACV_BACK = 0, ACV_PREV, ACV_NEXT, ACV_DELETE, ACV_MAX};

class CMenuModelObject;

class CLoadState : public IGameState
{
	CMenuModelObject*	m_pMenuPlayer;
	int					m_nFireSwordID;
	int					m_nIceSwordID;
	
	CMenuModelObject*	m_pMenuWizard;
	int					m_nWizardStaffID;

	int					m_nBackgroundID;

	int m_nFontID;


	/********************************************************************
	* CLoadState():	The constructor for the Achievement state
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
	explicit CLoadState(void)	{}

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Achievement state
	*
	* Ins:					CLoadState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CLoadState(const CLoadState&);

	/********************************************************************
	* operator=():			The assignment operator for the Achievement state
	*
	* Ins:					CLoadState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CLoadState& operator=(const CLoadState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Achievement state
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
	~CLoadState(void) {}

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CDifficultyState*
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CLoadState* GetInstance()
	{
		static CLoadState instance;
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

#endif		//CLOADSTATE_H_