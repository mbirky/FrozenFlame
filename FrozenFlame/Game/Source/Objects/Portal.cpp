/***********************************************
 * Filename:		Crystal.cpp		
 * Date:      		11/28/2012
 * Mod. Date: 		11/28/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		Crystal fuction definitions
 ************************************************/
#include "Portal.h"
#include "../Core/CWorldManager.h"
#include "../Renderer/View.h"
#include "../Renderer/CEmitterManager.h"

/********************************
* Public Functions
*********************************/
CPortal::CPortal()
{
	SetType(PORTAL_OBJECT);
	m_cEmit = nullptr;
}

CPortal::~CPortal()
{
	//if(m_cEmit != nullptr)
	//{
	//	delete m_cEmit;
	//	m_cEmit = nullptr;
	//}
}

void CPortal::Initialize(void)
{
	CEnvironmentObject::Initialize();
	SetMaxHealth(1);
	SetHealth(1);
	SetRenderID(CView::Push(this,"Portal", RENDER_INVISIBLE | RENDER_FULLY_TRANSLUCENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));
	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Portal_Swirl.anm");
	GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);
	SetTransparency(0.7f);

	m_bIsActive = false;
	m_bCanPlayAmbientSound = true;

	if(m_cEmit == nullptr)
	{
		//m_cEmit = new CEmitter();
		//m_cEmit->SetActive(false);
		//m_cEmit->SetLoop(true);
		//m_cEmit->SetEmittPos(GetPos());
		//m_cEmit->LoadParticle("Resource/Particles/Crystal Emitter.txt");
		//m_cEmit = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/Crystal Emitter.txt");
	}
}

void CPortal::Reinitialize(void)
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CPortal::Update(float fElapsedTime)
{
	SetSoundPos(GetPos());

	if(m_cEmit != nullptr)
	{
		m_cEmit->SetEmittPos(GetPos());

		/*if(m_cEmit->GetActive() == true)
		{
			m_cEmit->Update(fElapsedTime);
		}*/
	}

	if(CWorldManager::GetInstance()->GetObjectManager()->CheckActiveEnemies() && CWorldManager::GetInstance()->GetObjectManager()->GetWizard()->GetIsDefeated() && !m_bIsActive)
	{
		CView::SetIsVisible(GetRenderID(),true);


		if (m_bCanPlayAmbientSound)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_PORTALAMBIENT, GetSoundID());
			m_bCanPlayAmbientSound = false;
		}
		//if (m_cEmit)
		//{
		//	m_cEmit->SetActive(true);
		//	//m_cEmit->ResetParticles();
		//}

	}

	UpdateCollisionData();

	if(!CWorldManager::GetInstance()->GetObjectManager()->CheckActiveEnemies() && !m_bIsActive)
	{
		m_bIsActive = true;
	}

	

	/*if(GetHealth() <= 0)
	{
		CWorldManager::GetInstance()->SetSectionComplete(true);

		Uninitialize();
	}*/
}

void CPortal::OnAttack(int nDamage,int nElementType)
{
	SetHealth(GetHealth() - nDamage);
}

void CPortal::Uninitialize()
{
	if (CSoundManager::GetInstance() && GetSoundID())
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::STOP_FX_PORTALAMBIENT, GetSoundID());
	}

	//if(m_cEmit)
	//{
	//	CView::GetEmitterManager().KillEmitter(m_cEmit);
	//}
	CEnvironmentObject::Uninitialize();

}
