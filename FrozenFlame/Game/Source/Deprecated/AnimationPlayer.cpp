#if 0

#include "AnimationPlayer.h"
#include "AnimationManager.h"
#include "../Globals.h"
#include "../Renderer/View.h"
#include "../Renderer/RenderInstance.h"

#include "../Renderer/Managers.h"
#include "AnimationBlender.h"

#include "Animation.h"


CAnimationPlayer::CAnimationPlayer(void) //: curMesh(0)
{
	m_pAnimation		= nullptr;
	m_pBlendAnimation	= nullptr;
	m_cBlender			= nullptr;
	m_fTime				= 0.0f;
}

CAnimationPlayer::~CAnimationPlayer(void)
{


	delete m_cBlender;
}

void CAnimationPlayer::SetModelID(int nModelID)
{

}

void CAnimationPlayer::Play(const int nID, bool bIsLooping, double dTime)
{
	if(m_pAnimation != nullptr && (m_pAnimation->m_cFlags & eIS_PLAYING) != 0)
	{
		m_pBlendAnimation = CAssetManager::GetAnimation(nID);
		m_pBlendAnimation->m_cFlags = eIS_PLAYING;

		if(bIsLooping)
		{
			m_pBlendAnimation->m_cFlags |= eIS_LOOPING;
		}
	}

	if(m_pAnimation == nullptr || (m_pAnimation->m_cFlags & eIS_PLAYING) == 0)
	{
		m_pAnimation = CAssetManager::GetAnimation(nID);
		m_pAnimation->m_cFlags = eIS_PLAYING;// | m_pAnimation->eIS_LOOPING;
		//m_dTime = dTime;

		if(bIsLooping)
		{
			m_pAnimation->m_cFlags |= eIS_LOOPING;
		}

		if(m_cBlender)
		{
			m_cBlender->m_pInterpolator.SetAnimation(m_pAnimation);
			m_cBlender->m_pInterpolator.SetTime(dTime);
		}

		m_vpSphereMeshes.clear();

		for(size_t j = 0; j < m_pAnimation->m_vJoints.size(); ++j)
		{
			ID3DXMesh *pSphere = CView::sm_cRenderer.CreateSphere(0.2f, 10, 10);
			m_vpSphereMeshes.push_back(pSphere);
		}
	}

	if(m_pAnimation != nullptr && (m_pAnimation->m_cFlags & eIS_PLAYING) == 0)
	{
		m_pAnimation = CAssetManager::GetAnimation(nID);
		m_pAnimation->m_cFlags = eIS_PLAYING;// | m_pAnimation->eIS_LOOPING;


		if(bIsLooping)
		{
			m_pAnimation->m_cFlags |= eIS_LOOPING;
		}
	}
}

void CAnimationPlayer::Stop(const int nID)
{
	m_pAnimation->m_cFlags &= ~eIS_PLAYING;

	if(nID != -1)
	{
		Reset();
	}
}

void CAnimationPlayer::Reset(void)
{
	m_fTime = 0.0f;
}

void CAnimationPlayer::Update(float fElapsedTime)
{
	// If we don't have any current animation, return
	if(!m_pAnimation)
	{
		return;
	}

	// If the animation is playing
	if( (m_pAnimation->m_cFlags & eIS_PLAYING))
	{
		// Add in elapsed time
		m_fTime += fElapsedTime;

		if(m_fTime >= m_pAnimation->m_fDuration)
		{
			if( (m_pAnimation->m_cFlags & eIS_LOOPING) != 0)
			{
				m_fTime -= (float)m_pAnimation->m_fDuration;
			}
			else
			{
				Stop();
				Reset();
			}
		}
	}


	if(m_cBlender)
	{
		m_cBlender->Process(this);
	}

}

void CAnimationPlayer::Render(int nShaderID)
{
}

#endif