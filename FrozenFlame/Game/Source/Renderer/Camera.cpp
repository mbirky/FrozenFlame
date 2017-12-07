
// Our header
#include "Camera.h"

// We got a pointer to player object
#include "../Objects/PlayerObject.h"
#include "RenderInstance.h"
#include "../Core/CWorldManager.h"


void CCamera::Initialize(int nWindowWidth, int nWindowHeight)

{
	// Initialize the camera panning speed
	m_fCamSpeed = 40.0f;
	m_fWaitTime = 1.5f;
	m_fTimeCurrentlyWaited = 0.0f;
	m_chFlags = 0;

	m_fMinZ = 0.0f;
	m_fMaxZ = 0.0f;

	// Initialize Projection Matrix
	m_fFOV = D3DXToRadian(37.5f);
	m_fRatio = nWindowWidth / (float)nWindowHeight;
	m_fDistNear = 1.0f;
	m_fDistFar = 180.0f;
	D3DXMatrixPerspectiveFovLH(&m_d3dProjMat, m_fFOV, m_fRatio, m_fDistNear, m_fDistFar);

	// Initialize the camera transform
	D3DXMATRIX d3dTranslate, d3dRotate;
	D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, -45.0f);
	D3DXMatrixRotationYawPitchRoll(&d3dRotate, D3DXToRadian(135.0f), D3DXToRadian(45.264f), 0.0f);

	m_d3dCamTransform = d3dTranslate * d3dRotate;

	m_bHoldCameraZoom = false;
	m_fHardAttachDefaultDistance = -45.0f;
	m_fHardAttachCurrentDistance = m_fHardAttachDefaultDistance;

	m_vTargetOffset = vec3f(0, 0, 0);
}
void CCamera::Update(float fElapsedTime)
{
	// If we're waiting, wait until we're not waiting anymore
	if(m_chFlags & CAM_WAITING)
	{
		m_fTimeCurrentlyWaited += fElapsedTime;
		if(m_fTimeCurrentlyWaited >= m_fWaitTime)
		{
			// We reached the time so stop waiting
			m_chFlags &= ~CAM_WAITING;
			m_fTimeCurrentlyWaited = 0.0f;
		}
		else
		{
			// We haven't reached the time so do nothing and return
			return;
		}
	}


	// Matrix that represents the object the camera will be focused on
	D3DXMATRIX d3dObject;

	// If we're following the player, set our object matrix to a translation to the player position
	if(m_chFlags & CAM_FOLLOW)
	{
		if(!m_pPlayer || !m_pPlayer->GetInitialized()) return;
		D3DXMatrixTranslation(&d3dObject, m_pPlayer->GetWorldMatrix()->axis_pos.x + m_vTargetOffset.x, 
			m_pPlayer->GetWorldMatrix()->axis_pos.y + m_vTargetOffset.y, m_pPlayer->GetWorldMatrix()->axis_pos.z + m_vTargetOffset.z);
	}
	// Else, set it to the first waypoint
	else if(!m_WayPoints.empty())
	{
		D3DXMatrixTranslation(&d3dObject, m_WayPoints.front().x, m_WayPoints.front().y, m_WayPoints.front().z);
	}
	else return;

	// If we're transitioning, find a delta and move the matrix in that direction
	if((m_chFlags & CAM_TRANSITION) && !m_bCameraIsZooming)
	{
		// Matrix that represents the final camera position in world space (inversed)
		D3DXMATRIX d3dFinalInvCamera = m_d3dCamTransform * d3dObject;

		// Position the camera is going to be at when it reached its next waypoint
		D3DXVECTOR3 d3dFinalCamPos(d3dFinalInvCamera._41, d3dFinalInvCamera._42, d3dFinalInvCamera._43);

		// Camera current position
		D3DXVECTOR3 d3dCurCamPos(m_d3dInvCamera._41, m_d3dInvCamera._42, m_d3dInvCamera._43);

		// Vector TO the final camera FROM the current camera position
		D3DXVECTOR3 d3dDelta = d3dFinalCamPos - d3dCurCamPos;
		
		// Normalize the direction vector
		D3DXVECTOR3 d3dNormalizedDelta;
		D3DXVec3Normalize(&d3dNormalizedDelta, &d3dDelta);

		// If we're close enough to the waypoint
		if(ReachedWaypoint(d3dDelta))
		{
			// If we're following the player, no more transition
			if(m_chFlags & CAM_FOLLOW)
			{
				m_chFlags &= ~CAM_TRANSITION;
			}
			else
			{
				// We just reached a waypoint and we're not following the player
				// So let's wait!
				//m_chFlags |= CAM_WAITING;
				m_fTimeCurrentlyWaited = 0.0f;
				
				// Else, let's first pop this waypoint out of the queue
				m_WayPoints.pop();

				// If the queue is now empty, we should start following the player
				if(m_WayPoints.empty())
				{
					LockToPlayer();
				}
				// Else we'll just move on and next time we'll be following the next waypoint
			}
		}
		// If we haven't reached it yet
		else
		{
			// Update the current camera position
			//D3DXVec3Normalize(&d3dDelta, &d3dDelta);
			d3dCurCamPos = d3dCurCamPos + d3dNormalizedDelta * fElapsedTime * m_fCamSpeed;
			
			// Set the position directly on the inverse camera
			m_d3dInvCamera._41 = d3dCurCamPos.x;
			m_d3dInvCamera._42 = d3dCurCamPos.y;
			m_d3dInvCamera._43 = d3dCurCamPos.z;

			// Inverse the inverted camera to get the not-inverted camera, which by itself is already an inverted matrix btw
			D3DXMatrixInverse(&m_d3dViewMat, nullptr, &m_d3dInvCamera);
		}
	}
	// If we're not transitioning, just hard attach the matrix if we're not zooming and the zoom isn't being held
	else
	{
		if(!m_bCameraIsZooming)
			D3DXMatrixInverse(&m_d3dViewMat, nullptr, &(m_d3dCamTransform * d3dObject));
	}


	// If we're shaking...
	if(m_chFlags & CAM_SHAKE)
	{
		// Shake the camera
		D3DXMATRIX d3dShake;
		if(m_chFlags & CAM_SHAKE_UP)
		{
			D3DXMatrixTranslation(&d3dShake, 0, m_fShakeAmount * fElapsedTime, 0.0f);
			m_chFlags &= ~CAM_SHAKE_UP;
		}
		else
		{
			D3DXMatrixTranslation(&d3dShake, 0, -m_fShakeAmount * fElapsedTime, 0.0f);
			m_chFlags |= CAM_SHAKE_UP;
		}
		m_d3dViewMat = m_d3dViewMat * d3dShake;

		// Decrease elapsed time from the shake duration
		m_fShakeDuration -= fElapsedTime;

		// If the we reached zero or less, stop shaking
		if(m_fShakeDuration <= 0)
		{
			m_chFlags &= ~CAM_SHAKE;
		}
	}

	float fCenterZ = GetCenterPos().z;

#ifdef CHEAP_RENDERING // If on debug reduce the culling area
	m_fMinZ = fCenterZ + CWorldManager::GetInstance()->GetExtent() * 0.20f;
	m_fMaxZ = fCenterZ - CWorldManager::GetInstance()->GetExtent() * 1.1f;
#else
	m_fMinZ = fCenterZ + CWorldManager::GetInstance()->GetExtent() * 2.2f; // pulled out because it was too small during the panning intro, causing a culling issue
	m_fMaxZ = fCenterZ - CWorldManager::GetInstance()->GetExtent() * 1.3f;
#endif

	m_d3dViewProj = m_d3dViewMat * m_d3dProjMat;


	D3DXMatrixInverse(&m_d3dInvViewProj, nullptr, &m_d3dViewProj);
	D3DXMatrixInverse(&m_d3dInvCamera, nullptr, &m_d3dViewMat);

	// linear camera zoom
	UpdateLinearZoom(fElapsedTime, d3dObject);		

	m_d3dCamPos = D3DXVECTOR3(m_d3dInvCamera._41, m_d3dInvCamera._42, m_d3dInvCamera._43);

	CSoundManager::GetInstance()->SetListenerOrientation(GetPosition());
}
void CCamera::AddWayPoint(float fX, float fY, float fZ)
{
	m_WayPoints.push(D3DXVECTOR3(fX, fY, fZ));
}
void CCamera::AddWayPointToPlayer(void)
{
	if(m_pPlayer->GetInitialized())
		m_WayPoints.push(D3DXVECTOR3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, m_pPlayer->GetPos().z));
}

void CCamera::LockToPlayer(void)
{
	// If already following return
	if(m_chFlags & CAM_FOLLOW)
		return;

	// Follow player
	m_chFlags |= CAM_FOLLOW;
	// Start transition
	m_chFlags |= CAM_TRANSITION;

	CInput::GetInstance()->EnablePlayerInput();
}
void CCamera::StartPanning(void)
{
	// If we're not following the player, which means we're already panning, return
	if(!(m_chFlags & CAM_FOLLOW))
		return;

	// Stop folowing the player
	m_chFlags &= ~CAM_FOLLOW;
	// Start transition
	m_chFlags |= CAM_TRANSITION;
}

void CCamera::ZoomCamera(float zoomDistance, float zoomDurationTime, bool bHoldZoomPosition)
{
	m_bCameraIsZooming = true;
	m_fZoomElapsedTime = 0.0f;
	m_fZoomDurationTime = zoomDurationTime;
	m_fZoomDistance = zoomDistance;
	m_bHoldCameraZoom = bHoldZoomPosition;
	
	m_vStartZoomPosition = D3DXVECTOR3(m_d3dInvCamera._41, m_d3dInvCamera._42, m_d3dInvCamera._43);
}

void CCamera::Shake(float fShakeAmount, float fDuration)
{
	// If already shaking return
	if(m_chFlags & CAM_SHAKE)
		return;

	// Start shaking up
	m_chFlags |= CAM_SHAKE;
	m_chFlags |= CAM_SHAKE_UP;

	// Set the shake amount and duration
	m_fShakeAmount = fShakeAmount;
	m_fShakeDuration = fDuration;
}
void CCamera::SetPlayer(CBaseObject* pPlayer)
{
	m_pPlayer = pPlayer;

	// Follow player
	m_chFlags |= CAM_FOLLOW;
	// No transition
	m_chFlags &= ~CAM_TRANSITION;
}
const D3DXVECTOR3 CCamera::GetPlayerPos(void) const
{
	if(m_pPlayer)
	{
		vec3f pos = m_pPlayer->GetWorldMatrix()->axis_pos;
		return D3DXVECTOR3(pos.x, pos.y, pos.z);
	}
	return D3DXVECTOR3(0, 0, 0);
}

void CCamera::Clear(void)
{
	while(!m_WayPoints.empty())
	{
		m_WayPoints.pop();
	}
	LockToPlayer();
}

bool CCamera::IsObjectInside(IRenderThing* pInst)
{
	if(pInst->GetFlags() & RENDER_TWOD_PARTICLE || pInst->GetFlags() & RENDER_MENU_MODEL)
		return true;
	if(pInst->GetType() == IRenderThing::ATTACHED)
		return true;

	vec3f min, max;
	pInst->GetCollisionBounds(min, max);

	if((min.z >= m_fMinZ && min.z <= m_fMaxZ) ||
		(max.z >= m_fMinZ && max.z <= m_fMaxZ))
	{
#ifdef CHEAP_RENDERING		// If on debug don't render things below 0
		if(min.y <= 0 && max.y <= 0)
			return false;
#endif
		return true;
	}
	return false;
}

D3DXVECTOR3 CCamera::GetCenterPos(void) const
{
	D3DXVECTOR3 pos(m_d3dInvCamera._41, m_d3dInvCamera._42, m_d3dInvCamera._43);
	D3DXVECTOR3 forward(m_d3dInvCamera._31, m_d3dInvCamera._32, m_d3dInvCamera._33);

	D3DXVECTOR3 normForward;
	D3DXVec3Normalize(&normForward, &forward);

	return pos + forward * GetRadius();
}
float CCamera::GetRadius(void) const
{
	return abs(m_fDistFar - m_fDistNear) / 2.0f;
}

void CCamera::SetCurPos(vec3f& in)
{
	 m_d3dInvCamera._41 = in.x;
	 m_d3dInvCamera._42 = in.y;
	 m_d3dInvCamera._43 = in.z;
}

void CCamera::RestoreDefaultCamera()
{
	D3DXMATRIX d3dTranslate, d3dRotate;

	m_fHardAttachCurrentDistance = m_fHardAttachDefaultDistance;	
	D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, m_fHardAttachDefaultDistance);
	D3DXMatrixRotationYawPitchRoll(&d3dRotate, D3DXToRadian(135.0f), D3DXToRadian(45.264f), 0.0f);
	// set the hard attach matrix back to default
	m_d3dCamTransform = d3dTranslate * d3dRotate;

	// turn off zoom variables
	m_bCameraIsZooming = false;
	m_bHoldCameraZoom = false;
	m_fZoomElapsedTime = 0.0f;
	m_fZoomDistance = 0.0f;

	// reset to follow the hard attach target
	m_chFlags |= CAM_FOLLOW;
}

void CCamera::UpdateLinearZoom(float fElapsedTime, D3DXMATRIX hardAttachTarget)
{
	if (m_bCameraIsZooming)
	{
		m_fZoomElapsedTime += fElapsedTime;
		
		// if over, clamp to max time
		if (m_fZoomElapsedTime > m_fZoomDurationTime) 
		{
			m_fZoomElapsedTime = m_fZoomDurationTime;
		}		
		// zoom forward or backwards
		/* camera's new position = startPosition + normalizedForwardVector * zoomDistance * currentTimeRatio */
		D3DXVECTOR3 newPosition = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 forwardVector = D3DXVECTOR3(m_d3dInvCamera._31, m_d3dInvCamera._32, m_d3dInvCamera._33);
		D3DXVec3Normalize(&forwardVector, &forwardVector);

		// move forward/backward by the ratio of the total zoom distance
		float zoomRatio = m_fZoomDistance * (m_fZoomElapsedTime / m_fZoomDurationTime);

		m_vPositionLastFrame = D3DXVECTOR3(m_d3dInvCamera._41, m_d3dInvCamera._42, m_d3dInvCamera._43);
		newPosition = m_vStartZoomPosition + *(D3DXVec3Scale(&forwardVector, &forwardVector, zoomRatio));

		// set the camera's position to the new position
		m_d3dInvCamera._41 = newPosition.x;
		m_d3dInvCamera._42 = newPosition.y;
		m_d3dInvCamera._43 = newPosition.z;

		if (m_bHoldCameraZoom)
		{
			// set up for hard attach matrix (m_d3dCamTransform)
			D3DXMATRIX d3dTranslate, d3dRotate;
			D3DXMatrixTranslation(&d3dTranslate, 0.0f, 0.0f, m_fHardAttachCurrentDistance);
			D3DXMatrixRotationYawPitchRoll(&d3dRotate, D3DXToRadian(135.0f), D3DXToRadian(45.264f), 0.0f);

			D3DXVECTOR3 zoom = m_vPositionLastFrame - newPosition;
			d3dTranslate._43 += zoom.z; 
			
			// update the hard attach offset to the new one
			m_fHardAttachCurrentDistance = d3dTranslate._43;

			m_d3dCamTransform = d3dTranslate * d3dRotate;
			// set the view to the new hard attach
			D3DXMatrixInverse(&m_d3dViewMat, nullptr, &(m_d3dCamTransform * hardAttachTarget));
		}
		else 
		{
			// set the view to the new zoom
			D3DXMatrixInverse(&m_d3dViewMat, nullptr, &m_d3dInvCamera);
		}

		// reset timer, but not til after it's used for the ratio
		if (m_fZoomElapsedTime >= m_fZoomDurationTime) 
		{
			m_fZoomElapsedTime = 0.0f;
			m_bCameraIsZooming = false; 
		}
	}
}

float CCamera::GetDistanceSquared(const IRenderThing* pRenderThing) const
{
	const vec3f& vMin = pRenderThing->GetWorldMin();
	const vec3f& vMax = pRenderThing->GetWorldMax();
	D3DXVECTOR3 d3dMin(vMin.v);
	D3DXVECTOR3 d3dMax(vMax.v);

	d3dMin -= m_d3dCamPos;
	d3dMax -= m_d3dCamPos;

	float fDistMin = D3DXVec3LengthSq(&d3dMin);
	float fDistMax = D3DXVec3LengthSq(&d3dMax);

	return min(fDistMin, fDistMax);
}


vec3f CCamera::GetPositionInFrontOfCamera(float fX, float fY, float fZ)
{
	D3DXVECTOR3 vCamLateral(m_d3dInvCamera._11, m_d3dInvCamera._12, m_d3dInvCamera._13);
	D3DXVECTOR3 vCamUp(m_d3dInvCamera._21, m_d3dInvCamera._22, m_d3dInvCamera._23);
	D3DXVECTOR3 vCamForward(m_d3dInvCamera._31, m_d3dInvCamera._32, m_d3dInvCamera._33);
	
	D3DXVec3Normalize(&vCamLateral, &vCamLateral);
	D3DXVec3Normalize(&vCamUp, &vCamUp);
	D3DXVec3Normalize(&vCamForward, &vCamForward);


	D3DXVECTOR3 vPosition(m_d3dInvCamera._41, m_d3dInvCamera._42, m_d3dInvCamera._43);
	vPosition += vCamForward * (fZ + 20.0f);
	vPosition += vCamLateral * fX;
	vPosition += vCamUp * fY;

	return vec3f(vPosition.x, vPosition.y, vPosition.z);
}
