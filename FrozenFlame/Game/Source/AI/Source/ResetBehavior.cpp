#include "../Header/FloatBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/Interpolator.h"
#include "../../Animations/Animation.h"
#include "../../Core/CWorldManager.h"

CResetBehavior::CResetBehavior()
{
	m_vDestination.make_zero();
	m_bDestinationSet = false;
	//GetOwner()->GetIsChasing() = false;
	m_fPauseTimer = 0.0f;
}

void CResetBehavior::Enter()
{
	m_vDestination.make_zero();
	m_bDestinationSet = false;
	GetOwner()->GetIsChasing() = false;
	m_fPauseTimer = 0.0f;

	unsigned int nLastLevel = CWorldManager::GetInstance()->GetTotalCurrentLevels();// - 1;
	nLastLevel--;
	CWorldManager::GetInstance()->GetLevel(nLastLevel)->StopSpawners();
}

void CResetBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if(!m_bDestinationSet)
	{
		unsigned int nLastLevel = CWorldManager::GetInstance()->GetTotalCurrentLevels();// - 1;
		nLastLevel--;
		m_vDestination = vec3f(CWorldManager::GetInstance()->GetLevel(nLastLevel)->GetCenterX(),0.0f,CWorldManager::GetInstance()->GetLevel(nLastLevel)->GetCenterZ());
		m_bDestinationSet = true;
		GetOwner()->GetAgent()->SetVelocity(vec3f(0,0,0));
	}
	m_fPauseTimer += _fElapsedTime;


	vec3f vDirection = vec3f(m_vDestination - GetOwner()->GetAgent()->GetPos());
	vDirection.y = 0.0f;

	if(vDirection.magnitude() > 1.0f)
	{
		TurnTo(m_vDestination,_fElapsedTime);

		//vec3f vDirection = m_vDestination - GetOwner()->GetAgent()->GetPos();

		vDirection = vDirection.normalize();

		GetOwner()->GetAgent()->SetPos(GetOwner()->GetAgent()->GetPos() + vDirection * _fElapsedTime * 5);
		//GetOwner()->GetAgent()->SetVelocity(vDirection * GetOwner()->GetAgent()->GetMoveSpeed());

		m_fPauseTimer = 0.0f;
	}
	else if(m_fPauseTimer < 2.0f)
	{
		GetOwner()->GetAgent()->SetVelocity(vec3f(0,0,0));
		if(GetOwner()->GetAgent()->GetPos().y > 0.5f)
			GetOwner()->GetAgent()->SetPos(GetOwner()->GetAgent()->GetPos() + vec3f(0.0f,-5.0f * _fElapsedTime,0.0f));
		if(GetOwner()->GetAgent()->GetPos().y < 0.5f)
		{
			vec3f tempos = GetOwner()->GetAgent()->GetPos();
			GetOwner()->GetAgent()->SetPos(vec3f(tempos.x,0.5f,tempos.z));
		}
		TurnTo(GetOwner()->GetObjectManager()->GetPlayer()->GetPos(),_fElapsedTime);
	}
	else
	{
		GetOwner()->TransitionTo(FLOAT_BEHAVIOR);
		//CWorldManager::GetInstance()->GetLevel( CWorldManager::GetInstance()->GetLevelForObject(GetOwner()->GetAgent()))->ResetActive();//SetActive(false);
		m_bDestinationSet = false;
		CWorldManager::GetInstance()->GetLevel( CWorldManager::GetInstance()->GetTotalCurrentLevels() - 1 )->ResetActive();
		GetOwner()->GetAgent()->SetHealth(GetOwner()->GetAgent()->GetMaxHealth());
	}

}

void CResetBehavior::Exit()
{
	GetOwner()->GetAgent()->SetIsMoving(false);

}
