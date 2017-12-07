#ifndef HYPERDASHBEHAVIOR_H_
#define HYPERDASHBEHAVIOR_H_

#include "BaseBehavior.h"
#include "CFSM.h"

class CHyperdashBehavior : public CBaseBehavior
{
	float			m_fDashCooldown;
	float			m_fTrapTimer;
	bool			m_bTargetSet;
	vec3f			m_tEndPosition;
	unsigned char	m_ucSoundCounter;
	int				m_nDashCounter;
	vec3f			m_tStartPosition;
	char			m_cTurnDirection;
	float			m_fSpeedModifier;	// the wizard will run slower in easy mode
	float			m_fTrailTimer;

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
	char GetBehaviorType() {return HYPERDASH_BEHAVIOR;}



};

#endif /*HYPERFASHBEHAVIOR_H_*/