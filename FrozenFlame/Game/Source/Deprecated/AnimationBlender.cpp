#if 0

#include "AnimationBlender.h"
#include "Animation.h"
//#include "AnimationPlayer.h"

class CAnimationPlayer;

CAnimationBlender::CAnimationBlender(void)
{
}

CAnimationBlender::~CAnimationBlender(void)
{

}

char g_Playflag = 1;
void CAnimationBlender::Process(CAnimationPlayer* pAniPlayer)
{
	if(m_pInterpolator.GetAnimation() == nullptr /*&& m_pInterpolator.GetAnimation() != pInterpAnim*/)
	{
		m_pInterpolator.SetAnimation(pAniPlayer->m_pAnimation);
	}

	else if(m_pInterpolator.GetAnimation() != nullptr && (m_pInterpolator.GetAnimation()->GetFlags() & g_Playflag) == 0)
	{
		m_pInterpolator.SetAnimation(pAniPlayer->m_pAnimation);
	}

	m_pBlendingInterpolator.SetAnimation(pAniPlayer->m_pBlendAnimation);

	if(m_pBlendingInterpolator.GetAnimation() != nullptr && m_pInterpolator.GetAnimation() != m_pBlendingInterpolator.GetAnimation() )
	{
		
		m_pInterpolator.Process(pAniPlayer->m_fTime);
		pAniPlayer->m_fTime = 0.0f;
		m_pBlendingInterpolator.Process(pAniPlayer->m_fTime);
		
		m_pInterpolator.SetAnimation(pAniPlayer->m_pBlendAnimation);
		m_pBlendingInterpolator.SetAnimation(nullptr);
	}

	else
	{
		//m_pInterpolator.AddTime(dTime);
		m_pInterpolator.Process(pAniPlayer->m_fTime);
	}
}

#endif