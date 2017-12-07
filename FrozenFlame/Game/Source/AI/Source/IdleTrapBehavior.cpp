#include "../Header/IdleTrapBehavior.h"
#include "../Header/CFSM.h"
#include "../../Core/CWorldManager.h"



CIdleTrapBehavior::CIdleTrapBehavior()
{
	m_fDelayTimer = 0.0f;
	m_nLightID = -1;
	m_nFire = false;
}

void CIdleTrapBehavior::Enter()
{
	/*m_fWanderAmount = (float)rand() / (float)RAND_MAX * 2.0f;
	m_fFloatTimer = m_fWanderAmount;
	m_tPointToAdvance = GetOwner()->GetAgent()->GetMatrix()->axis_z;
	m_tPointToAdvance = m_tPointToAdvance.normalize();
	m_tPointToAdvance *= GetOwner()->GetAgent()->GetMoveSpeed();
	if (!m_bPositionSet)
	{
		if ((m_tCenterPoint.x != GetOwner()->GetAgent()->GetMatrix()->axis_pos.x) && (m_tCenterPoint.y != GetOwner()->GetAgent()->GetMatrix()->axis_pos.y) && (m_tCenterPoint.z != GetOwner()->GetAgent()->GetMatrix()->axis_pos.z))
		{
			m_tCenterPoint = GetOwner()->GetAgent()->GetMatrix()->axis_pos;
		}
		m_bPositionSet = true;
	}
	m_nFloatCount = 0;*/
}

void CIdleTrapBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	int nRand = ((rand() % 1)*2)-1;
	//char cType = 0;
	CFireTrap* pcFire = nullptr;
	CIceTrap* pcIce = nullptr;
	vec3f vPlayerPos = _pcPlayer->GetPos();
	vPlayerPos.y -= 3.0f;

	if(_pcPlayer->GetIdleTime() > 0.75f)
	{
		vec4f vDiff;
		if(nRand == -1)
		{
			vDiff = vec4f(0.0f, 0.0f, 1.0f, 1.0f);
			//cType = ICETRAP_OBJECT;
			pcIce = (CIceTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
		}

		else if(nRand == 1)
		{
			vDiff = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
			//cType = FIRETRAP_OBJECT;
			pcFire = (CFireTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
		}

		// New light system - Dan
		// m_nLightID = CView::CreateWorldLight(vPlayerPos, vDiff);
		m_nLightID = CView::MakeLight(vPlayerPos, true, vDiff);
		
		//CWorldManager::MakeObject(_pcPlayer->GetPos(), cType);
		
		_pcPlayer->SetIdleTime(0.0f);
	
		if(_pcPlayer->GetIdleTime() >= 1.0f)
		{
			//if(pcFire)
			//{
			//	//((CWizardEnemy*)GetOwner())->AddTrapToVector(pcFire);
			//	pcFire->SummonTrap(0.5f, vPlayerPos);
			//
			//	if(m_nLightID != -1)
			//	{
			//		CView::RemoveLight(m_nLightID);
			//	}
			//}
			//
			//else if(pcIce)
			//{
			//	pcIce->SummonTrap(0.5f, vPlayerPos);
			//
			//	if(m_nLightID != -1)
			//	{
			//		CView::RemoveLight(m_nLightID);
			//	}
			//}
		}
	}

	int nCurState = ((CWizardEnemy*)GetOwner())->GetWizardForm();

	if(1 == nCurState)
	{
//		((CWizardEnemy*)GetOwner())->GetFSM()->TransitionTo(FLOAT_BEHAVIOR);
	}

	if(2 == nCurState)
	{
	//	((CWizardEnemy*)GetOwner())->GetFSM()->TransitionTo(HYPERDASH_BEHAVIOR);
	}
	
}

void CIdleTrapBehavior::Exit()
{

}