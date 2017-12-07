#include "../Header/CFSM.h"

CFSM::CFSM()
{
	m_pcCurrBehavior = nullptr;
	m_pcPrevBehavior = nullptr;
	m_pcAgent = nullptr;
	m_bIsChasing = false;
	m_fCooldownTimer = 0.0f;
	m_bCooldown = false;
	m_pcOM = nullptr;
	m_bShutdown = false;
	m_fCurTime = 0.0f;
}

CFSM::~CFSM()
{
	Shutdown();
}

void CFSM::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	if (m_pcAgent->GetInitialized() == false/* || m_pcAgent->GetIsDying()*/)
		return;

	if(m_pcAgent->GetType() == WIZARD_OBJECT)
	{
		_fElapsedTime = m_fCurTime;
	}

	if (m_pcCurrBehavior)
	{
		m_pcCurrBehavior->Update(_pcPlayer, _tFocusForward, _fFlockingStrength, _fElapsedTime);
		//m_pcAgent->TurnTo(_pcPlayer->GetWorldPosition(), _fElapsedTime);
	}
}

void CFSM::TransitionTo(char _cBehavior)
{
	if (m_vBehaviors.size() == 0)
		return;

	for (unsigned int i = 0; i < m_vBehaviors.size(); ++i)
	{
		if (m_vBehaviors[i]->GetBehaviorType() == _cBehavior)
		{
			if (m_pcCurrBehavior)
				m_pcCurrBehavior->Exit();
			m_pcPrevBehavior = m_pcCurrBehavior;
			m_pcCurrBehavior = m_vBehaviors[i];
			m_pcCurrBehavior->Enter();
			break;
		}
	}
}

void CFSM::AddState(CBaseBehavior* _pcBehavior, bool _bSetAsCurrent)
{
	if (_pcBehavior)
	{
		for (unsigned int i = 0; i < m_vBehaviors.size(); ++i)
		{
			if (_pcBehavior->GetBehaviorType() == m_vBehaviors[i]->GetBehaviorType())
			{
				return;
			}
		}
		m_vBehaviors.push_back(_pcBehavior);
		_pcBehavior->SetOwner(this);
		if (_bSetAsCurrent || m_vBehaviors.size() == 1)
		{
			TransitionTo(m_vBehaviors.back()->GetBehaviorType());
			m_cInitialBehavior = m_vBehaviors.size() - 1;
		}
			
	}
}

void CFSM::Shutdown()
{
	for (unsigned int i = 0; i < m_vBehaviors.size(); ++i)
	{
		if (m_vBehaviors[i])
		{
			m_vBehaviors[i]->Exit();
			delete m_vBehaviors[i];
			m_vBehaviors[i] = nullptr;
		}
	}
	m_pcAgent = nullptr;

	if (!m_vBehaviors.empty())
		m_vBehaviors.clear();

	//if (m_pcCurrBehavior)
	//{
		m_pcCurrBehavior = nullptr;
	//}
	//if (m_pcPrevBehavior)
	//{
		m_pcPrevBehavior = nullptr;
	//}
	m_pcOM = nullptr;
	m_bShutdown = true;
}

void CFSM::SetToInital()
{
	m_pcCurrBehavior = m_vBehaviors[m_cInitialBehavior];
	m_bIsChasing = false;
}

void CFSM::RemoveState(char _cOldBehavior, char _cNewBehavior)
{
	if (m_vBehaviors.size() == 0)
		return;

	TransitionTo(_cNewBehavior);

	for (unsigned int i = 0; i < m_vBehaviors.size(); ++i)
	{
		if (m_vBehaviors[i]->GetBehaviorType() == _cOldBehavior)
		{
			delete m_vBehaviors[i];
			m_vBehaviors[i] = nullptr;
			vector<CBaseBehavior*>::iterator vecIter = m_vBehaviors.begin();
			m_vBehaviors.erase(vecIter + i);
			break;
		}
	}
}