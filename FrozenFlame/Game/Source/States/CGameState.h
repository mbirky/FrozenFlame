/********************************************************************
* Filename:			CGameState.h
* Date:				10/31/2012
* Mod. Date:		11/13/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the main game state
********************************************************************/

#ifndef CGAMESTATE_H
#define CGAMESTATE_H

#include "IGameState.h"
#include "../Util/CInput.h"
#include "../Objects/PlayerObject.h"
#include "../Objects/ObjectManager.h"
#include "../AI/Header/CBehaviorManager.h"
#include "../Objects/CIceTrap.h"
#include "../Objects/CFireTrap.h"

class CGameState : public IGameState
{
	//bool m_bRenderDamage;
	int m_nBackground;
	int m_nBorderID;
	int m_nFontID;
	int m_nRageButtonID;
	int m_nLoadScreenID;
	float m_fFlashTimer;
	float m_fFlashRate;
	float m_fDisplayTime;
	float m_fZoomTimer;		// how long the camera has been zooming
	float m_fZoomDistance;  // how far to zoom in when entering a portal
	float m_fZoomDuration;	// how long to zoom the camera before switching to portal fade

	TImage m_tBorder;
	TImage m_tHitBorder;
	TImage m_tEmptyBar;
	TImage m_tRageBar;
	TImage m_tHealthBar;
	TImage m_tGolieDot;
	TImage m_tPressEnter;
	WORD m_wRageButton;

	bool m_bWaiting;
	bool m_bDisplayMessage;
	bool m_bZoomCamera;			// to zoom or not to zoom
	bool m_bCameraIsZooming;	// whether the camera is currently zooming


private:
	/********************************************************************
	* CGameState():			The constructor for the Game state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CGameState(void);

	/********************************************************************
	* CGameState():			The copy constructor for the Game state
	*
	* Ins:					CGameState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CGameState(const CGameState&);

	/********************************************************************
	* operator=():			The assignment operator for the Game state
	*
	* Ins:					CGameState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	CGameState& operator=(const CGameState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Game state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	~CGameState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CGameState*
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CGameState* GetInstance()
	{
		static CGameState instance;
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
	* Mod. Date:			10/31/2012
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
	* Mod. Date:			10/31/2012
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
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Exit(void);

	void Reset();

	//void SetDisplayDamage(bool IsOn)	{m_bRenderDamage = IsOn;}
	void  SetPortalZoomOn(bool turnOn)	{ m_bZoomCamera = turnOn; }


	float GetPortalZoomDistance() const { return m_fZoomDistance; }
	float GetPortalZoomDuration() const { return m_fZoomDuration; }
};

#endif	// CGAMESTATE_H