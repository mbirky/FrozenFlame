#include "CHUD.h"
#include "../Renderer/View.h"
#include "../CGame.h"
#include"../Objects/PlayerObject.h"
#include "../Core/CWorldManager.h"

void CHud::SetXPosition(string fileName, int x)
{
	hudObjects[fileName].posX = x;
}

void CHud::SetYPosition(string fileName, int y)
{
	hudObjects[fileName].posY = y;
}

void CHud::SetXScale(string fileName, float fScale)
{
	hudObjects[fileName].Xscale = fScale;
}

void CHud::SetYScale(string fileName, float fScale)
{
	hudObjects[fileName].Yscale = fScale;
}

void CHud::SetRotation(string fileName, float fRotation)
{
	hudObjects[fileName].rotation = D3DXToRadian(fRotation);
}

void CHud::SetColor(string fileName, DWORD _color)
{
	hudObjects[fileName].color = _color;
}

int CHud::GetWidth(string fileName)
{
	return hudObjects[fileName].width;
}

int CHud::GetHeight(string fileName)
{
	return hudObjects[fileName].height;
}

float CHud::GetXScale(string fileName)
{
	return hudObjects[fileName].Xscale;
}

float CHud::GetYScale(string fileName)
{
	return hudObjects[fileName].Yscale;
}

float CHud::GetBaseXScale(string fileName)
{
	return hudObjects[fileName].baseXScale;
}

float CHud::GetBaseYScale(string fileName)
{
	return hudObjects[fileName].baseYScale;
}

void CHud::Update(float fElapsedTime)
{
	if (m_bFlashRageGlowOnce)
	{
		m_fRageGlowTimer += fElapsedTime;

		// increase the alpha of the glow
		if (m_bRageGlowAlphaUp)
		{   // don't have an alpha greater than one
			m_fRageGlowAlpha = min(m_fRageGlowTimer/m_fRageGlowDuration, 1.0f);
		}
		else
		{   // don't subtract more than one
			m_fRageGlowAlpha = (1 - min(m_fRageGlowTimer/m_fRageGlowDuration, 1.0f));
		}
		// once the rage glow is complete, turn off FlashRageGlowOnce
		if (false == m_bRageGlowAlphaUp && m_fRageGlowAlpha <= 0.01f)
		{
			m_bFlashRageGlowOnce = false;
		}
		// toggle whether the glow is increasing or decreasing
		if (m_fRageGlowTimer >= m_fRageGlowDuration)
		{
			m_bRageGlowAlphaUp = !m_bRageGlowAlphaUp;	
			m_fRageGlowTimer = 0;
		}		
	}
	else // pulse the glow continuously
	{
		if (m_bFlashGlowRage)
		{
			m_fRageGlowTimer += fElapsedTime;

			// increase the alpha of the glow
			if (m_bRageGlowAlphaUp)
			{   // don't have an alpha greater than one
				m_fRageGlowAlpha = min(m_fRageGlowTimer/m_fRageGlowDuration, 1.0f);
			}
			else
			{   // don't subtract more than one
				m_fRageGlowAlpha = (1 - min(m_fRageGlowTimer/m_fRageGlowDuration, 1.0f));
			}
			if (m_fRageGlowTimer >= m_fRageGlowDuration)
			{
				m_bRageGlowAlphaUp = !m_bRageGlowAlphaUp;	// toggle whether the glow is increasing or decreasing
				m_fRageGlowTimer = 0;
			}
		}
	}
}

///////////////////////////////////////////////////
//	Function	:	"LoadHUDObjects"
//	
//	Last Modified:	January 23, 2013 (SMS)
//
//	Input		:	none
//										
//	Return		:	none
//
//	Purpose		:	Loads the images the HUD will be using
//////////////////////////////////////////////////
void CHud::LoadHUDObjects()
{
	float xSafe = CGame::GetInstance()->GetScreenWidth() * 0.0f;
	float ySafe = CGame::GetInstance()->GetScreenHeight() * 0.0f;


	// the health/rage bars need a left and top offset into the sword
	int barLeft = (int)(0.0256f * CGame::GetInstance()->GetScreenWidth());
	int healthTop = (int)(0.093f * CGame::GetInstance()->GetScreenHeight());
	int rageTop = (int)(0.114f * CGame::GetInstance()->GetScreenHeight());
	CreateHudObject("DamageHUDOverlay2.png", 0, 0, 1.0f, 1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), false);		// red splash when the player takes damage
	
	CreateHudObject("ColdOverlay.png", 0, 0, 1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);
	CreateHudObject("HotOverlay.png", 0, 0, 1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);
	CreateHudObject("HotOverlay2.png", 0, 0, 1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);
	CreateHudObject("HotOverlay3.png", 0, 0, 1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), false);

	CreateHudObject("FireSword.png",		(int)xSafe, (int)ySafe, 1.0,  1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the sword when the player is HOT
	CreateHudObject("IceSword.png",			(int)xSafe, (int)ySafe, 1.0,  1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the sword when the player is COLD	
	CreateHudObject("NormalSword.png",		(int)xSafe, (int)ySafe, 1.0,  1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the sword when the player is NEUTRAL
	CreateHudObject("RageGlow.png",			(int)xSafe, (int)ySafe, 1.0,  1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the glow for the sword when the player is raging
	CreateHudObject("HealthBar.png",		(int)xSafe + barLeft, (int)ySafe + healthTop, 1.0f, 1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the health bar
	CreateHudObject("RageBar.png",			(int)xSafe + barLeft, (int)ySafe + rageTop,   1.0f, 1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the rage bar
	CreateHudObject("FullRage.png",			(int)xSafe + barLeft, (int)ySafe + rageTop,   1.0f, 1.0f, 0, D3DCOLOR_XRGB(255, 255, 255), true, 0.6f, 0.24f);		// the rage bar when full
	
}

///////////////////////////////////////////////////
//	Function	:	"CreateHudObject"
//	
//	Last Modified:	January 23, 2013 (SMS)
//
//	Input		:	none
//										
//	Return		:	none
//
//	Purpose		:	internal function to add a picture and its values to the HUD's map
//////////////////////////////////////////////////
void CHud::CreateHudObject(string fileName, int nPosX, int nPosY, float fScaleX, float fScaleY, float fRotation, DWORD dwColor, bool bScaleToScreen, float xScreenRatio, float yScreenRatio)
{
	hudObjects[fileName].name = fileName.c_str();
	hudObjects[fileName].posX = nPosX;
	hudObjects[fileName].posY = nPosY;
	hudObjects[fileName].Xscale = fScaleX;
	hudObjects[fileName].Yscale = fScaleY;
	hudObjects[fileName].baseXScale = fScaleX;
	hudObjects[fileName].baseYScale = fScaleY;
	hudObjects[fileName].rotation = D3DXToRadian(fRotation);
	hudObjects[fileName].color = dwColor;

	string texturePath = "HUD/";
	texturePath += fileName;

	hudObjects[fileName].imageID = CView::LoadSprite(texturePath.c_str(), &hudObjects[fileName].width, &hudObjects[fileName].height);
	hudObjects[fileName].centerX = hudObjects[fileName].width >> 1;
	hudObjects[fileName].centerY = hudObjects[fileName].height >> 1;

	if (bScaleToScreen)
	{
		// scale so the image covers xScreenRatio percentage of the screen horizontally
		float screenRatio = xScreenRatio * CGame::GetInstance()->GetScreenWidth();
		hudObjects[fileName].baseXScale = screenRatio / hudObjects[fileName].width;	
		hudObjects[fileName].Xscale = hudObjects[fileName].baseXScale;

		// scale so the image covers yScreenRatio percentage of the screen vertically
		screenRatio = yScreenRatio * CGame::GetInstance()->GetScreenHeight();
		hudObjects[fileName].baseYScale = screenRatio / hudObjects[fileName].height;
		hudObjects[fileName].Yscale = hudObjects[fileName].baseYScale;
	}
}

void CHud::Draw(string fileName)
{
	const char* filename = fileName.c_str();
	CView::Draw(hudObjects[fileName].imageID, hudObjects[fileName].posX, hudObjects[fileName].posY,
		hudObjects[fileName].Xscale, hudObjects[fileName].Yscale, 0, (float)hudObjects[fileName].centerX, (float)hudObjects[fileName].centerY, 
		hudObjects[fileName].rotation, hudObjects[fileName].color);
}

///////////////////////////////////////////////////
//	Function	:	"DrawARGB"
//	
//	Last Modified:	January 23, 2013 (SMS)
//
//	Input		:	fileName	the file name of the image you wish to draw
//					argbColor	the ARGB color change to make to the picture
//										
//	Return		:	none
//
//	Purpose		:	draws the image with a tint color and alpha value
//////////////////////////////////////////////////
void CHud::DrawARGB(string fileName, DWORD argbColor)
{
	const char* filename = fileName.c_str();
	CView::Draw(hudObjects[fileName].imageID, hudObjects[fileName].posX, hudObjects[fileName].posY,
		hudObjects[fileName].Xscale, hudObjects[fileName].Yscale, 0, (float)hudObjects[fileName].centerX, (float)hudObjects[fileName].centerY, 
		hudObjects[fileName].rotation, argbColor);
}

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
void CHud::DrawHealthAndRage(CPlayerObject* player, int left, int top)
{
	if (player->CanRage())
	{
		m_bFlashGlowRage = true;
	}
	else
	{
		m_bFlashGlowRage = false;
	}

	// First draw the sword's glow in the background, but only if the player is raging or can rage	
	if (m_bFlashRageGlowOnce || (player->CanRage() && m_bFlashGlowRage) )
	{
		DrawARGB("RageGlow.png", D3DCOLOR_ARGB((int)(m_fRageGlowAlpha * 255), 255, 255, 255));
	}
	else if (player->IsRaging())
	{
		Draw("RageGlow.png");
	}

	// now draw the health and rage bars
	SetXScale("HealthBar.png", (player->GetHealth()/player->GetMaxHealth()) * GetBaseXScale("HealthBar.png"));
	Draw("HealthBar.png");

	// if the player's rage meter is maxed or he is raging
	if (player->IsRaging() || player->CanRage())
	{
		SetXScale("FullRage.png", (player->GetRage()/(float)player->GetMaxPlayerRage()) * GetBaseXScale("FullRage.png"));	
		Draw("FullRage.png");
	}
	else // draw the normal rage bar as it's filling up
	{
		SetXScale("RageBar.png", (player->GetRage()/(float)player->GetMaxPlayerRage()) * GetBaseXScale("RageBar.png"));	
		Draw("RageBar.png");
	}

	/* lastly, draw the sword */
	// if the player is HOT
	if (player->GetElementalState() > CCollidableObject::NEUTRAL)
	{
		Draw("FireSword.png");
	}
	// if the player is COLD
	else if (player->GetElementalState() < CCollidableObject::NEUTRAL)
	{
		Draw("IceSword.png");
	}
	// if the player is in a normal state
	else
	{
		Draw("NormalSword.png");
	}	
}

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
void CHud::FlashRageGlow()
{
	m_bFlashRageGlowOnce = true;
	m_fRageGlowAlpha = 0;
	m_bRageGlowAlphaUp = true;
}

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
void CHud::DrawDamageSplash()
{
	RECT sourceRect;
	sourceRect.left = 0;
	sourceRect.right = 1280;
	sourceRect.top = 0;
	sourceRect.bottom = 720;
	CView::Draw(hudObjects["DamageHUDOverlay2.png"].imageID,0, 0, (float(CGame::GetInstance()->GetScreenWidth()) / sourceRect.right),
		(float(CGame::GetInstance()->GetScreenHeight()) / sourceRect.bottom), &sourceRect, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - CWorldManager::GetInstance()->GetHealthScale()));
}

void CHud::DrawEffectSplash(int nState)
{
	static float fAlpha = 0.0f;
	static float fAdd = 0.025f;
	static float fFade = 0.0f;

	if(nState < CCollidableObject::NEUTRAL)
	{
		float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(hudObjects["ColdOverlay.png"].width);
		float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(hudObjects["ColdOverlay.png"].height);
		CView::Draw(hudObjects["ColdOverlay.png"].imageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
		m_nPreviousHUD = -1;
		fFade = fAlpha;
	}
	else if(nState > CCollidableObject::NEUTRAL)
	{
		float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(hudObjects["HotOverlay.png"].width);
		float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(hudObjects["HotOverlay.png"].height);
		

		CView::Draw(hudObjects["HotOverlay2.png"].imageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
		CView::Draw(hudObjects["HotOverlay3.png"].imageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - fAlpha));
		m_nPreviousHUD = 1;
		fFade = fAlpha;
	}
	else if(m_nPreviousHUD){
		fFade -= 0.01f;

		if(fFade > 0.0f){
			if(m_nPreviousHUD == -1){
				float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(hudObjects["ColdOverlay.png"].width);
				float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(hudObjects["ColdOverlay.png"].height);
				CView::Draw(hudObjects["ColdOverlay.png"].imageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fFade));
			}
			else{
				float fScaleX = CGame::GetInstance()->GetScreenWidth() / float(hudObjects["HotOverlay.png"].width);
				float fScaleY = CGame::GetInstance()->GetScreenHeight() / float(hudObjects["HotOverlay.png"].height);
		
				CView::Draw(hudObjects["HotOverlay2.png"].imageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fFade));
				CView::Draw(hudObjects["HotOverlay3.png"].imageID, 0, 0, fScaleX, fScaleY, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, fFade));
			}
		}
		else{
			fFade = 0.0f;
			m_nPreviousHUD = 0;
		}
	}

	fAlpha += fAdd;
	if(fAlpha >= 1.0f)
	{
		fAlpha = 1.0f;
		fAdd = -fAdd;
	}
	else if(fAlpha <= 0.0f)
	{
		fAlpha = 0.0f;
		fAdd = -fAdd;
	}
}
