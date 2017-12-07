#include "AnimPlayer.h"
#include "../Renderer/AssetManager.h"
#include "../Animations/Animation.h"
#include "AnimEvent.h"
#include "Interpolator.h"

CAnimPlayer::CAnimPlayer(void)
{
	m_nCurAnim	= ANIM_NULL;
	m_nNextAnim = ANIM_NULL;
	m_nLastPlayedAnim = ANIM_NULL;
	m_fCurTime	= 0.0f;
	m_fNextTime = 0.0f;
	m_bBlending = true;

	m_bCurLooping = false;
	m_bNextLooping = false;
	m_bPlayToEnd = false;

	for(int i = ANIM_IDLE; i < ANIM_MAX; ++i)
		m_nAnimationIDs[i] = -1;

	m_bPaused = false;

	m_fAnimSpeed = 1.0f;
	m_fCurSpeedEventMultiplier = 1.0f;
	m_fNextSpeedEventMultiplier = 1.0f;
	m_vObjectTranslationAmount = vec3f(0, 0, 0);
}

void CAnimPlayer::CancelBlending(void)
{
	m_nNextAnim = -1;
	m_fNextTime = 0.0f;
	m_bNextLooping = false;
	m_bBlending = false;
}

const CAnimPlayer& CAnimPlayer::operator=(const CAnimPlayer& rhs)
{
	m_nCurAnim = rhs.m_nCurAnim;
	m_nNextAnim = rhs.m_nNextAnim;
	m_nLastPlayedAnim = rhs.m_nLastPlayedAnim;
	m_fCurTime = rhs.m_fCurTime;
	m_fNextTime = rhs.m_fNextTime;
	m_bCurLooping = rhs.m_bCurLooping;
	m_bNextLooping = rhs.m_bNextLooping;
	m_fAnimSpeed = rhs.m_fAnimSpeed;
	m_bBlending = rhs.m_bBlending;
	for(int type = 0; type < ANIM_MAX; type++)
		m_nAnimationIDs[type] = rhs.m_nAnimationIDs[type];
	m_fCurSpeedEventMultiplier = rhs.m_fCurSpeedEventMultiplier;
	m_fNextSpeedEventMultiplier = rhs.m_fNextSpeedEventMultiplier;
	m_vObjectTranslationAmount = rhs.m_vObjectTranslationAmount;
	m_bPaused = rhs.m_bPaused;

	return *this;
}

void CAnimPlayer::Pause(void)
{
	m_bPaused = true;
}

void CAnimPlayer::Play(int nAnimID, bool bLooping)
{
	m_bPaused = false;

	// If the animID is not valid return
	if(!CAssetManager::AssertAnimation(nAnimID))
	{
		return;
	}

	// If our current animation is -1 we can just start playing this new one and not worry
	if(m_nCurAnim == -1)
	{
		m_nCurAnim = nAnimID;
		m_bCurLooping = bLooping;
		m_fCurTime = 0.0f;
		return;
	}
	// Else if we're trying to play the same animation that already is playing...
	else if(m_nCurAnim == nAnimID)
	{
		// If we are sending a different "looping" bool, change it and return
		if(m_bCurLooping != bLooping)
		{
			m_bCurLooping = bLooping;

			// If we're now looping the current animation, we have to stop blending
			if(m_bCurLooping)
			{
				CancelBlending();
			}

			return;
		}
		// Else we're trying to do the same thing we're already doing so just return
		else
		{
			return;
		}
	}
	// Now if our current animation is not null and not the one we're trying to play...
	else
	{
		// We're about to set a new animation to be blended, so if the first one is looping we better stop that
		m_bCurLooping = false;

		// If the next animation is already this one, we'll check the looping just like we did for the current
		if(m_nNextAnim == nAnimID)
		{
			if(m_bNextLooping != bLooping)
			{
				m_bNextLooping = bLooping;
				return;
			}
			// Else we're repeating code so return
			else
			{
				return;
			}
		}
		// If the next anim is different than the one we're trying to blend, let's just set
		else
		{
			m_nNextAnim = nAnimID;
			m_bNextLooping = bLooping;
			m_fNextTime = 0.0f;
			m_bBlending = true;
			return;
		}
	}
}

void CAnimPlayer::PlayImmediate(int nAnimID, bool bLooping)
{
	m_bPaused = false;

	// If the animID is not valid return
	if(!CAssetManager::AssertAnimation(nAnimID))
	{
		return;
	}

	m_nLastPlayedAnim = m_nCurAnim;
	if(m_nCurAnim != nAnimID)
	{
		m_nCurAnim = nAnimID;
		m_bCurLooping = bLooping;
		m_fCurTime = 0.0f;
	}

	CancelBlending();

	return;
}

void CAnimPlayer::Update(float fElapsedTime)
{
	// if we're not playing any animation return
	if(m_nCurAnim <= -1 || m_bPaused) return;

	// First things first, let's calculate the events outputs
	CalculateEventsOutput(fElapsedTime);

	// add the time
	m_fCurTime += fElapsedTime * m_fAnimSpeed * m_fCurSpeedEventMultiplier;
	// if we've gone beyond the animation duration
	CAnimation* pCurAnim = CAssetManager::GetAnimation(m_nCurAnim);
	if(pCurAnim && m_fCurTime >= pCurAnim->GetDuration())
	{
		// if we're not looping, we stop here and set the next one to be our current
		if(!m_bCurLooping)
		{
			m_nLastPlayedAnim = m_nCurAnim;

			
			if(m_bPlayToEnd){
				m_bPaused = true;
			}
			else if(m_nNextAnim == -1)
			{
				m_nCurAnim = m_nAnimationIDs[ANIM_IDLE];
				m_fCurTime = 0.0f;
				m_bCurLooping = true;
			}
			else
			{
				m_nCurAnim = m_nNextAnim;
				m_fCurTime = m_fNextTime;
				m_bCurLooping = m_bNextLooping;
			}

			CancelBlending();
		}
		// But if we're looping we should just reset our stuff
		else
		{
			m_fCurTime = 0.0f;
		}
	}

	// if we're blending to an animation add to its timer as well
	if(m_nNextAnim != -1 && m_bBlending)
	{
		m_fNextTime += fElapsedTime * m_fAnimSpeed * m_fNextSpeedEventMultiplier;

		CAnimation* pNextAnim = CAssetManager::GetAnimation(m_nNextAnim);
		// if we've gone beyond the blend animation duration
		if(pNextAnim && m_fNextTime >= pNextAnim->GetDuration())
		{
			// If the next is not looping, we stop here
			if(!m_bNextLooping)
			{
				CancelBlending();
			}
			// Else we'll just keep going
			else
			{
				m_fNextTime = 0.0f;
			}
		}
	}

	bool bFinishedBlending = false;
	CInterpolator::Update(m_nCurAnim, m_fCurTime, m_nNextAnim, m_fNextTime, bFinishedBlending);
	m_vCurPose = CInterpolator::GetCurPose();

	// If the interpolator says we've finished blending, set the current one to the next
	if(m_bBlending && bFinishedBlending)
	{
		m_nLastPlayedAnim = m_nCurAnim;
		if(m_nNextAnim == -1)
		{
			m_nCurAnim = m_nAnimationIDs[ANIM_IDLE];
			m_fCurTime = 0.0f;
			m_bCurLooping = true;
		}
		else
		{
			m_nCurAnim = m_nNextAnim;
			m_fCurTime = m_fNextTime;
			m_bCurLooping = m_bNextLooping;
		}

		CancelBlending();
	}
}

void CAnimPlayer::Stop(void)
{
	m_nCurAnim = -1;
	m_nNextAnim = -1;
	m_fCurTime = 0.0f;
	m_fNextTime = 0.0f;
	m_bCurLooping = false;
	m_bNextLooping = false;
	m_bBlending = false;
	m_bPaused = false;
}



// used to load animation ids
void CAnimPlayer::LoadAnimation(EAnimType eType, string szName)
{
	m_nAnimationIDs[eType] = CAssetManager::LoadAnimation(szName);
}

// used to play animation ids
void CAnimPlayer::PlayAnimation(EAnimType eType, bool bLooping)
{
	Play(m_nAnimationIDs[eType], bLooping);
}

// used to play the animation without blending
void CAnimPlayer::PlayAnimationImmediate(EAnimType eType, bool bLooping)
{
	PlayImmediate(m_nAnimationIDs[eType], bLooping);
}

void CAnimPlayer::PlayAnimationToEnd(EAnimType eType)
{
	m_bPlayToEnd = true;
	PlayImmediate(m_nAnimationIDs[eType], false);
}

float CAnimPlayer::GetCurrentAnimDuration(void) const
{
	if(m_nCurAnim != -1)
	{
		CAnimation* pAnim = CAssetManager::GetAnimation(m_nCurAnim);

		if(pAnim)
		{
			return pAnim->GetDuration();
		}
	}
	return 0.0f;
}

EAnimType CAnimPlayer::GetLastPlayedAnimType(void) const
{
	if(m_nLastPlayedAnim == -1) return ANIM_NULL;

	for(int i = ANIM_IDLE; i < ANIM_MAX; ++i)
	{
		if(m_nAnimationIDs[i] == m_nLastPlayedAnim)
		{
			return (EAnimType)i;
		}
	}
	return ANIM_NULL;
}

float	CAnimPlayer::GetAnimDuration(EAnimType eType) const
{
	if(eType > ANIM_NULL && eType < ANIM_MAX)
	{
		CAnimation* pAnim = CAssetManager::GetAnimation(m_nAnimationIDs[eType]);

		if(pAnim)
		{
			return pAnim->GetDuration();
		}
	}
	return 0.0f;
}

// Calculates the speed multiplier for the current animations
// and the amount the object that has this animation player
// should be translated by
void CAnimPlayer::CalculateEventsOutput(float fElapsedTime)
{
	// First we'll do everything for the current animation
	CAnimation* pCurAnim = CAssetManager::GetAnimation(m_nCurAnim);
	if(!pCurAnim) return;

	vec3f vCurAnimTranslation(0, 0, 0);		// Amount to translate for the current anim
	m_fCurSpeedEventMultiplier = 1.0f;		// Speed multiplier for the current anim

	// Get the number of the current key frame for our root joint
	int nCurFrame = CInterpolator::GetKeyframeNumber(m_nCurAnim, 0, m_fCurTime);
	
	// Loop through all the events for the current animation
	for(vector<const IAnimEvent*>::const_iterator iter = pCurAnim->GetAnimEvents().begin();
		iter != pCurAnim->GetAnimEvents().end(); ++iter)
	{
		// If we're between the start and end frame of this event
		if(nCurFrame >= (*iter)->GetStartFrame() && nCurFrame < (*iter)->GetEndFrame())
		{
			// Do what needs to be done based on the event type
			switch((*iter)->GetType())
			{
			case ANMEVENT_SPEED:
				m_fCurSpeedEventMultiplier *= ((CSpeedAnimEvent*)*iter)->GetSpeedMultiplier();
				break;
			case ANMEVENT_TRANSLATE:
				vCurAnimTranslation += ((CTranslateAnimEvent*)*iter)->GetTranslation();
				break;
			}
		}
	}

	// Set the object translation amount based on all this
	m_vObjectTranslationAmount = vCurAnimTranslation * fElapsedTime * m_fAnimSpeed * m_fCurSpeedEventMultiplier;

	// Now we do everything for the next animation, if there's any
	CAnimation* pNextAnim = CAssetManager::GetAnimation(m_nNextAnim);
	if(!pNextAnim) return;

	vec3f vNextAnimTranslation(0, 0, 0);	// Amount to translate for the next anim
	m_fNextSpeedEventMultiplier = 1.0f;		// Speed multiplier for the next anim

	// Get the number of the current key frame for our root joint
	int nNextAnimCurFrame = CInterpolator::GetKeyframeNumber(m_nNextAnim, 0, m_fNextTime);

	// Loop through all the events for the next animation
	for(vector<const IAnimEvent*>::const_iterator iter = pNextAnim->GetAnimEvents().begin();
		iter != pNextAnim->GetAnimEvents().end(); ++iter)
	{
		// If we're between the start and end frame of this event
		if(nNextAnimCurFrame >= (*iter)->GetStartFrame() && nNextAnimCurFrame < (*iter)->GetEndFrame())
		{
			// Do what needs to be done based on the event type
			switch((*iter)->GetType())
			{
			case ANMEVENT_SPEED:
				m_fNextSpeedEventMultiplier *= ((CSpeedAnimEvent*)*iter)->GetSpeedMultiplier();
				break;
			case ANMEVENT_TRANSLATE:
				vNextAnimTranslation += ((CTranslateAnimEvent*)*iter)->GetTranslation();
				break;
			}
		}
	}

	// The object translation will be an average of the current with the next one
	m_vObjectTranslationAmount += vNextAnimTranslation * fElapsedTime * m_fAnimSpeed * m_fNextSpeedEventMultiplier;
	m_vObjectTranslationAmount *= 0.5f;
}
