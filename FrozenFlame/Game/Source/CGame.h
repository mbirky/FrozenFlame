/********************************************************************
* Filename:			CGame.h
* Date:				10/30/2012
* Mod. Date:		11/14/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This class is used to manage the game calling the
*					current states functions.
********************************************************************/

#ifndef CGAME_H
#define CGAME_H

#include <Windows.h>

#include "Util/CTimer.h"
#include "Util/CInput.h"
#include "States/IGameState.h"
#include "Sound/CSoundManager.h"
#include "Globals.h"
#include "Achievements\Header\CAchievementManager.h"

class CGame
{
private:
	IGameState			*m_pCurrentState;
	CInput				*m_pInput;
	CSoundManager		*m_pSound;
	CAchievementManager	*m_pcAchievementManager;

	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	CTimer			m_Timer;
	TImage			m_tLoadingBackground;
	TImage			m_tLoadingSwordEmpty;
	TImage			m_tLoadingSwordFull;
	WINDOWPLACEMENT	m_WindowPlacement;

	bool	m_bDoneSplashing;
	bool	m_bIsMuteOn;
	bool	m_bFromPause;
	bool	m_bFullScreen;
	bool	m_bWaiting;
	bool	m_bVictory;

	int	m_nScreenWidth;
	int	m_nScreenHeight;
	int	m_nDifficultyLevel;
	int	m_nSoundBank;
	int	m_nSoundBank2;
	int	m_nFontID;
	int m_nMinionKillCount;		// how many minions were harmed in the playing of this game?

	float	m_fGamma;
	float	m_fElapsedTime;
	float	m_fAccumulatedPlayTime;	// how long the player has been playing this game, used in credit screen

	unsigned short	m_fMasterVolume;
	unsigned short	m_fMusicVolume;
	unsigned short	m_fSFXVolume;

	/********************************************************************
	* CGame():				The constructor for the game class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	CGame();

	/********************************************************************
	* CGame():				The Copy constructor for the game class
	*
	* Ins:					CGame&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	CGame(const CGame&);

	/********************************************************************
	* operator=():			The assignement operator of the game class
	*
	* Ins:					CGame&
	*
	* Outs:					void
	*
	* Returns:				CGame&
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	CGame& operator=(const CGame&);

	/********************************************************************
	* ~Cgame():				The destructor for the game class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	~CGame(void);

	/********************************************************************
	* Input():				Handels all input for the game.  Calls the
	*						current state so that it handels its input
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
	bool Input(void);

	/********************************************************************
	* Update():				Updates the game and calls update on the
	*						current state.
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
	void Update(void);

	/********************************************************************
	* Render():				Renders what it needs and calls the states
	*						render
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

	

	void SetFullScreenOn(void);
	void SetFullScreenOff(void);

public:
	void DrawLoading(float _fPercentage);
	/********************************************************************
	* GetInstance():		Gets the instance of the game.  The class is
	*						a singelton so there is only one instace of
	*						it
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CGame*
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	static CGame* GetInstance(void)
	{
		static CGame instance;
		return &instance;
	}

	/********************************************************************
	* Init():				Initilizes the game class
	*
	* Ins:					HWND
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Init(HWND hWnd, HINSTANCE _hInstance, int _nScreenWidth, int _nScreenHeight, bool _bFullScreen);

	/********************************************************************
	* Main():				Calls all the main functions needed to run
	*						the game
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				bool
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	eGameReturn Main(void);

	/********************************************************************
	* Shutdown():			Shutsdown the game
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/30/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Shutdown(void);

	/********************************************************************
	* ChangeState():		Clears the State stack and sets the current
	*						state to the state passed in
	*
	* Ins:					IGameState*
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	void ChangeState(IGameState* _pNewState);

	/********************************************************************
	* PushState():			Pushes a new state on top of the stack making
	*						it the current state
	*
	* Ins:					IGameState*
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/31/2012
	* Mod. Initials:		MB
	********************************************************************/
	void PushState(IGameState* _pNewState);

	/********************************************************************
	* PopState():			Pops the state on top of the stack making its
	*						prev state the current state
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
	void PopState(void);

	/********************************************************************
	* StartTimer():			Starts the timer in CGame
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
	void StartTimer(void);

	/********************************************************************
	* StopTimer():			Stops the timer in CGame
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
	void StopTimer(void);

	/********************************************************************
	* StopTimer():			Returns the running time since starting the timer
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				the running time since starting the timer
	*
	* Mod. Date:			02/08/2013
	* Mod. Initials:		SMS
	********************************************************************/
	float GetRunningGameTime(void);

	void LoseFocus(void);
	void GainFocus(void);

	void SetRenderFlags(unsigned char ucRenderFlags);

	// minion kill count for the end credits
	void ResetMinionKillCount()									{ m_nMinionKillCount = 0; }
	int  GetMinionKillCount() const								{ return m_nMinionKillCount; }
	void IncrementMinionKillCount()								{ m_nMinionKillCount++; }
	// total time to complete the game, used in the credits
	void ResetAccumulatedPlayTime()								{ m_fAccumulatedPlayTime = 0.0f; }
	void AddToPlayTime(float fElapsedTime)						{ m_fAccumulatedPlayTime += fElapsedTime; }
	int  GetAccumulatedPlayMinutes()							{ return ((int)m_fAccumulatedPlayTime / 60); }
	int  GetAccumulatedPlaySeconds()							{ return ((int)m_fAccumulatedPlayTime % 60); }

	/***************
	 *  Accessors
	 ***************/
	inline int GetSoundBank(void)								{return m_nSoundBank;}
	inline int GetSoundBank2(void)								{return m_nSoundBank2;}
	inline int GetScreenWidth(void)								{return m_nScreenWidth;}
	inline int GetScreenHeight(void)							{return m_nScreenHeight;}
	inline IGameState const * const GetCurrentState(void) const	{return m_pCurrentState;}
	inline bool GetSplashing(void)								{return m_bDoneSplashing;}
	inline unsigned short GetMusicVol(void)						{return m_fMusicVolume;}
	inline unsigned short GetSFXVol(void)						{return m_fSFXVolume;}
	inline HWND GetHWND(void)									{return m_hWnd;}
	inline HINSTANCE GetHINSTANCE(void)							{return m_hInstance;}
	inline int GetDifficultyLevel(void)							{return m_nDifficultyLevel;}
	inline bool GetFromPause(void)								{return m_bFromPause;}
	inline float& GetGammaValue(void)							{return m_fGamma;}
	inline bool GetVictory(void)								{return m_bVictory;}
	inline bool GetWaiting(void)								{return m_bWaiting;}
	inline bool GetIsFullScreen(void) const						{ return m_bFullScreen; }
	/***************
	 * Mutators
	 ***************/
	inline void SetScreenDimensions(int _nScreenWidth, int _nScreenHeight)
	{
		m_nScreenWidth = _nScreenWidth;
		m_nScreenHeight = _nScreenHeight;
	}
	inline void SetScreenWidth(int _nScreenWidth)			{m_nScreenWidth = _nScreenWidth;}
	inline void SetScreenHeight(int _nScreenHeight)			{m_nScreenHeight = _nScreenHeight;}
	inline void SetSplashing(bool state)					{m_bDoneSplashing = state;}
	inline void SetMusicVol(unsigned short fAmount)			{m_fMusicVolume = fAmount;}
	inline void SetSFXVol(unsigned short fAmount)			{m_fSFXVolume = fAmount;}
	inline void SetDifficultyLevel(int _nDifficultyLevel)	{m_nDifficultyLevel = _nDifficultyLevel;}
	inline void SetFromPause(bool isPaused)					{m_bFromPause = isPaused;}
	inline void SetGammaValue(float _fGamma)				{m_fGamma = _fGamma;}
	inline void SetVictory(bool _bVictory)					{m_bVictory = _bVictory;}
	inline void SetWaiting(bool _bWaiting)					{m_bWaiting = _bWaiting;}
};

#endif	//CGAEM_H