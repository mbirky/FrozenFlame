#include "../Header/BreathBehavior.h"
#include "../Header/CFSM.h"
#include "../../Objects/IceEnemy.h"

const float CBreathBehavior::sm_fPrepareDuration = 1.5f;
const float CBreathBehavior::sm_fRunDuration = 2.0f;
const float CBreathBehavior::sm_fAttackCooldown = 1.0f;
const float CBreathBehavior::sm_fMaxRange = BREATH_DISTANCE - 2.0f;

CBreathBehavior::CBreathBehavior() : m_pBreathParticleEmitter(nullptr), m_pBreathCloudEmitter(nullptr)
{
	m_fAttackTimer = 0.0f;
	m_eState = STATE_NULL;
	m_fPrepareTimer = 0,.0f;
	m_fRunTimer = 0.0f;
}

void CBreathBehavior::Enter()
{
	GetOwner()->GetAgent()->SetIsMoving(false);
	m_cSoundCounter = 0;
	m_fPrepareTimer = sm_fPrepareDuration;
	m_fRunTimer = sm_fRunDuration;
	m_eState = STATE_NULL;

	if(m_pBreathParticleEmitter){
		CView::GetEmitterManager().KillEmitter(m_pBreathParticleEmitter);
	}
	if(m_pBreathCloudEmitter){
		CView::GetEmitterManager().KillEmitter(m_pBreathCloudEmitter);
	}
	m_pBreathCloudEmitter = nullptr;
	m_pBreathParticleEmitter = nullptr;
	// If we don't have a ice breath mist emitter, load it in
	if(!m_pBreathParticleEmitter){ 
		m_pBreathParticleEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/IceBreathParticles.txt", GetOwner()->GetAgent()->GetPos(),
			vec3f(0, 0, 0), EMIT_LOOP | EMIT_SPRAY, GetOwner()->GetAgent());
	}
	if(!m_pBreathCloudEmitter){ 
		m_pBreathCloudEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/IceBreathCloud.txt", GetOwner()->GetAgent()->GetPos(),
			vec3f(0, 0, 0), EMIT_LOOP | EMIT_SPRAY, GetOwner()->GetAgent());
	}
}

void CBreathBehavior::PrepareBlow(float fElapsedTime, CPlayerObject* pTarget)
{
	// At least for now we're just waiting on this function
	m_fPrepareTimer -= fElapsedTime;

	// Once timer is done, finish this state
	if(m_fPrepareTimer <= 0)
	{
		CIceEnemy* pAgent = dynamic_cast<CIceEnemy*>(GetOwner()->GetAgent());
		if(!pAgent) return;

		// Reset our timers, go to the next state
		m_fPrepareTimer = sm_fPrepareDuration;
		m_eState = STATE_EXHALE;
		m_pBreathParticleEmitter->SetActive(true);
		m_pBreathCloudEmitter->SetActive(true);
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_ICEGOLEM_BREATHATTACK, pAgent->GetSoundID());

		// This should start emitting the Ice Breath
		pAgent->SetIceBreathEnabled(true);

		// We've done all the preparations so it's time to do the job
		if (IceBreathCheck(GetOwner()->GetAgent(), pTarget))
		{
			pTarget->OnAttack(GetOwner()->GetAgent()->GetDamage(), GetOwner()->GetAgent()->GetAttackElement());
			pTarget->SetElementalState(CCollidableObject::FROZEN);
		}

		// Put the Ice beast back on cooldown
		GetOwner()->GetCooldown() = true;
		GetOwner()->GetCooldownTimer() = 0.0f;

		// Make the Ice beast stop moving
		pAgent->SetVelocity(vec3f(0, 0, 0));
		pAgent->SetIsMoving(false);
	}

	return;
}

void CBreathBehavior::RunAway(vec3f toTarget, float fElapsedTime)
{
	CIceEnemy* pAgent = dynamic_cast<CIceEnemy*>(GetOwner()->GetAgent());
	if(!pAgent) return;

	// Decrease the runaway timer
	m_fRunTimer -= fElapsedTime;

	// If we've gone past the run time or we're already in range for snowballs
	if(m_fRunTimer <= 0 || toTarget.magnitude() >= sm_fMaxRange * 1.5f)
	{
		// Reset the timer and cooldown
		m_fRunTimer = sm_fRunDuration;
		GetOwner()->GetCooldown() = true;

		// Change to snowball behavior
		GetOwner()->TransitionTo(SNOWBALL_BEHAVIOR);

		// Stop attacking and moving
		pAgent->SetIsMoving(false);
		pAgent->SetIsAttacking(false);
		pAgent->SetVelocity(vec3f(0, 0, 0));
	}
}

void CBreathBehavior::SetEmitterPositions(CIceEnemy* pAgent)
{
	// emmiterMat here is the matrix for the ice golem's jaw joint
	const CRenderInstance* pAttach = CView::GetAttachmentInstance(-1, pAgent->GetIceBreathAttackID());
	const D3DXMATRIX* emitterMat = &pAttach->GetWorldTransform();
	//m_pBreathMistEmitter->SetEmittPos(emitterMat->_41, emitterMat->_42, emitterMat->_43);
	//m_pBreathMistEmitter->SetPos(vec3f(emitterMat->_41, emitterMat->_42, emitterMat->_43));
	vec3f emitterOffset(emitterMat->_41,emitterMat->_42,emitterMat->_43);
	emitterOffset = emitterOffset - pAgent->GetPos();
	m_pBreathParticleEmitter->SetOffset(emitterOffset);
	m_pBreathCloudEmitter->SetOffset(emitterOffset);

	// start at the ice golem's position, at its "feet"
	vec3f emitterDestination = pAgent->GetPos();				
	vec3f golemForwardVector = pAgent->GetWorldMatrix()->axis_z;
	golemForwardVector.normalize();

	// move forward along the golem's forward vector
	emitterDestination += golemForwardVector * 7.0f;
	m_pBreathParticleEmitter->SetDestinationPos(emitterDestination);
	m_pBreathCloudEmitter->SetDestinationPos(emitterDestination);
}

void CBreathBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	// Get a pointer to the agent to simplify code later
	CIceEnemy* pAgent = dynamic_cast<CIceEnemy*>(GetOwner()->GetAgent());
	if(!pAgent)
		return;

	// Player position
	vec3f vPlayerPos = _pcPlayer->GetWorldMatrix()->axis_pos;

	// Vector to player
	vec3f toPlayer = vPlayerPos - pAgent->GetWorldMatrix()->axis_pos;
	float toPlayerDist = toPlayer.magnitude();

	switch(m_eState)
	{
	case STATE_NULL:
		{
			// If our state is null we'll be on a cooldown until we blow again
			if(GetOwner()->GetCooldown())
			{
				// If the beast is downed or it's far away transition to snowball behavior
				if(pAgent->GetElementalState() > CCollidableObject::NEUTRAL ||
					toPlayerDist > sm_fMaxRange)
				{
					GetOwner()->TransitionTo(SNOWBALL_BEHAVIOR);
					return;
				}

				// Increase the timer and turn to the player
				GetOwner()->GetCooldownTimer() += _fElapsedTime;
				TurnTo(vPlayerPos, _fElapsedTime);

				// If we got past the cooldown, go to next state
				if(GetOwner()->GetCooldownTimer() >= sm_fAttackCooldown)
				{
					GetOwner()->GetCooldown() = false;
					GetOwner()->GetCooldownTimer() = 0.0f;
					pAgent->SetIsAttacking(true);
					m_eState = STATE_INHALE;

					// Tell the agent to prepare to blow and start inhaling
					pAgent->PrepareToBlow();
				}
			}
		}
		break;
	case STATE_INHALE:
		{
			// This function will take care of preparing for the blow
			PrepareBlow(_fElapsedTime, _pcPlayer);
		}
		break;
	case STATE_EXHALE:
		{
			// When we exhale we'll be on a quick cooldown before we run away
			if(GetOwner()->GetCooldown())
			{
				SetEmitterPositions(pAgent);

				// Increase the timer
				GetOwner()->GetCooldownTimer() += _fElapsedTime;

				// If we got past the cooldown, start running
				if(GetOwner()->GetCooldownTimer() >= sm_fAttackCooldown)
				{
					GetOwner()->GetCooldown() = false;
					GetOwner()->GetCooldownTimer() = 0.0f;
					pAgent->SetIsAttacking(false);
					pAgent->SetIceBreathEnabled(false);
					m_pBreathParticleEmitter->SetActive(false);
					m_pBreathCloudEmitter->SetActive(false);

					if(pAgent->GetElementalState() > CCollidableObject::NEUTRAL ||
						toPlayerDist > sm_fMaxRange)
					{
						GetOwner()->TransitionTo(SNOWBALL_BEHAVIOR);
						pAgent->SetIceBreathEnabled(false);
					}
					else
					{
						m_eState = STATE_RUN;
						pAgent->StartRunning();
						pAgent->SetVelocity(toPlayer * -1.0f);
						pAgent->SetIsAttacking(false);
					}
				}
			}
		}
		break;
	case STATE_RUN:
		{
			RunAway(toPlayer, _fElapsedTime);
		}
		break;
	}


#if 0
		if (m_fAttackTimer >= GetOwner()->GetAgent()->GetAttackSpeed())
		{
			m_cSoundCounter += 1;
			if (m_cSoundCounter % 10 == 0)
				CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_3D_WaterSplash");
			GetOwner()->GetCooldownTimer() -= _fElapsedTime;
			//GetOwner()->GetAgent()->SetColor(vec3f(1.0f, 1.0f, 0.0f));
			if (GetOwner()->GetCooldownTimer() <= 0.0f)
			{
				GetOwner()->GetCooldownTimer() = 0.0f;
				GetOwner()->GetCooldown() = true;
				m_fAttackTimer = 0.0f;

				if(GetOwner()->GetAgent()->GetType() == ICE_OBJECT)
				{
					// - Dan commented this out
					// ((CIceEnemy*)GetOwner()->GetAgent())->SetIsBlowing(false);
				}

				else
				{
					GetOwner()->GetAgent()->SetIsAttacking(false);
				}
			}
#endif
}

void CBreathBehavior::Exit()
{
	if(m_pBreathParticleEmitter){
		CView::GetEmitterManager().KillEmitter(m_pBreathParticleEmitter);
	}
	m_pBreathParticleEmitter = nullptr;

	if(m_pBreathCloudEmitter){
		CView::GetEmitterManager().KillEmitter(m_pBreathCloudEmitter);
	}
	m_pBreathCloudEmitter = nullptr;
}