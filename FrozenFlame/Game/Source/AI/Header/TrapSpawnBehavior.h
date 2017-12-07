#ifndef TRAP_SPAWN_BEHAVIOR_H
#define TRAP_SPAWN_BEHAVIOR_H
#include "BaseBehavior.h"

class CTrapSpawnBehavior : public CBaseBehavior
{
	float m_fSpawnTimer;
	float m_fSpawnCooldown;		// the wizard summons traps every SpawnCooldown system

public:

	CTrapSpawnBehavior();

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
	char GetBehaviorType() {return TRAP_SPAWN_BEHAVIOR;}

};

#endif
