#if 0
/***********************************************
 * Filename:  		AnimationPlayer.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		To allow each animated object
 *					to control when its animation
 *					can play using an indexed vector
 *					of Animations that it accesses
 *					from the Animation Manager
 ************************************************/

#ifndef _ANIMATION_PLAYER_H_
#define _ANIMATION_PLAYER_H_

#include "../Renderer/RenderDefinitions.h"
#include "Animation.h"

class CRenderInstance;
class CAnimation;
class CModel;
 
class CAnimationPlayer
{
	friend class CAnimationManager;
	friend class CView;
	friend class CAnimationBlender;
	friend class CRenderInstance;

	// Pointer to the render instance we're attached to
	CRenderInstance*	m_pRenderInstance;

	float				m_fTime;


	CModel*				pModel;
	CAnimationBlender*	m_cBlender;
	CAnimation*			m_pAnimation;
	CAnimation*			m_pBlendAnimation;
	bool				m_isPlaying;

public:
	vector<ID3DXMesh*> m_vpSphereMeshes;
	
	/*****************************************************************
	* CAnimationPlayer():	Default constructor for the Animation Player
	*						that initializes all variables to safe values
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CAnimationPlayer(void);

	/*****************************************************************
	* ~CAnimationPlayer():	Default destructor for the Animation Player
	*						de-initializes all its variables
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	~CAnimationPlayer(void);

	/*****************************************************************
	* SetModelID():			Sets the model ID for the player for later use
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void SetModelID(int nModelID);

	/*****************************************************************
	* GetModelID():			Gets the ID of the model to use as an index
	*						into the mesh manager's container of meshes
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				int
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	//int GetModelID(void) { return m_nModelID; }


	/*****************************************************************
	* Render():				Renders the verts of the model
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void Render(int nShaderID);


	/***********************************************************************************
	* Play():		Sets the current animation's flag to playing; Checks to 
	*				see if there is another animation that is waiting to be played.  
	*				If there is, it sets another CAnimation* to the Animation Manager's 
	*				indexed animation.
	*
	* Ins:					const int, bool
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/08/2012
	* Mod. Initials:	    JM
	***********************************************************************************/

	void Play(const int nID, bool bIsLooping = true, double dTime = 0.0);

	////////////////////////////////////////////////////////////////////////
	// Name: Stop														  //
	//																	  //
	// Parameters: ID - the index of the animation being played			  //
	//																	  //
	// Return: void														  //
	//																	  //
	// Description: Sets the IS_PLAYING flag of theAnimation at the		  //
	//              passed index to false								  //
	////////////////////////////////////////////////////////////////////////

	/*****************************************************************
	* Stop():				Toggles the Player's animation flag
	*						into the mesh manager's container of meshes
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				int
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void Stop(const int nID = -1);

	////////////////////////////////////////////////////////////////////////
	// Name: Reset														  //
	//																	  //
	// Parameters: ID - the index of the animation being played			  //
	//																	  //
	// Return: void														  //
	//																	  //
	// Description: Sets the IS_PLAYING flag of theAnimation at the		  //
	//              passed index to true								  //
	////////////////////////////////////////////////////////////////////////

	/*****************************************************************
	* Reset():				Sets the Player's timer to 0.0 to allow the
	*						Player to access the first KeyFrame
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void Reset(void);

	/*****************************************************************
	* Update():				Updates the Player's Animation Blender
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/09/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void Update(float fElapsedTime);

	/*****************************************************************
	* GetCurAnimation():	Returns the Player's m_pAnimation
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CAnimation*
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CAnimation* GetCurAnimation(void)const {return m_pAnimation;}

	/*****************************************************************
	* GetAnimationBlender(): Returns the Player's Animation Blender
	*						 instance
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CAnimationBlender
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CAnimationBlender* GetAnimationBlender(void) {return m_cBlender;}

	/*****************************************************************
	* SetRenderInstance():	Sets the Player's RenderInstance pointer to
	*						the passed in value
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void SetRenderInstance(CRenderInstance *pRInstance) {m_pRenderInstance = pRInstance;}
};

#endif // _ANIMATION_PLAYER_H_

#endif