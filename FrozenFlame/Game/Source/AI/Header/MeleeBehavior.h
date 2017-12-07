

#ifndef MELEE_BEHAVIOR_H
#define MELEE_BEHAVIOR_H
#include "BaseBehavior.h"

//class CBaseBehavior;

class CMeleeBehavior : public CBaseBehavior
{
	float m_fAttackTimer;
	bool m_bShakeLeft;
	bool m_bFirstShake;
	vec3f m_tOriginialPosition;
	bool m_bAttacked;
public:

	CMeleeBehavior();

	/////////////////////////////////////
	//	Enter() : Initializes any and all data members of the behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void Enter();

	/////////////////////////////////////
	//	Update() : Adds to an attack timer and attacks when the timer hits a threshold.
	//
	//	Ins:			  CPlayer* _pcPlayer, TNode* _ptSlots, float _fElapsedTime
	//	
	//	Outs:		      TNode* _ptSlots
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime);

	/////////////////////////////////////
	//	Exit() : Frees all dynamic memory of the behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void Exit();

	/////////////////////////////////////
	//	GetBehaviorType() : Returns the type of behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  char
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	char GetBehaviorType() {return MELEE_BEHAVIOR;}

	void AgentShake(float _fShakeAmount);

	void IceBeastPunch(CPlayerObject* pTarget);

	void ResetAttackTimer()				{ m_fAttackTimer = 0; }
};

#endif
