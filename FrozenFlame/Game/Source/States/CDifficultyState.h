/********************************************************************
* Filename:			CDifficultyState.h
* Date:				1/19/2012
* Mod. Date:		1/19/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the Difficulty state
********************************************************************/

#ifndef CDifficultyState_H
#define CDifficultyState_H

#include "../Globals.h"
#include "IGameState.h"

enum eDifficultyStateOptions{DSO_MIN, DSO_EASY, DSO_MEDIUM, DSO_HARD, DSO_BACK, DSO_MAX};

class CDifficultyState : public IGameState
{
private:
	int m_nSelected;
	int m_nFontID;

	TImage m_tBackground;
	TImage m_tButtonNormal;
	TImage m_tButtonHover;
	/********************************************************************
	* CDifficultyState():			The constructor for the Loss state
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
	explicit CDifficultyState(void)	{}

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Loss state
	*
	* Ins:					CDifficultyState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CDifficultyState(const CDifficultyState&);

	/********************************************************************
	* operator=():			The assignment operator for the Loss state
	*
	* Ins:					CDifficultyState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	CDifficultyState& operator=(const CDifficultyState&);

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
	~CDifficultyState(void) {}

	/********************************************************************
	* DrawButton():			Draws a button
	*
	* Ins:					string, bool
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/6/2012
	* Mod. Initials:		MB
	********************************************************************/
	void DrawButton(int _nMidX, int _nMidY, float _fScaleX, float _fScaleY, string _szText, bool _bHover, D3DXCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), float _fNumButtonWidthOffsets = 0, float _fNumButtonHeightOffsets = 0);

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
	static CDifficultyState* GetInstance()
	{
		static CDifficultyState instance;
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

#endif	// CDifficultyState_H