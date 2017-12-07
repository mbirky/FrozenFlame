/********************************************************************
* Filename:			CCreditsState.h
* Date:				11/26/2012
* Mod. Date:		11/26/2012
* Mod. Initials:	JM
* Author:			Justin Maza
* Purpose:			This is the option menu state for sounds and music
********************************************************************/
#ifndef _CCREDITSSTATE_H_
#define _CCREDITSSTATE_H_

#include "../Globals.h"
#include "IGameState.h"
#include "../Util/CInput.h"

class CCreditsState : public IGameState
{
	TImage m_tBackground;
	TImage m_tTeamImage;
	TImage m_tButtonNormal;
	TImage m_tLogoImage;
	TImage m_tTeamPics[8];

	int m_nFontID;
	const float m_fResetPoint;
	const float m_fRiseRate;
	float m_fPosition;
	float m_fRiseTimer;
	//float m_fPosTrack;
	bool m_bNeedReset;
	float m_fFadeOut;
	float m_fFontScaleX;
	float m_fFontScaleY;

	float m_fResetTimer;
	
	/********************************************************************
	* CCreditsState():		The constructor for the Credits state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			12/3/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CCreditsState(void);

	/********************************************************************
	* CSoundOptionState():	The copy constructor for the Credits state
	*
	* Ins:					CCreditsState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			12/3/2012
	* Mod. Initials:		JM
	********************************************************************/
	explicit CCreditsState(const CCreditsState&);

	/********************************************************************
	* operator=():			The assignment operator for the Credits state
	*
	* Ins:					CCreditsState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			12/3/2012
	* Mod. Initials:		JM
	********************************************************************/
	CCreditsState& operator=(const CCreditsState&);

	/********************************************************************
	* ~CCreditsState():		The deconstructor for the Credits state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			12/3/2012
	* Mod. Initials:		JM
	********************************************************************/
	~CCreditsState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static CCreditsState*
	*
	* Mod. Date:			12/3/2012
	* Mod. Initials:		JM
	********************************************************************/
	static CCreditsState* GetInstance()
	{
		static CCreditsState instance;
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
	* Mod. Date:			12/3/2012
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
	* Mod. Date:			12/3/2012
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
	* Mod. Date:			12/3/2012
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
	* Mod. Date:			12/3/2012
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
	* Mod. Date:			12/3/2012
	* Mod. Initials:		JM
	********************************************************************/
	void Exit(void);
};
#endif // _CCREDITSSTATE_H_
