#include "../Header/PanicBehavior.h"
#include "../../AI/Header/CFSM.h"

void CPanicBehavior::Enter()
{
	m_fTimeToChange = (float)rand() / (float)RAND_MAX * 2.0f;
	m_fChangeTimer = 0.0f;
}

void CPanicBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	//CEnemy* tempEnemy = GetOwner()->GetAgent();

	//if (tempEnemy->GetElementalState() < CCollidableObject::BURNING)
	//	GetOwner()->TransitionTo(HOMING_BEHAVIOR);

	//vec3f zAxis = tempEnemy->GetWorldMatrix()->axis_z;
	//zAxis = zAxis.normalize();
	//zAxis *= tempEnemy->GetMoveSpeed();
	//tempEnemy->SetVelocity(zAxis);
	////tempEnemy->GetMatrix()->rotate_y_pre(D3DXToRadian(10.0f));
	//GetOwner()->GetAgent()->SetIsMoving(true);

	m_fChangeTimer += _fElapsedTime;
	CEnemy* tempEnemy = GetOwner()->GetAgent();

	//if (tempEnemy->GetElementalState() < CCollidableObject::BURNING)
		//GetOwner()->TransitionTo(HOMING_BEHAVIOR);

	if (m_fChangeTimer < m_fTimeToChange)
	{
		vec3f zAxis = tempEnemy->GetWorldMatrix()->axis_z;
		zAxis = zAxis.normalize();
		zAxis *= tempEnemy->GetMoveSpeed();
		tempEnemy->SetVelocity(zAxis);
		//tempEnemy->GetWorldMatrix()->rotate_y_pre(D3DXToRadian(10.0f));
		matrix4f tempMat = *tempEnemy->GetWorldMatrix();
		tempMat.rotate_y_pre(D3DXToRadian(10.0f));
		tempEnemy->SetWorldMatrix(tempMat);
		GetOwner()->GetAgent()->SetIsMoving(true);
	}
	else
	{
		vec3f zAxis = tempEnemy->GetWorldMatrix()->axis_z;
		zAxis = zAxis.normalize();
		zAxis *= tempEnemy->GetMoveSpeed();
		tempEnemy->SetVelocity(zAxis);
	}
}

void CPanicBehavior::Exit()
{

}
