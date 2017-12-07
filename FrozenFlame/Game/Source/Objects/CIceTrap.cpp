/***********************************************
 * Filename:  		CIceTrap.cpp
 * Date:      		11/05/2012
 * Mod. Date: 		11/06/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		The ice trap object class definitions
 ************************************************/
#include "CIceTrap.h"
#include "AttackObject.h"
#include "ObjectManager.h"
#include "../CGame.h"

/*********************************
* Public Functions
**********************************/

CIceTrap::CIceTrap()
{
	SetType(ICETRAP_OBJECT);
	m_pAnimatedModel = nullptr;
	m_nLightID = -1;
	m_nAttachedLightID = -1;
}

CIceTrap::~CIceTrap()
{
	// If we have an animated model uninitialize it
	if(m_pAnimatedModel)
	{
		m_pAnimatedModel->Uninitialize();
		delete m_pAnimatedModel;
		m_pAnimatedModel = nullptr;
	}

	if(m_nLightID != -1)
	{
		CView::DestroyLight(m_nLightID);
		m_nLightID = -1;
	}
	if(m_nAttachedLightID != -1)
	{
		CView::DestroyLight(m_nAttachedLightID);
		m_nAttachedLightID = -1;
	}
}

void CIceTrap::Initialize(void)
{
	// Call the parent Initialize
	CEnvironmentObject::Initialize();

	// Push to the View so it's rendered
	SetRenderID(CView::Push(this, "AttackCube", RENDER_INVISIBLE));

	// If there's no animated model make one
	if(!m_pAnimatedModel)
	{
		m_pAnimatedModel = new CEnvironmentObject;
	}

	// If we now have an animated model
	if(m_pAnimatedModel)
	{
		// Initialize the animated model
		m_pAnimatedModel->Initialize();

		// Set that model's position
		m_pAnimatedModel->SetPos(vec3f(0, 0, 0));

		// Set its render id
		m_pAnimatedModel->SetRenderID(CView::Push(m_pAnimatedModel, "IceTrap", RENDER_FULLY_TRANSLUCENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));

		if(m_pAnimatedModel->GetRenderID() > -1)
		{
			// Get the animation player and store it
			m_pAnimatedModel->SetAnimPlayer(CView::GetAnimPlayer(m_pAnimatedModel->GetRenderID()));

			// Load the necessary animations
			m_pAnimatedModel->GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "IceTrap/IceTrap_Idle.anm");
			m_pAnimatedModel->GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "IceTrap/IceTrap_Up.anm");
		}
	}

	SetSphereRad(5.0f);
	m_fAttackCooldown = 1.5f;
	m_fTotalElapsedTime = 0.0f;
	m_bIsReady = false;

	m_cState = T_IDLE;
	m_fDelayTimer = 0.0f;
	m_fAliveTimer = 0.0f;

	SetDamage(5);
	m_nLightID = -1;

}

void CIceTrap::Reinitialize(void)
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CIceTrap::SetModelPosition(const vec3f& vPos)
{
	if(m_pAnimatedModel)
	{
		m_pAnimatedModel->SetPos(vPos);
	}
}

void CIceTrap::Update(float fElapsedTime)
{
	// Update Collision data
	CEnvironmentObject::Update(fElapsedTime);
	SetSphereRad(5.0f);

	matrix4f matrix = *GetWorldMatrix();

	if(m_pAnimatedModel)
	{
		m_pAnimatedModel->Update(fElapsedTime);
	}
	
	static vec3f vOldPos = GetPos();
	m_fTotalElapsedTime += fElapsedTime;

	if(m_cState & T_SUMMONED)
	{
		if(m_fTotalElapsedTime >= m_fDelayTimer)
		{
			m_cState |= T_RISE;
			m_cState &= ~T_IDLE;

			if(m_nLightID != -1)
			{
				// New light system - Dan
				// CView::RemoveWorldLight(m_nLightID);
				CView::DestroyLight(m_nLightID);
				m_nLightID = -1;
			}
		}
	}
	else
	{
		if(m_cState & T_RISE)
		{
			// New light system - Dan
			// CView::AttachLight(GetRenderID(), vec4f(0,0,1,1), 20.0f, vec4f(0.1f,0.1f,0.1f,1.0f), vec4f(0.1f,0.5f,1.0f,1), 12, 3);
			m_nAttachedLightID = CView::MakeLight(GetRenderID(), true, vec3f(0, 5, 0), vec4f(0, 0, 1, 1), 10.0f, vec4f(0.1f, 0.1f, 0.1f, 1.0f), vec4f(0.1f, 0.5f, 1.0f, 1));
			
			// and if the total time passed is over than the cooldown then rise up
			if(m_fTotalElapsedTime < m_fAttackCooldown)
			{
				// This was causing the animation to play twice in one iteration of the Ice Trap so
				// I commented it out - Dan
				/*
				if(!m_pAnimatedModel->GetAnimationPlayer()->IsPlaying(ANIM_ATTACK))
				{
					m_pAnimatedModel->GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK, false);
				}
				*/

				m_cState |= T_IDLE;
				m_cState &= ~T_RISE;
			}
		}
	}

	if(m_cState & T_RISE)
	{
		// raise the object in the y position
		if(!m_pAnimatedModel->GetAnimationPlayer()->IsPlaying(ANIM_ATTACK))
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_RELEASE, GetSoundID());
			m_pAnimatedModel->GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK, false);
		}

		SetPos(vec3f(GetPos().x, GetPos().y + 20.0f * fElapsedTime, GetPos().z));
		
		if(GetPos().y >= vOldPos.y + 4.0f)
		{
			SetPos(vec3f(GetPos().x, vOldPos.y + 4.0f, GetPos().z));

			m_cState |= T_DOWN;
			m_cState &= ~T_RISE;
			m_cState &= ~T_IDLE;
		}
	}

	if(m_cState & T_DOWN)
	{
		// slowly return to original position
		SetPos(vec3f(GetPos().x, GetPos().y - 3.5f * fElapsedTime, GetPos().z));

		// if back at original position return to original state
		if(GetPos().y <= vOldPos.y)
		{
			m_pAnimatedModel->GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);

			SetPos(vec3f(GetPos().x, vOldPos.y, GetPos().z));
			m_fTotalElapsedTime = 0.0f;

			// New light system - Dan
			//CView::DisableAttachedLight(GetRenderID());
			CView::SetLightEnabled(m_nAttachedLightID, false);

			m_cState |= T_IDLE;
			m_cState &= ~T_DOWN;

			if(m_cState & T_SUMMONED)
			{
				Uninitialize();
			}
		}
	}

	if(m_cState & T_SUMMONED)
	{
		m_fAliveTimer += fElapsedTime;
		if(/*m_fTotalElapsedTime >= */m_fAliveTimer + m_fDelayTimer >= 2.0f)
		{
			Uninitialize();
		}
	}
}

void CIceTrap::Uninitialize(void)
{
	// If there's an animated model, uninitialize it
	if(m_pAnimatedModel)
	{
		m_pAnimatedModel->Uninitialize();
	}

	CEnvironmentObject::Uninitialize();
}

void CIceTrap::SummonTrap(float fDelay, vec3f vPosition, int nLightID)
{
	m_cState |= T_SUMMONED;
	m_fDelayTimer = fDelay;
	SetPos(vPosition);
	//SetEmittersPosition(vPosition);
	vPosition.y += 3.0f;
	m_pAnimatedModel->SetPos(vPosition);
	m_nLightID = nLightID;
}
