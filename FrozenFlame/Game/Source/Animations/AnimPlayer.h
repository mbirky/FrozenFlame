// AnimPlayer Class
// by Justin Maza & Daniel Lima
// - This holds the information for a RenderInstance's animation player
// - It controls the timer for the animations being played
// - It keeps IDs for the current and the next animation to be played
// updated 11/24/2012

#ifndef _ANIM_PLAYER_H_
#define _ANIM_PLAYER_H_

#include <string>
using std::string;
#include "../Globals.h"
#include "KeyFrame.h"

// Don't get confused - those IDs are for the index on the array of IDs that the Animation Player has.
// They don't represent the actual universal IDs for the animation, but should be used to get to them.
// For example, the actual ID of this AnimationPlayer's Idle animation is m_nAnimationIDs[ANIM_IDLE], and not just ANIM_IDLE.
enum EAnimType { ANIM_NULL = -1, ANIM_IDLE = 0, ANIM_ATTACK, ANIM_ATTACK2, ANIM_ATTACK3, ANIM_WALK, ANIM_DEATH, ANIM_DAMAGED, ANIM_SPECIAL_A, ANIM_SPECIAL_B, ANIM_MAX };

class CAnimPlayer
{
	friend class CEmitter;

	// animation IDs
	int				m_nCurAnim;
	int				m_nNextAnim;
	int				m_nLastPlayedAnim;
	// timers
	float			m_fCurTime;
	float			m_fNextTime;
	// is the current animation looping?
	bool			m_bCurLooping;
	bool			m_bNextLooping;

	float			m_fAnimSpeed;
	bool			m_bBlending;
	bool			m_bPaused;
	bool			m_bPlayToEnd;

	// ids for animations this will be playing
	int m_nAnimationIDs[ANIM_MAX];

	// plays an animation of the passed in ID
	void Play(int nAnimID, bool bLooping);
	void PlayImmediate(int nAnimID, bool bLooping);

	// Those variables are calculated and set every frame
	// by CalculateEventsOutput() function
	float			m_fCurSpeedEventMultiplier;
	float			m_fNextSpeedEventMultiplier;
	vec3f			m_vObjectTranslationAmount;

	// This is the vector with the current pose for all joints calculated by the interpolator
	vector<CKeyFrame> m_vCurPose;

	// Helper functions
	void CancelBlending(void);

	// Calculates the speed multiplier for the current animations
	// and the amount the object that has this animation player
	// should be translated by
	void CalculateEventsOutput(float fElapsedTime);

public:
	CAnimPlayer(void);

	// Assignment operator - Just copies everything from rhs to this
	const CAnimPlayer& operator=(const CAnimPlayer& rhs);

	// updates the timers and calls update on the interpolator
	void Update(float fElapsedTime);
	// stops all animations being played and resets the timer
	void Stop(void);
	// pauses the current animation
	void Pause(void);

	// used to load animation ids
	void LoadAnimation(EAnimType eType, string szName);

	// used to play animation ids
	void PlayAnimation(EAnimType eType, bool bLooping = false);

	// used to play the animation without blending
	void PlayAnimationImmediate(EAnimType eType, bool bLooping = false);

	// used to play an animation only to end and then pause
	void PlayAnimationToEnd(EAnimType eType);


	EAnimType			GetLastPlayedAnimType(void) const;
	float				GetCurrentAnimDuration(void) const;
	float				GetAnimDuration(EAnimType eType) const;

	inline bool			IsPlaying(void) const				{ return m_nCurAnim != -1; }
	inline bool			IsPlaying(EAnimType eType) const	{ return m_nCurAnim == m_nAnimationIDs[eType]; }
	inline float		GetAnimationSpeed(void) const		{ return m_fAnimSpeed; }
	inline float		GetCurTime(void) const				{ return m_fCurTime; }
	inline const vec3f& GetEventTranslation(void) const		{ return m_vObjectTranslationAmount; }
	inline const vector<CKeyFrame>& GetCurPose(void) const	{ return m_vCurPose; }
	
	inline const CKeyFrame* GetCurFrameHandle(UINT uiJoint) const
	{
		if(uiJoint < m_vCurPose.size())
			return &m_vCurPose[uiJoint];
		return nullptr;
	}

	inline void			SetCurTime(float _fCurTime)			{m_fCurTime = _fCurTime;}
	inline void			SetAnimationSpeed(float fAnimSpeed) { m_fAnimSpeed = fAnimSpeed; }
};

#endif