#ifndef HYPER_SNOWBALL_BEHAVIOR_H
#define HYPER_SNOWBALL_BEHAVIOR_H
#include "BaseBehavior.h"

class CHyperSnowballBehavior : public CBaseBehavior
{

	float			m_fAttackTimer;
	float			m_fSummonCooldown;		// the wizard spawns meteors every SummonCooldown seconds
	unsigned int	m_unNumMeteors;

public:

	CHyperSnowballBehavior();

	/////////////////////////////////////
	//	Enter() : Initializes any and all data members of the behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  02-06-2013
	//	Mod. Initials:	  CM
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
	//	Mod. Date:		  02-06-2013
	//	Mod. Initials:	  CM
	/////////////////////////////////////
	void Update(CPlayerObject * _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime);

	/////////////////////////////////////
	//	Exit() : Frees all dynamic memory of the behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  02-06-2013
	//	Mod. Initials:	  CM
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
	//	Mod. Date:		  02-06-2013
	//	Mod. Initials:	  CM
	/////////////////////////////////////
	char GetBehaviorType() {return HYPERSNOWBALL_BEHAVIOR;}


};

#endif