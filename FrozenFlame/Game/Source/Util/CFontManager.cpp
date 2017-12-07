/***********************************************
 * Filename:  		CFontManager.h
 * Date:      		11/02/2012
 * Mod. Date: 		11/02/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		A Manager for Bitmap fonts, defenitions
 ************************************************/
#include "CFontManager.h"

#define FONTLOC "Resource/Sprites/Fonts/"

/**********************************
* Private Functions
***********************************/

RECT CFontManager::CellAlgorithm(int nCharID, int nFontID)
{
	RECT rText = {0,0,0,0};
	tBitmapFont& Font = m_vFonts[nFontID];

	rText.left = LONG((nCharID % Font.m_nNumCol) * Font.m_pCharWidths[nCharID]);
	rText.top = LONG((nCharID / Font.m_nNumRow) * Font.m_nCharHeight);
	rText.right = LONG(rText.left + Font.m_pCharWidths[nCharID]);
	rText.bottom = LONG(rText.top + Font.m_pCharWidths[nCharID]);

	return rText;
}

/**********************************
* Public Functions
***********************************/

CFontManager::CFontManager(void)
{
}

CFontManager::~CFontManager(void)
{
	m_vFonts.clear();
}

int CFontManager::LoadFont(const char* pfilename)
{
	fstream in;

	// add the default path for all fonts
	string name = FONTLOC;
	name += pfilename;

	// Open requested file
	in.open(name.c_str(),ios_base::in);

	// if the file is open lets load the font
	if(in.is_open())
	{
		string fontname;

		// load the name of the font
		in >> fontname;
		in.ignore(INT_MAX,'\n');
		
		fontname = "Fonts/" + fontname;

		tBitmapFont tFont;

		// use the view class to load the sprite and catch it's id
		tFont.m_nTextureID = CView::LoadSprite(fontname.c_str());

		unsigned int uSize = m_vFonts.size();

		// loop throug our fonts to see if we have allready loaded this one
		for(unsigned int i = 0; i < uSize; i++)
		{
			if(m_vFonts[i].m_nTextureID == tFont.m_nTextureID)
			{
				// if so close the file and return the id
				in.close();
				return i;
			}
		}

		// if a bad font or not loaded properly return -1
		if(tFont.m_nTextureID < 0)
		{
			in.close();
			return -1;
		}

		// get the number of columns and rows
		in >> tFont.m_nNumCol;
		in >> tFont.m_nNumRow;
		in.ignore(INT_MAX,'\n');

		int test = 0;

		// get the first character in the font
		in >> test;
		tFont.m_cFirstIndex = test;
		in.ignore(INT_MAX,'\n');

		// get the last character in the font
		in >> test;
		tFont.m_cLastIndex = test;
		in.ignore(INT_MAX,'\n');

		int nWidth = 0;

		// get the default width
		in >> nWidth;
		in >> tFont.m_nCharHeight;

		// get the number of characters in the font
		int nSize = tFont.m_cLastIndex - tFont.m_cFirstIndex;

		// allocate the new size to store all the widths
		//tFont.m_pCharWidths = new int[nSize];

		for(int i = 0; i < nSize; i++)
		{
			tFont.m_pCharWidths[i] = nWidth;
		}

		// we will account for any special case widths in the font
		//while(!in.eof())
		//{
		//	in.ignore(INT_MAX,'\n');

		//	char read;
		//	in >> read;

		//	int wid;
		//	in >> wid;

		//	if(wid != nWidth)
		//	{
		//		tFont.m_pCharWidths[read] = wid;
		//	}
		//}

		// add to our vector of fonts
		m_vFonts.push_back(tFont);

	}
	else
	{
		in.close();
		return -1;
	}

	// close the file
	in.close();

	// return the actual index of the current font
	return m_vFonts.size() - 1;
}

void CFontManager::Write(int nFontID,string szText, RECT* pArea, float fScale, D3DCOLOR d3dColor)
{
	if(nFontID < 0 || (unsigned int)nFontID >= m_vFonts.size())
	{
		return;
	}

	unsigned int uSize = szText.size();

	// for each character we are going to use the cell algorithm and render it
	for(unsigned int i = 0; i < uSize; i++)
	{
		RECT rSourceRect = CellAlgorithm(szText[i] - m_vFonts[nFontID].m_cFirstIndex,nFontID);
		CView::Draw(m_vFonts[nFontID].m_nTextureID,int(64*fScale)*i+pArea->left,pArea->top,fScale,fScale,&rSourceRect, 0.0f, 0.0f, 0.0f, d3dColor);
	}
}

void CFontManager::Write(int nFontID,string szText, RECT* pArea, float fScaleX, float fScaleY, D3DCOLOR d3dColor)
{
	if(nFontID < 0 || (unsigned int)nFontID >= m_vFonts.size())
	{
		return;
	}

	unsigned int uSize = szText.size();

	// for each character we are going to use the cell algorithm and render it
	for(unsigned int i = 0; i < uSize; i++)
	{
		RECT rSourceRect = CellAlgorithm(szText[i] - m_vFonts[nFontID].m_cFirstIndex,nFontID);
		CView::Draw(m_vFonts[nFontID].m_nTextureID, int(64*fScaleX)*i+pArea->left, pArea->top, fScaleX, fScaleY, &rSourceRect, 0.0f, 0.0f, 0.0f, d3dColor);
	}
}

void CFontManager::WriteCentered(int nFontID,string szText, RECT* pArea, float fScale, D3DCOLOR d3dColor)
{
	if(nFontID < 0 || (unsigned int)nFontID >= m_vFonts.size())
	{
		return;
	}

	size_t uSize = szText.size();

	int nTotalStringLength = int(64 * fScale) * szText.size();
	int nTotalStringHeight = int(64 * fScale);

	int nRectXMid = (pArea->left + pArea->right) >> 1;
	int nRectYMid = (pArea->top + pArea->bottom) >> 1;

	int nDrawPosX = nRectXMid - (nTotalStringLength >> 1);
	int nDrawPosY = nRectYMid - (nTotalStringHeight >> 1);

	RECT rSourceRect = {0,0,0,0};

	for(unsigned int i = 0; i < uSize; ++i)
	{
		rSourceRect = CellAlgorithm(szText[i] - m_vFonts[nFontID].m_cFirstIndex,nFontID);
		CView::Draw(m_vFonts[nFontID].m_nTextureID, nDrawPosX, nDrawPosY, fScale, fScale,&rSourceRect, 0.0f, 0.0f, 0.0f, d3dColor);
		nDrawPosX += int(64 * fScale);
	}
}

void CFontManager::WriteCentered(int nFontID,string szText, RECT* pArea, float fScaleX, float fScaleY, D3DCOLOR d3dColor)
{
	if(nFontID < 0 || (unsigned int)nFontID >= m_vFonts.size())
	{
		return;
	}

	size_t uSize = szText.size();

	int nTotalStringLength = int(64 * fScaleX) * szText.size();
	int nTotalStringHeight = int(64 * fScaleY);

	int nRectXMid = (pArea->left + pArea->right) >> 1;
	int nRectYMid = (pArea->top + pArea->bottom) >> 1;

	int nDrawPosX = nRectXMid - (nTotalStringLength >> 1);
	int nDrawPosY = nRectYMid - (nTotalStringHeight >> 1);

	RECT rSourceRect = {0,0,0,0};

	for(unsigned int i = 0; i < uSize; ++i)
	{
		rSourceRect = CellAlgorithm(szText[i] - m_vFonts[nFontID].m_cFirstIndex,nFontID);
		CView::Draw(m_vFonts[nFontID].m_nTextureID, nDrawPosX, nDrawPosY, fScaleX, fScaleY, &rSourceRect, 0.0f, 0.0f, 0.0f, d3dColor);
		nDrawPosX += int(64 * fScaleX);
	}
}