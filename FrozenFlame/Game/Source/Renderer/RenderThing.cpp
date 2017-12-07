#include "RenderThing.h"
#include "RenderInstance.h"

IRenderThing::IRenderThing(void)
	: m_bIsFlashing(false), m_fFlashTimer(0.0f), m_ucFlags(0),
	m_d3dAddedColor(0.0f, 0.0f, 0.0f, 0.0f)
{
	D3DXMatrixIdentity(&m_d3dWorldTransform);
	m_vWorldMin = vec3f(0,0,0);
	m_vWorldMax = vec3f(0,0,0);
}

// This is used for sorting - returns true if this thing is behind the other
bool IRenderThing::IsBehind(const IRenderThing* pOtherInstance) const
{
	if(CView::GetCamera().GetDistanceSquared(this) > CView::GetCamera().GetDistanceSquared(pOtherInstance))
		return true;
	return false;
}

bool IRenderThing::operator<(const IRenderThing& rhs) const
{
	unsigned short lhsFlags = m_ucFlags;
	unsigned short rhsFlags = rhs.m_ucFlags;

	if((lhsFlags & (RENDER_TRANSPARENT | RENDER_FULLY_TRANSLUCENT | RENDER_EFFECT)) != 0)
	{
		if((rhsFlags & (RENDER_TRANSPARENT | RENDER_FULLY_TRANSLUCENT | RENDER_EFFECT)) != 0)
			return IsBehind(&rhs);
	}
	
	if(lhsFlags != rhsFlags)
		return lhsFlags < rhsFlags;

	return !IsBehind(&rhs);
}

void IRenderThing::Update(float fElapsedTime)
{
	// If we're flashing make sure we decrease the timer
	if(m_bIsFlashing)
	{
		m_fFlashTimer -= fElapsedTime;

		if(m_fFlashTimer <= 0.0f)
		{
			m_fFlashTimer = 0.0f;
			m_bIsFlashing = false;
			m_d3dAddedColor = D3DXVECTOR4(0, 0, 0, 0);
		}
	}
}

void IRenderThing::Flash(const D3DXVECTOR4& d3dFlashColor, float fFlashDuration)
{
	if(!m_bIsFlashing)
	{
		m_bIsFlashing = true;
		m_d3dAddedColor = d3dFlashColor;
		m_fFlashTimer = fFlashDuration;
	}
}

// gets the pre-calculated collision bounds
void IRenderThing::GetCollisionBounds(vec3f& vWorldMin, vec3f& vWorldMax) const
{
	RecalculateBounds();
	vWorldMin = m_vWorldMin;
	vWorldMax = m_vWorldMax;
}
