/***********************************************
 * Filename:  		CSignpost.h
 * Date:      		2/6/2013
 * Mod. Date: 		2/6/2013
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		This is the file for the Signs
 *					
 ************************************************/

#ifndef _CSIGNPOST_H_
#define _CSIGNPOST_H_

#include "../Globals.h"
#include "EnvironmentObject.h"
//#include

class CSignpost : public CEnvironmentObject
{
	string m_szDescription;
	int m_nLevel;
	int m_nSection;
	int m_nFontID;
	int m_nLightID;
	TImage m_tPopup;
	float m_fPopupScale;
	vector<TImage> m_vtTutorial;
	TImage m_tTutorial;

public:

	public:
	/********************************************************************
	* CSignpost():			Constructor for the Signpost class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/6/2013
	* Mod. Initials:		JM
	********************************************************************/
	CSignpost();

	/********************************************************************
	* ~CSignpost():			Deconstructor for the Signpost class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/6/2013
	* Mod. Initials:		JM
	********************************************************************/
	~CSignpost();

	/********************************************************************
	* Initialize():			Sets up the varibales in the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/6/2013
	* Mod. Initials:		JM
	********************************************************************/
	void Initialize(void);

	/********************************************************************
	* Reinitialize:			Resets all the variable making the Signpost
	*						ready to be used again
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/6/2013
	* Mod. Initials:		JM
	********************************************************************/
	void Reinitialize(void);

	/********************************************************************
	* Update():				Passes a time varable in so that the Signpost
	*						can keep track of when it needs to display its
	*						description
	*
	* Ins:					float
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			2/6/2013
	* Mod. Initials:		JM
	********************************************************************/
	void Update(float fElapsedTime);

	void Uninitialize(void);

	/***************
	*  Accessors
	***************/

	// Returns the text associated with the sign
	inline const char* GetDescription(void)const		  {return m_szDescription.c_str();}
	int GetFontID(void)const	    					  {return m_nFontID;}
	inline TImage GetPopup(void)const					  {return m_tPopup;}
	inline vector<TImage> GetTutorialPopup(void)const	  {return this->m_vtTutorial;}
	int GetLevel(void)const								  {return m_nLevel;}
	int GetSection(void)const							  {return m_nSection;}
	inline float GetPopupScale(void)const				  {return m_fPopupScale;}

	/***************
	*  Mutators
	***************/
	void SetDescription(char* _szDesc);
	void SetLevel(int _nLevel)			{m_nLevel = _nLevel;}
	void SetSection(int _nSection)		{m_nSection = _nSection;}
	void SetPopupScale(float _fScale)	{m_fPopupScale = _fScale;}
	//void SetTutorialPopup(TImage _tBG)	{m_tTutorial = _tBG;}
	
};

#endif // CSIGNPOST_H_