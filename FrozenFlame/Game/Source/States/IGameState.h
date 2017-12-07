/********************************************************************
* Filename:			IGameState.h
* Date:				10/31/2012
* Mod. Date:		11/12/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the interface for all the states in the
*					game
********************************************************************/

#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include "../Util/CInput.h"
#include "../Sound/CSoundManager.h"

class IGameState
{
private:
	IGameState *m_pPrevState;

protected:
	CInput *m_pInput;
	CSoundManager *m_pSound;

	/********************************************************************
	* IGameState():			This is the constructor for IGameState.  It
	*						sets the prevstate pointer to NULL
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit IGameState(void)
	{
		m_pPrevState = NULL;
		m_pInput = CInput::GetInstance();
		m_pSound = CSoundManager::GetInstance();
	}

	/********************************************************************
	* CMainMenuState():		The copy constructor for the IGameState
	*						Interface class
	*
	* Ins:					IGameState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/12/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit IGameState(const IGameState&);

	/********************************************************************
	* operator=():			The assignment operator for the IGameState
	*
	* Ins:					IGameState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/12/2012
	* Mod. Initials:		MB
	********************************************************************/
	IGameState& operator=(const IGameState&);

	/********************************************************************
	* ~IGameState():		This is a pure virtual deconstructor to make this
	*						class an abstract base class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	virtual ~IGameState() = 0 {};

public:
	/********************************************************************
	* Enter():				This is a virtual function that enters into
	*						the state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	virtual void Enter(void) = 0;

	/********************************************************************
	* Input():				This is a virtual function that handels input
	*						for the state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	virtual bool Input(void) = 0;

	/********************************************************************
	* Update():				This is a virtual update for the state
	*
	* Ins:					double
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	virtual void Update(float _fElapsedTime) = 0;

	/********************************************************************
	* Render():				This is a virtual function that calls the
	*						view class for rendering things
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	virtual void Render(void) = 0;

	/********************************************************************
	* Exit():				This is a virtual function that exits the
	*						state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	virtual void Exit(void) = 0;

	/***************
	*  Accessors
	***************/
	IGameState* GetPrevState(void)	{ return m_pPrevState; }

	/***************
	* Mutators
	***************/
	void SetPrevState(IGameState* _pGameState)	{ m_pPrevState = _pGameState; }
};

#endif		// IGAMESTATE_H