#ifndef HYPERORBBEHAVIOR_H_
#define HYPERORBBEHAVIOR_H_

#include "BaseBehavior.h"

class CHyperOrbBehavior : public CBaseBehavior
{
	float m_fAttackTimer;
	int m_nOrbCount;
	bool m_bAttacked;
	bool m_bReady;
	float m_fDelay;
	int m_nAttackType;
	vec3f m_vNextAttackPos;
	
	float m_fAmountToAddX;
	float m_fAmountToAddZ;
	int m_nOrbChangeDirCount;

	float m_fPhaseModifier;
	float m_fDifficultyModifier;		// modifies the amount of orbs fired based on game difficulty

public:
	CHyperOrbBehavior();
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
	char GetBehaviorType() {return HYPERORB_BEHAVIOR;}
};

#endif /*HYPERORBBEHAVIOR_H_*/