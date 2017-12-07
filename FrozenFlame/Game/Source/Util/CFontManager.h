/***********************************************
 * Filename:  		CFontManager.h
 * Date:      		11/02/2012
 * Mod. Date: 		11/05/2012
 * Mod. Initials:	MB
 * Author:    		Roberto J. Rubiano
 * Purpose:   		A Manager for Bitmap fonts
 ************************************************/
#ifndef CFONTMANAGER_H_
#define CFONTMANAGER_H_

#include "../Globals.h"
#include "../Renderer/View.h"

class CFontManager
{
private:

	class tBitmapFont
	{
	public:

		unsigned char m_cFirstIndex;
		unsigned char m_cLastIndex;
		int m_nTextureID;
		int m_nNumCol;
		int m_nNumRow;
		int m_nCharHeight;
		int m_pCharWidths[255];

		/**************************************************************
		* tBitmapFont()	:	The default constructor
		* Ins			:	None
		* Outs			:	None
		* Returns		:	None
		* Mod. Date		:	11/11/2012
		* Mod. Initals	:	RJR
		***************************************************************/
		tBitmapFont(): m_cFirstIndex(0), m_cLastIndex(0), m_nTextureID(-1), 
						m_nNumCol(0), m_nNumRow(0), m_nCharHeight(0)
		{
			ZeroMemory(m_pCharWidths, 255);
		}

		/**************************************************************
		* ~tBitmapFont()	:	The destructor
		* Ins				:	None
		* Outs				:	None
		* Returns			:	None
		* Mod. Date			:	11/11/2012
		* Mod. Initals		:	RJR
		***************************************************************/
		~tBitmapFont() {}
	};

	/**************************************************************
	* RECT CellAlgorithm(unsigned char nChar, int nFontID);
					:	Preforms the cell algorithm in order to determine the
					:	rect of the character to be rendered.
	* Ins			:	nChar, nFontID
	* Outs			:	None.
	* Returns		:	RECT
	* Mod. Date		:	11/03/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	RECT CellAlgorithm(int nCharID, int nFontID);

	vector<tBitmapFont> m_vFonts;

public:

	/**************************************************************
	* CFontManager(void)	:	The Constructor for this class
	* Ins					:	None
	* Outs					:	None
	* Returns				:	None
	* Mod. Date				:	11/02/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	CFontManager(void);

	/**************************************************************
	* ~CFontManager(void)	:	The Destructor for this class
	* Ins					:	None
	* Outs					:	None
	* Returns				:	None
	* Mod. Date				:	11/02/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	~CFontManager(void);

	/**************************************************************
	* int LoadFont(const char* pfilename)	:	Loads the specified font texture
	* Ins									:	pfilename
	* Outs									:	None
	* Returns								:	int
	* Mod. Date								:	11/02/2012
	* Mod. Initals							:	RJR
	***************************************************************/
	int LoadFont(const char* pfilename);

	/**************************************************************
	* void Write(int nFontID,string szText, RECT* pArea, float pScale)
	*				:	Writes out the text as bitmap font
	* Ins			:	nFontID, szText, pArea, pScale
	* Outs			:	None	
	* Returns		:	void
	* Mod. Date		:	11/02/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	void Write(int nFontID,string szText, RECT* pArea, float fScale, D3DCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	/**************************************************************
	* void Write(int nFontID,string szText, RECT* pArea, float pScale)
	*				:	Writes out the text as bitmap font
	* Ins			:	nFontID, szText, pArea, pScaleX, pScaleY
	* Outs			:	None	
	* Returns		:	void
	* Mod. Date		:	2/21/2013
	* Mod. Initals	:	MB
	***************************************************************/
	void Write(int nFontID,string szText, RECT* pArea, float fScaleX, float fScaleY, D3DCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	/**************************************************************
	* void WriteCentered(int nFontID,string szText, RECT* pArea, float pScale)
	*				:	Writes out the text as bitmap font centered on
	*					the RECT
	* Ins			:	nFontID, szText, pArea, pScale
	* Outs			:	None	
	* Returns		:	void
	* Mod. Date		:	11/05/2012
	* Mod. Initals	:	MB
	***************************************************************/
	void WriteCentered(int nFontID,string szText, RECT* pArea, float fScale, D3DCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	/**************************************************************
	* void WriteCentered(int nFontID,string szText, RECT* pArea, float pScale)
	*				:	Writes out the text as bitmap font centered on
	*					the RECT
	* Ins			:	nFontID, szText, pArea, pScale
	* Outs			:	None	
	* Returns		:	void
	* Mod. Date		:	11/05/2012
	* Mod. Initals	:	MB
	***************************************************************/
	void WriteCentered(int nFontID,string szText, RECT* pArea, float fScaleX, float fScaleY, D3DCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
};

#endif