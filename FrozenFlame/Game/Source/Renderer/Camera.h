/********************************************************************
* Filename:			Camera.h
* Date:				10/29/2012
* Mod. Date:		11/12/2012
* Mod. Initials:	DL
* Author:			Daniel Lima
* Purpose:			This module controls the view and the projection
*					matrices of the game and provides an interface for
*					locking the camera to the player or having it
*					move through a queue of waypoints.
********************************************************************/

// Header protection
#ifndef _CAMERA_H_
#define _CAMERA_H_

// Includes
#include <d3d9.h>
#include <d3dx9.h>

// Queue for the waypoints
#include <queue>
using std::queue;

#include "../Collision/ED2CollisionLibrary.h"
#include "../Util/vec3.h"

// Forward declarations
class CBaseObject;
class CRenderInstance;
class IRenderThing;

// If the distance between the camera and a waypoint is less than this epsilon value, we consider
// the waypoint reached.
#define CAM_EPSILON 1.5f

// Camera class
class CCamera
{
	friend class CRenderer;
	friend class CView;

	// Camera View matrix
	D3DXMATRIX		m_d3dViewMat;
	// Camera Projection matrix
	D3DXMATRIX		m_d3dProjMat;

	// Precalculated concatenations
	D3DXMATRIX		m_d3dViewProj;
	D3DXMATRIX		m_d3dInvViewProj;

	// Camera flags
	unsigned char	m_chFlags;

	D3DXVECTOR3		m_d3dCamPos;
	
	enum ECamFlags {
		CAM_FOLLOW = 1,		// true - Follow the player			false - Follow waypoints
		CAM_TRANSITION = 2, // true - Transition to next point	false - Hard attach to next point
		CAM_SHAKE = 4,		// true - Currently shaking			false - Not currently shaking
		CAM_SHAKE_UP = 8,	// true - Shake currently up		false - Shake currently down
		CAM_WAITING = 16,	// true - Currently waiting			false - Not currently waiting
	};

	// For when the camera is shaking
	float			m_fShakeDuration;
	float			m_fShakeAmount;

	// Keep a pointer to the player so we can follow him
	//CPlayerObject*	m_pPlayer;
	CBaseObject*	m_pPlayer;

	// Precalculated camera transform that is set on the update
	D3DXMATRIX		m_d3dCamTransform;
	// Speed of the camera when transitioning
	float			m_fCamSpeed;
	
	// Time, in seconds, to wait when a waypoint is reached
	float			m_fWaitTime;
	// Time, in seconds, already waited in the current waypoint
	float			m_fTimeCurrentlyWaited;

	// Points to lock to when we're not following the player
	// Everytime the camera reaches one of this points, we pop it from the queue
	queue<D3DXVECTOR3> m_WayPoints;

	// camera zoom
	bool  m_bCameraIsZooming;			// flag for whether the camera should be zooming during Update()
	bool  m_bHoldCameraZoom;			// true - hold the camera's position when done zooming	false - go back to hard attach when finished zooming
	D3DXVECTOR3 m_vStartZoomPosition;	// the position where the camera was when it started zooming
	float m_fZoomElapsedTime;			// how long the camera has been zooming
	float m_fZoomDurationTime;			// how long to zoom the camera
	float m_fZoomDistance;				// The distance to zoom in or out, set each time ZoomCamera() is called
	float m_fHardAttachDefaultDistance; // (0, 0, -45)
	float m_fHardAttachCurrentDistance;	// if the zoom is held, this holds the current Z offset for the hard attach
	D3DXVECTOR3 m_vPositionLastFrame;	// the camera's position last frame
	vec3f  m_vTargetOffset;				// offset the postion of the target lock by this

	float m_fFOV;
	float m_fRatio;
	float m_fDistNear;
	float m_fDistFar;

	// for camera culling
	float m_fMinZ;
	float m_fMaxZ;

	D3DXMATRIX m_d3dInvCamera;

	// Returns true if our delta is close enough to zero for the camera
	// to start moving towards the next waypoint
	inline bool ReachedWaypoint(float deltaX, float deltaY, float deltaZ)
	{ return (abs(deltaX) < CAM_EPSILON && abs(deltaY) < CAM_EPSILON && abs(deltaZ) < CAM_EPSILON); }
	inline bool ReachedWaypoint(const D3DXVECTOR3& delta)
	{ return (abs(delta.x) < CAM_EPSILON && abs(delta.y) < CAM_EPSILON && abs(delta.z) < CAM_EPSILON); }

	void UpdateLinearZoom(float fElapsedTime, D3DXMATRIX hardAttachTarget);

public:

	/********************************************************************
	* Initialize():			Initializes the view and the projection matrices
	* Ins:					Dimensions of the window
	* Returns:				void
	* Mod. Date:			11/10/2012
	* Mod. Initials:		DL
	********************************************************************/
	void Initialize(int nWindowWidth, int nWindowHeight);

	/********************************************************************
	* Update():				Updates the view matrix based on the current
	*						camera behavior
	* Ins:					Time elapsed since last frame
	* Returns:				void
	* Mod. Date:			11/10/2012
	* Mod. Initials:		DL
	********************************************************************/
	void Update(float fElapsedTime);

	/********************************************************************
	* AddWayPoint():		Adds a position to the queue of waypoints
	* Ins:					Vector of the position to be added
	* Returns:				void
	* Mod. Date:			11/10/2012
	* Mod. Initials:		DL
	********************************************************************/
	void AddWayPoint(float fX, float fY, float fZ);
	void AddWayPointToPlayer(void);

	/********************************************************************
	* LockToPlayer():		Sets the camera to be locked to the player position
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/10/2012
	* Mod. Initials:		DL
	********************************************************************/
	void LockToPlayer(void);

	/********************************************************************
	* LockToPlayer():		Sets the camera to start panning through the waypoints
	* Ins:					void
	* Returns:				void
	* Mod. Date:			11/10/2012
	* Mod. Initials:		DL
	********************************************************************/
	void StartPanning(void);

	/********************************************************************
	* ZoomCamera():			Zooms the camera in or out over the course of the duration,
	*						moving linearly from start to end position zoomDistance away from start
	* Ins:					zoomDistance - the distance you want the camera to zoom
	*						   positive to zoom forward, negative value to zoom back\
	*						zoomDurationTime - it will take this long, in seconds
	*						   to zoom in or out the zoomDistance
	* Returns:				void
	* Mod. Date:			02/15/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void ZoomCamera(float zoomDistance, float zoomDurationTime, bool bHoldZoomPosition = true);

	/********************************************************************
	* RestoreDefaultCamera(): turns off zoom and resets the hard attach distance
	* Ins:					none
	* Returns:				void
	* Mod. Date:			02/18/2013
	* Mod. Initials:		SMS
	********************************************************************/
	void RestoreDefaultCamera();

	/********************************************************************
	* Shake():				Sets the camera to start shaking
	* Ins:					fShakeAmount - intensity of "shakiness"
	*						fDuration - duration in seconds
	* Returns:				void
	* Mod. Date:			11/11/2012
	* Mod. Initials:		DL
	********************************************************************/
	void Shake(float fShakeAmount, float fDuration);

	void TurnOffShake()						{ m_chFlags  &= ~CAM_SHAKE; }

	void Clear(void);

	void ClearOffset()						{ m_vTargetOffset = vec3f(0, 0, 0); }

	bool IsObjectInside(IRenderThing* pInst);


	vec3f GetPositionInFrontOfCamera(float fX, float fY, float fZ);
	
	/***************
	*	Accessors
	***************/
	inline const D3DXMATRIX& GetViewProjection(void) const		{ return m_d3dViewProj; }
	inline const D3DXMATRIX& GetInvViewProjection(void) const	{ return m_d3dInvViewProj; }
	inline const D3DXVECTOR3& GetPosition(void) const			{ return m_d3dCamPos; }
	inline void  StopTransitions(void)							{ m_chFlags &= ~CAM_TRANSITION; }
	inline bool  IsTransitioning(void) const					{ return (m_chFlags & CAM_TRANSITION) ? true : false; }
	inline bool  IsZoomBeingHeld(void) const					{ return m_bHoldCameraZoom; }
	inline bool  isFollowing(void) const						{ return (m_chFlags & CAM_FOLLOW) ? true : false; }

	D3DXVECTOR3 GetCenterPos(void) const;
	float GetRadius(void) const;
	const D3DXVECTOR3 GetPlayerPos(void) const;
	float GetDistanceSquared(const IRenderThing* pRenderThing) const;

	/***************
	*	Mutators
	***************/
	void SetPlayer(CBaseObject* pPlayer);
	void SetCurPos(vec3f& in);
	void SetHoldingTheZoom(bool bHoldZoomPosition)				{ m_bHoldCameraZoom = bHoldZoomPosition; }
	void SetOffset(vec3f offset)								{ m_vTargetOffset = offset; }

};


#endif