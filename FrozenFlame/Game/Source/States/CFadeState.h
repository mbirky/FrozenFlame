/********************************************************************
* Filename:			CFadeState.h
* Date:				2/5/2013
* Mod. Date:		2/5/2013
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Console state
********************************************************************/

#ifndef CFadeState_H
#define CFadeState_H

#include "../Globals.h"
#include "IGameState.h"
#include "../Core/CWorldManager.h"

class CFadeState : public IGameState
{
private:
	bool m_bSectionFade;

	TImage m_tImage;
	float fScale;
	float m_fRot;
	bool m_bIsWizardLevel;

	CWorldManager *m_pWorldManager;

	/********************************************************************
	* CFadeState():		The constructor for the Win state
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
	explicit CFadeState(void) {};

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Win state
	*
	* Ins:					CFadeState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CFadeState(const CFadeState&);

	/********************************************************************
	* operator=():			The assignment operator for the Win state
	*
	* Ins:					CFadeState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CFadeState& operator=(const CFadeState&);

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
	~CFadeState(void) {};

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CFadeState*
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CFadeState* GetInstance()
	{
		static CFadeState instance;
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

#endif	// CFadeState_H