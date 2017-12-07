#ifndef BREATH_BEHAVIOR_H
#define BREATH_BEHAVIOR_H
#include "BaseBehavior.h"

class CBreathBehavior : public CBaseBehavior
{
	enum EState { STATE_NULL, STATE_INHALE, STATE_EXHALE, STATE_TURN, STATE_RUN };
	EState m_eState;

	float m_fAttackTimer;

	unsigned char m_cSoundCounter;

	CEmitter* m_pBreathParticleEmitter;		// the ice particles
	CEmitter* m_pBreathCloudEmitter;		// the misty cloud effect

	// Timer used for the preparation of the ice breath
	float m_fPrepareTimer;

	// Timer used for the running state
	float m_fRunTimer;

	// Time it takes to prepare the breath
	static const float sm_fPrepareDuration;

	// Time the Ice Beast stays running
	static const float sm_fRunDuration;

	// Time the Ice Beast is on cooldown after using its ice breath
	static const float sm_fAttackCooldown;

	// Maximum range for him to use the ice breath
	static const float sm_fMaxRange;

	// Helper functions
	void PrepareBlow(float fElapsedTime, CPlayerObject* pTarget);
	void RunAway(vec3f toTarget, float fElapsedTime);
	void SetEmitterPositions(CIceEnemy* pAgent);

public:

	CBreathBehavior();

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
	char GetBehaviorType() {return BREATH_BEHAVIOR;}

};

#endif