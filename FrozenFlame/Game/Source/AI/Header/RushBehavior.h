#ifndef RUSH_BEHAVIOR_H
#define RUSH_BEHAVIOR_H
#include "BaseBehavior.h"

class CRushBehavior : public CBaseBehavior
{
	float m_fExplosionTimer;
	bool m_bBombLit;
	bool m_bPlayerHit;
	int m_nSoundCounter;
	int m_nDegreesToSpin;
public:

	CRushBehavior();

	~CRushBehavior();

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
	char GetBehaviorType() {return RUSH_BEHAVIOR;}

	bool GetIsBombLit(void)const {return m_bBombLit;}

	void SetIsBombLit(bool bIsBombLit)	{m_bBombLit = bIsBombLit;}
	void SetBombTimer(float fTimer)		{m_fExplosionTimer = fTimer;}

};

#endif