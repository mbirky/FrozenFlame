
#ifndef BASE_BEHAVIOR_H
#define BASE_BEHAVIOR_H

#include "../../Objects/ObjectManager.h"
#include "../../Objects/Enemy.h"
//#include "CFSM.h"
#include "../../Objects/BaseObject.h"
#include "../../Renderer/Emitter.h"
#include "../../CGame.h"
#include "../../Collision/CollisionLibrary.h"

enum eBehaviorTypes {APPROACH_BEHAVIOR = 0, WANDER_BEHAVIOR, MELEE_BEHAVIOR, FLOCK_BEHAVIOR, HOMING_BEHAVIOR, RANGED_BEHAVIOR, RUSH_BEHAVIOR,
					ALIGN_BEHAVIOR, ROLL_BEHAVIOR, SNOWBALL_BEHAVIOR, BREATH_BEHAVIOR, FLOAT_BEHAVIOR, EXHAUST_BEHAVIOR, HYPERDASH_BEHAVIOR,
					HYPERORB_BEHAVIOR, IDLETRAP_BEHAVIOR, PANIC_BEHAVIOR,RESET_BEHAVIOR, ESCAPE_BEHAVIOR, TRAP_SPAWN_BEHAVIOR,HYPERSNOWBALL_BEHAVIOR,
					GLOAT_BEHAVIOR, NUM_BEHAVIORS};

#define MELEE_TIME 0.4f
#define APPROACH_DISTANCE 2.0f
#define BREATH_DISTANCE 14.0f
#define WANDER_DISTANCE 25.0f
#define ROLL_DISTANCE 15.0f
#define SNOWBALL_RANGE 25.0f
#define ROLL_RANGE 30.0f
#define PORTAL_RANGE 2.0f

//#include "CBehaviors.h"

class CFSM;
class CBehaviorManager;
struct TNode;

class CBaseBehavior
{
public:
	friend class CBaseObject;
	friend class CEnemy;


	CBaseBehavior() {m_pcOwner = nullptr;}

	~CBaseBehavior()	{m_pcOwner = nullptr;}

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
	virtual void Enter() = 0;

	/////////////////////////////////////
	//	Update() : Defines how the behavior will process world data.
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
	virtual void Update(CPlayerObject* _pcPlayer, vec3f _tFocusForward, float* _fFlockingStrength, float _fElapsedTime) = 0;

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
	virtual void Exit() = 0;

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
	virtual char GetBehaviorType() {return APPROACH_BEHAVIOR;}

	/////////////////////////////////////
	//	GetOwner() : Returns the Finite State Machine this behavior is contained within.
	//
	//	Ins:			  void
	//	
	//	Outs:		      void
	//	
	//	Returns:		  CFSM*
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	CFSM* GetOwner()	{return m_pcOwner;}

	/////////////////////////////////////
	//	SetOwner() : Sets the State Machine pointer to the one passed in.
	//
	//	Ins:			  CFSM* _pcFSM
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void SetOwner(CFSM* _pcFSM)	{m_pcOwner = _pcFSM;}

	/////////////////////////////////////
	//	Move() : Calculates the amount to move the agent.
	//
	//	Ins:			  vec3f _tVec, float _fElapsedTime
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void Move(vec3f _tVec, float _fElapsedTime);

	/////////////////////////////////////
	//	TurnTo() : Turns the agent towards the passed in point.
	//
	//	Ins:			  vec3f _tVec, float _fElapsedTime
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  10/29/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void TurnTo(vec3f _tVec, float _fElapsedTime);

	/////////////////////////////////////
	// LookAt():	Manipulates the object's vector to look at the position passed in
	//
	// Ins:			 vec3f _tVec, float _fElapsedTime
	//				 
	// Outs:		 void
	//				 
	// Returns:		 void
	//				 
	// Mod. Date:	 10/29/2012
	// Mod. Initials: SW
	////////////////////////////////////
	void LookAt(vec3f _tVec, float _fElapsedTime);
	
	bool CheckTrapDanger(vector<CCollidableObject*> &cCloseTraps);

	void TrapReaction(vector<CCollidableObject*> &cCloseTraps,float fElapsedTime);

	void AvoidTraps(float fElapsedTime);

	void SoftAttach(vec3f vPos, float fElapsedTime);

protected:
	CFSM* m_pcOwner;

};


#endif