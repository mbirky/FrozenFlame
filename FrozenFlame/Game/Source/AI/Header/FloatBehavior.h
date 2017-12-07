#ifndef FLOAT_BEHAVIOR_H
#define FLOAT_BEHAVIOR_H
#include "BaseBehavior.h"

class CFloatBehavior : public CBaseBehavior
{
	float m_fFloatTimer;
	vec3f m_tPointToAdvance;
	vec3f m_tCenterPoint;
	float m_fRadius;
	bool m_bPositionSet;
	bool m_bStopWandering;
	float m_fWanderAmount;
	int m_nFloatCount;
	float m_fSinFloatTimer;
	bool m_bReturnToCenter;
	bool m_bStrafeLeft;
	vec3f m_tStrafePoint;
	bool m_bDirectionSet;
	vec3f m_tOriginalPosition;
public:

	CFloatBehavior();

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
	//	Update() : Searches for an open slot near the player, and goes to it.
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
	char GetBehaviorType() {return FLOAT_BEHAVIOR;}
};

#endif