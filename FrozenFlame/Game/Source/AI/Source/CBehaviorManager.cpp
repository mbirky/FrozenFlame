#include "../Header/CBehaviorManager.h"
#include "../../Objects/Enemy.h"
#include "../../Core/CWorldManager.h"

// TRUE for Flockin on, FALSE for Flocking off - Dan
#define FLOCKING TRUE

CBehaviorManager::CBehaviorManager()
{
	// alignment
	m_fFlockingStrength[0] = 1.0f;
	// cohesion
	m_fFlockingStrength[1] = 1.0f;
	// separation
	// HACK
#if _DEBUG
	m_fFlockingStrength[2] = 15.0f;
#else
	m_fFlockingStrength[2] = 50.0f;
#endif
	m_tFocusForward = vec3f(0.0f, 0.0f, 0.0f);
}

CBehaviorManager::~CBehaviorManager()
{
	//Shutdown();
}

//void CBehaviorManager::AddFSM(CFSM* _pcFSM)
//{
//	if (_pcFSM)
//	{
//		m_vStateMachines.push_back(_pcFSM);
//	}
//}

void CBehaviorManager::AddEnemy(CEnemy* pEnemy)
{
	if (pEnemy)
	{
		m_vEnemies.push_back(pEnemy);			
	}
}

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
void CBehaviorManager::AddEnemyToFlock(CEnemy* pEnemy)
{
	if (pEnemy /*&& m_vFlockingEnemies.size() < 1*/)
	{
		m_vFlockingEnemies.push_back(static_cast<CMeleeEnemy*>(pEnemy));
	}
}

void CBehaviorManager::Update(float _fElapsedTime)
{	
	m_vFlockingEnemies.clear();

	for (unsigned int i = 0; i < m_vEnemies.size(); ++i)
	{
		// if the enemy is an initialized melee enemy and it is within a range of 20 from the player, add it to the flock
		// the range thing is technically a HACK
		if(m_vEnemies[i]->GetInitialized() && m_vEnemies[i]->GetType() == MELEE_OBJECT &&
			vec3f(CWorldManager::GetInstance()->GetObjectManager()->GetPlayer()->GetPos() - m_vEnemies[i]->GetPos()).magnitude() <= 20)
		{
			m_vFlockingEnemies.push_back((CMeleeEnemy*)(m_vEnemies[i]));
		}
		// if the enemy has died, remove it from the lists
		/*if (nullptr == m_vEnemies[i]->GetFSM())
		{
			RemoveEntityFromFlock(m_vEnemies[i]);

			vector<CEnemy*>::iterator iter = m_vEnemies.begin();
			iter += i;
			m_vEnemies.erase(iter, iter+1);
			--i;
		}*/
	}

	RemoveUninitializedEnemies();

#if FLOCKING
	// as long as there are at least 2 entities in the flock
	if (m_vFlockingEnemies.size() > 1)
	{
		UpdateFlock(_fElapsedTime);
	}
#else
	CalcFocusForward();
#endif

	unsigned int enemyListSize = m_vEnemies.size();
	for (unsigned int i = 0; i < enemyListSize; ++i)
	{
		if (m_vFlockingEnemies.size() > 0 && m_vEnemies[i] == m_vFlockingEnemies[0])
		{
			int x = 10;
		}
		if (m_vEnemies[i] && m_vEnemies[i]->GetFSM())
		{
#if FLOCKING
			m_vEnemies[i]->GetFSM()->Update(m_pcPlayer, m_vEnemies[i]->GetVelocity()/*m_tFocusForward*/, m_fFlockingStrength, _fElapsedTime);
#else
			m_vEnemies[i]->GetFSM()->Update(m_pcPlayer, /*m_vEnemies[i]->GetVelocity()*/m_tFocusForward, m_fFlockingStrength, _fElapsedTime);
#endif
		}
	}
}

void CBehaviorManager::Shutdown()
{
	m_pcPlayer = nullptr;
	m_vEnemies.clear();
}

void CBehaviorManager::CalcFocusForward()
{
	vec3f tempVec = vec3f(0.0f, 0.0f, 0.0f);
	unsigned int enemyListSize = m_vEnemies.size();
	for (unsigned int i = 0; i < enemyListSize; ++i)
	{
		if (m_vEnemies[i]->GetFSM())
		{
			if (m_vEnemies[i]->GetFSM()->GetCurrBehavior() && m_vEnemies[i]->GetFSM()->GetCurrBehavior()->GetBehaviorType() == HOMING_BEHAVIOR)
			{
				tempVec += m_vEnemies[i]->GetWorldMatrix()->axis_z;
			}
		}
	}
	tempVec = tempVec.normalize();
	m_tFocusForward = tempVec;
}

void CBehaviorManager::Reset()
{
	unsigned int enemyListSize = m_vEnemies.size();
	for (unsigned int i = 0; i < enemyListSize; ++i)
	{
		m_vEnemies[i]->GetFSM()->SetToInital();
	}
}

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
void CBehaviorManager::RemoveEntityFromFlock(CEnemy* pEnemy)
{
	for (vector<CMeleeEnemy*>::iterator iter = m_vFlockingEnemies.begin();
		iter != m_vFlockingEnemies.end(); iter++)
	{
		if (pEnemy == (*iter))
		{
			m_vFlockingEnemies.erase(iter);
			break;
		}
	}
}

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
void CBehaviorManager::RemoveUninitializedEnemies(void)
{
	vector<int> deleteIndices;
	unsigned int listSize = m_vEnemies.size();
	for (unsigned int index = 0; index < listSize; index++)
	{
		if (m_vEnemies[index]->GetInitialized() == false)
		{
			// record this index to be deleted from m_vEnemies
			deleteIndices.push_back(index);
		}
	}
	unsigned int numDelete = deleteIndices.size();
	if (numDelete)
	{
		for (int index = numDelete - 1; index >= 0; index--)
		{
			m_vEnemies.erase(m_vEnemies.begin() + deleteIndices[index]);
		}
		deleteIndices.clear();
	}
}

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
void CBehaviorManager::CalculateFlockingAverages()
{
	m_fAveragePosition = vec3f(0, 0, 0);
	unsigned int flockSize = m_vFlockingEnemies.size();
	for(unsigned int index = 0; index < flockSize; index++)
	{
		m_fAveragePosition += m_vFlockingEnemies[index]->GetPos();
	}
	// average the positions
	m_fAveragePosition *= (1.0f / flockSize);

	m_fAverageForward = vec3f(0, 0, 0);
	for(unsigned int index = 0; index < flockSize; index++)
	{
		m_fAverageForward += m_vFlockingEnemies[index]->GetVelocity();
	}
	// average the forward velocities
	m_fAverageForward *= (1.0f / flockSize);
}

///////////////////////////////////////////////////
//	Function	:	"CalculateAlignmentAcceleration"
//	
//	Last Modified:	January 10, 2013
//
//	Input		:	pEnemy - a pointer to an entity in a flock
//
//	Return		:	none
//
//	Purpose		:	To calculate the average alignment of all entities in a flock
//////////////////////////////////////////////////
vec3f CBehaviorManager::CalculateAlignmentAcceleration(CEnemy* pEnemy)
{
	vec3f acceleration = vec3f(0, 0, 0);
	if(pEnemy->GetMoveSpeed())
	{
		acceleration.x = m_fAverageForward.x / pEnemy->GetMoveSpeed();
		acceleration.y = m_fAverageForward.y / pEnemy->GetMoveSpeed();
		acceleration.z = m_fAverageForward.z / pEnemy->GetMoveSpeed();
	}
	if (acceleration.magnitude() > 1.0f)
	{
		acceleration.normalize();
	}
	// acceleration * alignment strength
	return acceleration * m_fFlockingStrength[0];
}

///////////////////////////////////////////////////
//	Function	:	"CalculateCohesionAcceleration"
//	
//	Last Modified:	January 10, 2013
//
//	Input		:	pEnemy - a pointer to an entity in a flock
//
//	Return		:	none
//
//	Purpose		:	To calculate the average cohesion of all entities in a flock
//////////////////////////////////////////////////
vec3f CBehaviorManager::CalculateCohesionAcceleration(CEnemy* pEnemy)
{
	vec3f acceleration = vec3f(0, 0, 0);
	acceleration = m_fAveragePosition - pEnemy->GetPos();

	// The following code only occurs if there is more than one entity in the flock.
	// If there is only one entity, the average position equals the entities position, 
	// resulting in an acceleration of (0, 0, 0).
	// This check prevents a division of zero and returning an undefined vector.
	if (acceleration.x != 0 && acceleration.y != 0 && acceleration.z != 0)
	{
		float distance = acceleration.magnitude();
		acceleration.normalize();
		
		if (distance < m_fFlockRadius)
			acceleration *= distance / m_fFlockRadius;
	}

	// acceleration * cohesion strength
	return acceleration * m_fFlockingStrength[1];
}

///////////////////////////////////////////////////
//	Function	:	"CalculateSeparationAcceleration"
//	
//	Last Modified:	January 10, 2013
//
//	Input		:	pEnemy - a pointer to an entity in a flock
//
//	Return		:	none
//
//	Purpose		:	To calculate the average separation of all entities in a flock
//////////////////////////////////////////////////
vec3f CBehaviorManager::CalculateSeparationAcceleration(CEnemy* pEnemy)
{
	vec3f sum = vec3f(0, 0, 0);

	unsigned int flockSize = m_vFlockingEnemies.size();
	for(unsigned int index = 0; index < flockSize; index++)
	{
		// for each entity in the flock that isn't the passed in entity
		if (m_vFlockingEnemies[index] != pEnemy)
		{
			vec3f accel = pEnemy->GetPos() - m_vFlockingEnemies[index]->GetPos();
			float distance = accel.magnitude();
			float safeDistance = pEnemy->GetSeparationRadius() + m_vFlockingEnemies[index]->GetSeparationRadius();

			// if a collision is likely
			if (distance < safeDistance)
			{
				// scale according to distance between boids
				accel.normalize();
				accel *= (safeDistance - distance) / safeDistance;
				sum += accel;
			}
		}
	}

	if (sum.magnitude() > 1.0f)
		sum.normalize();

	// sum * separation strength
	return sum * m_fFlockingStrength[2];
}

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
void CBehaviorManager::UpdateFlock(float fElapsedTime)
{
	CalculateFlockingAverages();

	vec3f accel = vec3f(0, 0, 0);
	unsigned int flockSize = m_vFlockingEnemies.size();
	for(unsigned int index = 0; index < flockSize; index++)
	{
		CEnemy* thisEntity = m_vFlockingEnemies[index];

		// don't give a flocking velocity if the enemy is in melee behavior
		if (thisEntity->GetFSM() && (MELEE_BEHAVIOR == thisEntity->GetFSM()->GetCurrBehavior()->GetBehaviorType() || PANIC_BEHAVIOR == thisEntity->GetFSM()->GetCurrBehavior()->GetBehaviorType()))
			continue;

		accel = CalculateAlignmentAcceleration(thisEntity);
		accel += CalculateCohesionAcceleration(thisEntity);
		accel += CalculateSeparationAcceleration(thisEntity);
		accel *= thisEntity->GetMoveSpeed() * fElapsedTime;

		thisEntity->SetVelocity(thisEntity->GetVelocity() + accel );

		// uncomment to set a cap enemies' speed to a max
		/*if (thisEntity->GetVelocity().magnitude() > thisEntity->GetMoveSpeed() )
		{
			thisEntity's velocity.normalize()
			thisEntity's velocity *= maxSpeed
		}*/
		//thisEntity->Update(fElapsedTime);
	}
}