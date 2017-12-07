#include "../Header/SnowballBehavior.h"
#include "../Header/CFSM.h"
#include "../../Objects/IceEnemy.h"

const float CSnowballBehavior::sm_fPrepareDuration = 0.9f;
const float CSnowballBehavior::sm_fAttackCooldown = 1.2f;
const float CSnowballBehavior::sm_fMinRange = BREATH_DISTANCE - 2.0f;
const float CSnowballBehavior::sm_fMaxRange = SNOWBALL_RANGE + 2.0f;
const float CSnowballBehavior::sm_fMaxHeight = 15.0f;

CSnowballBehavior::CSnowballBehavior()
{
	m_eState = STATE_NULL;
}

void CSnowballBehavior::Enter()
{
	m_pSnowballObject = nullptr;
	GetOwner()->GetCooldown() = true;
	m_eState = STATE_NULL;
	m_fPrepareTimer = sm_fPrepareDuration;
}

void CSnowballBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
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
			// If our state is null we'll be on a cooldown until we make a new snow ball
			if(GetOwner()->GetCooldown())
			{
				// If the beast is downed or it's far away transition to homing behavior
				if(pAgent->GetElementalState() > CCollidableObject::NEUTRAL ||
					toPlayerDist > sm_fMaxRange)
				{
					GetOwner()->TransitionTo(HOMING_BEHAVIOR);
					return;
				}
				// Else if the player is too close use breath
				else if(toPlayerDist < sm_fMinRange)
				{
					GetOwner()->TransitionTo(BREATH_BEHAVIOR);
					return;
				}

				// Increase the timer and turn to the player
				GetOwner()->GetCooldownTimer() += _fElapsedTime;
				TurnTo(vPlayerPos, _fElapsedTime);

				// If we got pass the cooldown, go to next state
				if(GetOwner()->GetCooldownTimer() >= sm_fAttackCooldown)
				{
					GetOwner()->GetCooldown() = false;
					GetOwner()->GetCooldownTimer() = 0.0f;
					pAgent->SetIsAttacking(true);
					m_eState = STATE_MAKE;
				}
			}
		}
		break;
	case STATE_MAKE:
		{
			// Make the snowball
			MakeSnowball(_pcPlayer);
		}
		break;
	case STATE_PREPARE:
		{
			// Do whatever needs to be done to prepare the snowball to be thrown
			PrepareSnowball(_fElapsedTime);
		}
		break;
	case STATE_THROW:
		{
			// Actually throw the snowball
			ThrowSnowball(_pcPlayer);
			// Set the cooldown back to true
			GetOwner()->GetCooldown() = true;
		}
		break;
	}
}

void CSnowballBehavior::MakeSnowball(CBaseObject* pcTarget)
{
	// Create the snowball object
	m_pSnowballObject = (CSnowball*)(GetOwner()->GetObjectManager()->NewObject(SNOWBALL_OBJECT));

	// Get the agent object's matrix
	matrix4f agentMatrix = *GetOwner()->GetAgent()->GetWorldMatrix();
	vec3f agentPos = agentMatrix.axis_pos;

	// Get the direction our agent is facing
	vec3f agentZ = agentMatrix.axis_z;
	agentZ.normalize();

	// Get the target position
	vec3f targetPos = pcTarget->GetWorldMatrix()->axis_pos;
	vec3f toTarget = targetPos - agentPos;
	toTarget.normalize();
	if(dot_product(agentZ, toTarget) <= 0)
	{
		m_eState = STATE_NULL;
		GetOwner()->GetCooldown() = true;
		return;
	}

	// Move the snowball based on the agent's z to place it near his hand
	matrix4f snowballMatrix = *m_pSnowballObject->GetWorldMatrix();
	snowballMatrix.axis_pos = agentPos;
	snowballMatrix.axis_pos += agentZ * 5.0f;
	snowballMatrix.axis_pos.y += 4.0f;

	// Set the matrix for the snowball object
	m_pSnowballObject->SetWorldMatrix(snowballMatrix);

	// Scale our snowball's matrix by the initial scale
	snowballMatrix = *m_pSnowballObject->GetWorldMatrix();
	snowballMatrix.scale_post(0.1f, 0.1f, 0.1f);
	m_pSnowballObject->SetWorldMatrix(snowballMatrix);

	// Find out what the scale per second should be
	m_vScalePerSecond = snowballMatrix.scale();
	snowballMatrix.scale_post(10.0f, 10.0f, 10.0f);
	m_vScalePerSecond = (snowballMatrix.scale() - m_vScalePerSecond) / m_fPrepareTimer;

	// Set the state to prepare
	m_eState = STATE_PREPARE;

	// Tell the agent we'll be throwing soon
	((CIceEnemy*)GetOwner()->GetAgent())->PrepareToThrow();

	// play sound effect	
	CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_THROW,GAME_OBJECT_ID_PLAYER);
}

void CSnowballBehavior::PrepareSnowball(float fElapsedTime)
{
	m_fPrepareTimer -= fElapsedTime;

	// Once timer is done, finish this state
	if(m_fPrepareTimer <= 0)
	{
		m_fPrepareTimer = sm_fPrepareDuration;
		m_eState = STATE_THROW;
	}
	// Else, scale the snowball
	else
	{
		matrix4f snowballMatrix = *m_pSnowballObject->GetWorldMatrix();
		
		snowballMatrix.xx += m_vScalePerSecond.x * fElapsedTime;
		snowballMatrix.yy += m_vScalePerSecond.y * fElapsedTime;
		snowballMatrix.zz += m_vScalePerSecond.z * fElapsedTime;
		
		m_pSnowballObject->SetWorldMatrix(snowballMatrix);
	}

	return;
}

void CSnowballBehavior::ThrowSnowball(CBaseObject* pcTarget)
{
	// Get the target position
	vec3f targetPos = pcTarget->GetWorldMatrix()->axis_pos;

	// Calculate the snowball's velocity
	matrix4f snowballMatrix = *m_pSnowballObject->GetWorldMatrix();
	vec3f toTarget = targetPos - snowballMatrix.axis_pos;

	// This is real physics! =D
	float height = (sm_fMaxHeight - snowballMatrix.axis_pos.y);
	float raiseTime = sqrt((2 * height) / -EARTH_GRAVITY.y);
	float airTime = raiseTime * 2;
	vec3f vSnowballVelocity = (toTarget / airTime) - (EARTH_GRAVITY * airTime) / 2;

	// Set the snowball's velocity so it's thrown
	m_pSnowballObject->SetVelocity(vSnowballVelocity);
	m_pSnowballObject->SetWasThrown(true);

	// Play a sound to indicate that the Ice Beast has just thrown
	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_Throw");

	// Change state back to null
	m_eState = STATE_NULL;
}


void CSnowballBehavior::Exit()
{
}