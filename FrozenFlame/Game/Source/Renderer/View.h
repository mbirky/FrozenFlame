/********************************************************************
* Filename:			View.h
* Date:				10/25/2012
* Mod. Date:		11/5/2012
* Mod. Initials:	MB
* Author:			Daniel Lima
* Purpose:			Wrapper to all rendering and animation related engines.
********************************************************************/
#ifndef _VIEW_H_
#define _VIEW_H_

#include <list>
using std::list;
#include "Renderer.h"

// Forward declarations
class CPlayerObject;
class CBaseObject;
class CCollidableObject;
class CFontManager;
class CEmitterManager;
class CMeleeEnemy;
class CAnimPlayer;
class CHud;
class CLight;
class CDirLight;

// View class
class CView
{
	// Friend classes
	friend class CRenderer;
	friend class CModel;
	friend class CRenderInstance;
	friend class CCollidableObject;
	friend class CRenderTarget;
	friend class CAssetManager;
	friend class CEmitter;
	friend class CPointLight;
	friend class CDynamicShape;
	friend class CPostProcess;
	friend class CRenderInstanceAnimated;
	friend class CRenderInstanceParticles;

	// The Renderer is part of the View. All classes that need to
	// directly access the Renderer are friends of the View. The
	// idea is that those would only be graphics-related classes.
	static CRenderer			sm_cRenderer;

	// The Font Manager is part of the View. It has functions that
	// wrap its functionality (like "LoadFont" and "Write)
	static CFontManager			sm_cFontManager;

	// The Emitter Manager is responsible for the storing, updating, and removal of emitters.
	static CEmitterManager		sm_cEmitterManager;

	// Initialize Flags
	static bool					sm_bInitialized;		// Is the View initialized?
	static bool					sm_bSceneStarted;		// Have we called BeginScene for rendering?
	static bool					sm_bSpriteStarted;		// Have we called BeginSprite?

	// Render Flags
	static bool					sm_bRenderCollisionBoxes;
	static bool					sm_bRenderCollisionSpheres;
	static bool					sm_bRender2dParticles;
	static bool					sm_bRenderDamage;

	// For Fading
	static char					sm_cFadeState;		// The state that the fade is in
	static float			    sm_fFade;			// The fade alpha
	static TImage				sm_tFade;			// The image to fade to

	static int					sm_nFontID;

	static CPostProcess*		sm_pRageEffect;

	static const CPostProcess*	sm_pHeatDistortion;
	static float				sm_fHeatOffset;

	// For buttons in menues
	static TImage				sm_tButtonNormal;
	static TImage				sm_tButtonHover;

public:
	/********************************************************************
	* Initialize():			Initializes the Renderer and the Animation Manager
	* Ins:					hWnd - handle to the window we're rendering to
	* Returns:				A value from EViewCodes that informs if the function
	*						succeeded or failed on initializing the Renderer
	* Mod. Date:			11/14/2012
	* Mod. Initials:		DL
	********************************************************************/
	static int	Initialize(HWND hWnd, int nClientWidth, int nClientHeight);

	/********************************************************************
	* Update():				Updates the Renderer and all the damage images
	* Ins:					fElapsedTime - time elapsed since last frame
	* Returns:				void
	* Mod. Date:			11/14/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Update(float fElapsedTime);

	/********************************************************************
	* Render():				Calls the Renderer's Render() function to
	*						render all the render instances
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/14/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Render(void);
	static void RenderMenuModels(int nBackgroundID);

	/********************************************************************
	* Shutdown():			Cleans up all the allocated memory
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/14/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Shutdown(void);

	/********************************************************************
	* Push():				Pushes a new Render Instance to the Renderer
	*						based on the passed in object and model file
	* Ins:					pObject - object the instance is attached to
	*						szModel - relative name of the model
	*						ucFlags
	*						 - RENDER_INVISIBLE - True if RI should not be visible
	*						 - RENDER_TRANSPARENT - True if RI should be transparent
	*						 - RENDER_NOTSHADOWCASTER - True if RI should not cast shadows
	* Returns:				The ID of the instance on the Renderer's vector
	*						or -1 if failed
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static int	Push(CBaseObject* pObject, string szModel, unsigned short ucFlags = 0);
	static int	PushAttachment(int nRenderInstID, string szAttachmentModel, string szJointToAttach, string szAttachmentJoint = "", unsigned short ucFlags = 0);

	/********************************************************************
	* Push():				Removes the Render Instance of the passed in
	*						ID from the Renderer's vector
	* Ins:					nRenderInstID - the ID of the instance
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Pop(int nRenderInstID);

	/********************************************************************
	* Clear():				Clears the whole vector of Render Instances
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Clear(void);

	/********************************************************************
	* Start():				Begins the scene for rendering and clears the
	*						color, depth and stencil buffers
	* Ins:					d3dColor - the color to clear the screen to
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Start(D3DCOLOR d3dColor = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));

	/********************************************************************
	* Finish():				Ends the scene for rendering, ends the sprite
	*						manager, if it has been initialized, and
	*						presents the backbuffer
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Finish(void);

	/********************************************************************
	* LoadSprite():			Loads a sprite using the passed in filename
	* Ins:					szFilename - file to load the texture from
	* Outs:					pnWidth - the width of the loaded texture
	*						pnHeight - the height of the loaded texture
	* Returns:				The ID of the sprite on the texture manager
	* Mod. Date:			11/14/2012
	* Mod. Initials:		DL
	********************************************************************/
	static int LoadSprite(const char* szFilename, int* pnWidth = nullptr, int* pnHeight = nullptr);
	
	/********************************************************************
	* LoadFont():			Loads a font using the passed in filename
	* Ins:					szFilename - file to load the font from
	* Returns:				The ID of the font on the font manager
	* Mod. Date:			11/14/2012
	* Mod. Initials:		DL
	********************************************************************/
	static int LoadFont(const char* szFilename);

	/********************************************************************
	* Write():				Writes text to the screen using a bitmap font,
	*						needs to be called every frame while the text
	*						should be being displayed
	* Ins:					nFontID - ID of the font to use
	*						szText - text to be print
	*						pArea - area to render to
	*						bCentered - true for centered, false for left-align
	*						fScale - scale of the text
	*						d3dColor - color to write the text with
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Write(int nFontID, string szText, RECT* pArea, bool bCentered, float fScale = 1.0f, D3DCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	/********************************************************************
	* Write():				Writes text to the screen using a bitmap font,
	*						needs to be called every frame while the text
	*						should be being displayed
	* Ins:					nFontID - ID of the font to use
	*						szText - text to be print
	*						pArea - area to render to
	*						bCentered - true for centered, false for left-align
	*						fScaleX - scale of the text on the X
	*						fScaleY - scale of the text on the Y
	*						d3dColor - color to write the text with
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Write(int nFontID, string szText, RECT* pArea, bool bCentered, float fScaleX, float fScaleY, D3DCOLOR d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	/********************************************************************
	* Draw():				Draws a sprite using the passed in parameters
	* Ins:					nSpriteID - ID of the sprite to draw
	*						fPosX/Y - position on the screen to render
	*						fScaleX/Y - scale of the sprite
	*						pSourceRect - source rectangle on the sprite
	*						fRotCenterX/Y - position of the center of
	*							rotation of the sprite
	*						fRotation - angle of rotation of the sprite
	*						dwColor - color to be applied to the sprite
	* Returns:				void
	* Mod. Date:			11/13/2012
	* Mod. Initials:		DL
	********************************************************************/
	static void Draw(int nSpriteID, int nPosX, int nPosY, float fScaleX = 1.0f, float fScaleY = 1.0f,
		RECT* pSourceRect = nullptr, float fRotCenterX = 0.0f, float fRotCenterY = 0.0f, float fRotation = 0.0f, DWORD dwColor = 0xFFFFFFFF);


	// From here on the commenting is not following the standard but it will
	// be changed later. Probably. Eventually.
	// - Dan

	// DEPRECATED FUNCTION
	static CLight* AttachLight(int nRenderInstID, const vec4f& vDiffColor, float fRange = 10.0f,
		const vec4f& vAmbientColor = vec4f(0.1f, 0.1f, 0.1f, 0.1f), const vec4f& vSpecColor = vec4f(1.0f, 1.0f, 1.0f, 1.0f),
		float fSpecPower = 16.0f, float fSpecIntensity = 3.0f, const vec3f& vAttenuation = vec3f(0.1f, 0.015f, 0.035f),
		const vec3f& vLightPositionOffset = vec3f(0, 0, 0)) { return nullptr; }

	// Returns a pointer to the animation player of the render instance of
	// the passed in ID, if there is one.
	static CAnimPlayer* GetAnimPlayer(int nRenderInstID);

	// Gets an AABB surrouding the object of the passed in render ID,
	// vMin and vMax are output parameters that define the AABB
	static void GetCollisionAabb(int nRenderInstID, vec3f& vMin, vec3f& vMax);
	
	// Gets a sphere surrounding the object of the passed in render ID,
	// vPos and fRadius are output parameters that define the sphere 
	static void GetCollisionSphere(int nRenderInstID, vec3f& vPos, float& fRadius);

	// Creates a dynamic shape and returns its ID
	static int DynamicShapeCreate(int nRenderInstID, bool bAttachedRotation = false, vec3f scale = vec3f(1.0f, 1.0f, 1.0f));

	// Makes the dynamic shape of the passed in ID into a plane with the passed in width and depth
	static void DynamicShapeMakePlane(int nShapeID, float fWidth, float fDepth);

	// Sets the local matrix of the dynamic shape of the passed in ID to the passed in matrix
	static void DynamicShapeSetLocalMatrix(int nShapeID, const matrix4f& tMatrix);

	// Sets the textures and a color multiplier of the dynamic shape of the passed in ID
	static void DynamicShapeSetTextures(int nShapeID, int nDiffuseID, int nSpecularID, const vec4f& color = vec4f(1, 1, 1, 1));

	// Removed the Dynamic Shape from the Renderer
	static void DynamicShapeRemove(int nShapeID);

	static void SetAttachmentEnabled(int nAttachmentID, bool bEnabled);

	static void FlashRenderInstance(int nRenderInstID, vec4f vColor, float fDuration = 0.1f);

	static void SetIsVisible(int nRenderInstID, bool bIsVisible);

	static void SetCameraToPan(void);
	
	///////////////////////////////////////////
	// New Light System
	///////////////////////////////////////////

	// Makes a light and attaches it to the render instance of the passed in ID.
	// This will cause the light to stay at this render instance's position plus the passed in offset,
	// and it will update itself as the render instance is updated.
	static int MakeLight(int nOwnerRenderID, bool bStartEnabled = true, const vec3f& vOffset = vec3f(0, 0, 0), const vec4f& vDiffuseColor = vec4f(1, 1, 1, 1),
		float fRange = 10.0f, const vec4f& vAmbientColor = vec4f(0.1f, 0.1f, 0.1f, 0.1f), const vec4f& vSpecularColor = vec4f(1, 1, 1, 1),
		float fSpecPower = 16.0f, float fSpecIntensity = 3.0f, const vec3f& vAttenuation = vec3f(0.1f, 0.015f, 0.035f));

	// Makes a static light fixed at the passed in world position.
	static int MakeLight(const vec3f& vPosition, bool bStartEnabled = true, const vec4f& vDiffuseColor = vec4f(1, 1, 1, 1), float fRange = 10.0f,
		const vec4f& vAmbientColor = vec4f(0.1f, 0.1f, 0.1f, 0.1f), const vec4f& vSpecularColor = vec4f(1, 1, 1, 1),
		float fSpecPower = 16.0f, float fSpecIntensity = 3.0f, const vec3f& vAttenuation = vec3f(0.1f, 0.015f, 0.035f));

	// Destroys the light of the passed in ID.
	static void DestroyLight(int nLightID);

	// Returns a const handle to the light of the passed in ID.
	static const CPointLight* GetLight(int nLightID);

	// Sets the light of the passed in ID to be enabled or disabled.
	static void SetLightEnabled(int nLightID, bool bEnabled);

	// Sets the diffuse color of the light of the passed in ID.
	static void SetLightDiffuseColor(int nLightID, const vec4f& vDiffuseColor);

	// Sets the offset of the light of the passed in ID.
	static void SetLightOffset(int nLightID, const vec3f& vOffset);

	///////////////////////////////////////////

	// So that when the player is hit the view can render the damage being made - Roberto
	static void SetDisplayDamage(bool bOn) {sm_bRenderDamage = bOn;}

	// this will render our fading for transitions - Roberto
	static void Fade(void);

	// Allows objects to get access to their attachments for *reasons* - JC
	static CRenderInstance* GetAttachmentInstance(int nRenderInstID, int nAttachmentID);

	static void SetRenderInstanceFlags(int nRenderInstID, unsigned short ucFlags);
	static void SetViewFlags(unsigned char ucFlags);

	// Allows objects to access their RenderInstance
	static const CRenderInstance* GetRenderInstance(int nRenderInstID);

	/********************************************************************
	* ResetFade():			This function sets the fade value to 0.0 and 
	*						makes the fade state set to FADE_NOT
	*						
	* Ins:					void
	*
	* Returns:				void
	* Mod. Date:			2/9/2013
	* Mod. Initials:		MB
	********************************************************************/
	static void ResetFade(void);

	// this will update our fading for transitions - Roberto
	static void UpdateFade(float _fElapsedTime);

	static void StartRaging(void);
	static void StopRaging(void);

	static void DrawButton(int _nMidX, int _nMidY, float _fScaleX, float _fScaleY, string _szText, bool _bHover, D3DXCOLOR d3dButtonColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), float _fNumButtonWidthOffsets = 0.0, float _fNumButtonHeightOffsets = 0.0, D3DXCOLOR d3dTextColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0));

	//////////////////////////////////////////////////////////////////////////
	//	------------------------------------------------------------------	//
	//								ACCESSORS								//
	//	------------------------------------------------------------------	//
	//////////////////////////////////////////////////////////////////////////	
	static inline int GetScreenW(void)					{ return sm_cRenderer.GetScreenWidth(); }
	static inline int GetScreenH(void)					{ return sm_cRenderer.GetScreenHeight(); }
	static CCamera& GetCamera(void)						{ return sm_cRenderer.GetCamera(); }	
	static CEmitterManager& GetEmitterManager()			{ return sm_cEmitterManager;}
	static bool GetDisplayDamage(void)					{ return sm_bRenderDamage;}
	static inline float GetFade(void)					{ return sm_fFade;}
	static inline bool GetRenderCollisionBoxes(void)	{return sm_bRenderCollisionBoxes;}
	static inline bool GetRenderCollisionSpheres(void)	{return sm_bRenderCollisionSpheres;}
	static inline bool GetRender2dParticles(void)		{return sm_bRender2dParticles;}
	static inline char GetFadeState(void)				{return sm_cFadeState;}	

	//////////////////////////////////////////////////////////////////////////
	//	------------------------------------------------------------------	//
	//								Mutators								//
	//	------------------------------------------------------------------	//
	//////////////////////////////////////////////////////////////////////////	
	static inline void SetFadeState(char _cFadeState)							{sm_cFadeState = _cFadeState;}
	static inline void SetRenderCollisionBoxes(bool _bRenderCollisionBoxes)		{sm_bRenderCollisionBoxes = _bRenderCollisionBoxes;}
	static inline void SetRenderCollisionSpheres(bool _bRenderCollisionSpheres)	{sm_bRenderCollisionSpheres = _bRenderCollisionSpheres;}
	static inline void SetRender2dParticles(bool _bRender2dparticles)			{sm_bRender2dParticles = _bRender2dparticles;}
};

#endif