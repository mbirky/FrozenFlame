#if 0

#include "AnimationManager.h"


CAnimationManager* CAnimationManager::sm_pInstance = nullptr;

CAnimationManager::CAnimationManager(void)
{

}

CAnimationManager* CAnimationManager::GetInstance(void)
{
	if(!sm_pInstance)
		sm_pInstance = new CAnimationManager;
	return sm_pInstance;
}

void CAnimationManager::DeleteInstance(void)
{
	if(sm_pInstance)
	{
		delete sm_pInstance;
		sm_pInstance = nullptr;
	}
}

void CAnimationManager::Initialize(void)
{
}

int CAnimationManager::LoadAnimation(const char* filename)
{
	CAnimation* anim = new CAnimation;
	anim->Load(filename);

	m_vpAnimations.push_back(anim);
	return m_vpAnimations.size() - 1;
}

void CAnimationManager::Shutdown(void)
{
	for(unsigned int i = 0; i < m_vpAnimations.size(); ++i)
	{
		if(m_vpAnimations[i])
		{
			delete m_vpAnimations[i];
			m_vpAnimations[i] = nullptr;
		}
	}

	m_vpAnimations.clear();
}

#endif