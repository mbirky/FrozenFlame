#ifndef RESET_BEHAVIOR_H
#define RESET_BEHAVIOR_H
#include "BaseBehavior.h"

class CResetBehavior : public CBaseBehavior
{

	vec3f m_vDestination;
	bool m_bDestinationSet;
	float m_fPauseTimer;

public:

	CResetBehavior();

	void Enter();

	void Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime);

	void Exit();

	char GetBehaviorType() {return RESET_BEHAVIOR;}

};


#endif