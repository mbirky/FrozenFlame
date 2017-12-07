/////////////////////////////////////
//	Filename		:	FSM.h
//	Date			:	10/29/2012
//	Mod. Date		:	11/11/2012
//	Mod. Initials	:	SW
//	Author			:	Sean Welch
//	Purpose			:	This class defines the data members and functionality 
//					:	for a Finite State Machine Data Structure for AI behaviors.
/////////////////////////////////////
#ifndef CFSM_H_
#define CFSM_H_
#include <vector>
#include "CBehaviors.h"
//#include "BaseBehavior.h"
#include "../../Objects/ObjectManager.h"

class CBehaviorManager;

//class CBehaviors;
struct TNode;

class CFSM
{
	friend class CBaseBehavior;
	std::vector<CBaseBehavior*> m_vBehaviors;
	CBaseBehavior* m_pcCurrBehavior;
	CBaseBehavior* m_pcPrevBehavior;
	CEnemy* m_pcAgent;
	//float m_fMaxSpeed;
	bool m_bIsChasing;
	char m_cInitialBehavior;
	bool m_bCooldown;
	float m_fCooldownTimer;
	CObjectManager* m_pcOM;
	bool m_bShutdown;
	float m_fCurTime;

public:
	CFSM();
	~CFSM();
	
	/////////////////////////////////////
	//	Update() : Calls the current state's Update function.
	//
	//	Ins:			  float _fElapsedTime
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime);

	/////////////////////////////////////
	//	TransitionTo() : Changes the current state to the behavior passed in, if it exists
	//					 within the vector.
	//
	//	Ins:			  CBaseBehavior* _pcBehavior
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void TransitionTo(char _cBehavior);
	
	/////////////////////////////////////
	//	AddState() : Adds the passed in behavior to the vector, if one does not already exist.
	//
	//	Ins:			  CBaseBehavior* _pcBehavior
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void AddState(CBaseBehavior* _pcBehavior, bool _bSetAsCurrent = false);

	/////////////////////////////////////
	//	RemoveState() : Removes the passed in behavior to the vector, if one already exists.
	//
	//	Ins:			  char _cOldBehavior, char _cNewBehavior
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void RemoveState(char _cOldBehavior, char _cNewBehavior);

	/////////////////////////////////////
	//	GetAgent() : Returns the agent this FSM is attached to.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  CEnemy*
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	CEnemy* GetAgent()	{return m_pcAgent;}

	/////////////////////////////////////
	//	SetAgent() : Sets the CEnemy* to the passed in parameter
	//
	//	Ins:			  CEnemy* _pcAgent
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void SetAgent(CEnemy* _pcAgent)	{m_pcAgent = _pcAgent;}

	/////////////////////////////////////
	//	Shutdown() : Shutsdown the FSM.
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
	void Shutdown();

	/////////////////////////////////////
	//	GetMaxSpeed() : Returns the agent's maximum speed.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  float&
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	//float& GetMaxSpeed()	{return m_fMaxSpeed;}

	/////////////////////////////////////
	//	GetCurrBehavior() : Returns the FSM's current behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  CBaseBehavior*
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	CBaseBehavior* GetCurrBehavior()	{return m_pcCurrBehavior;}

	/////////////////////////////////////
	//	GetIsChasing() : Returns a bool determining if the agent is chasing the player.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  bool&
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	bool& GetIsChasing()	{return m_bIsChasing;}

	/////////////////////////////////////
	//	SetToInitial() : Returns the CFSM to it's initial behavior.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  11/12/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void SetToInital();

	/////////////////////////////////////
	//	GetCooldown() : Returns a bool about the cooldown of the agent
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  bool&
	//	
	//	Mod. Date:		  11/13/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	bool& GetCooldown()	{return m_bCooldown;}

	/////////////////////////////////////
	//	GetCooldownTimer() : Returns a timer for the agent's cooldown
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  float&
	//	
	//	Mod. Date:		  11/13/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	float& GetCooldownTimer()	{return m_fCooldownTimer;}

	CObjectManager* GetObjectManager()	{return m_pcOM;}

	void SetObjectManager(CObjectManager* _pcOM)	{m_pcOM = _pcOM;}

	std::vector<CBaseBehavior*> GetBehaviors()	{return m_vBehaviors;}

	bool GetShutdown()	{return m_bShutdown;}

	void SetCurTime(float _fCurTime) {m_fCurTime = _fCurTime;}
};

#endif /*CFSM_H_*/