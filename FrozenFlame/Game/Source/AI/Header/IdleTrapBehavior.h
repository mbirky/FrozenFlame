/***********************************************
 * Filename:  		CIdleTrapBehavior.h
 * Date:      		12/06/2012
 * Mod. Date: 		12/06/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		To allow the Wizard the ability
 *					to summon traps below the player
 *					when he is idle
 ************************************************/
#ifndef _IDLETRAPBEHAVIOR_H_
#define _IDLETRAPBEHAVIOR_H_

#include "BaseBehavior.h"

class CIdleTrapBehavior : public CBaseBehavior
{
	vec3f vOldPlayerPos;
	float m_fDelayTimer;
	int m_nLightID;
	bool m_nFire;
	//vector<CFireTrap> m_vFireTraps;
	//vector<CIceTrap> m_vIceTraps;

public:

	
	CIdleTrapBehavior();
	/////////////////////////////////////
	//	Enter() : Initializes any and all data members of the behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  12/6/2012
	//	Mod. Initials:	  JM
	/////////////////////////////////////
	void Enter();

	/////////////////////////////////////
	//	Update() : Updates the behavior and performs the appropriate actions
	//
	//	Ins:			  CPlayer* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  12/6/2012
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
	//	Mod. Date:		  12/6/2012
	//	Mod. Initials:	  JM
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
	//	Mod. Date:		  12/6/2012
	//	Mod. Initials:	  JM
	/////////////////////////////////////
	char GetBehaviorType() {return IDLETRAP_BEHAVIOR;}

	/**************************************************************
	* void SummonTrap()					:	Summons a trap at a specific position
	* Ins								:	float, vec3f
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	12/6/12
	* Mod. Initals						:	JM
	***************************************************************/
	//void SummonTrap(float fDelay, vec3f vPosition);

};
#endif