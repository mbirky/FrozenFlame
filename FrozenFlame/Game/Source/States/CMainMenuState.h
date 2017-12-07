/********************************************************************
* Filename:			CMainMenuState.h
* Date:				10/31/2012
* Mod. Date:		11/5/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is the main game state
********************************************************************/

#ifndef CMAINMENUSTATE_H
#define CMAINMENUSTATE_H

#include "../Globals.h"
#include "IGameState.h"
#include "../Util/CInput.h"

enum eSelcetedOption{SO_MIN, SO_START_GAME, SO_DIFFICULTY, SO_OPTIONS, SO_ACHIEVEMENTS, SO_CREDITS, SO_EXIT, SO_MAX};

enum SplashScreens {SPLASH_GPGAMES, SPLASH_BITBOSSES, SPLASH_8BITBACON, SPLASH_BITSHIFT, SPLASH_DIRECTX, SPLASH_WWISE, SPLASH_TOTAL};

class CMainMenuState : public IGameState
{
private:

	struct tSplashScreen
	{
		float m_fCurrentLife;
		float m_fLifeTime;
		int m_nTextureID;
		int width;
		int height;
		D3DXCOLOR m_cColor;
		D3DXCOLOR m_cStartColor;
		D3DXCOLOR m_cEndColor;
	};

	bool m_bIsSplashing;
	
	int m_cSplash;
	int m_nSelected;
	int m_nFontID;
	TImage m_tBackground;
	TImage m_tButtonNormal;
	TImage m_tButtonHover;
	tSplashScreen m_tScreens[SPLASH_TOTAL];
	bool m_bConfirmation;
	bool m_bExit;
	float m_fChangeTimer;

	/********************************************************************
	* CMainMenuState():		The constructor for the Main menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/2/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CMainMenuState(void);

	/********************************************************************
	* CMainMenuState():		The copy constructor for the Main menu state
	*
	* Ins:					CMainMenuState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	explicit CMainMenuState(const CMainMenuState&);

	/********************************************************************
	* operator=():			The assignment operator for the Main menu state
	*
	* Ins:					CMainMenuState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/1/2012
	* Mod. Initials:		MB
	********************************************************************/
	CMainMenuState& operator=(const CMainMenuState&);

	/********************************************************************
	* ~CGameState():		The deconstructor for the Main menu state
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
	~CMainMenuState(void);

	void DrawButton(int _nMidX, int _nMidY, float _fScaleX, float _fScaleY, string _szText, bool _bHover, D3DXCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), float _fNumButtonWidthOffsets = 0, float _fNumButtonHeightOffsets = 0);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CMainMenuState*
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CMainMenuState* GetInstance()
	{
		static CMainMenuState instance;
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
	* Mod. Date:			11/4/2012
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
	* Mod. Date:			11/2/2012
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
};

#endif	// CMAINMENUSTATE_H