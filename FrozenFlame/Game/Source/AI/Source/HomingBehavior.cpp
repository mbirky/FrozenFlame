////////////////////////////////////////////////
//	File			:	"HomingBehavior.cpp"
//
//	Author			:	Sean Welch(SW) and Steven Seufert (SMS)
//			
//	Last Modified	:	January 9, 2013
//
//	Purpose			:	To define all variables and functions for the enemy homing behavior
/////////////////////////////////////////////////
#include "../Header/HomingBehavior.h"
#include "../Header/CFSM.h"

void CHomingBehavior::Enter()
{
	m_fIceWalkTimer = 0.0f;
	m_fIceWalkBuffer = 0.4f;
}

void CHomingBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	m_fIceWalkTimer += _fElapsedTime;
	// if this is an ice golem, play its walk sound every third of a second while walking
	if (GetOwner()->GetAgent()->GetType() == ICE_OBJECT && m_fIceWalkTimer > m_fIceWalkBuffer)
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_ICEGOLEM_WALK, GetOwner()->GetAgent()->GetSoundID());
		m_fIceWalkTimer = 0.0f;
	}

	vec3f tempVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

	if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::FROZEN && GetOwner()->GetAgent()->GetType() != ICE_OBJECT)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		return;
	}
	else if (GetOwner()->GetAgent()->GetType() == FIRE_OBJECT)
	{
		if(GetOwner()->GetAgent()->GetElementalState() > CCollidableObject::COLD)
		{
			if (tempVec.magnitude() < ROLL_RANGE)
			{
				GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
				//GetOwner()->TransitionTo(ROLL_BEHAVIOR);
			}
		}
	
	}
	else if (GetOwner()->GetAgent()->GetType() == ICE_OBJECT && GetOwner()->GetAgent()->GetElementalState() < CCollidableObject::HOT && (_pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos).magnitude() <= SNOWBALL_RANGE )
	{
		GetOwner()->TransitionTo(SNOWBALL_BEHAVIOR);
	}
	else if (GetOwner()->GetAgent()->GetType() == EXPLOSIVE_OBJECT && GetOwner()->GetAgent()->GetElementalState() > CCollidableObject::COLD)
	{
		GetOwner()->TransitionTo(RUSH_BEHAVIOR);
	}
	
	vec3f a = vec3f(0.0f, 0.0f, 0.0f);

	a += CalcAlignmentAccel(_tFocusForward, _fFlockingStrength[0], _fElapsedTime);
	a += CalcCohesionAccel(_pcPlayer, _fFlockingStrength[1], _fElapsedTime);
	a.y = 0.0f;
	
	//tempVec += a;
	tempVec += GetOwner()->GetAgent()->GetVelocity();

	/*GetOwner()->GetAgent()->SetVelocity(*GetOwner()->GetAgent()->GetVelocityVector() + a);

	GetOwner()->GetAgent()->SetVelocity(GetOwner()->GetAgent()->GetVelocityVector()->normalize());

	GetOwner()->GetAgent()->SetVelocity(*GetOwner()->GetAgent()->GetVelocityVector() * GetOwner()->GetAgent()->GetMoveSpeed());*/
	TurnTo(_pcPlayer->GetWorldMatrix()->axis_pos, _fElapsedTime);
	Move(tempVec, _fElapsedTime);
	//Move(GetOwner()->GetAgent()->GetVelocity(), _fElapsedTime);

	GetOwner()->GetAgent()->SetIsMoving(true);

	vec3f approachVec = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

	if (approachVec.magnitude() < APPROACH_DISTANCE)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		GetOwner()->GetAgent()->SetIsMoving(false);
		m_pcOwner->TransitionTo(MELEE_BEHAVIOR);
	}
}

void CHomingBehavior::Exit()
{

}

vec3f CHomingBehavior::CalcAlignmentAccel(vec3f _tFocusForward, float _fFlockStrength, float _fElapsedTime)
{
	vec3f acc = _tFocusForward * _fFlockStrength * _fElapsedTime;

	return acc;
}

vec3f CHomingBehavior::CalcCohesionAccel(CPlayerObject* _pcPlayer, float _fFlockStrength, float _fElapsedTime)
{
	vec3f acc = _pcPlayer->GetWorldMatrix()->axis_pos - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

	vec3f temp = acc * _fFlockStrength * _fElapsedTime;

	return temp;
}