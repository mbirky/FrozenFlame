////////////////////////////////////////////////
//	File			:	"HomingBehavior.h"
//
//	Author			:	Sean Welch(SW) and Steven Seufert (SMS)
//			
//	Last Modified	:	January 9, 2013
//
//	Purpose			:	To declare all variables and functions for the enemy homing behavior
/////////////////////////////////////////////////
#ifndef HOMING_BEHAVIOR_H
#define HOMING_BEHAVIOR_H

#include "BaseBehavior.h"

class CHomingBehavior : public CBaseBehavior
{
private:
	float	m_fIceWalkTimer;	// how long it's been since the ice golem played its ice walk sound
	float	m_fIceWalkBuffer;	// how often the ice golem's walk sound can be played
public:

	/////////////////////////////////////
	//	Enter() : Initializes any and all data members of the behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  11/10/2012
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
	//	Mod. Date:		  11/10/2012
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
	//	Mod. Date:		  11/10/2012
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
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	char GetBehaviorType() {return HOMING_BEHAVIOR;}

	/////////////////////////////////////
	//	CalcAlignmentAccel() : Creates and returns an alignment vector.
	//
	//	Ins:			  vec3f _tFocusForward, float _fElapsedTime
	//	
	//	Outs:		      void
	//	
	//	Returns:		  vec3f
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	vec3f CalcAlignmentAccel(vec3f _tFocusForward, float _fFlockStrength, float _fElapsedTime);

	/////////////////////////////////////
	//	CalcCohesionAccel() : Creates and returns a cohesion vector.
	//
	//	Ins:			  CPlayerObject* _pcPlayer, float _fElapsedTime
	//	
	//	Outs:		      void
	//	
	//	Returns:		  vec3f
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	vec3f CalcCohesionAccel(CPlayerObject* _pcPlayer, float _fFlockStrength, float _fElapsedTime);

	/////////////////////////////////////
	//	CalcSeperationAccel() : Creates and returns a seperation vector.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  vec3f
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	vec3f CalcSeperationAccel();
};

#endif