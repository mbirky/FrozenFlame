#ifndef ESCAPE_BEHAVIOR_H_
#define ESCAPE_BEHAVIOR_H_

#include "BaseBehavior.h"
#include "../../Objects/Portal.h"

class CEscapeBehavior : public CBaseBehavior
{
	CPortal* m_pcPortal;

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
	//	Mod. Date:		  1/26/2013
	//	Mod. Initials:	  JM
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
	//	Mod. Date:		  1/26/2013
	//	Mod. Initials:	  JM
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
	//	Mod. Date:		  1/26/2013
	//	Mod. Initials:	  JM
	/////////////////////////////////////
	void Exit();

	char GetBehaviorType() {return ESCAPE_BEHAVIOR;}
	void SetTarget(CPortal* _pcPortal) { m_pcPortal = _pcPortal;}



};

#endif
