//////////////////////////////////////////////////////////////////////////
//	File			:	"CHud.h"
//
//	Author			:	Steven Seufert (SMS) 
//	Last Modified	:	January 23, 2013 (SMS)
//
//	Purpose			:	to define the singleton to handle miscellaneous HUD images
/////////////////////////////////////////////////////////////////////////// 
#ifndef CHUD_H
#define CHUD_H

#include "../Globals.h"

class CPlayerObject;

class CHud
{
private:
	struct tHUDObject
	{
		string name;
		int posX;
		int posY;
		int width;
		int height;
		float Xscale;
		float Yscale;
		float baseXScale;
		float baseYScale;
		float rotation;
		int centerX;
		int centerY;
		DWORD color;
		int imageID;
	};

	int m_nPreviousHUD;

	float m_fRageGlowTimer;		// the amount of time the rage glow has been on
	float m_fRageGlowDuration;	// the rage glow will glow for this long
	bool  m_bRageGlowOn;		// Is the rage glow glowing?
	bool  m_bFlashGlowRage;		// flash the rage glow effect
	bool  m_bRageGlowAlphaUp;	// true = increase the alpha on the rage glow, false = decrement the alpha
	float m_fRageGlowAlpha;		// the alpha value of the rage glow effect
	bool  m_bFlashRageGlowOnce;	// if on, will flash the rage glow a single time
	// constructor
	CHud(void) : m_fRageGlowTimer(0), m_fRageGlowDuration(0.5f), m_bRageGlowOn(false), m_bFlashGlowRage(false), m_bRageGlowAlphaUp(true), m_fRageGlowAlpha(0), m_bFlashRageGlowOnce(false), m_nPreviousHUD(0) {}
	
	// destructor
	~CHud(void) {}

	// copy constructor
	CHud(const CHud&);

	// assignment operator
	CHud& operator=(const CHud&);

	map <string, tHUDObject> hudObjects;

	///////////////////////////////////////////////////
	//	Function	:	"CreateHudObject"
	//	
	//	Last Modified:	January 23, 2013 (SMS)
	//
	//	Input		:	bScaleToScreen	- the image gets scaled to match a certain portion of the screen
	//										
	//	Return		:	none
	//
	//	Purpose		:	internal function to add a picture and its values to the HUD's map
	//////////////////////////////////////////////////
	void CreateHudObject(string fileName, int nPosX, int nPosY, float fScaleX, float fScaleY, float fRotation, DWORD dwColor, bool bScaleToScreen, float xScreenRatio = 0.5f, float yScreenRatio = 0.5f);

public:
	static CHud* GetInstance()			{ static CHud instance; return &instance; }

	void SetXPosition(string fileName, int x);
	void SetYPosition(string fileName, int y);
	void SetXScale(string fileName, float fScale);
	void SetYScale(string fileName, float fScale);
	void SetRotation(string fileName, float fRotation);
	void SetColor(string fileName, DWORD _color);

	int GetWidth(string fileName);
	int GetHeight(string fileName);
	float GetXScale(string fileName);
	float GetYScale(string fileName);
	float GetBaseXScale(string fileName);
	float GetBaseYScale(string fileName);

	void Update(float fElapsedTime);

	///////////////////////////////////////////////////
	//	Function	:	"LoadHUDObjects"
	//	
	//	Last Modified:	January 23, 2013 (SMS)
	//
	//	Input		:	none
	//										
	//	Return		:	none
	//
	//	Purpose		:	Loads the damage splash, sword, and health and rage meters
	//////////////////////////////////////////////////
	void LoadHUDObjects();	

	///////////////////////////////////////////////////
	//	Function	:	"Draw"
	//	
	//	Last Modified:	January 23, 2013 (SMS)
	//
	//	Input		:	fileName	the file name of the image you wish to draw
	//										
	//	Return		:	none
	//
	//	Purpose		:	draws the image with its current values so long
	//					as it has been loaded in
	//////////////////////////////////////////////////
	void Draw(string fileName);

	///////////////////////////////////////////////////
	//	Function	:	"DrawARGB"
	//	
	//	Last Modified:	February 13, 2012 (SMS)
	//
	//	Input		:	fileName	the file name of the image you wish to draw
	//					argbColor	the ARGB color change to make to the picture
	//										
	//	Return		:	none
	//
	//	Purpose		:	draws the image with a tint color and alpha value
	//////////////////////////////////////////////////
	void DrawARGB(string fileName, DWORD argbColor);

	///////////////////////////////////////////////////
	//	Function	:	"DrawPlayerInfo"
	//	
	//	Last Modified:	January 23, 2013 (SMS)
	//
	//	Input		:	player			a pointer to the player
	//					left			the left x position to place the HUD object
	//					top				the top y position to place the HUD object
	//										
	//	Return		:	none
	//
	//	Purpose		:	draws the HUD health/rage bars with the sword
	//////////////////////////////////////////////////
	void DrawHealthAndRage(CPlayerObject* player, int left, int top);

	///////////////////////////////////////////////////
	//	Function	:	"FlashRageGlow"
	//	
	//	Last Modified:	January 26, 2013 (SMS)
	//
	//	Input		:	none
	//										
	//	Return		:	none
	//
	//	Purpose		:	flashes the rage glow once
	//////////////////////////////////////////////////
	void FlashRageGlow();

	///////////////////////////////////////////////////
	//	Function	:	"DrawDamageSplash"
	//	
	//	Last Modified:	January 23, 2013 (SMS)
	//
	//	Input		:	none
	//										
	//	Return		:	none
	//
	//	Purpose		:	draws the red overlay on screen when the player takes damage
	//////////////////////////////////////////////////
	void DrawDamageSplash();


	void DrawEffectSplash(int nState);
};

#endif