/********************************************************************
* Filename:			CVideoOptionsState.h
* Date:				11/23/2012
* Mod. Date:		11/23/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the option menu state for video settings
********************************************************************/
#ifndef _CVIDEOOPTIONSSTATE_H_
#define _CVIDEOOPTIONSSTATE_H_

#include "../Globals.h"
#include "IGameState.h"
#include "../Util/CInput.h"

enum eVidOption{VID_MIN, VID_FULLSCREEN, VID_RESOLUTION, VID_GAMMA, VID_POSTPROCESS, VID_PARTICLES, VID_EXIT, VID_SAVE, VID_DEFAULT, VID_MAX};

enum eRenderFlags{
	RF_FULLSCREEN		= 0x1,
	RF_RES_640x480		= 0x2,
	RF_RES_1024x768		= 0x4,
	RF_RES_1280x720		= 0x8,
	RF_RES_1680x1050	= 0x10,
	RF_RES_1920x1080	= 0x20,
	RF_RES_ALL_ON		= 0x3E,		// This is equivalent to all resolutions turned on
	RF_POST_PROCESS		= 0x40,
	RF_PARTICLES		= 0x80,
};

class CVideoOptionsState : public IGameState
{

	int m_nSelected;

	TImage m_tBackground;

	unsigned int m_nGammaPower;
	float m_fActualGamma;
	float m_fIncrementTimer;
	float m_fFlashTimer;
	bool m_bDisplayControls;
	bool m_bPrevLeft;
	bool m_bPrevRight;

	unsigned char m_ucRenderFlags;
	unsigned char m_ucOldResolutionFlag;
	bool m_bChangedResolution;

	int m_nFontID;
	CTimer m_tTimer;			// the in-games' timer needs to be paused, so each state needs its own timer for fElapsedTime



	/********************************************************************
	* CVideoOptionsState():		The constructor for the Video menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CVideoOptionsState(void);

	/********************************************************************
	* CVideoOptionsState():		The copy constructor for the Video menu state
	*
	* Ins:					CVideoOptionsState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CVideoOptionsState(const CVideoOptionsState&);

	/********************************************************************
	* operator=():			The assignment operator for the Video menu state
	*
	* Ins:					CVideoOptionsState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	CVideoOptionsState& operator=(const CVideoOptionsState&);

	/********************************************************************
	* ~CVideoOptionsState():	The deconstructor for the Video menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	~CVideoOptionsState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static CVideoOptionsState*
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	static CVideoOptionsState* GetInstance()
	{
		static CVideoOptionsState instance;
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
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
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
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	bool Input(void);

	/********************************************************************
	* Update():				This is the function that handles update for
	*						this state
	*
	* Ins:					float
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Update(float _fElapsedTime);

	/********************************************************************
	* Render():				This is the function that handles render for
	*						this state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
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
	* Mod. Date:			11/23/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Exit(void);

	/********************************************************************
	* LoadSettings():	Checks to see if there is a pre-set settings file and
	*					then changes the video settings to these presets
	*					if they exist.
	*
	* Ins:				std::string
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/23/2012
	* Mod. Initials:	JM
	********************************************************************/
	void LoadSettings(string szFilename);

	/********************************************************************
	* SaveSettings():	Checks to see if there is a settings file and
	*					then opens it if it exists, otherwise it creates
	*					a new one and saves out all current vid setting data.
	*
	* Ins:				std::string
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/23/2012
	* Mod. Initials:	JM
	********************************************************************/
	void SaveSettings(string szFilename);

	void RestoreDefaults();

	void UpdateFlags();

	unsigned char GetRenderFlags(void) {return m_ucRenderFlags;}

};
#endif // _CVIDEOOPTIONSSTATE_H_
