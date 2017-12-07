// AnimEvent.h
// File that keeps the abstract base class IAnimEvent and all the
// events that derive from it.
// Animation events are used to have the animation or the object
// that is playing it be modified during specific frames.

#ifndef _ANIM_EVENT_H_
#define _ANIM_EVENT_H_

#include "../Globals.h"

// Types of Animation Events
enum EAnimEvents { ANMEVENT_TRANSLATE, ANMEVENT_SPEED };

// IAnimEvent ABC
class IAnimEvent
{
	// Frame in which the event starts happening
	int m_nStartFrame;
	// Frame in which the event stops happening
	// (it's going up until EndFrame but not including it)
	int m_nEndFrame;

	// Type of animation events - set by its children
	EAnimEvents m_eType;

public:
	// Constructor that takes in a type, start and end frames
	IAnimEvent(EAnimEvents eType, int nStartFrame = -1, int nEndFrame = -1)
		: m_eType(eType), m_nStartFrame(nStartFrame), m_nEndFrame(nEndFrame) {
	}
	
	// Pure virtual destructor
	virtual ~IAnimEvent(void) = 0 {
	}

	// Accessors
	inline int			GetStartFrame(void) const		{ return m_nStartFrame; }
	inline int			GetEndFrame(void)	const		{ return m_nEndFrame; }
	inline EAnimEvents	GetType(void)		const		{ return m_eType; }
	
	// Mutators
	inline void			SetStartFrame(int nStartFrame)	{ m_nStartFrame = nStartFrame; }
	inline void			SetEndFrame(int nEndFrame)		{ m_nEndFrame = nEndFrame; }
};

// CTranslateAnimEvent
// Animation event class that is supposed to translate an object
// between start and end frames
class CTranslateAnimEvent : public IAnimEvent
{
	// Translation amount, in units per second
	vec3f m_vTranslation;

public:
	// Default constructor
	CTranslateAnimEvent(void)
		: IAnimEvent(ANMEVENT_TRANSLATE) {
	}

	// Constructor that takes in translation amount as vector, start and end frames
	CTranslateAnimEvent(const vec3f& vTranslation, int nStartFrame = -1, int nEndFrame = -1)
		: IAnimEvent(ANMEVENT_TRANSLATE, nStartFrame, nEndFrame), m_vTranslation(vTranslation) {
	}

	// Constructor that takes in translation amount as 3 floats, start and end frames
	CTranslateAnimEvent(float fX, float fY, float fZ, int nStartFrame = -1, int nEndFrame = -1)
		: IAnimEvent(ANMEVENT_TRANSLATE, nStartFrame, nEndFrame), m_vTranslation(fX, fY, fZ) {
	}

	// Accessors
	inline const vec3f& GetTranslation(void)const					{ return m_vTranslation; }

	// Mutators
	inline void			SetTranslation(const vec3f& vTranslation)	{ m_vTranslation = vTranslation; }
};

// CSpeedAnimEvent
// Animation event class that is supposed to multiply the speed
// an animation player has while interpolating between start and end frames
class CSpeedAnimEvent : public IAnimEvent
{
	// Speed multiplier, ratio to multiply ElapsedTime by while
	// an animation player is between start and end frames
	float m_fSpeedMultiplier;

public:
	// Default constructor
	CSpeedAnimEvent(void)
		: IAnimEvent(ANMEVENT_SPEED) {
	}

	// Constructor that takes in the speed multiplier as a float, start and end frames
	CSpeedAnimEvent(float fSpeedMultiplier, int nStartFrame = -1, int nEndFrame = -1)
		: IAnimEvent(ANMEVENT_SPEED, nStartFrame, nEndFrame), m_fSpeedMultiplier(fSpeedMultiplier) {
	}

	// Accessors
	inline float GetSpeedMultiplier(void) const				{ return m_fSpeedMultiplier; }
	
	// Mutators
	inline void SetSpeedMultiplier(float fSpeedMultiplier)	{ m_fSpeedMultiplier = fSpeedMultiplier; }
};

#endif 