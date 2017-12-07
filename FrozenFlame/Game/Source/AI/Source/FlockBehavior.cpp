
// COMMENT BY DAN
// I removed this code because the current build was not compiling due to the commented header.


#if 0

#include "../Header/FlockBehavior.h"
#include "../Header/CFSM.h"

void CFlockBehavior::Enter()
{

}

void CFlockBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if (GetOwner()->GetAgent()->GetElementalState() == CCollidableObject::FROZEN && GetOwner()->GetAgent()->GetType() != ICE_OBJECT)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		return;
	}
	else if (GetOwner()->GetAgent()->GetType() == FIRE_OBJECT && GetOwner()->GetAgent()->GetElementalState() > CCollidableObject::COLD)
	{
		GetOwner()->TransitionTo(ALIGN_BEHAVIOR);
	}
	else if (GetOwner()->GetAgent()->GetType() == ICE_OBJECT && GetOwner()->GetAgent()->GetElementalState() < CCollidableObject::HOT && (_pcPlayer->GetMatrix()->axis_pos - GetOwner()->GetAgent()->GetMatrix()->axis_pos).magnitude() <= SNOWBALL_RANGE )
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
	vec3f tempVec = _pcPlayer->GetMatrix()->axis_pos - GetOwner()->GetAgent()->GetMatrix()->axis_pos;
	tempVec += a;

	/*GetOwner()->GetAgent()->SetVelocity(*GetOwner()->GetAgent()->GetVelocityVector() + a);

	GetOwner()->GetAgent()->SetVelocity(GetOwner()->GetAgent()->GetVelocityVector()->normalize());

	GetOwner()->GetAgent()->SetVelocity(*GetOwner()->GetAgent()->GetVelocityVector() * GetOwner()->GetAgent()->GetMoveSpeed());*/
	TurnTo(_pcPlayer->GetMatrix()->axis_pos, _fElapsedTime);
	Move(tempVec, _fElapsedTime);

	GetOwner()->GetAgent()->SetIsMoving(true);

	vec3f approachVec = _pcPlayer->GetMatrix()->axis_pos - GetOwner()->GetAgent()->GetMatrix()->axis_pos;

	if (approachVec.magnitude() < APPROACH_DISTANCE)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0, 0, 0));
		GetOwner()->GetAgent()->SetIsMoving(false);
		m_pcOwner->TransitionTo(MELEE_BEHAVIOR);
	}
}

void CFlockBehavior::Exit()
{

}

vec3f CFlockBehavior::CalcAlignmentAccel(vec3f _tFocusForward, float _fFlockStrength, float _fElapsedTime)
{
	vec3f acc = _tFocusForward * _fFlockStrength * _fElapsedTime;

	return acc;
}

vec3f CFlockBehavior::CalcCohesionAccel(CPlayerObject* _pcPlayer, float _fFlockStrength, float _fElapsedTime)
{
	vec3f acc = _pcPlayer->GetMatrix()->axis_pos - GetOwner()->GetAgent()->GetMatrix()->axis_pos;

	vec3f temp = acc * _fFlockStrength * _fElapsedTime;

	return temp;
}

#endif