/********************************************************************
* Filename:			CAchievementState.h
* Date:				1/24/2013
* Mod. Date:		1/24/2013
* Mod. Initials:	SW
* Author:			Sean Welch
* Purpose:			This is the Achievement state
********************************************************************/
#ifndef CACHIEVEMENTSTATE_H_
#define CACHIEVEMENTSTATE_H_
#include "../Globals.h"
#include "IGameState.h"

enum eAchievementOptions {ACV_BACK = 0, ACV_PREV, ACV_NEXT, ACV_DELETE, ACV_MAX};

class CAchievementState : public IGameState
{
	int m_nSelected;
	int m_nFontID;
	unsigned int m_nCurrentPage;
	unsigned int m_nMaxPages;

	TImage m_tBackground;
	TImage m_tButtonNormal;
	TImage m_tButtonHover;
	bool m_bPrevLeft;
	bool m_bPrevRight;

	/********************************************************************
	* CAchievementState():	The constructor for the Achievement state
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
	explicit CAchievementState(void)	{}

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Achievement state
	*
	* Ins:					CAchievementState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CAchievementState(const CAchievementState&);

	/********************************************************************
	* operator=():			The assignment operator for the Achievement state
	*
	* Ins:					CAchievementState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CAchievementState& operator=(const CAchievementState&);

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
	~CAchievementState(void) {}

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
	static CAchievementState* GetInstance()
	{
		static CAchievementState instance;
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

#endif /*CACHIEVEMENTSTATE_H_*/