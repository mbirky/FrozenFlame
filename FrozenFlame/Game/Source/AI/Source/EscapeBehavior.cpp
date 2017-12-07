#include "../Header/EscapeBehavior.h"
#include "../Header/CFSM.h"
#include "../../Animations/AnimPlayer.h"
#include "../../Animations/Animation.h"
#include "../../Objects/Portal.h"
#include "../../Objects/WizardEnemy.h"
#include "../../Core/CWorldManager.h"

void CEscapeBehavior::Enter()
{
	m_pcPortal = CWorldManager::GetInstance()->GetObjectManager()->GetPortal();
	CView::GetCamera().SetPlayer(GetOwner()->GetAgent());
	 ((CWizardEnemy*)GetOwner()->GetAgent())->HideBubble();
}

void CEscapeBehavior::Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime)
{
	//CPortal* pcPortal = ((CPortal*)_pcPlayer);
	if(m_pcPortal)
	{
		vec3f distanceVec = m_pcPortal->GetPos() - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;
		distanceVec += GetOwner()->GetAgent()->GetVelocity();

		TurnTo(m_pcPortal->GetPos(), _fElapsedTime);
		
		SoftAttach(m_pcPortal->GetPos(), _fElapsedTime);

		if(distanceVec.magnitude() <= PORTAL_RANGE)
		{
			((CWizardEnemy*)GetOwner()->GetAgent())->SetEscaped(true);
		}
	}
}

void CEscapeBehavior::Exit()
{
	CView::GetCamera().SetPlayer(CWorldManager::GetInstance()->GetObjectManager()->GetPlayer());
}