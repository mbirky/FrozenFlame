#ifndef EXHAUST_BEHAVIOR_H_
#define EXHAUST_BEHAVIOR_H_

#include "BaseBehavior.h"
#include "CFSM.h"
#include "../../Globals.h"
//#include "../../Core/CWorldManager.h"

class CExhaustBehavior : public CBaseBehavior
{
	// This timer is how long the agent will spend floating downwards towards the ground.
	float m_fFloatTimer;

	// This timer is for how long the agent stays in an exhausted state.
	float m_fExhaustTimer;
	
	bool m_bTimerStart;
	vec3f m_tOriginalColor;
	bool m_bEnemiesSpawned;
	bool m_bIsExhausted;

	public:

	CExhaustBehavior();

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
	char GetBehaviorType() {return EXHAUST_BEHAVIOR;}

};

#endif /*EXHAUST BEHAVIOR_H_*/