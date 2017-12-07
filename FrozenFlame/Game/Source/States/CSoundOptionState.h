/********************************************************************
* Filename:			CSoundOptionState.h
* Date:				11/26/2012
* Mod. Date:		11/26/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the option menu state for sounds and music
********************************************************************/
#ifndef _CSOUNDOPTIONSTATE_H_
#define _CSOUNDOPTIONSTATE_H_

#include "../Globals.h"
#include "IGameState.h"
#include "../Util/CInput.h"

enum eSoundOption{SND_MIN, SND_MASTER, SND_MUSIC, SND_SOUND, SND_MUTE, SND_EXIT, SND_DEFAULT, SND_SAVE, SND_MAX};

class CSoundOptionState : public IGameState
{
	bool m_bDisplayControls;
	bool isMuteOn;
	bool m_bIsMuteChangeable;
	unsigned char m_ucRenderFlags;
	unsigned short m_usSoundVolume;
	unsigned short m_usMusicVolume;
	unsigned short m_usMasterVolume;
	float m_fIncrementTimer;
	float m_fFlashTimer;
	int m_nSelected;
	int m_nFontID;
	float m_fPlayTimer;
	TImage m_tBackground;
	CTimer m_tTimer;			// the in-games' timer needs to be paused, so each state needs its own timer for fElapsedTime
	bool m_bPrevLeft;
	bool m_bPrevRight;

	/********************************************************************
	* CSoundOptionState():		The constructor for the Sound menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/26/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CSoundOptionState(void);

	/********************************************************************
	* CSoundOptionState():		The copy constructor for the Sound menu state
	*
	* Ins:					CSoundOptionState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/26/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CSoundOptionState(const CSoundOptionState&);

	/********************************************************************
	* operator=():			The assignment operator for the Sound menu state
	*
	* Ins:					CSoundOptionState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/26/2012
	* Mod. Initials:		JM
	********************************************************************/
	CSoundOptionState& operator=(const CSoundOptionState&);

	/********************************************************************
	* ~CSoundOptionState():	The deconstructor for the Sound menu state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/26/2012
	* Mod. Initials:		JM
	********************************************************************/
	~CSoundOptionState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static CSoundOptionState*
	*
	* Mod. Date:			11/26/2012
	* Mod. Initials:		JM
	********************************************************************/
	static CSoundOptionState* GetInstance()
	{
		static CSoundOptionState instance;
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
	* Mod. Date:			11/26/2012
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
	* Mod. Date:			11/26/2012
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
	* Mod. Date:			11/26/2012
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
	* Mod. Date:			11/26/2012
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
	* Mod. Date:			11/26/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Exit(void);

	/********************************************************************
	* LoadSettings():	Checks to see if there is a pre-set settings file and
	*					then changes the sound settings to these presets
	*					if they exist.
	*
	* Ins:				std::string
	*
	* Outs:				void
	*
	* Returns:			void
	*
	* Mod. Date:		11/26/2012
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
};
#endif // _CSOUNDOPTIONSTATE_H_