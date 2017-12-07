#ifndef CPOPUPSTATE_H_
#define CPOPUPSTATE_H_

#include "../Globals.h"
#include "IGameState.h"
#include "../Objects/Signpost.h"

class CPopupState : public IGameState
{
	int m_nImageID;
	int m_nNumScreens;
	int m_nCurScreen;
	float m_fFadeTimer;
	bool m_bFadeIn;
	bool m_bFadeOut;
	TImage m_tImage;
	CSignpost* m_pCurrSign;
	unsigned int  m_nCurSignID;

	vector<int> m_vnImages;

	/********************************************************************
	* CPopupState():		The constructor for the Popup state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/7/2013
	* Mod. Initials:		JM
	********************************************************************/
	explicit CPopupState(void);

	/********************************************************************
	* CPopupState():		The copy constructor for the Popup state
	*
	* Ins:					CPopupState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/7/2013
	* Mod. Initials:		JM
	********************************************************************/
	explicit CPopupState(const CPopupState&);

	/********************************************************************
	* operator=():			The assignment operator for the Popup state
	*
	* Ins:					CPopupState&
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/7/2013
	* Mod. Initials:		JM
	********************************************************************/
	CPopupState& operator=(const CPopupState&);

	/********************************************************************
	* ~CPopupState():	The deconstructor for the Popup state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/7/2013
	* Mod. Initials:		JM
	********************************************************************/
	~CPopupState(void);

public:
	/********************************************************************
	* GetInstance():		Gets an instance of this class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				static CPopupState*
	*
	* Mod. Date:			2/7/2013
	* Mod. Initials:		JM
	********************************************************************/
	static CPopupState* GetInstance()
	{
		static CPopupState instance;
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
	* Mod. Date:			2/7/2013
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
	* Mod. Date:			2/7/2013
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
	* Mod. Date:			2/7/2013
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
	* Mod. Date:			2/7/2013
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
	* Mod. Date:			2/7/2013
	* Mod. Initials:		JM
	********************************************************************/
	void Exit(void);

	/*******************
		Accessors
	*******************/
	int GetImageID(void)const			{return m_nImageID;}
	float GetFadeTimer(void)const		{return m_fFadeTimer;}
	bool GetFadeIn(void)const			{return m_bFadeIn;}
	bool GetFadeOut(void)const			{return m_bFadeOut;}
	CSignpost* GetCurrSign(void)const	{return m_pCurrSign;}

	/*******************
		Mutators
	*******************/
	void SetImageID(int _nImageID)	   {m_nImageID = _nImageID;}
	void SetFadeIn(bool _bFade)		   {m_bFadeIn = _bFade;}
	void SetFadeOut(bool _bFade)	   {m_bFadeOut = _bFade;}
	void SetCurSign(CSignpost* _pSign) {m_pCurrSign = _pSign;}
};

#endif // CPOPUPSTATE_H_