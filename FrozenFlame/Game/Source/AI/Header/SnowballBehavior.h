#ifndef SNOWBALL_BEHAVIOR_H
#define SNOWBALL_BEHAVIOR_H
#include "BaseBehavior.h"

class CSnowballBehavior : public CBaseBehavior
{
	enum EState { STATE_NULL, STATE_MAKE, STATE_PREPARE, STATE_THROW };
	EState m_eState;

	// Snowball this will create
	CSnowball* m_pSnowballObject;

	// Helper functions
	void MakeSnowball(CBaseObject* pcTarget);
	void PrepareSnowball(float fElapsedTime);
	void ThrowSnowball(CBaseObject* pcTarget);

	// Timer used for the preparation of the snowball
	float m_fPrepareTimer;

	// Amount to scale the snowball per second
	vec3f m_vScalePerSecond;

	// Time it takes to prepare the snowball
	static const float sm_fPrepareDuration;

	// Time the Ice Beast is on cooldown after throwing a snowball
	static const float sm_fAttackCooldown;

	// Minimun range for him to throw a snowball
	static const float sm_fMinRange;

	// Maximum range for him to throw a snowball
	static const float sm_fMaxRange;

	// Max height the snowball will always reach
	static const float sm_fMaxHeight;

public:

	CSnowballBehavior();

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
	char GetBehaviorType() {return SNOWBALL_BEHAVIOR;}

};

#endif