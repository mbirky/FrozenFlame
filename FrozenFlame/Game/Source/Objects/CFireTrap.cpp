/***********************************************
* Filename:  		CFireTrap.c
* Date:      		11/11/2012
* Mod. Date: 		11/11/2012
* Mod. Initials:	MB
* Author:    		Justin Maza
* Purpose:   		This is the file for the Fire
*					traps in the game
************************************************/

#include "CFireTrap.h"
#include "../Globals.h"
#include "../Renderer/CEmitterManager.h"

CFireTrap::CFireTrap() : m_fAttackLength(2.5), m_fAttackCooldownLength(3.0)
{
	SetType(FIRETRAP_OBJECT);
	m_pEmitter = nullptr;
	m_pFireEmit = nullptr;
	m_pBase = nullptr;
	m_nAttachedLightID = -1;
	m_nLightID = -1;
}

CFireTrap::~CFireTrap()
{
	SetInitialized(false);

	if(m_pBase != nullptr)
	{
		m_pBase->Uninitialize();
		delete m_pBase;
		m_pBase = nullptr;
	}

	// New light system - Dan
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

	// Proper emitter cleanup. - JC
	RemoveEmitters();

	m_pEmitter = nullptr;
	m_pFireEmit = nullptr;
}

void CFireTrap::Initialize(void)
{
	CEnvironmentObject::Initialize();

	SetRenderID(CView::Push(this, "AttackCube", RENDER_INVISIBLE));
	SetColor(vec3f(0.0f, 0.0f, 0.0f));

	m_cState = T_IDLE;
	m_fTotalElapsedTime = 0.0f;
	m_bFirstUpdate = true;
	m_fDelayTimer = 0.0f;
	m_fAliveTimer = 0.0f;

	SetSphereRad(10.0);
	m_nLightID = -1;

	m_pEmitter = nullptr;
	m_pFireEmit = nullptr;

	m_bCanPlayEruptionSFX = true;
	m_bCanPlayWarningSFX = true;;

	if(m_pBase == nullptr)
	{
		m_pBase = new CEnvironmentObject();
	}

	if(m_pBase)
	{
		m_pBase->Initialize();
		m_pBase->SetRenderID(CView::Push(m_pBase,"FireTrap", RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	}
	// give the light a vertical offset so it's not flush with the ground
	// the default height of the base won't show light against the ground
	m_nAttachedLightID = CView::MakeLight(m_pBase->GetRenderID(), true, vec3f(0, 3, 0), vec4f(0.6f, 0.4f, 0.0f, 1.0f), 16.0f,
		vec4f(0.1f, 0.1f, 0.1f, 1.0f), vec4f(1.0f, 1.0f, 1.0f, 1.0f), 16.0f, 3.0f);
	
	if(m_pEmitter == nullptr)
	{
		m_pEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/Smokey.txt");
		m_pEmitter->AreaRotation(false);
	}

	if(m_pFireEmit == nullptr)
	{
		m_pFireEmit = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/FireModel.txt");
		m_pFireEmit->SetOffset( vec3f(0.0f, 1.0f, 0.0f) );
	}
}

void CFireTrap::Reinitialize(void)
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CFireTrap::Update(float fElapsedTime)
{
	CEnvironmentObject::Update(fElapsedTime);

	if(m_bFirstUpdate)
	{
		m_fStartYPos = GetPos().y + 2.0f;
		m_bFirstUpdate = false;
	}

	m_fTotalElapsedTime += fElapsedTime;

	if(m_cState & T_SUMMONED)
	{
		if (m_bCanPlayEruptionSFX)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_FIRETRAP_ERUPTION, GetSoundID());
			m_bCanPlayEruptionSFX = false;
		}

		if(m_fTotalElapsedTime > m_fDelayTimer)
		{
			m_pEmitter->SetActive(false);
			SetEmittersPosition(this->m_pBase->GetPos() + vec3f(0.0f, 2.0f, 0.0f));
			m_pFireEmit->SetActive(true);
			m_fTotalElapsedTime -= m_fDelayTimer;
			m_cState |= T_RISE;
			m_cState &= ~T_IDLE;

			if(m_nLightID != -1)
			{
				CView::DestroyLight(m_nLightID);
				m_nLightID = -1;
			}
		}
	}
	else
	{
		if(m_fTotalElapsedTime > m_fAttackCooldownLength && (m_cState & T_IDLE))
		{
			m_fTotalElapsedTime -= m_fAttackCooldownLength;
			m_cState |= T_RISE;
			m_cState &= ~T_IDLE;
			CView::SetLightEnabled(m_nAttachedLightID, true);
		}
	}

	if(m_cState & T_IDLE)
	{
		// make sure not to be playing the eruption sound anymore
		if (m_bCanPlayWarningSFX)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::STOP_FX_FIRETRAP_ERUPTING_LOOP, GetSoundID());
		}
		// reset for next time the trap erupts
		m_bCanPlayEruptionSFX = true;

		// when the timer is halfway to erupting, play the warning sound
		if(m_fTotalElapsedTime > m_fAttackLength * 0.75f)
		{
			if (m_bCanPlayWarningSFX)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_FIRETRAP_WARNINGLOOP, GetSoundID());
				m_bCanPlayWarningSFX = false;
			}
		}
		m_pEmitter->SetActive(true);
		m_pFireEmit->SetActive(false);
		CView::SetLightEnabled(m_nAttachedLightID, false);
	}

	if(m_cState & T_RISE)
	{
		// stop the warning loop sound and play the eruption sound
		if (m_bCanPlayEruptionSFX)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::STOP_FX_FIRETRAP_WARNINGLOOP, GetSoundID());
		}
		// reset for next time trap can warn play of eruption
		m_bCanPlayWarningSFX = true;
		if (m_bCanPlayEruptionSFX)
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_FIRETRAP_ERUPTION, GetSoundID());
			m_bCanPlayEruptionSFX = false;
		}

		m_pEmitter->SetActive(false);
		m_pFireEmit->SetActive(true);
		CView::SetLightEnabled(m_nAttachedLightID, true);

		if(GetPos().y <= m_fStartYPos + 6.0f)
		{
			SetPos(vec3f(GetPos().x, GetPos().y + 15.0f * fElapsedTime, GetPos().z));
			SetColor(vec3f(1.0f, 0.0f, 0.0f));
		}

		if(m_fTotalElapsedTime >= m_fAttackLength)
		{
			m_cState |= T_DOWN;
			m_cState &= ~T_RISE;

			m_fTotalElapsedTime -= m_fAttackLength;
		}
	}


	if(m_cState & T_DOWN)
	{
		m_pEmitter->SetActive(false);

		// slowly return to original position
		if(GetPos().y >= m_fStartYPos)
		{
			SetPos(vec3f(GetPos().x, GetPos().y - 1.5f * fElapsedTime, GetPos().z));
			m_fTotalElapsedTime -= fElapsedTime;
		}

		else
		{
			SetPos(vec3f(GetPos().x, m_fStartYPos, GetPos().z));

			if(m_cState & T_SUMMONED)
			{
				Uninitialize();
			}

			else
			{
				m_cState |= T_IDLE;
				m_cState &= ~T_DOWN;
				SetColor(vec3f(0.0f, 0.0f, 0.0f));
			}
		}
	}

	if(m_cState & T_SUMMONED)
	{
		m_pFireEmit->SetActive(true);
		m_fAliveTimer += fElapsedTime;
		if(m_fAliveTimer + m_fDelayTimer >= 3.0f)
		{
			Uninitialize();
		}
	}
}

void CFireTrap::SummonTrap(float fDelay, vec3f vPosition, int nLightID)
{
	m_cState |= T_SUMMONED;
	m_fDelayTimer = fDelay;

	CView::GetEmitterManager().KillEmitter(m_pFireEmit);
	m_pFireEmit = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/FireBurst.txt");
	m_pFireEmit->SetOffset( vec3f(0.0f, 1.0f, 0.0f) );

	SetPos(vPosition);
	vPosition.y += 3.0f;
	m_pBase->SetPos(vPosition);
	m_nLightID = nLightID;
}

void CFireTrap::RemoveEmitters(void)
{
	if(m_pEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pEmitter);
	}
	m_pEmitter = nullptr;

	if(m_pFireEmit)
	{
		CView::GetEmitterManager().KillEmitter(m_pFireEmit);
	}
	m_pFireEmit = nullptr;
}

void CFireTrap::SetEmittersPosition(const vec3f& vPos)
{
	if(m_pEmitter)
	{
		m_pEmitter->SetEmittPos(vPos);
	}
	if(m_pFireEmit)
	{
		m_pFireEmit->SetEmittPos(vPos);
	}
}

void CFireTrap::SetBaseObjPosition(const vec3f& vPos)
{
	if(m_pBase)
	{
		m_pBase->SetPos(vPos);
	}
}

void CFireTrap::Uninitialize(void)
{	
	if (CSoundManager::GetInstance() && GetSoundID())
	{
		if (false == m_bCanPlayWarningSFX) // it's currently playing
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::STOP_FX_FIRETRAP_WARNINGLOOP, GetSoundID());
		if (false == m_bCanPlayEruptionSFX) // it's currently playing
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::STOP_FX_FIRETRAP_ERUPTING_LOOP, GetSoundID());
	}
	RemoveEmitters();

	if(m_pBase)
		m_pBase->Uninitialize();

	CEnvironmentObject::Uninitialize();
}
