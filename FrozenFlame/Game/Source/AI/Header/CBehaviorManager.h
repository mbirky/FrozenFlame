/////////////////////////////////////
//	Filename		:	CBehaviorManager.h
//	Date			:	10/31/2012
//	Mod. Date		:	11/11/2012
//	Mod. Initials	:	SW
//	Author			:	Sean Welch
//	Purpose			:	This file contains the data members and functionality
//						of the behavior manager, which updates behavioral state machines.
/////////////////////////////////////
#ifndef CBEHAVIORMANAGER_H_
#define CBEHAVIORMANAGER_H_
#include "../../Objects/ObjectManager.h"
#include "../../Objects/PlayerObject.h"
#include <vector>
#include "CFSM.h"
#include "../../Util/vec3.h"

class CFSM;
class CEnemy;
class CMeleeEnemy;

class CBehaviorManager
{
	//std::vector<CFSM*> m_vStateMachines;
	std::vector<CEnemy*> m_vEnemies;				// a list of all the enemies on this stage
	CPlayerObject* m_pcPlayer;
	
	// Flocking variables
	std::vector<CMeleeEnemy*> m_vFlockingEnemies;	// a list of all the melee enemies, used to limit the number of appropriate enemies to calculate flocking behavior
	float m_fFlockingStrength[3];	// 0 - Alignment Strength
									// 1 - Cohesion Strength
									// 2 - Seperation Strength
	vec3f m_fAveragePosition;			// the average position of all the entities in a flock
	vec3f m_fAverageForward;			// the average forward vector of all the entities in a flock
	float m_fFlockRadius;				// the max radius of the flock's size
	vec3f m_tFocusForward;

public:

	CBehaviorManager();
	~CBehaviorManager();

	/////////////////////////////////////
	//	Update() : Loops through and calls each state machine's update.
	//
	//	Ins:			float _fElapsedTime
	//	
	//	Outs:		    void
	//	
	//	Returns:		void
	//	
	//	Mod. Date:		10/31/2012
	//	Mod. Initials:	SW
	/////////////////////////////////////
	void Update(float _fElapsedTime);

	///////////////////////////////////////
	////	AddFSM() : Adds the passed in Finite State Machine to the vector.
	////
	////	Ins:			CFSM* _pcFSM
	////	
	////	Outs:		    void
	////	
	////	Returns:		void
	////	
	////	Mod. Date:		10/31/2012
	////	Mod. Initials:	SW
	///////////////////////////////////////
	//void AddFSM(CFSM* _pcFSM);

	/////////////////////////////////////
	//	AddEnemy() : Adds the passed enemy to the vector of enemies, if non-null
	//
	//	Ins:			pEnemy	- a pointer to a CEnemy object
	//	
	//	Outs:		    void
	//	
	//	Returns:		void
	//	
	//	Mod. Date:		01/09/2012
	//	Mod. Initials:	SMS
	/////////////////////////////////////
	void AddEnemy(CEnemy* pEnemy);

	///////////////////////////////////////////////////
	//	Function	:	"AddEnemyToFlock"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	none
	//
	//	Return		:	none
	//
	//	Purpose		:	To add an enemy to the behavior manager's flock
	//////////////////////////////////////////////////
	void AddEnemyToFlock(CEnemy* pEnemy);

	/////////////////////////////////////
	//	SetPlayer() : Sets the CPlayer* to the passed in parameter
	//
	//	Ins:			  CPlayerObject* _pcPlayer
	//	
	//	Outs:		      void
	//	
	//	Returns:		  void
	//	
	//	Mod. Date:		  11/10/2012
	//	Mod. Initials:	  SW
	/////////////////////////////////////
	void SetPlayer(CPlayerObject* _pcPlayer)	{m_pcPlayer = _pcPlayer;}

	/////////////////////////////////////
	//	Shutdown() : Shuts down all state machines, then re-initialize them.
	//
	//	Ins:			void
	//	
	//	Outs:		    void
	//	
	//	Returns:		void
	//	
	//	Mod. Date:		10/31/2012
	//	Mod. Initials:	SW
	/////////////////////////////////////
	void Shutdown();

	/////////////////////////////////////
	//	CalcFocusForward() : Calculates the average forward vector for the agents to follow.
	//
	//	Ins:			void
	//	
	//	Outs:		    void
	//	
	//	Returns:		void
	//	
	//	Mod. Date:		11/10/2012
	//	Mod. Initials:	SW
	/////////////////////////////////////
	void CalcFocusForward();

	/////////////////////////////////////
	//	Reset() : Resets all CFSMs to their initial behaviors.
	//
	//	Ins:			void
	//	
	//	Outs:		    void
	//	
	//	Returns:		void
	//	
	//	Mod. Date:		11/12/2012
	//	Mod. Initials:	SW
	/////////////////////////////////////
	void Reset();

	///////////////////////////////////////////////////
	//	Function	:	"RemoveEntityFromFlock"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	none
	//
	//	Return		:	none
	//
	//	Purpose		:	To remove this entity from the flock vector
	//////////////////////////////////////////////////
	void RemoveEntityFromFlock(CEnemy* pEnemy);

	///////////////////////////////////////////////////
	//	Function	:	"RemoveUninitializedEnemies"
	//	
	//	Last Modified:	January 17, 2013
	//
	//	Input		:	none
	//
	//	Return		:	none
	//
	//	Purpose		:	To remove any uninitialized enemies from m_vEnemies
	//////////////////////////////////////////////////
	void RemoveUninitializedEnemies(void);

	///////////////////////////////////////////////////
	//	Function	:	"CalculateFlockingAverages"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	none
	//
	//	Return		:	none
	//
	//	Purpose		:	To calculate the average position and the average forward vector of all entities in a flock
	//////////////////////////////////////////////////
	void CalculateFlockingAverages();

	///////////////////////////////////////////////////
	//	Function	:	"CalculateAlignmentAcceleration"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	pEnemy - a pointer to an entity in a flock
	//
	//	Return		:	the alignment velocity of this entity
	//
	//	Purpose		:	To calculate the alignment vector for an enemy in a flock
	//////////////////////////////////////////////////
	vec3f CalculateAlignmentAcceleration(CEnemy* pEnemy);

	///////////////////////////////////////////////////
	//	Function	:	"CalculateCohesionAcceleration"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	pEnemy - a pointer to an entity in a flock
	//
	//	Return		:	the cohesion velocity of the entity
	//
	//	Purpose		:	To calculate the cohesion vector for an entity in a flock
	//////////////////////////////////////////////////
	vec3f CalculateCohesionAcceleration(CEnemy* pEnemy);

	///////////////////////////////////////////////////
	//	Function	:	"CalculateSeparationAcceleration"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	pEnemy - a pointer to an entity in a flock
	//
	//	Return		:	the separation velocity of the entity
	//
	//	Purpose		:	To calculate the separation vector for an entity in a flock
	//////////////////////////////////////////////////
	vec3f CalculateSeparationAcceleration(CEnemy* pEnemy);

	///////////////////////////////////////////////////
	//	Function	:	"CalculateAlignmentAcceleration"
	//	
	//	Last Modified:	January 10, 2013
	//
	//	Input		:	fElapsedTime - the time elapsed since the last frame
	//
	//	Return		:	none
	//
	//	Purpose		:	To update the final velocity of each entity in the flock
	//////////////////////////////////////////////////
	void UpdateFlock(float fElapsedTime);

	//void ClearBehaviorList()	{m_vStateMachines.clear();}
	void ClearEnemyList()		{ m_vEnemies.clear(); }

};

#endif /*CBEHAVIORMANAGER_H_*/