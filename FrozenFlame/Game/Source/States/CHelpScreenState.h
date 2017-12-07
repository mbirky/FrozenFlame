/////////////////////////////////////////////////////////////////////////////
//	File			:	"CHelpScreenState.h"
//
//	Author			:	Steven Seufert (SMS)
//			
//	Last Modified	:	January 18, 2013
//
//	Purpose			:	To declare all variables and functions for the in-game help screen
/////////////////////////////////////////////////////////////////////////////

#ifndef CHELP_SCREEN_STATE_H
#define CHELP_SCREEN_STATE_H

#include "../Globals.h"
#include "IGameState.h"

class CHelpScreenState : public IGameState
{
private:
	enum eHelpSelectedOptions{HSO_MIN, HSO_RESUME_GAME, HSO_CONTROLS, HSO_MAX };
	enum ePageEnums{HELP_PAGE_ONE, HELP_PAGE_TWO, HELP_PAGE_THREE, NUM_HELP_PAGES};

	int m_nSelected;
	int m_nCurrentPage;

	TImage m_tButtonNormal;
	TImage m_tButtonHover;
	TImage m_tPageLeft;
	TImage m_tPageRight;

	int m_nFontID;

	// buffer the timing of turning pages
	bool  m_bPageHasBeenTurned;			// true if the page was recently turned, false if the user can currently turn the page
	float m_fPageBufferTimer;			// How long it's been since the page was turned
	float m_fPageBufferDuration;		// how long until the page can be turned again
	CTimer m_helpScreenTimer;			// the in-games' timer needs to be paused, so help screen state gets its own timer for fElapsedTime

	/********************************************************************
	* CPauseState():		The constructor for the help screen state
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				void
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	explicit CHelpScreenState(void);

	/********************************************************************
	* CPauseState():		The copy constructor for the Help Screen state
	*
	* Ins:					CHelpScreenstate&
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	explicit CHelpScreenState(const CHelpScreenState&);

	/********************************************************************
	* operator=():			The assignment operator for the Help Screen state
	*
	* Ins:					CHelpScreenState&
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	CHelpScreenState& operator=(const CHelpScreenState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Help screen state
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	~CHelpScreenState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				CHelpScreenstate*
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	static CHelpScreenState* GetInstance()
	{
		static CHelpScreenState instance;
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
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void Enter(void);

	/********************************************************************
	* Input():				This is the function that handels input for
	*						this state
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				bool
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
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
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
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
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void Render(void);

	/********************************************************************
	* RenderPageOne():		displays everything that needs to be rendered 
	*						on the first page
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void RenderPageOne(void);

	/********************************************************************
	* RenderPageTwo():		displays everything that needs to be rendered 
	*						on the second page
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void RenderPageTwo(void);

	/********************************************************************
	* RenderPageThree():	displays everything that needs to be rendered 
	*						on the third page
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void RenderPageThree(void);

	/********************************************************************
	* RenderStatusBarHelp():	displays an explanation to the health and rage meters
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void RenderStatusBarHelp(void);

	/********************************************************************
	* RenderButtons():		displays the buttons on the help screen
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void RenderButtons(void);

	/********************************************************************
	* Exit():				This exits the state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			01/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void Exit(void);
};

#endif		// CHELP_SCREEN_STATE_H