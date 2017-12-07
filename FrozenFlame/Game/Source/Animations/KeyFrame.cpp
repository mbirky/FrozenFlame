#include "KeyFrame.h"

CKeyFrame::CKeyFrame(void)
{
	m_pParent = nullptr;
	m_fKeyTime = 0.0f;
	m_bDirty = false;
	m_uiJointIndex = 0;
	m_fTweenTime = 0.0f;
}

void CKeyFrame::AddChild(CKeyFrame* pFrame)
{
	if(!pFrame) return;

	pFrame->m_pParent = this;
	m_vpChildren.push_back(pFrame);
}

void CKeyFrame::RemoveChild(CKeyFrame* pFrame)
{
	if(!pFrame || pFrame->m_pParent != this) return;

	for(unsigned int n = 0; n < m_vpChildren.size(); ++n)
	{
		if(pFrame == m_vpChildren[n])
		{
			m_vpChildren.erase(m_vpChildren.begin() + n);
			pFrame->m_pParent = NULL;
		}
	}
}

void CKeyFrame::Update(void)
{
	if(!m_bDirty)
	{
		m_bDirty = true;

		for(unsigned int c = 0; c < m_vpChildren.size(); ++c)
		{
			m_vpChildren[c]->Update();
		}
	}
}

const D3DXMATRIX& CKeyFrame::GetWorldTransform(void) const
{
	if(m_bDirty)
	{
		if(m_pParent)
		{
			m_d3dWorldTransform = m_d3dLocalTransform * m_pParent->GetWorldTransform();
		}
		else
		{
			m_d3dWorldTransform = m_d3dLocalTransform;
		}

		m_bDirty = false;
	}
	return m_d3dWorldTransform;
}

CKeyFrame::~CKeyFrame(void)
{
	m_vpChildren.clear();
}