#include "SceneryObject.h"

CSceneryObject::CSceneryObject(void)
{
	SetType(SCENERY_OBJECT);
	m_fLifetime = -1.0f;
	m_bIsTransparent = false;
	m_fDuration = 0.0f;
}

CSceneryObject::~CSceneryObject(void)
{
	SetInitialized(false);
}

void CSceneryObject::Initialize(void)
{
	CBaseObject::Initialize();

	m_fLifetime = -1.0f;
	m_pAniPlayer = nullptr;

	if(!m_bIsTransparent)
	{
		SetRenderID(CView::Push(this, m_szName));
	}
	else
	{
		SetRenderID(CView::Push(this, m_szName, RENDER_TRANSPARENT));
	}
}

void CSceneryObject::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CSceneryObject::Uninitialize()
{
	m_pAniPlayer = nullptr;
	CBaseObject::Uninitialize();
}

void CSceneryObject::Update(float fElapsedTime)
{
	if(m_bIsUpdatable)
	{
		if(m_fLifetime > 0)
		{
			m_fLifetime -= fElapsedTime;

			if(m_fLifetime <= 0){
				Uninitialize();
			}
			if(m_pAniPlayer){
				m_pAniPlayer->Update(fElapsedTime);
			}

		}
		if(m_fDuration){
			SetTransparency(m_fLifetime / m_fDuration);
		}
	}
}