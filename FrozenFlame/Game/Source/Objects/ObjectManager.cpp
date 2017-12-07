/***********************************************
* Filename:  		ObjectManager.cpp
* Date:      		10/31/2012
* Mod. Date: 		1/9/2013
* Mod. Initials:	JC
* Author:    		Charles Meade
* Purpose:   		All function definitions for the object manager 
*			
************************************************/

#include "ObjectManager.h"
#include "../Util/CInput.h"
#include "../Renderer/View.h"
#include "../Collision/CollisionLibrary.h"
#include "../AI/Header/CFSM.h"
#include "../AI/Header/CBehaviors.h"
#include "../Core/CWorldManager.h"
#include "SceneryObject.h"
#include "../Achievements/Header/CAchievementManager.h"
#include "../Renderer/CHUD.h"
#include "../Renderer/CEmitterManager.h"
#include "../States/CGameState.h"


CObjectManager::CObjectManager(void)
{
	this->m_bInitialized = false;
}


CObjectManager::~CObjectManager(void)
{
	Shutdown();
}


bool CObjectManager::Initialize(int nNumMelee,int nNumRanged, int nNumExplosive, int nNumIce, int nNumFire, int nNumCollidable, int nNumEnvironment, int nNumProjectile, int nNumFireTraps, int nNumIceTraps, int nNumWalls, int nNumSnowballs,int nNumPortals, int nNumPickups)
{
	// check if initialized already
	if(m_bInitialized)
	{
		return false;
	}
	// load level and create environment objects
	if(!LoadLevel("FILE NAME FOR LEVEL HERE"))
	{
		return false;
	}
	//store number of each object
	this->m_unNumMeleeEnemies		= (unsigned int)nNumMelee;
	this->m_unNumRangedEnemies		= (unsigned int)nNumRanged;
	this->m_unNumExplosiveEnemies	= (unsigned int)nNumExplosive;
	this->m_unNumIceEnemies			= (unsigned int)nNumIce;
	this->m_unNumFireEnemies		= (unsigned int)nNumFire;
	this->m_unNumCollidableObjects	= (unsigned int)nNumCollidable;
	this->m_unNumProjectiles		= (unsigned int)nNumProjectile;
	this->m_unNumAttackObjects		= 10;
	this->m_unNumIceTraps			= (unsigned int)nNumIceTraps;
	this->m_unNumFireTraps			= (unsigned int)nNumFireTraps;
	this->m_unNumWalls				= (unsigned int)nNumWalls;
	this->m_unNumSnowballs			= (unsigned int)nNumSnowballs;
	this->m_unNumPortals			= (unsigned int)nNumPortals;
	this->m_unNumEnvironmentObjects = (unsigned int)nNumEnvironment;
	this->m_unNumPickups			= (unsigned int)nNumPickups;
	this->m_unNumSpawners			= 20;
	this->m_unNumScenery			= 50;
	this->m_unNumSigns				= 20;

	m_pcCurrSign = nullptr;

	// create objects there will only be one of
	this->m_pcPlayer = new CPlayerObject();
	m_pcPlayer->Initialize();
	this->m_pcWizardEnemy = new CWizardEnemy();
	//m_pcWizardEnemy->Initialize();
	m_bWizardActive = false;
	// create all other objects
	CreateObjects();

	this->m_vpcTrapVector.clear();

	// set initialized to true
	this->m_bInitialized = true;
	return true;
}


CPlayerObject* CObjectManager::GetPlayer()
{
	return this->m_pcPlayer;
}

CWizardEnemy* CObjectManager::GetWizard()
{
	return m_pcWizardEnemy;
}


CBaseObject* CObjectManager::NewObject(int nObjectType, int nSubType, const char* szName)
{
	if(!m_bInitialized)
	{
		return nullptr;
	}

	CBaseObject * pcReturn = nullptr;

	switch(nObjectType)
	{
	case ENVIRONMENT_OBJECT:
		{
			if(this->m_vpcInactiveEnvironmentObjects.size() <= 0)
				ExpandObjectList(ENVIRONMENT_OBJECT,10);
			if(this->m_vpcInactiveEnvironmentObjects.size() >0)
			{
				pcReturn = this->m_vpcInactiveEnvironmentObjects[m_vpcInactiveEnvironmentObjects.size()-1];
				pcReturn->Reinitialize();
				((CEnvironmentObject*)pcReturn)->SetSubType(nSubType);
				this->m_vpcInactiveEnvironmentObjects.pop_back();
				this->m_vpcActiveEnvironmentObjects.push_back((CEnvironmentObject*)pcReturn);
			}
			break;
		}
	case PROJECTILE_OBJECT:
		{
			if(this->m_vpcInactiveProjectiles.size() <= 0)
				ExpandObjectList(PROJECTILE_OBJECT,10);
			if(this->m_vpcInactiveProjectiles.size() >0)
			{
				pcReturn = this->m_vpcInactiveProjectiles[m_vpcInactiveProjectiles.size()-1];
				pcReturn->Reinitialize();
				((CExplosiveEnemy*)pcReturn)->SetSubType(nSubType);
				this->m_vpcInactiveProjectiles.pop_back();
				this->m_vpcActiveProjectiles.push_back((CProjectileObject*)pcReturn);
			}
			break;
		}
	case EXPLOSIVE_OBJECT:
		{
			if(this->m_vpcInactiveExplosiveEnemies.size() <= 0)
				ExpandObjectList(EXPLOSIVE_OBJECT,10);
			if(this->m_vpcInactiveExplosiveEnemies.size() >0)
			{
				pcReturn = this->m_vpcInactiveExplosiveEnemies[m_vpcInactiveExplosiveEnemies.size()-1];
				pcReturn->Reinitialize();
				((CExplosiveEnemy*)pcReturn)->GetFSM()->SetObjectManager(this);
				this->m_vpcInactiveExplosiveEnemies.pop_back();
				m_vpcActiveExplosiveEnemies.push_back((CExplosiveEnemy*)pcReturn);
			}
			break;
		}
	case RANGED_OBJECT:
		{
			if(this->m_vpcInactiveRangedEnemies.size() <= 0)
				ExpandObjectList(RANGED_OBJECT,10);
			if(this->m_vpcInactiveRangedEnemies.size() >0)
			{
				pcReturn = this->m_vpcInactiveRangedEnemies[m_vpcInactiveRangedEnemies.size()-1];
				pcReturn->Reinitialize();
				((CRangedEnemy*)pcReturn)->GetFSM()->SetObjectManager(this);
				this->m_vpcInactiveRangedEnemies.pop_back();
				m_vpcActiveRangedEnemies.push_back((CRangedEnemy*)pcReturn);
			}
			break;
		}
	case MELEE_OBJECT:
		{
			if(this->m_vpcInactiveMeleeEnemies.size() <= 0)
				ExpandObjectList(MELEE_OBJECT,10);
			if(this->m_vpcInactiveMeleeEnemies.size() >0)
			{
				pcReturn = this->m_vpcInactiveMeleeEnemies[m_vpcInactiveMeleeEnemies.size()-1];
				pcReturn->Reinitialize();
				((CMeleeEnemy*)pcReturn)->GetFSM()->SetObjectManager(this);
				this->m_vpcInactiveMeleeEnemies.pop_back();
				m_vpcActiveMeleeEnemies.push_back((CMeleeEnemy*)pcReturn);
			}
			break;
		}
	case FIRE_OBJECT:
		{
			if(this->m_vpcInactiveFireEnemies.size() <= 0)
				ExpandObjectList(FIRE_OBJECT,10);
			if(this->m_vpcInactiveFireEnemies.size() >0)
			{
				pcReturn = this->m_vpcInactiveFireEnemies[m_vpcInactiveFireEnemies.size()-1];
				pcReturn->Reinitialize();
				((CFireEnemy*)pcReturn)->GetFSM()->SetObjectManager(this);
				this->m_vpcInactiveFireEnemies.pop_back();
				m_vpcActiveFireEnemies.push_back((CFireEnemy*)pcReturn);
			}
			break;
		}
	case ICE_OBJECT:
		{
			if(this->m_vpcInactiveIceEnemies.size() <= 0)
				ExpandObjectList(ICE_OBJECT,10);
			if(this->m_vpcInactiveIceEnemies.size() >0)
			{
				pcReturn = this->m_vpcInactiveIceEnemies[m_vpcInactiveIceEnemies.size()-1];
				pcReturn->Reinitialize();
				((CIceEnemy*)pcReturn)->GetFSM()->SetObjectManager(this);
				this->m_vpcInactiveIceEnemies.pop_back();
				m_vpcActiveIceEnemies.push_back((CIceEnemy*)pcReturn);
			}
			break;
		}
	//case COLLIDABLE_OBJECT:
	//	{
	//		if(this->m_vpcInactiveCollidableObjects.size() <= 0)
	//			ExpandObjectList(COLLIDABLE_OBJECT,10);
	//		if(this->m_vpcInactiveCollidableObjects.size() >0)
	//		{
	//			pcReturn = this->m_vpcInactiveCollidableObjects[m_vpcInactiveCollidableObjects.size()-1];
	//			pcReturn->Reinitialize();
	//			this->m_vpcInactiveCollidableObjects.pop_back();
	//			m_vpcActiveCollidableObjects.push_back((CCollidableObject*)pcReturn);
	//		}
	//		break;
	//	}
	case ATTACK_OBJECT:
		{
			if(this->m_vpcInactiveAttackObjects.size() <= 0)
				ExpandObjectList(ATTACK_OBJECT,10);
			if(this->m_vpcInactiveAttackObjects.size() >0)
			{
				pcReturn = this->m_vpcInactiveAttackObjects[m_vpcInactiveAttackObjects.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactiveAttackObjects.pop_back();
				m_vpcActiveAttackObjects.push_back((CAttackObject*)pcReturn);
			}
			break;
		}
	case FIRETRAP_OBJECT:
		{
			if(this->m_vpcInactiveFireTraps.size() <= 0)
				ExpandObjectList(FIRETRAP_OBJECT,10);
			if(this->m_vpcInactiveFireTraps.size() >0)
			{
				pcReturn = this->m_vpcInactiveFireTraps[m_vpcInactiveFireTraps.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactiveFireTraps.pop_back();
				m_vpcActiveFireTraps.push_back((CFireTrap*)pcReturn);
			}
			break;
		}
	case ICETRAP_OBJECT:
		{
			if(this->m_vpcInactiveIceTraps.size() <= 0)
				ExpandObjectList(ICETRAP_OBJECT,10);
			if(this->m_vpcInactiveIceTraps.size() >0)
			{
				pcReturn = this->m_vpcInactiveIceTraps[m_vpcInactiveIceTraps.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactiveIceTraps.pop_back();
				m_vpcActiveIceTraps.push_back((CIceTrap*)pcReturn);
			}
			break;
		}
	case WALL_OBJECT:
		{
			if(this->m_vpcInactiveWalls.size() <= 0)
				ExpandObjectList(WALL_OBJECT,10);
			if(this->m_vpcInactiveWalls.size() >0)
			{
				pcReturn = this->m_vpcInactiveWalls[m_vpcInactiveWalls.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactiveWalls.pop_back();
				m_vpcActiveWalls.push_back((CWall*)pcReturn);
			}
			break;
		}
	case SNOWBALL_OBJECT:
		{
			if(this->m_vpcInactiveSnowballs.size() <= 0)
				ExpandObjectList(SNOWBALL_OBJECT,10);
			if(this->m_vpcInactiveSnowballs.size() >0)
			{
				pcReturn = this->m_vpcInactiveSnowballs[m_vpcInactiveSnowballs.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactiveSnowballs.pop_back();
				m_vpcActiveSnowballs.push_back((CSnowball*)pcReturn);
			}
			break;
		}
	case PORTAL_OBJECT:
		{
			if(this->m_vpcInactivePortals.size() <= 0)
				ExpandObjectList(PORTAL_OBJECT,10);
			if(this->m_vpcInactivePortals.size() >0)
			{
				pcReturn = this->m_vpcInactivePortals[m_vpcInactivePortals.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactivePortals.pop_back();
				m_vpcActivePortals.push_back((CPortal*)pcReturn);
			}
			break;
		}

	case PICKUP_OBJECT:
		{
			if(this->m_vpcInactivePickups.size() <= 0)
				ExpandObjectList(PICKUP_OBJECT,10);
			if(this->m_vpcInactivePickups.size() >0)
			{
				pcReturn = this->m_vpcInactivePickups[m_vpcInactivePickups.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactivePickups.pop_back();
				((CPickupObject*)pcReturn)->SetPickupType(nSubType);
				m_vpcActivePickups.push_back((CPickupObject*)pcReturn);
			}
			break;
		}

	case SPAWNER_OBJECT:
		{
			if(this->m_vpcInactiveSpawners.size() <= 0)
				ExpandObjectList(SPAWNER_OBJECT,10);
			if(this->m_vpcInactiveSpawners.size() >0)
			{
				pcReturn = this->m_vpcInactiveSpawners[m_vpcInactiveSpawners.size()-1];
				pcReturn->Reinitialize();
				this->m_vpcInactiveSpawners.pop_back();
				m_vpcActiveSpawners.push_back((CSpawner*)pcReturn);
			}
			break;
		}
	case WIZARD_OBJECT:
		{
			if(this->m_pcWizardEnemy)
			{
				m_pcWizardEnemy->Uninitialize();
				m_pcWizardEnemy->SetWizardSubtype(nSubType);
				m_pcWizardEnemy->Reinitialize();
				pcReturn = m_pcWizardEnemy;
				((CWizardEnemy*)pcReturn)->GetFSM()->SetObjectManager(this);
			}
			break;
		}

	case SCENERY_OBJECT:
		{
			if(this->m_vpcInactiveSceneryObjects.size() <= 0)
				ExpandObjectList(SCENERY_OBJECT, 10);

			if(this->m_vpcInactiveSceneryObjects.size() > 0)
			{
				pcReturn = this->m_vpcInactiveSceneryObjects[this->m_vpcInactiveSceneryObjects.size() - 1];
				((CSceneryObject*)pcReturn)->SetName(szName);
				pcReturn->Reinitialize();
				this->m_vpcInactiveSceneryObjects.pop_back();
				this->m_vpcActiveSceneryObjects.push_back((CSceneryObject*)pcReturn);
				//pcReturn->SetRenderID(CView::Push(pcReturn, ((CSceneryObject*)pcReturn)->GetName()));
			}
				
			break;
		}
	case SIGN_OBJECT:
		{
			if(this->m_vpcInactiveSignPosts.size() <= 0)
				ExpandObjectList(SIGN_OBJECT, 10);

			if(this->m_vpcInactiveSignPosts.size() > 0)
			{
				pcReturn = this->m_vpcInactiveSignPosts[this->m_vpcInactiveSignPosts.size() - 1];
				// In this case the szName variable will be used to transfer the block of text we want to display to the object
				((CSignpost*)pcReturn)->SetDescription(const_cast<char*>(szName));
				//int nLevel;
				//nLevel = CWorldManager::GetInstance()->GetCurrentLevel();
				//((CSignpost*)pcReturn)->SetLevel(nLevel);
				
				//((CSignpost*)pcReturn)->SetSection(CWorldManager::GetInstance()->GetCurrentSection());
				pcReturn->Reinitialize();
				this->m_vpcInactiveSignPosts.pop_back();
				this->m_vpcActiveSignPosts.push_back((CSignpost*)pcReturn);
			}
			break;
		}
	default:
		{
			LOG("INCORRECT TYPE PASSED INTO NEWOBJECT\n");
			break;
		}
	};

	return pcReturn;

}


// sort objects so that active objects remain lower in the list and objects to be deleted go at the end
bool SortDeletedObjectsLast(const CBaseObject* first, const CBaseObject* second)
{
	return (first->GetIsToBeDeleted() < second->GetIsToBeDeleted());
}

void CObjectManager::Update(float fElapsedTime)
{
	m_pcPlayer->UpdateInput(CInput::GetInstance()->GetCharacterKeys());
	m_pcPlayer->Update(fElapsedTime);
	//m_pcPlayer->GetMatrix()->axis_pos.y -= 0.981f * fElapsedTime;
	// Going to handle player collisions separate from all other collisions. - JC
	m_vpcCollisionCheckVector.push_back(m_pcPlayer);

	m_vpcTrapVector.clear();

	m_pcCurrSign = nullptr;

	for(unsigned i = 0; i < m_vpcActiveSignPosts.size(); ++i)
	{
		CSignpost* pcSign = m_vpcActiveSignPosts[i];

		float dist = (pcSign->GetPos() - m_pcPlayer->GetPos()).magnitude();

		if(dist <= 5.0f)
		{
			m_pcCurrSign = pcSign;
		}
	}
	

	if(m_pcWizardEnemy && m_pcWizardEnemy->GetInitialized())
	{
		m_pcWizardEnemy->Update(fElapsedTime);
		m_vpcCollisionCheckVector.push_back(m_pcWizardEnemy);
	}
	static bool bDeleteActiveObjects = false;	// Are there any objects that need to be deleted this frame?
	//unsigned int activeCollidableObjectSize = this->m_vpcActiveCollidableObjects.size();
	//for(unsigned int i = 0; i < activeCollidableObjectSize; i++)
	//{
	//	if(m_vpcActiveCollidableObjects[i]->GetInitialized() == false)
	//	{
	//		//m_vpcActiveAttackObjects[i]->Uninitialize();
	//		this->m_vpcInactiveCollidableObjects.push_back(m_vpcActiveCollidableObjects[i]);
	//		// mark this object to be deleted when finished iterating through this list
	//		m_vpcActiveCollidableObjects[i]->SetIsToBeDeleted(1);
	//		bDeleteActiveObjects = true;
	//		//vector<CCollidableObject *>::iterator iter;
	//		//iter = m_vpcActiveCollidableObjects.begin()+i;
	//		//m_vpcActiveCollidableObjects.erase(iter);//,iter+1);
	//		//i--;
	//		continue;
	//	}
	//	else
	//	{
	//		m_vpcActiveCollidableObjects[i]->Update(fElapsedTime);
	//		m_vpcCollisionCheckVector.push_back(m_vpcActiveCollidableObjects[i]);
	//	}
	//}
	//// if any objects need to be deleted
	//if (bDeleteActiveObjects && !m_vpcActiveCollidableObjects.empty())
	//{
	//	// Sort the active collidable objects list with all the objects to be deleted at the end
	//	sort(m_vpcActiveCollidableObjects.begin(), m_vpcActiveCollidableObjects.end(), SortDeletedObjectsLast);
	//	// now that all the deletable items are at the end of the vector, delete all these objects in one fell swoop
	//	unsigned int listSize = m_vpcActiveCollidableObjects.size();
	//	for (unsigned int index = 0; index < listSize; index++)
	//	{
	//		if (m_vpcActiveCollidableObjects[index]->GetIsToBeDeleted())
	//		{
	//			m_vpcActiveCollidableObjects.erase(m_vpcActiveCollidableObjects.begin() + index, m_vpcActiveCollidableObjects.end());
	//			break;
	//		}
	//	}
	//}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activeEnvironmentObjectSize = this->m_vpcActiveEnvironmentObjects.size();
	for(unsigned int i = 0; i < activeEnvironmentObjectSize; i++)
	{
		if(m_vpcActiveEnvironmentObjects[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveEnvironmentObjects.push_back(m_vpcActiveEnvironmentObjects[i]);
			m_vpcActiveEnvironmentObjects[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CEnvironmentObject *>::iterator iter;
			//iter = m_vpcActiveEnvironmentObjects.begin()+i;
			//m_vpcActiveEnvironmentObjects.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveEnvironmentObjects[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveEnvironmentObjects[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// Sort the active environment objects list with all the objects to be deleted at the end
		sort(m_vpcActiveEnvironmentObjects.begin(), m_vpcActiveEnvironmentObjects.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all these objects at once
		unsigned int listSize = m_vpcActiveEnvironmentObjects.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveEnvironmentObjects[index]->GetIsToBeDeleted())
			{
				m_vpcActiveEnvironmentObjects.erase(m_vpcActiveEnvironmentObjects.begin() + index, m_vpcActiveEnvironmentObjects.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int projectilesSize = this->m_vpcActiveProjectiles.size();
	for(unsigned int i = 0; i < projectilesSize; i++)
	{
		if(m_vpcActiveProjectiles[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveProjectiles.push_back(m_vpcActiveProjectiles[i]);
			m_vpcActiveProjectiles[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CProjectileObject *>::iterator iter;
			//iter = m_vpcActiveProjectiles.begin()+i;
			//m_vpcActiveProjectiles.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveProjectiles[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveProjectiles[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// Sort the active projectiles list with all the objects to be deleted at the end
		sort(m_vpcActiveProjectiles.begin(), m_vpcActiveProjectiles.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the these objects at once
		unsigned int listSize = m_vpcActiveProjectiles.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveProjectiles[index]->GetIsToBeDeleted())
			{
				m_vpcActiveProjectiles.erase(m_vpcActiveProjectiles.begin() + index, m_vpcActiveProjectiles.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false; // reset for this list
	unsigned int explosiveEnemiesSize = this->m_vpcActiveExplosiveEnemies.size();
	for(unsigned int i = 0; i < explosiveEnemiesSize; i++)
	{
		if(m_vpcActiveExplosiveEnemies[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveExplosiveEnemies.push_back(m_vpcActiveExplosiveEnemies[i]);
			m_vpcActiveExplosiveEnemies[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CExplosiveEnemy *>::iterator iter;
			//iter = m_vpcActiveExplosiveEnemies.begin()+i;
			//m_vpcActiveExplosiveEnemies.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveExplosiveEnemies[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveExplosiveEnemies[i]);
		}
		//m_vpcActiveExplosiveEnemies[i]->GetMatrix()->axis_pos.y -= 0.981f * fElapsedTime;
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// Sort the active explosive enemy list with all the objects to be deleted at the end
		sort(m_vpcActiveExplosiveEnemies.begin(), m_vpcActiveExplosiveEnemies.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all these objects at once
		unsigned int listSize = m_vpcActiveExplosiveEnemies.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveExplosiveEnemies[index]->GetIsToBeDeleted())
			{
				m_vpcActiveExplosiveEnemies.erase(m_vpcActiveExplosiveEnemies.begin() + index, m_vpcActiveExplosiveEnemies.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int meleeEnemiesSize = this->m_vpcActiveMeleeEnemies.size();
	for(unsigned int i = 0; i < meleeEnemiesSize; i++)
	{
		if(m_vpcActiveMeleeEnemies[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveMeleeEnemies.push_back(m_vpcActiveMeleeEnemies[i]);
			m_vpcActiveMeleeEnemies[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CMeleeEnemy *>::iterator iter;
			//iter = m_vpcActiveMeleeEnemies.begin()+i;
			//m_vpcActiveMeleeEnemies.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveMeleeEnemies[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveMeleeEnemies[i]);
		}
		//m_vpcActiveMeleeEnemies[i]->GetMatrix()->axis_pos.y -= 0.981f * fElapsedTime;
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// Sort the active melee enemy list with all the objects to be deleted at the end
		sort (m_vpcActiveMeleeEnemies.begin(), m_vpcActiveMeleeEnemies.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all these objects at once
		unsigned int listSize = m_vpcActiveMeleeEnemies.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveMeleeEnemies[index]->GetIsToBeDeleted())
			{
				m_vpcActiveMeleeEnemies.erase(m_vpcActiveMeleeEnemies.begin() + index, m_vpcActiveMeleeEnemies.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int rangedEnemiesSize = this->m_vpcActiveRangedEnemies.size();
	for(unsigned int i = 0; i < rangedEnemiesSize; i++)
	{
		if(m_vpcActiveRangedEnemies[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveRangedEnemies.push_back(m_vpcActiveRangedEnemies[i]);
			m_vpcActiveRangedEnemies[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CRangedEnemy *>::iterator iter;
			//iter = m_vpcActiveRangedEnemies.begin()+i;
			//m_vpcActiveRangedEnemies.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveRangedEnemies[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveRangedEnemies[i]);
		}
		//m_vpcActiveRangedEnemies[i]->GetMatrix()->axis_pos.y -= 0.981f * fElapsedTime;
	}
	
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// Sort the active ranged enemies list with all the objects to be deleted at the end
		sort (m_vpcActiveRangedEnemies.begin(), m_vpcActiveRangedEnemies.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveRangedEnemies.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveRangedEnemies[index]->GetIsToBeDeleted())
			{
				m_vpcActiveRangedEnemies.erase(m_vpcActiveRangedEnemies.begin() + index, m_vpcActiveRangedEnemies.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int fireEnemiesSize = this->m_vpcActiveFireEnemies.size();
	for(unsigned int i = 0; i < fireEnemiesSize; i++)
	{
		if(m_vpcActiveFireEnemies[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveFireEnemies.push_back(m_vpcActiveFireEnemies[i]);
			m_vpcActiveFireEnemies[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CFireEnemy *>::iterator iter;
			//iter = m_vpcActiveFireEnemies.begin()+i;
			//m_vpcActiveFireEnemies.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveFireEnemies[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveFireEnemies[i]);
		}
		//m_vpcActiveFireEnemies[i]->GetMatrix()->axis_pos.y -= 0.981f * fElapsedTime;
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// Sort the active fire enemies list with all the objects to be deleted at the end
		sort (m_vpcActiveFireEnemies.begin(), m_vpcActiveFireEnemies.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveFireEnemies.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveFireEnemies[index]->GetIsToBeDeleted())
			{
				m_vpcActiveFireEnemies.erase(m_vpcActiveFireEnemies.begin() + index, m_vpcActiveFireEnemies.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int iceEnemiesSize = this->m_vpcActiveIceEnemies.size();
	for(unsigned int i = 0; i < iceEnemiesSize; i++)
	{
		if(m_vpcActiveIceEnemies[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveIceEnemies.push_back(m_vpcActiveIceEnemies[i]);
			m_vpcActiveIceEnemies[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CIceEnemy *>::iterator iter;
			//iter = m_vpcActiveIceEnemies.begin()+i;
			//m_vpcActiveIceEnemies.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveIceEnemies[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveIceEnemies[i]);
		}
		//m_vpcActiveIceEnemies[i]->GetMatrix()->axis_pos.y -= 0.981f * fElapsedTime;
	}
	// if any obejcts need to be deleted
	if(bDeleteActiveObjects)
	{
		// Sort the active ice enemies list with all the objects to be deleted at the end
		sort (m_vpcActiveIceEnemies.begin(), m_vpcActiveIceEnemies.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the obejcts at once
		unsigned int listSize = m_vpcActiveIceEnemies.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveIceEnemies[index]->GetIsToBeDeleted())
			{
				m_vpcActiveIceEnemies.erase(m_vpcActiveIceEnemies.begin() + index, m_vpcActiveIceEnemies.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int attackObjectsSize = this->m_vpcActiveAttackObjects.size();
	for(unsigned int i = 0; i < attackObjectsSize; i++)
	{
		m_vpcActiveAttackObjects[i]->Update(fElapsedTime);

		if(m_vpcActiveAttackObjects[i]->GetActiveTimer() <0.0f)
		{
			m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveAttackObjects.push_back(m_vpcActiveAttackObjects[i]);
			m_vpcActiveAttackObjects[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			/*vector<CAttackObject*>::iterator iter;
			iter = m_vpcActiveAttackObjects.begin()+i;
			m_vpcActiveAttackObjects.erase(iter);
			i--;*/
			continue;
		}
		else
		{
			m_vpcCollisionCheckVector.push_back(m_vpcActiveAttackObjects[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active attack objects list with all the objects to be deleted at the end
		sort (m_vpcActiveAttackObjects.begin(), m_vpcActiveAttackObjects.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveAttackObjects.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if(m_vpcActiveAttackObjects[index]->GetIsToBeDeleted())
			{
				m_vpcActiveAttackObjects.erase(m_vpcActiveAttackObjects.begin() + index, m_vpcActiveAttackObjects.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int fireTrapSize = this->m_vpcActiveFireTraps.size();
	for(unsigned int i = 0; i < fireTrapSize; i++)
	{
		if(m_vpcActiveFireTraps[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveFireTraps.push_back(m_vpcActiveFireTraps[i]);
			m_vpcActiveFireTraps[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CFireTrap *>::iterator iter;
			//iter = m_vpcActiveFireTraps.begin()+i;
			//m_vpcActiveFireTraps.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveFireTraps[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveFireTraps[i]);
			m_vpcTrapVector.push_back(m_vpcActiveFireTraps[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active fire traps list with all the objects to be deleted at the end
		sort (m_vpcActiveFireTraps.begin(), m_vpcActiveFireTraps.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all these objects at once
		unsigned int listSize = m_vpcActiveFireTraps.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveFireTraps[index]->GetIsToBeDeleted())
			{
				m_vpcActiveFireTraps.erase(m_vpcActiveFireTraps.begin() + index, m_vpcActiveFireTraps.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int iceTrapsSize = this->m_vpcActiveIceTraps.size();
	for(unsigned int i = 0; i < iceTrapsSize; i++)
	{
		if(m_vpcActiveIceTraps[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveIceTraps.push_back(m_vpcActiveIceTraps[i]);
			m_vpcActiveIceTraps[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CIceTrap *>::iterator iter;
			//iter = m_vpcActiveIceTraps.begin()+i;
			//m_vpcActiveIceTraps.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveIceTraps[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveIceTraps[i]);
			m_vpcTrapVector.push_back(m_vpcActiveIceTraps[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active ice traps list with all the objects to be deleted at the end
		sort (m_vpcActiveIceTraps.begin(), m_vpcActiveIceTraps.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveIceTraps.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveIceTraps[index]->GetIsToBeDeleted())
			{
				m_vpcActiveIceTraps.erase(m_vpcActiveIceTraps.begin() + index, m_vpcActiveIceTraps.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activeWallsSize = this->m_vpcActiveWalls.size();
	for(unsigned int i = 0; i < activeWallsSize; i++)
	{
		if(m_vpcActiveWalls[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveWalls.push_back(m_vpcActiveWalls[i]);
			m_vpcActiveWalls[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CWall *>::iterator iter;
			//iter = m_vpcActiveWalls.begin()+i;
			//m_vpcActiveWalls.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveWalls[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveWalls[i]);
			m_vpcTrapVector.push_back(m_vpcActiveWalls[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active walls with all the objects to be deleted at the end
		sort (m_vpcActiveWalls.begin(), m_vpcActiveWalls.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveWalls.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveWalls[index]->GetIsToBeDeleted())
			{
				m_vpcActiveWalls.erase(m_vpcActiveWalls.begin() + index, m_vpcActiveWalls.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activeSnowballSize = this->m_vpcActiveSnowballs.size();
	for(unsigned int i = 0; i < activeSnowballSize; i++)
	{
		if(m_vpcActiveSnowballs[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveSnowballs.push_back(m_vpcActiveSnowballs[i]);
			m_vpcActiveSnowballs[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CSnowball *>::iterator iter;
			//iter = m_vpcActiveSnowballs.begin()+i;
			//m_vpcActiveSnowballs.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveSnowballs[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveSnowballs[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active walls with all the objects to be deleted at the end 
		sort (m_vpcActiveSnowballs.begin(), m_vpcActiveSnowballs.end(), SortDeletedObjectsLast);
		// now that all the deteable obejcts are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveSnowballs.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveSnowballs[index]->GetIsToBeDeleted())
			{
				m_vpcActiveSnowballs.erase(m_vpcActiveSnowballs.begin() + index, m_vpcActiveSnowballs.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activePortalsSize = this->m_vpcActivePortals.size();
	for(unsigned int i = 0; i < activePortalsSize; i++)
	{
		if(m_vpcActivePortals[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactivePortals.push_back(m_vpcActivePortals[i]);
			m_vpcActivePortals[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CPortal *>::iterator iter;
			//iter = m_vpcActivePortals.begin()+i;
			//m_vpcActivePortals.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActivePortals[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActivePortals[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active Portals with all the objects to be deleted at the end
		sort (m_vpcActivePortals.begin(), m_vpcActivePortals.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActivePortals.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActivePortals[index]->GetIsToBeDeleted())
			{
				m_vpcActivePortals.erase(m_vpcActivePortals.begin() + index, m_vpcActivePortals.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activePickupSize = this->m_vpcActivePickups.size();
	for(unsigned int i = 0; i < activePickupSize; i++)
	{
		if(m_vpcActivePickups[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactivePickups.push_back(m_vpcActivePickups[i]);
			m_vpcActivePickups[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CPickupObject *>::iterator iter;
			//iter = m_vpcActivePickups.begin()+i;
			//m_vpcActivePickups.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActivePickups[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActivePickups[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active pickups with all the obejcts to be deleted at the end
		sort (m_vpcActivePickups.begin(), m_vpcActivePickups.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActivePickups.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActivePickups[index]->GetIsToBeDeleted())
			{
				m_vpcActivePickups.erase(m_vpcActivePickups.begin() + index, m_vpcActivePickups.end());
				break;
			}
		}
	}

	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activeSpawnersSize = this->m_vpcActiveSpawners.size();
	for(unsigned int i = 0; i < activeSpawnersSize; i++)
	{
		if(m_vpcActiveSpawners[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveSpawners.push_back(m_vpcActiveSpawners[i]);
			m_vpcActiveSpawners[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CSpawner *>::iterator iter;
			//iter = m_vpcActiveSpawners.begin()+i;
			//m_vpcActiveSpawners.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveSpawners[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveSpawners[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active spawners with all the objects to be deleted at the end
		sort (m_vpcActiveSpawners.begin(), m_vpcActiveSpawners.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveSpawners.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveSpawners[index]->GetIsToBeDeleted())
			{
				m_vpcActiveSpawners.erase(m_vpcActiveSpawners.begin() + index, m_vpcActiveSpawners.end());
				break;
			}
		}
	}

	/**********/
	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activeScenerySize = this->m_vpcActiveSceneryObjects.size();
	for(unsigned int i = 0; i < activeScenerySize; i++)
	{
		if(m_vpcActiveSceneryObjects[i]->GetInitialized() == false)
		{
			this->m_vpcInactiveSceneryObjects.push_back(m_vpcActiveSceneryObjects[i]);
			m_vpcActiveSceneryObjects[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			continue;
		}
		else
		{
			m_vpcActiveSceneryObjects[i]->Update(fElapsedTime);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active scenery objects with all the objects to be deleted at the end
  		sort (m_vpcActiveSceneryObjects.begin(), m_vpcActiveSceneryObjects.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveSceneryObjects.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveSceneryObjects[index]->GetIsToBeDeleted())
			{
				m_vpcActiveSceneryObjects.erase(m_vpcActiveSceneryObjects.begin() + index, m_vpcActiveSceneryObjects.end());
				break;
			}
		}
	}

	/**********/
	bDeleteActiveObjects = false;	// reset for this list
	unsigned int activeSignSize = this->m_vpcActiveSignPosts.size();
	for(unsigned int i = 0; i < activeSignSize; i++)
	{
		if(m_vpcActiveSignPosts[i]->GetInitialized() == false)
		{
			//m_vpcActiveAttackObjects[i]->Uninitialize();
			this->m_vpcInactiveSignPosts.push_back(m_vpcActiveSignPosts[i]);
			m_vpcActiveSignPosts[i]->SetIsToBeDeleted(1);
			bDeleteActiveObjects = true;
			//vector<CSpawner *>::iterator iter;
			//iter = m_vpcActiveSpawners.begin()+i;
			//m_vpcActiveSpawners.erase(iter);//,iter+1);
			//i--;
			continue;
		}
		else
		{
			m_vpcActiveSignPosts[i]->Update(fElapsedTime);
			m_vpcCollisionCheckVector.push_back(m_vpcActiveSignPosts[i]);
		}
	}
	// if any objects need to be deleted
	if (bDeleteActiveObjects)
	{
		// sort the active spawners with all the objects to be deleted at the end
		sort (m_vpcActiveSignPosts.begin(), m_vpcActiveSignPosts.end(), SortDeletedObjectsLast);
		// now that all the deletable objects are at the end of the vector, delete all the objects at once
		unsigned int listSize = m_vpcActiveSignPosts.size();
		for (unsigned int index = 0; index < listSize; index++)
		{
			if (m_vpcActiveSignPosts[index]->GetIsToBeDeleted())
			{
				m_vpcActiveSignPosts.erase(m_vpcActiveSignPosts.begin() + index, m_vpcActiveSignPosts.end());
				break;
			}
		}
	}
	for(unsigned int i = 0; i < m_vpcCollisionCheckVector.size();i++)
	{
		if(m_vpcCollisionCheckVector[i]->GetType() != SNOWBALL_OBJECT && m_vpcCollisionCheckVector[i]->GetType() != WIZARD_OBJECT 
			&& m_vpcCollisionCheckVector[i]->GetType() != PROJECTILE_OBJECT && m_vpcCollisionCheckVector[i]->GetType() != RANGED_OBJECT 
			&& (m_vpcCollisionCheckVector[i]->GetType() != ENVIRONMENT_OBJECT 
			|| ((CEnvironmentObject*)(m_vpcCollisionCheckVector[i]))->GetSubType() == CEnvironmentObject::ENVOBJ_ROCK1 && m_vpcCollisionCheckVector[i]->GetType() != SIGN_OBJECT) )
			GroundClamp(m_vpcCollisionCheckVector[i]);
	}

	this->CheckCollisions();
}


void CObjectManager::ExpandObjectList(int nObjectType, int nNumToAdd)
{
	switch(nObjectType)
	{
	case ENVIRONMENT_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n ENVIRONMENT_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumEnvironmentObjects += nNumToAdd;

			for(int i = 0; i < nNumToAdd;i++)
			{
				CEnvironmentObject* pcTemp = new CEnvironmentObject();
				this->m_vpcInactiveEnvironmentObjects.push_back(pcTemp);
			}
			break;
		}
	case PROJECTILE_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n PROJECTILE_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumProjectiles += nNumToAdd;

			for(int i = 0; i < nNumToAdd;i++)
			{
				CProjectileObject * pcTemp = new CProjectileObject();
				this->m_vpcInactiveProjectiles.push_back(pcTemp);
			}

			break;
		}
	case EXPLOSIVE_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n EXPLOSIVE_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumExplosiveEnemies += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CExplosiveEnemy * pcTemp = new CExplosiveEnemy();
				this->m_vpcInactiveExplosiveEnemies.push_back(pcTemp);
			}
			break;
		}
	case RANGED_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n RANGED_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumRangedEnemies += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CRangedEnemy * pcTemp = new CRangedEnemy();
				this->m_vpcInactiveRangedEnemies.push_back(pcTemp);
			}
			break;
		}
	case MELEE_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n MELEE_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumMeleeEnemies += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CMeleeEnemy * pcTemp = new CMeleeEnemy();
				this->m_vpcInactiveMeleeEnemies.push_back(pcTemp);
			}
			break;
		}
	case FIRE_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n FIRE_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumFireEnemies += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CFireEnemy * pcTemp = new CFireEnemy();
				this->m_vpcInactiveFireEnemies.push_back(pcTemp);
			}
			break;
		}
	case ICE_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n ICE_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumIceEnemies += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CIceEnemy * pcTemp = new CIceEnemy();
				this->m_vpcInactiveIceEnemies.push_back(pcTemp);
			}
			break;
		}
	//case COLLIDABLE_OBJECT:
	//	{
	//		LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n COLLIDABLE_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
	//		this->m_unNumCollidableObjects += nNumToAdd;

	//		for( int i = 0; i < nNumToAdd;i++)
	//		{
	//			CCollidableObject * pcTemp = new CCollidableObject();
	//			this->m_vpcInactiveCollidableObjects.push_back(pcTemp);
	//		}
	//		break;
	//	}
	case ATTACK_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n ATTACK_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumAttackObjects += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CAttackObject * pcTemp = new CAttackObject();
				this->m_vpcInactiveAttackObjects.push_back(pcTemp);
			}
			break;
		}
	case FIRETRAP_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n FIRETRAP_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumFireTraps += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CFireTrap * pcTemp = new CFireTrap();
				this->m_vpcInactiveFireTraps.push_back(pcTemp);
			}
			break;
		}
	case ICETRAP_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n ICETRAP_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumIceTraps += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CIceTrap * pcTemp = new CIceTrap();
				this->m_vpcInactiveIceTraps.push_back(pcTemp);
			}
			break;
		}
	case WALL_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n WALL_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumWalls += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CWall * pcTemp = new CWall();
				this->m_vpcInactiveWalls.push_back(pcTemp);
			}
			break;
		}
	case SNOWBALL_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n SNOWBALL_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumSnowballs += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CSnowball * pcTemp = new CSnowball();
				this->m_vpcInactiveSnowballs.push_back(pcTemp);
			}
			break;
		}
	case PORTAL_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n Portal_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumPortals += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CPortal * pcTemp = new CPortal();
				this->m_vpcInactivePortals.push_back(pcTemp);
			}
			break;
		}

	case PICKUP_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n PICKUP_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumPickups += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CPickupObject * pcTemp = new CPickupObject();
				this->m_vpcInactivePickups.push_back(pcTemp);
			}
			break;
		}

	case SPAWNER_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n SPAWNER_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumSpawners += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CSpawner * pcTemp = new CSpawner();
				this->m_vpcInactiveSpawners.push_back(pcTemp);
			}
			break;
		}
	case SCENERY_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n SCENERY_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumScenery += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CSceneryObject * pcTemp = new CSceneryObject();
				this->m_vpcInactiveSceneryObjects.push_back(pcTemp);
			}
			break;
		}
	case SIGN_OBJECT:
		{
			//LOG("ERROR: EXPAND OBJECT LIST CALLED FOR:\n SIGN_OBJECT\n ADDING " << nNumToAdd << " NEW OBJECTS\n"); 
			this->m_unNumSigns += nNumToAdd;

			for( int i = 0; i < nNumToAdd;i++)
			{
				CSignpost * pcTemp = new CSignpost();
				this->m_vpcInactiveSignPosts.push_back(pcTemp);
			}
			break;
		}
	default:
		{
			LOG("INCORRECT TYPE PASSED INTO EXPANDOBJECTLIST\n");
			break;
		}
	};
}


void CObjectManager::Shutdown()
{
	if(m_bInitialized)
	{
		//delete		this->m_pcPlayer;
		//delete		this->m_pcWizardEnemy;

		DeleteObjects();

		//this->m_pcPlayer			 = nullptr;
		//this->m_pcWizardEnemy		 = nullptr;
		
		m_unNumEnvironmentObjects = 0;
		m_unNumProjectiles		  = 0;
		m_unNumExplosiveEnemies	  = 0;
		m_unNumMeleeEnemies		  = 0;
		m_unNumRangedEnemies	  = 0;
		m_unNumFireEnemies		  = 0;
		m_unNumIceEnemies		  = 0;
		m_unNumCollidableObjects  = 0;
		m_unNumAttackObjects	  = 0;
		m_unNumFireTraps = 0;
		m_unNumIceTraps = 0;
		m_unNumWalls = 0;
		m_unNumPortals = 0;
		m_unNumSpawners = 0;

		m_bInitialized = false;

		//this->m_pcPlayer->SetObjectManager(this);

	}
}


bool CObjectManager::LoadLevel(char* szFileName)
{
	/*
	m_unNumEnvironmentObjects = 50;

	for(unsigned int i = 0; i < m_unNumEnvironmentObjects;i++)
	{
		CEnvironmentObject* pcTemp = new CEnvironmentObject();
		this->m_vpcInactiveEnvironmentObjects.push_back(pcTemp);
	}
	*/

	return true;
}


void CObjectManager::CheckCollisions()
{
	//std::cout << "start collision check" << std::endl;

	// Handle player environmental collisions and corrections first - JC
	PlayerCollisions();

	unsigned int nLastVecIndex = this->m_vpcCollisionCheckVector.size()-1;

	while(nLastVecIndex>0)
	{
		vec3f vCorrectionVector(0.0f,0.0f,0.0f); // averaged correction vector - JC

		for(unsigned int i = 0; i < nLastVecIndex;i++)
		{
			if(m_vpcCollisionCheckVector[i]->GetInitialized() == false || m_vpcCollisionCheckVector[nLastVecIndex]->GetInitialized() == false)
				continue;

			CPlayerObject * pcPlayer = nullptr;
			CEnemy * pcObject = nullptr;
			CWall * pcWall = nullptr;
			//CPortal * pCPortal = nullptr;

			// Ice breath collision check, ugh this is gonna be gross - JC
			// if this is an ice beast and it's breath is on, we check all objects in collision check vector for hits (it's not perfect)
			if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ICE_OBJECT && static_cast<CIceEnemy*>(m_vpcCollisionCheckVector[nLastVecIndex])->GetIceBreathEnabled())
			{
				// Get the ice beast
				CIceEnemy* iceEnemy = static_cast<CIceEnemy*>(m_vpcCollisionCheckVector[nLastVecIndex]);				

				unsigned int tempSize = m_vpcCollisionCheckVector.size();
				for(unsigned int iceIndex = 0; iceIndex < tempSize; ++iceIndex)
				{
					if(m_vpcCollisionCheckVector[iceIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[iceIndex]->GetType() < WIZARD_OBJECT)
					{
						// Don't test against ourself. - JC
						if(m_vpcCollisionCheckVector[iceIndex] == iceEnemy)
							continue;

						// FREEZE THAT SMIZMAR! - JC
						if(IceBreathCheck(iceEnemy,m_vpcCollisionCheckVector[iceIndex]))
						{
							m_vpcCollisionCheckVector[iceIndex]->OnAttack(iceEnemy->GetDamage(),iceEnemy->GetAttackElement());
							m_vpcCollisionCheckVector[iceIndex]->SetElementalState(CCollidableObject::FROZEN);
						}
					}
				}
			}

			// player attack checks
			if((m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT) )
			{
				if((m_vpcCollisionCheckVector[i]->GetType() >= PROJECTILE_OBJECT && m_vpcCollisionCheckVector[i]->GetType() <= SPAWNER_OBJECT) || (m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= PROJECTILE_OBJECT && m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= SPAWNER_OBJECT))
				{
					

					if(m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT)
					{
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[i];
						if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == WALL_OBJECT)
							pcWall = (CWall*)m_vpcCollisionCheckVector[nLastVecIndex];
						//else if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == Portal_OBJECT)
						//	pCPortal = (CPortal*)m_vpcCollisionCheckVector[nLastVecIndex];
						else
							pcObject = (CEnemy*)m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[nLastVecIndex];
						if(m_vpcCollisionCheckVector[i]->GetType() == WALL_OBJECT)
							pcWall = (CWall*)m_vpcCollisionCheckVector[i];
						//else if(m_vpcCollisionCheckVector[i]->GetType() == Portal_OBJECT)
						//	pCPortal = (CPortal*)m_vpcCollisionCheckVector[i];
						else
							pcObject = (CEnemy*)m_vpcCollisionCheckVector[i];
					}

					if(pcObject->GetType() == RANGED_OBJECT)
					{
						continue;
					}

					if(pcPlayer->GetIsAttacking())
					{
						if(pcObject && !pcObject->GetIsDying())
						{
							if(pcObject->GetType() == WIZARD_OBJECT)
							{
								//int i = 0;
								int nWizardBehavior = ((CWizardEnemy*)pcObject)->GetFSM()->GetCurrBehavior()->GetBehaviorType();
								if(nWizardBehavior == FLOAT_BEHAVIOR || nWizardBehavior == RANGED_BEHAVIOR || nWizardBehavior == HYPERORB_BEHAVIOR)
								{
									continue;
								}
							}

							else if(pcObject->GetType() == SNOWBALL_OBJECT)
							{
								float fDist = (pcObject->GetPos() - pcPlayer->GetPos()).magnitude();

								if((fDist * fDist) < (pcPlayer->GetSphereRad() + pcObject->GetSphereRad()) * (pcPlayer->GetSphereRad() + pcObject->GetSphereRad()) )
								{
									if(pcPlayer->GetAttackElement() == FIRE)
									{
										//((CSnowball*)pcObject)->SetHitByFire(true);
									}
								}
							}
							if(PlayerAttackCheck(pcPlayer,pcObject))
							{
								if(pcObject->GetType() == PROJECTILE_OBJECT && !((CProjectileObject*)(pcObject))->GetDeflected())
								{
									if(pcPlayer->GetAttackElement() == ((CProjectileObject*)(pcObject))->GetAttackElement())
									{
										CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_SWORDHIT_PROJECTILE, GAME_OBJECT_ID_PLAYER);
										
										if(PlayerDeflectCheck(pcPlayer,(CProjectileObject*)pcObject))
										{
											pcObject->OnAttack(pcPlayer->GetDamage(), pcPlayer->GetAttackElement());
											pcPlayer->SetAttackedLastFrame(true);
										}
										else if(!((CProjectileObject*)pcObject)->GetDeflected())
										{											
											((CProjectileObject*)pcObject)->SetLifeTime(2.0f);
											//((CProjectileObject*)pcObject)->SetDeflected(true);
											
											vec3f cNewVel = pcObject->GetPos() - (pcPlayer->GetPos());
											cNewVel.y += 2.0f;
											cNewVel.normalize();
											pcObject->SetVelocity(cNewVel);
										}

										
									}
									else // if the player uses an opposing element on a projectile
									{
										m_pcPlayer->StunThePlayer();
										
										CView::GetEmitterManager().AddEmitter(false, true, pcObject->GetPos(), "Resource/Particles/IceExplosion.txt");
																			
										CView::GetEmitterManager().AddEmitter(false, true, pcObject->GetPos(), "Resource/Particles/FireExplosion.txt");

										pcObject->Uninitialize();
									}
								}
								/*else if(pcObject->GetType() == Portal_OBJECT && ((CPortal*)(pcObject))->GetDestructable())
								{
									pcObject->OnAttack(pcPlayer->GetDamage()*2, pcPlayer->GetAttackElement());
								}*/
								else if(pcObject->GetType() == SPAWNER_OBJECT)
								{
									if(!pcObject->GetIsHit())
									{
										CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDHIT_CRYSTAL,GAME_OBJECT_ID_PLAYER);
										pcObject->OnAttack(pcPlayer->GetDamage(), pcPlayer->GetAttackElement());
										if (pcObject->GetHealth() <= 0)
											CAchievementManager::GetInstance()->SetIsSpawnerDestroyed(true);
										pcObject->SetIsHit(true);
										pcPlayer->SetAttackedLastFrame(true);
									}
								}
								else if(!pcPlayer->IsRaging())
								{
									if(!pcObject->GetIsHit())
									{
										pcObject->OnAttack(pcPlayer->GetDamage(), pcPlayer->GetAttackElement());//->Uninitialize();
										pcPlayer->SetHitLastFrame(true);
										pcObject->SetIsHit(true);
										pcPlayer->SetAttackedLastFrame(true);
										switch(pcObject->GetType())
										{
										case MELEE_OBJECT:{
											CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_SWORDHIT_MELEEMINION,GAME_OBJECT_ID_PLAYER);
														  } break;
										case FIRE_OBJECT:{
											CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_SWORDHIT_FIREGOLEM,GAME_OBJECT_ID_PLAYER);
														 } break;
										case ICE_OBJECT:{
											CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_SWORDHIT_ICEGOLEM,GAME_OBJECT_ID_PLAYER);
														} break;
										default:{
											CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDHIT,GAME_OBJECT_ID_PLAYER);
												} break;

										}
										//CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDHIT,GAME_OBJECT_ID_PLAYER);

										if (pcObject->GetType() == FIRE_OBJECT && pcObject->GetHealth() <= 0)
										{
											CAchievementManager::GetInstance()->SetIsFireBeastDefeated(true);
										}
										else if (pcObject->GetType() == ICE_OBJECT && pcObject->GetHealth() <= 0)
										{
											CAchievementManager::GetInstance()->SetIsIceBeastDefeated(true);
										}

										// Set player emitter offset and emitter on for hit (originally in PlayerObject)
										// There's alotta math going on, but basically I'm moving the player's hit particles
										// along the vector between the enemy and player by distance between them. - JC
										switch (m_pcPlayer->GetAttackElement()) {
											case FIRE: {
												pcPlayer->SetFireEmitterOn(true);
												pcPlayer->SetFireOffset((pcPlayer->GetPos() + (pcPlayer->GetPos() - pcObject->GetPos()).normalize()
													* -(pcObject->GetPos() - pcPlayer->GetPos()).magnitude() ) - pcPlayer->GetPos()); 
												break; }
											case ICE: {
												pcPlayer->SetIceEmitterOn(true);
												pcPlayer->SetIceOffset((pcPlayer->GetPos() + (pcPlayer->GetPos() - pcObject->GetPos()).normalize() 
													* -(pcObject->GetPos() - pcPlayer->GetPos()).magnitude() ) - pcPlayer->GetPos()); 
												break; }
										}
									}
								}
								else
								{
									if(!pcObject->GetIsHit())
									{
										if (GetPlayer()->GetIsSpeedRaging())
										{	// much faster attacks
											pcObject->OnAttack(pcPlayer->GetDamage(), pcPlayer->GetAttackElement());
										}
										else
										{	// double damage
											pcObject->OnAttack(pcPlayer->GetDamage()*2, pcPlayer->GetAttackElement()*2);
										}										
										pcObject->SetIsHit(true);
										pcPlayer->SetAttackedLastFrame(true);
										CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDHIT,GAME_OBJECT_ID_PLAYER);

										if (pcObject->GetType() == FIRE_OBJECT && pcObject->GetHealth() <= 0)
										{
											CAchievementManager::GetInstance()->SetIsFireBeastDefeated(true);
										}
										else if (pcObject->GetType() == ICE_OBJECT && pcObject->GetHealth() <= 0)
										{
											CAchievementManager::GetInstance()->SetIsIceBeastDefeated(true);
										}

										// Set player emitter offset and emitter on for hit (originally in PlayerObject)
										// There's alotta math going on, but basically I'm moving the player's hit particles
										// along the vector between the enemy and player by distance between them. - JC
										switch (m_pcPlayer->GetAttackElement()) {
											case FIRE: {
												pcPlayer->SetFireEmitterOn(true);
												pcPlayer->SetFireOffset((pcPlayer->GetPos() + (pcPlayer->GetPos() - pcObject->GetPos()).normalize()
													* -(pcObject->GetPos() - pcPlayer->GetPos()).magnitude() ) - pcPlayer->GetPos()); 
												break; }
											case ICE: {
												pcPlayer->SetIceEmitterOn(true);
												pcPlayer->SetIceOffset((pcPlayer->GetPos() + (pcPlayer->GetPos() - pcObject->GetPos()).normalize()
													* -(pcObject->GetPos() - pcPlayer->GetPos()).magnitude() ) - pcPlayer->GetPos()); 
												break; }
										}
									}
								}

								if (pcObject->GetType() >= ENEMY_OBJECT && pcObject->GetType() <= WIZARD_OBJECT)
									CWorldManager::GetInstance()->ActivateObjectLevel(pcObject);
							}
						}
						/*else if(pCPortal && pCPortal->GetDestructable() && PlayerAttackCheck(pcPlayer,(CEnemy*)pCPortal))
						{
							pCPortal->OnAttack(pcPlayer->GetDamage(), pcPlayer->GetAttackElement());
						}*/
						else
						{
							if(pcWall && PlayerAttackCheck(pcPlayer,(CEnemy*)pcWall))
							{
								pcWall->OnAttack(pcPlayer->GetDamage(), pcPlayer->GetAttackElement());
							}
						}
					}
				}
				else if(m_vpcCollisionCheckVector[i]->GetType() == SNOWBALL_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == SNOWBALL_OBJECT)
				{
					CSnowball * pcSnowball = nullptr;

					if(m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT)
					{
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[i];
						pcSnowball = (CSnowball*)m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcSnowball = (CSnowball*)m_vpcCollisionCheckVector[i];
					}

					if(CheckCollision(pcPlayer,pcSnowball,AABB_TO_SPHERE))
					{
						if(pcPlayer->GetIsAttacking() && pcPlayer->GetAttackElement() == FIRE)
						{
							vec3f PlayerZ = pcPlayer->GetWorldMatrix()->axis_z;
							vec3f PlayerY = pcPlayer->GetWorldMatrix()->axis_y;
							vec3f PlayerX = pcPlayer->GetWorldMatrix()->axis_x;

							PlayerZ.normalize();
							PlayerY.normalize();
							PlayerX.normalize();

							vec3f toPlayer = (pcPlayer->GetPos() - pcSnowball->GetPos()).normalize();

							float fDProdZ = dot_product(PlayerZ, toPlayer);
							float fDProdY = dot_product(PlayerY, toPlayer);
							float fDProdX = dot_product(PlayerX, toPlayer);

							if(fDProdZ > 0.0f && fDProdY < 0.0f && abs(fDProdX) <= 0.5f)
							//if(PlayerHalfSpaceTest(pcPlayer, pcSnowball))
							{
								pcSnowball->SetHitByFire(true);
							}
						}

						else
						{
							pcPlayer->OnAttack(5,pcSnowball->GetAttackElement());
							pcPlayer->StunThePlayer();
						}
					}

				}
				else if (m_vpcCollisionCheckVector[i]->GetType() == ENVIRONMENT_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ENVIRONMENT_OBJECT)
				{
					CEnvironmentObject* pcEnviro = nullptr;

					if(m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT)
					{
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[i];
						pcEnviro = (CEnvironmentObject*)m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcEnviro = (CEnvironmentObject*)m_vpcCollisionCheckVector[i];
					}

					if (pcEnviro->GetSubType() == CEnvironmentObject::ENVOBJ_ICECUBE)
					{
						if (CheckCollision(pcPlayer, pcEnviro, AABB_TO_AABB))
						{
							if (pcPlayer->IsDashing())
							{
								if (pcEnviro->GetOwner())
								{
									pcEnviro->GetOwner()->SetHealth(0.0f);
									((CMeleeEnemy*)pcEnviro->GetOwner())->SetEffectTimer(2.5f);
								}
							}
						}
					}

					if (pcEnviro->GetSubType() == CEnvironmentObject::ENVOBJ_SIGN)
					{
						if (CheckCollision(pcPlayer, pcEnviro, AABB_TO_AABB))
						{
							CGame* pInst = CGame::GetInstance();
							CView::Draw(((CSignpost*)pcEnviro)->GetPopup().m_nImageID, int(pInst->GetScreenWidth() * 0.3f), int(pInst->GetScreenHeight() * 0.85f) - int(((CSignpost*)pcEnviro)->GetPopup().m_nImageHeight / 2), 3.0f);
						}

					}
				}

			}


			// other collision checks other than player attack
			if(CheckCollision(m_vpcCollisionCheckVector[i],m_vpcCollisionCheckVector[nLastVecIndex],AABB_TO_AABB))
			{	
				if((m_vpcCollisionCheckVector[i]->GetType() == SPAWNER_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == SPAWNER_OBJECT))
					continue;

				if((m_vpcCollisionCheckVector[i]->GetType() == ENVIRONMENT_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ENVIRONMENT_OBJECT))//||(m_vpcCollisionCheckVector[i]->GetType() == Portal_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == Portal_OBJECT))
				{
					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() <= WIZARD_OBJECT))
					{
						CEnvironmentObject * pcEnviroObject = nullptr;
						CCollidableObject * pcObject = nullptr;

						if(m_vpcCollisionCheckVector[i]->GetType() == ENVIRONMENT_OBJECT)
						{
							pcEnviroObject = (CEnvironmentObject*)m_vpcCollisionCheckVector[i];
							pcObject = m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							pcEnviroObject = (CEnvironmentObject*)m_vpcCollisionCheckVector[nLastVecIndex];
							pcObject = m_vpcCollisionCheckVector[i];
						}

						// Environment collision for player is handled in PlayerCollisions - JC
						if(pcObject->GetType() != PLAYER_OBJECT)
						{
							// Adding a check to make Ice cubes not collide infinitely with the minions.
							if (pcEnviroObject->GetSubType() != CEnvironmentObject::ENVOBJ_ICECUBE && pcEnviroObject->GetSubType() > CEnvironmentObject::ENVOBJ_CLIFF && CEnvironmentObject::ENVOBJ_LAVA != pcEnviroObject->GetSubType())
								EnviromentCollisionReaction(pcEnviroObject,pcObject);
						}/*
						else if (pcObject->GetType() == PLAYER_OBJECT)
						{
							if (((CPlayerObject*)pcObject)->IsDashing())
							{
								if(pcEnviroObject->GetOwner())
								{
									pcEnviroObject->GetOwner()->SetHealth(0.0f);
									((CMeleeEnemy*)pcEnviroObject->GetOwner())->SetEffectTimer(2.5f);
								}
							}
						}*/
				
					}
					else if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PROJECTILE_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PROJECTILE_OBJECT)
					{
						if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PROJECTILE_OBJECT)
						{
							m_vpcCollisionCheckVector[nLastVecIndex]->Uninitialize();
						}
						else
						{
							m_vpcCollisionCheckVector[i]->Uninitialize();
						}
					}

				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == PORTAL_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PORTAL_OBJECT))
				{

					CBaseObject * pcObj;
					CPortal * pCPortal;

					if(m_vpcCollisionCheckVector[i]->GetType() == PORTAL_OBJECT)
					{
						pCPortal = (CPortal*)m_vpcCollisionCheckVector[i];
						pcObj = m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pCPortal = (CPortal*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcObj = m_vpcCollisionCheckVector[i];
					}

					if(!pCPortal->GetActive())
					{
						continue;
					}

					if(pcObj->GetType() == PLAYER_OBJECT)// || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT)
					{
						vec3f vDistance = pcObj->GetPos() - pCPortal->GetPos();
						vDistance.y = 0.0f;
						if(vDistance.magnitude() < 1.5f)
						{
							// stop the player from dashing through the portal
							GetPlayer()->SetVelocity(vec3f(0, 0, 0));
							CWorldManager::GetInstance()->SetSectionComplete(true);
							CGameState::GetInstance()->SetPortalZoomOn(true);
							CGame::GetInstance()->StartTimer();
						}
						if (int(((CPlayerObject*)pcObj)->GetRage()) == ((CPlayerObject*)pcObj)->GetMaxPlayerRage())
							CAchievementManager::GetInstance()->ActivateAchievement("These Are My Angry Eyes");
					}
					else if(pcObj->GetType() == WIZARD_OBJECT)// || m_vpcCollisionCheckVector[i]->GetType() == WIZARD_OBJECT)
					{
						pcObj->Uninitialize();
					}
				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == WALL_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == WALL_OBJECT))
				{
					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() <= WIZARD_OBJECT))
					{
						CWall * pcEnviroObject = nullptr;
						CCollidableObject * pcObject = nullptr;

						if(m_vpcCollisionCheckVector[i]->GetType() == WALL_OBJECT)
						{
							pcEnviroObject = (CWall*)m_vpcCollisionCheckVector[i];
							pcObject = m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							pcEnviroObject = (CWall*)m_vpcCollisionCheckVector[nLastVecIndex];
							pcObject = m_vpcCollisionCheckVector[i];
						}

						// Environment collision for player is handled in PlayerCollisions - JC
						if(pcObject->GetType() != PLAYER_OBJECT)
							EnviromentCollisionReaction(pcEnviroObject,pcObject);	
					

						if(pcObject->GetType() == PLAYER_OBJECT)
						{
							
							pcObject->OnAttack(10,((CWall*)(pcEnviroObject))->GetAttackElement());
						}

					}

				}

				else if((m_vpcCollisionCheckVector[i]->GetType() == ICETRAP_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ICETRAP_OBJECT))
				{
					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() <= WIZARD_OBJECT))
					{
						CIceTrap * pcIcetrap;
						CCollidableObject * pcCharacter;
						if(m_vpcCollisionCheckVector[i]->GetType() == ICETRAP_OBJECT)
						{
							pcIcetrap = (CIceTrap*)m_vpcCollisionCheckVector[i];
							pcCharacter = m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							pcIcetrap = (CIceTrap*)m_vpcCollisionCheckVector[nLastVecIndex];
							pcCharacter = m_vpcCollisionCheckVector[i];
						}

						//after copy/paste put logic for other half where if check is
						if(pcIcetrap->GetState() & T_DOWN)
						{
							//if(pcIcetrap->GetState() & T_SUMMONED)
							//{
							//
							//	if(pcIcetrap->GetIsReady())
							//	{
							//		pcCharacter->OnAttack(pcIcetrap->GetDamage(),ICE);
							//
							//	}
							//}
							//
							//else
							//{
							if (pcCharacter->GetType() != WIZARD_OBJECT)
							{
								pcCharacter->OnAttack(pcIcetrap->GetDamage(),ICE);

								// If this is a fire beast and they die from it, give an achievement.
								if (FIRE_OBJECT == pcCharacter->GetType())
								{
									if (pcCharacter->GetHealth() <= 0)
										CAchievementManager::GetInstance()->ActivateAchievement("Ow, My Face!");
								}
								// if the player runs into an active ice trap
								if (PLAYER_OBJECT == pcCharacter->GetType() && CInput::GetInstance()->GetIfUsingController() )
								{
									GetPlayer()->InitiateIceTrapRumble();
									if (pcCharacter->GetHealth() <= 0)
										CAchievementManager::GetInstance()->ActivateAchievement("Cold as Ice");
								}
							}
						}
					}
				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == FIRETRAP_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == FIRETRAP_OBJECT))
				{

					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() <= WIZARD_OBJECT))
					{
						CFireTrap * pcFiretrap;
						CCollidableObject * pcCharacter;
						if(m_vpcCollisionCheckVector[i]->GetType() == FIRETRAP_OBJECT)
						{
							pcFiretrap = (CFireTrap*)m_vpcCollisionCheckVector[i];
							pcCharacter = m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							pcFiretrap = (CFireTrap*)m_vpcCollisionCheckVector[nLastVecIndex];
							pcCharacter = m_vpcCollisionCheckVector[i];
						}

						//after copy/paste put logic for other half where if check is
						if((pcFiretrap->GetState() & T_IDLE) == 0)
						{
							if (pcCharacter->GetType() != WIZARD_OBJECT)
							{
								if(pcCharacter->GetType() == FIRE_OBJECT)
								{
									if ( (pcFiretrap->GetState() & T_SUMMONED) == 0)
									{
										pcCharacter->OnAttack(pcFiretrap->GetDamage(),FIRE);
										// if the player runs into an active fire trap
										if (PLAYER_OBJECT == pcCharacter->GetType() && CInput::GetInstance()->GetIfUsingController() )
										{
											GetPlayer()->InitiateFireTrapRumble();
										}
									}
								}

								else
								{
									pcCharacter->OnAttack(pcFiretrap->GetDamage(),FIRE);

									// If the object is an ice beast and they die, give an achievement.
									if (ICE_OBJECT == pcCharacter->GetType())
									{
										if (pcCharacter->GetHealth() <= 0)
											CAchievementManager::GetInstance()->ActivateAchievement("Ow, My Face!");
									}

									// if the player runs into an active fire trap
									if (PLAYER_OBJECT == pcCharacter->GetType() && CInput::GetInstance()->GetIfUsingController() )
									{
										GetPlayer()->InitiateFireTrapRumble();
										if (pcCharacter->GetHealth() <= 0)
											CAchievementManager::GetInstance()->ActivateAchievement("Don't Stand in the Fire!");
									}
								}
								
							}
						}
					}

				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == PROJECTILE_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PROJECTILE_OBJECT))
				{
					CProjectileObject * pcProj;
					CCollidableObject * pcObj;

					if(m_vpcCollisionCheckVector[i]->GetType() == PROJECTILE_OBJECT)
					{
						pcProj = (CProjectileObject*)m_vpcCollisionCheckVector[i];
						pcObj = m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcProj = (CProjectileObject*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcObj = m_vpcCollisionCheckVector[i];
					}

					//if(pcObj->GetType() == PLAYER_OBJECT && !pcProj->GetDeflected() || pcObj->GetType() == RANGED_OBJECT && pcProj->GetDeflected() || pcObj->GetType() == WIZARD_OBJECT && pcProj->GetDeflected())
					{
						if(pcObj->GetType() == PROJECTILE_OBJECT)
						{
							continue;
						}
						// if the wizard is getting hit by a projectile
						else if(pcObj->GetType() == WIZARD_OBJECT)
						{
							if(((CWizardEnemy*)pcObj)->GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERORB_BEHAVIOR && pcProj->GetDeflected())//pcObj->GetPos().y > CWorldManager::GetInstance()->GetGroundHeight() + 5.0f)
							{
								pcObj->OnAttack((int)pcProj->GetDamage() - 5,pcProj->GetAttackElement());
								pcProj->Uninitialize();
								pcObj->SetIsShaking(true);
								//pcObj->ShakeObject(2.0f);
								continue;
							}
						}
						// if a ranged minion is getting hit by a projectile
						else if(pcObj->GetType() == RANGED_OBJECT)
						{
							if(pcProj->GetDeflected())
							{
								pcObj->OnAttack((int)pcProj->GetDamage(),pcProj->GetAttackElement());
								CAchievementManager::GetInstance()->SetDeflectionCount(CAchievementManager::GetInstance()->GetDeflectionCount() + 1);
								if (CAchievementManager::GetInstance()->GetDeflectionCount() == 10)
									CAchievementManager::GetInstance()->ActivateAchievement("Rebounder");
								((CRangedEnemy*)pcProj)->Uninitialize();
								continue;
							}
						}
						// if the player is getting hit by a projectile
						else if (pcObj->GetType() == PLAYER_OBJECT)
						{
							pcObj->OnAttack((int)pcProj->GetDamage(),pcProj->GetAttackElement());
							pcProj->Uninitialize();
							if (CInput::GetInstance()->GetIfUsingController())
							{
								pcPlayer->InitiateProjectileRumble();
							}
						}
						else	// if anything else is getting by a projectile
						{
							pcObj->OnAttack((int)pcProj->GetDamage(),pcProj->GetAttackElement());
							pcProj->Uninitialize();
							continue;
						}

						
					}

				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == SNOWBALL_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == SNOWBALL_OBJECT))
				{
					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ENVIRONMENT_OBJECT ||  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ENVIRONMENT_OBJECT)
					{
						CSnowball * snowball = nullptr;

						if(m_vpcCollisionCheckVector[i]->GetType() == SNOWBALL_OBJECT)
						{
							snowball = (CSnowball*)m_vpcCollisionCheckVector[i];
						}
						else
						{
							snowball = (CSnowball*)m_vpcCollisionCheckVector[nLastVecIndex];
						}

						snowball->Uninitialize();

					}
					else if((m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() < WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() < WIZARD_OBJECT)) 
					{
						CSnowball * snowball = nullptr;
						CEnemy*		pEnemy = nullptr;

						if(m_vpcCollisionCheckVector[i]->GetType() == SNOWBALL_OBJECT)
						{
							snowball = (CSnowball*)m_vpcCollisionCheckVector[i];
							pEnemy  =  (CEnemy*)m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							snowball = (CSnowball*)m_vpcCollisionCheckVector[nLastVecIndex];
							pEnemy = (CEnemy*)m_vpcCollisionCheckVector[i];
						}

						// Apply snowball damage to enemies. - JC

						if(pEnemy->GetType() != ICE_OBJECT)
						{
							pEnemy->OnAttack(5,snowball->GetAttackElement());
						}
						
					}

				}

				else if((m_vpcCollisionCheckVector[i]->GetType() == PICKUP_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PICKUP_OBJECT))
				{
					CPickupObject * pcProj;
					CCollidableObject * pcObj;

					if(m_vpcCollisionCheckVector[i]->GetType() == PROJECTILE_OBJECT)
					{
						pcProj = (CPickupObject*)m_vpcCollisionCheckVector[i];
						pcObj = m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcProj = (CPickupObject*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcObj = m_vpcCollisionCheckVector[i];
					}

					if(pcObj->GetType() == PLAYER_OBJECT)
					{
						CAchievementManager::GetInstance()->SetPickupsCollected(CAchievementManager::GetInstance()->GetPickupsCollected() + 1);
						if (CAchievementManager::GetInstance()->GetPickupsCollected() == 750)
							CAchievementManager::GetInstance()->ActivateAchievement("Look at All This Change...");
						if(pcProj->GetPickupType() == HEALTH_PICKUP)
						{
							if(pcObj->GetHealth() >= pcObj->GetMaxHealth())
							{
								((CPickupObject*)pcProj)->Uninitialize();
								//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(), "SFX_PickUpObject");
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_PICKUPOBJECT,GAME_OBJECT_ID_PLAYER);
								continue;
							}
							else if((pcObj->GetMaxHealth() - pcObj->GetHealth()) < pcProj->GetHealthIncrease())
							{
								pcObj->SetHealth(pcObj->GetHealth() + (pcObj->GetMaxHealth() - pcObj->GetHealth()));
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_PICKUPOBJECT,GAME_OBJECT_ID_PLAYER);
							}
							else
							{
								pcObj->SetHealth(pcObj->GetHealth() + pcProj->GetHealthIncrease());
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_PICKUPOBJECT,GAME_OBJECT_ID_PLAYER);
							}
						}
						else
						{

							if(((CPlayerObject*)pcObj)->GetRage() >= ((CPlayerObject*)pcObj)->GetMaxPlayerRage())
							{
								((CPickupObject*)pcProj)->Uninitialize();
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_PICKUPOBJECT,GAME_OBJECT_ID_PLAYER);
								continue;
							}
							else if((((CPlayerObject*)pcObj)->GetMaxPlayerRage() - ((CPlayerObject*)pcObj)->GetRage()) < pcProj->GetRageIncrease())
							{
								((CPlayerObject*)pcObj)->SetRage(((CPlayerObject*)pcObj)->GetRage() + (((CPlayerObject*)pcObj)->GetMaxPlayerRage() - ((CPlayerObject*)pcObj)->GetRage()));
								CHud::GetInstance()->FlashRageGlow(); // give a single pulse of rage glow
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_PICKUPOBJECT,GAME_OBJECT_ID_PLAYER);
							}
							else
							{
								((CPlayerObject*)pcObj)->SetRage(((CPlayerObject*)pcObj)->GetRage() + pcProj->GetRageIncrease());
								CHud::GetInstance()->FlashRageGlow(); // give a single pulse of rage glow
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_PICKUPOBJECT,GAME_OBJECT_ID_PLAYER);
							}
						}

						((CPickupObject*)pcProj)->Uninitialize();
						continue;
					}

				}
				//else if(pcPlayer && pcObject && pcObject->GetType() == FIRE_OBJECT && ((CFireEnemy*)(pcObject))->GetIsRolling())
				//{
				//	FireRollingCollisionReaction((CFireEnemy*)pcObject,pcPlayer);
				//	pcPlayer->OnAttack(15 * (pcObject->GetElementalState()+1),1);
				//	pcObject->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
				//
				//}
				else if(!pcPlayer && (m_vpcCollisionCheckVector[i]->GetType() == WIZARD_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == WIZARD_OBJECT))
				{
					CWizardEnemy * pcWiz;
					CCollidableObject * pOtherObject;
					if(m_vpcCollisionCheckVector[i]->GetType() == WIZARD_OBJECT)
					{
						pcWiz = (CWizardEnemy*)m_vpcCollisionCheckVector[i];
						pOtherObject = m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcWiz = (CWizardEnemy*)m_vpcCollisionCheckVector[nLastVecIndex];
						pOtherObject = m_vpcCollisionCheckVector[i];
					}
				
					EnviromentCollisionReaction((CEnvironmentObject*)pcWiz,pOtherObject);
				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == FIRE_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == FIRE_OBJECT))
				{
					CFireEnemy * pcProj;
					CCollidableObject * pcObj;

					if(m_vpcCollisionCheckVector[i]->GetType() == FIRE_OBJECT)
					{
						pcProj = (CFireEnemy*)m_vpcCollisionCheckVector[i];
						pcObj = m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcProj = (CFireEnemy*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcObj = m_vpcCollisionCheckVector[i];
					}

					if(pcObj->GetType() == RANGED_OBJECT)
					{
						continue;
					}

					if(pcObj->GetType() != WIZARD_OBJECT && pcProj->GetIsRolling()/* && pcObj->GetType() != FIRETRAP_OBJECT && pcObj->GetType() != ICETRAP_OBJECT*/)
					{
						//if(pcObj->GetType() == ENVIRONMENT_OBJECT)
						//{
						//	EnviromentCollisionReaction((CEnvironmentObject*)pcObj, pcProj);
						//}

						FireRollingCollisionReaction((CFireEnemy*)pcProj,pcObj);

						float fMag;

						fMag = pcProj->GetVelocity().magnitude();
						if(/*PLAYER_OBJECT == pcObj->GetType() &&*/ abs(fMag) > 0.0f)
						{
							pcObj->OnAttack(pcProj->GetRollingDamage() * (pcProj->GetElementalState()+1),1);
							// play the really loud reverb collision sound if the golem rolls into the player or an ice golem
							if (pcObj->GetType() == PLAYER_OBJECT || pcObj->GetType() == ICE_OBJECT)
							{	
								CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_FIREGOLEM_COLLISION, GAME_OBJECT_ID_PLAYER);
							}
						}

						// if the player is getting hit by a roll attack and a controller is plugged in
						if (PLAYER_OBJECT == pcObj->GetType() && CInput::GetInstance()->GetIfUsingController())
						{
							GetPlayer()->InitiateFireBeastRollRumble(pcProj->GetElementalState());
						}

						if(pcObj->GetType() != MELEE_OBJECT && pcObj->GetType() != EXPLOSIVE_OBJECT && pcObj->GetType() != RANGED_OBJECT)
						{
							
							pcProj->CollisionReaction(pcObj);
							pcProj->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
						}
					}
				}
				else if(pcPlayer && !pcPlayer->IsDashing() && (m_vpcCollisionCheckVector[i]->GetType() == MELEE_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == MELEE_OBJECT))
				{
					//EnviromentCollisionReaction((CEnvironmentObject*) m_vpcCollisionCheckVector[i],m_vpcCollisionCheckVector[nLastVecIndex]);
					CMeleeEnemy * pcEnemy;
					//CPlayerObject * pcPlayer;

					if(m_vpcCollisionCheckVector[i]->GetType() == MELEE_OBJECT)
					{
						pcEnemy = (CMeleeEnemy*)m_vpcCollisionCheckVector[i];
						pcPlayer = (CPlayerObject*)m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcEnemy = (CMeleeEnemy*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcPlayer = (CPlayerObject*) m_vpcCollisionCheckVector[i];
					}

					if(pcEnemy->GetType() == RANGED_OBJECT)
					{
						continue;
					}

					if(pcPlayer->GetIsMoving())
					{
						NormalCollisionReaction(pcPlayer,pcEnemy);
					}
					else
					{
						EnviromentCollisionReaction((CEnvironmentObject*)pcPlayer,pcEnemy);
					}

				}

				// the player has been hit by the wizard dashing about
				else if (pcPlayer && pcObject && pcObject->GetType() == WIZARD_OBJECT && ((CWizardEnemy*)pcObject)->GetFSM()->GetCurrBehavior()->GetBehaviorType() == HYPERDASH_BEHAVIOR)
				{
					EnviromentCollisionReaction((CEnvironmentObject*)pcObject,pcPlayer);
					//FireRollingCollisionReaction(((CFireEnemy*)pcObject), pcPlayer);
					pcPlayer->OnAttack(20, pcObject->GetAttackElement());
					//pcObject->GetFSM()->TransitionTo(EXHAUST_BEHAVIOR);
					if (CInput::GetInstance()->GetIfUsingController())
					{
						GetPlayer()->InitiateFireBeastRollRumble(pcObject->GetElementalState());
					}
				}
				else if(pcPlayer && pcObject && pcPlayer->IsDashing() && pcObject->GetType() != WIZARD_OBJECT)
				{
					// Add in last attack element damage on enemies dashed into if raging. -JC
					if(pcPlayer->IsRaging()) {
						pcObject->OnAttack(pcPlayer->GetDamage(),pcPlayer->GetAttackElement()); }

					PlayerDashingCollisionReaction(pcPlayer,pcObject);
				}

				// Collision check for the signs
				else if((m_vpcCollisionCheckVector[i]->GetType() == SIGN_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == SIGN_OBJECT))
				{
					// Re-Enabling collision with the signposts - Dan
					CSignpost* pcSign;
					CCollidableObject * pcObj;
					
					if(m_vpcCollisionCheckVector[i]->GetType() == SIGN_OBJECT)
					{
						pcSign = (CSignpost*)m_vpcCollisionCheckVector[i];
						pcObj = m_vpcCollisionCheckVector[nLastVecIndex];
					}
					else
					{
						pcSign = (CSignpost*)m_vpcCollisionCheckVector[nLastVecIndex];
						pcObj = m_vpcCollisionCheckVector[i];
					}
					
					EnviromentCollisionReaction(pcSign,pcObj);
					if(pcObj->GetType() == PLAYER_OBJECT /*&& !pcPlayer->IsDashing()*/)
					{
						m_pcCurrSign = pcSign;
						
					}
				}
				else
				{
					NormalCollisionReaction(m_vpcCollisionCheckVector[i],m_vpcCollisionCheckVector[nLastVecIndex]);
				}

			
			}

			

			if(CheckCollision(m_vpcCollisionCheckVector[i],m_vpcCollisionCheckVector[nLastVecIndex],SPHERE_TO_SPHERE))
			{
				if((m_vpcCollisionCheckVector[i]->GetType() == ICETRAP_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == ICETRAP_OBJECT))
				{
					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() <= WIZARD_OBJECT))
					{
						CIceTrap * pcIcetrap;
						CCollidableObject * pcCharacter;
						if(m_vpcCollisionCheckVector[i]->GetType() == ICETRAP_OBJECT)
						{
							pcIcetrap = (CIceTrap*)m_vpcCollisionCheckVector[i];
							pcCharacter = m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							pcIcetrap = (CIceTrap*)m_vpcCollisionCheckVector[nLastVecIndex];
							pcCharacter = m_vpcCollisionCheckVector[i];
						}

						// check for smaller radius to tune activation radius of ice trap

						if( pcIcetrap->GetState() == T_IDLE && IceTrapActivationCheck(pcIcetrap,pcCharacter))
						{
							pcIcetrap->SetState(T_RISE);
						}
					}
				}
				else if((m_vpcCollisionCheckVector[i]->GetType() == EXPLOSIVE_OBJECT || m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == EXPLOSIVE_OBJECT))
				{
					if(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() == PLAYER_OBJECT || m_vpcCollisionCheckVector[i]->GetType() == PLAYER_OBJECT ||
						(m_vpcCollisionCheckVector[nLastVecIndex]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[nLastVecIndex]->GetType() <= WIZARD_OBJECT) ||
						(m_vpcCollisionCheckVector[i]->GetType() >= ENEMY_OBJECT &&  m_vpcCollisionCheckVector[i]->GetType() <= WIZARD_OBJECT))
					{
						CExplosiveEnemy* pcMinion;
						CCollidableObject * pcCharacter;

						if(m_vpcCollisionCheckVector[i]->GetType() == EXPLOSIVE_OBJECT)
						{
							pcMinion = (CExplosiveEnemy*)m_vpcCollisionCheckVector[i];
							pcCharacter = m_vpcCollisionCheckVector[nLastVecIndex];
						}
						else
						{
							pcMinion = (CExplosiveEnemy*)m_vpcCollisionCheckVector[nLastVecIndex];
							pcCharacter = m_vpcCollisionCheckVector[i];
						}

						if(pcCharacter->GetType() == PORTAL_OBJECT)
							continue;

						if (pcMinion->GetExploding())
						{
							if (CheckCollision(pcMinion, pcCharacter, SPHERE_TO_SPHERE))
							{
								pcCharacter->OnAttack(pcMinion->GetDamage(), pcMinion->GetAttackElement());
								pcMinion->SetIsDamageDealt(true);
							}
						}
					}
				}
			}

		}
		
		m_vpcCollisionCheckVector.pop_back();
		nLastVecIndex = this->m_vpcCollisionCheckVector.size()-1;

	}

	//std::cout << "end collision check" << std::endl;

	this->m_vpcCollisionCheckVector.clear();
}


void CObjectManager::PlayerCollisions()
{
	// Iterate through all collidable objects in collision check vector
	// and determine an average correction vector for the player - JC

	CPlayerObject * pcPlayer = m_pcPlayer;
	CEnemy * pcObject = nullptr;
	CWall * pcWall = nullptr;
	CPortal * pCPortal = nullptr;

	vec3f vCorrectionVector(0.0f,0.0f,0.0f);

	// Start at 1 because 0 is player
	for(unsigned int i = 1; i < m_vpcCollisionCheckVector.size(); ++i)
	{		
		if(m_vpcCollisionCheckVector[i]->GetType() == ENVIRONMENT_OBJECT && ((CEnvironmentObject*)( m_vpcCollisionCheckVector[i]))->GetSubType() != CEnvironmentObject::ENVOBJ_LAVA)
			if(CheckCollision(m_pcPlayer,m_vpcCollisionCheckVector[i],AABB_TO_AABB))		
				AverageCollisionReaction(m_pcPlayer,m_vpcCollisionCheckVector[i],vCorrectionVector);		
	}

	// Add in correction
	matrix4f playerPosition = *(m_pcPlayer->GetWorldMatrix());
	playerPosition.axis_pos += vCorrectionVector;
	m_pcPlayer->SetWorldMatrix(playerPosition);
	//m_pcPlayer->GetWorldMatrix()->axis_pos += vCorrectionVector;

}

void CObjectManager::CreateObjects()
{
	for(unsigned int i = 0; i < m_unNumEnvironmentObjects; ++i)
	{
		CEnvironmentObject* pcTemp = new CEnvironmentObject();
		this->m_vpcInactiveEnvironmentObjects.push_back(pcTemp);
	}

	for(unsigned int i = 0; i < m_unNumMeleeEnemies;i++)
	{
		CMeleeEnemy* pcTemp = new CMeleeEnemy();
		this->m_vpcInactiveMeleeEnemies.push_back(pcTemp);
		//pcTemp->Initialize(MELEE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumAttackObjects;i++)
	{
		CAttackObject* pcTemp = new CAttackObject();
		this->m_vpcInactiveAttackObjects.push_back(pcTemp);
		//pcTemp->Initialize(MELEE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumRangedEnemies;i++)
	{
		CRangedEnemy* pcTemp = new CRangedEnemy();
		this->m_vpcInactiveRangedEnemies.push_back(pcTemp);
		//pcTemp->Initialize(RANGED_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumExplosiveEnemies;i++)
	{
		CExplosiveEnemy* pcTemp = new CExplosiveEnemy();
		this->m_vpcInactiveExplosiveEnemies.push_back(pcTemp);
		//pcTemp->Initialize(EXPLOSIVE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumIceEnemies;i++)
	{
		CIceEnemy* pcTemp = new CIceEnemy();
		this->m_vpcInactiveIceEnemies.push_back(pcTemp);
		//pcTemp->Initialize(ICE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumFireEnemies;i++)
	{
		CFireEnemy* pcTemp = new CFireEnemy();
		this->m_vpcInactiveFireEnemies.push_back(pcTemp);
		//pcTemp->Initialize(FIRE_OBJECT);
	}

	//for(unsigned int i = 0; i < m_unNumCollidableObjects;i++)
	//{
	//	CCollidableObject* pcTemp = new CCollidableObject();
	//	this->m_vpcInactiveCollidableObjects.push_back(pcTemp);
	//	//pcTemp->Initialize(COLLIDABLE_OBJECT);
	//}

	for(unsigned int i = 0; i < m_unNumProjectiles;i++)
	{
		CProjectileObject* pcTemp = new CProjectileObject();
		this->m_vpcInactiveProjectiles.push_back(pcTemp);
		//pcTemp->Initialize(PROJECTILE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumFireTraps;i++)
	{
		CFireTrap* pcTemp = new CFireTrap();
		this->m_vpcInactiveFireTraps.push_back(pcTemp);
		//pcTemp->Initialize(PROJECTILE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumIceTraps;i++)
	{
		CIceTrap* pcTemp = new CIceTrap();
		this->m_vpcInactiveIceTraps.push_back(pcTemp);
		//pcTemp->Initialize(PROJECTILE_OBJECT);
	}

	for(unsigned int i = 0; i < m_unNumWalls;i++)
	{
		CWall * pcTemp = new CWall();
		this->m_vpcInactiveWalls.push_back(pcTemp);
	}

	for(unsigned int i = 0; i < m_unNumSnowballs;i++)
	{
		CSnowball * pcTemp = new CSnowball();
		this->m_vpcInactiveSnowballs.push_back(pcTemp);
	}

	for(unsigned int i = 0; i < m_unNumPortals;i++)
	{
		CPortal * pcTemp = new CPortal();
		this->m_vpcInactivePortals.push_back(pcTemp);
	}

	for(unsigned int i = 0; i < m_unNumPickups;i++)
	{
		CPickupObject * pcTemp = new CPickupObject();
		this->m_vpcInactivePickups.push_back(pcTemp);
	}

	for(unsigned int i = 0; i < m_unNumSpawners;i++)
	{
		CSpawner * pcTemp = new CSpawner();
		this->m_vpcInactiveSpawners.push_back(pcTemp);
	}
}

void CObjectManager::DeleteObjects()
{
	if(m_pcPlayer != nullptr)
	{
		this->m_pcPlayer->Uninitialize();
		delete this->m_pcPlayer;
		this->m_pcPlayer = nullptr;
	}
	if(this->m_pcWizardEnemy != nullptr)
	{
		this->m_pcWizardEnemy->Uninitialize();
		delete this->m_pcWizardEnemy;
		this->m_pcWizardEnemy = nullptr;
	}

	for(unsigned int i = 0;i < m_vpcActiveAttackObjects.size();i++)
	{
		m_vpcActiveAttackObjects[i]->Uninitialize();
		delete m_vpcActiveAttackObjects[i];
	}

	for(unsigned int i = 0;i < m_vpcInactiveAttackObjects.size();i++)
	{
		//m_vpcInactiveAttackObjects[i]->Uninitialize();
		delete m_vpcInactiveAttackObjects[i];
	}

	for(unsigned int i = 0;i < m_vpcActiveEnvironmentObjects.size();i++)
	{
		m_vpcActiveEnvironmentObjects[i]->Uninitialize();
		delete m_vpcActiveEnvironmentObjects[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveProjectiles.size();i++)
	{
		m_vpcActiveProjectiles[i]->Uninitialize();
		delete m_vpcActiveProjectiles[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveExplosiveEnemies.size();i++)
	{
		m_vpcActiveExplosiveEnemies[i]->Uninitialize();
		delete m_vpcActiveExplosiveEnemies[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveMeleeEnemies.size();i++)
	{
		m_vpcActiveMeleeEnemies[i]->Uninitialize();
		delete m_vpcActiveMeleeEnemies[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveRangedEnemies.size();i++)
	{
		m_vpcActiveRangedEnemies[i]->Uninitialize();
		delete m_vpcActiveRangedEnemies[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveFireEnemies.size();i++)
	{
		m_vpcActiveFireEnemies[i]->Uninitialize();
		delete m_vpcActiveFireEnemies[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveIceEnemies.size();i++)
	{
		m_vpcActiveIceEnemies[i]->Uninitialize();
		delete m_vpcActiveIceEnemies[i];
	}
	
	//for(unsigned int i = 0;i < m_vpcActiveCollidableObjects.size();i++)
	//{
	//	m_vpcActiveCollidableObjects[i]->Uninitialize();
	//	delete m_vpcActiveCollidableObjects[i];
	//}

	for(unsigned int i = 0;i < m_vpcActivePickups.size();i++)
	{
		m_vpcActivePickups[i]->Uninitialize();
		delete m_vpcActivePickups[i];
	}

	for(unsigned int i = 0;i < m_vpcInactiveEnvironmentObjects.size();i++)
	{
		//[i]->Uninitialize();
		delete m_vpcInactiveEnvironmentObjects[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveProjectiles.size();i++)
	{
		//m_vpcInactiveProjectiles[i]->Uninitialize();
		delete m_vpcInactiveProjectiles[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveExplosiveEnemies.size();i++)
	{
		//m_vpcInactiveExplosiveEnemies[i]->Uninitialize();
		delete m_vpcInactiveExplosiveEnemies[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveMeleeEnemies.size();i++)
	{
		//m_vpcInactiveMeleeEnemies[i]->Uninitialize();
		delete m_vpcInactiveMeleeEnemies[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveRangedEnemies.size();i++)
	{
		//m_vpcInactiveRangedEnemies[i]->Uninitialize();
		delete m_vpcInactiveRangedEnemies[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveFireEnemies.size();i++)
	{
		//m_vpcInactiveFireEnemies[i]->Uninitialize();
		delete m_vpcInactiveFireEnemies[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveIceEnemies.size();i++)
	{
		//m_vpcInactiveIceEnemies[i]->Uninitialize();
		delete m_vpcInactiveIceEnemies[i];
	}
	//for(unsigned int i = 0;i < m_vpcInactiveCollidableObjects.size();i++)
	//{
	//	//m_vpcInactiveCollidableObjects[i]->Uninitialize();
	//	delete m_vpcInactiveCollidableObjects[i];
	//}
	for(unsigned int i = 0;i < m_vpcInactiveFireTraps.size();i++)
	{
		//m_vpcInactiveFireTraps[i]->Uninitialize();
		delete m_vpcInactiveFireTraps[i];
	}
	for(unsigned int i = 0;i < m_vpcActiveFireTraps.size();i++)
	{
		m_vpcActiveFireTraps[i]->Uninitialize();
		delete m_vpcActiveFireTraps[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveIceTraps.size();i++)
	{
		//m_vpcInactiveIceTraps[i]->Uninitialize();
		delete m_vpcInactiveIceTraps[i];
	}
	for(unsigned int i = 0;i < m_vpcActiveIceTraps.size();i++)
	{
		m_vpcActiveIceTraps[i]->Uninitialize();
		delete m_vpcActiveIceTraps[i];
	}

	for(unsigned int i = 0;i < m_vpcInactiveWalls.size();i++)
	{
		//m_vpcInactiveWalls[i]->Uninitialize();
		delete m_vpcInactiveWalls[i];
	}
	for(unsigned int i = 0;i < m_vpcActiveWalls.size();i++)
	{
		m_vpcActiveWalls[i]->Uninitialize();
		delete m_vpcActiveWalls[i];
	}
	for(unsigned int i = 0;i < m_vpcActiveSnowballs.size();i++)
	{
		m_vpcActiveSnowballs[i]->Uninitialize();
		delete m_vpcActiveSnowballs[i];
	}
	for(unsigned int i = 0;i < m_vpcInactiveSnowballs.size();i++)
	{
		//m_vpcInactiveSnowballs[i]->Uninitialize();
		delete m_vpcInactiveSnowballs[i];
	}
	for(unsigned int i = 0;i < m_vpcActivePortals.size();i++)
	{
		//m_vpcActivePortals[i]->Uninitialize();
		delete m_vpcActivePortals[i];
	}
	
	for(unsigned int i = 0;i < m_vpcInactivePortals.size();i++)
	{
		//m_vpcInactivePortals[i]->Uninitialize();
		delete m_vpcInactivePortals[i];
	}
	
	for(unsigned int i = 0;i < m_vpcInactivePickups.size();i++)
	{
		//m_vpcInactivePickups[i]->Uninitialize();
		delete m_vpcInactivePickups[i];
	}
	
	for(unsigned int i = 0;i < m_vpcActiveSpawners.size();i++)
	{
		m_vpcActiveSpawners[i]->Uninitialize();
		delete m_vpcActiveSpawners[i];
	}

	for(unsigned int i = 0;i < m_vpcInactiveSpawners.size();i++)
	{
		//m_vpcInactiveSpawners[i]->Uninitialize();
		delete m_vpcInactiveSpawners[i];
	}

	for(unsigned int i = 0;i < m_vpcActiveSceneryObjects.size();i++)
	{
		m_vpcActiveSceneryObjects[i]->Uninitialize();
		delete m_vpcActiveSceneryObjects[i];
	}

	for(unsigned int i = 0;i < m_vpcInactiveSceneryObjects.size();i++)
	{
		//m_vpcInactiveSceneryObjects[i]->Uninitialize();
		delete m_vpcInactiveSceneryObjects[i];
	}

	for(unsigned int i = 0;i < m_vpcActiveSignPosts.size();i++)
	{
		m_vpcActiveSignPosts[i]->Uninitialize();
		delete m_vpcActiveSignPosts[i];
	}

	for(unsigned int i = 0;i < m_vpcInactiveSignPosts.size();i++)
	{
		//m_vpcInactiveSceneryObjects[i]->Uninitialize();
		delete m_vpcInactiveSignPosts[i];
	}

	m_vpcActiveEnvironmentObjects.clear();
	m_vpcActiveProjectiles.clear();
	m_vpcActiveExplosiveEnemies.clear();
	m_vpcActiveMeleeEnemies.clear();
	m_vpcActiveRangedEnemies.clear();
	m_vpcActiveFireEnemies.clear();
	m_vpcActiveIceEnemies.clear();
	//m_vpcActiveCollidableObjects.clear();
	m_vpcInactiveEnvironmentObjects.clear();
	m_vpcInactiveProjectiles.clear();
	m_vpcInactiveExplosiveEnemies.clear();
	m_vpcInactiveMeleeEnemies.clear();
	m_vpcInactiveRangedEnemies.clear();
	m_vpcInactiveFireEnemies.clear();
	m_vpcInactiveIceEnemies.clear();
	//m_vpcInactiveCollidableObjects.clear();
	m_vpcInactiveAttackObjects.clear();
	m_vpcActiveAttackObjects.clear();
	m_vpcInactiveFireTraps.clear();
	m_vpcActiveFireTraps.clear();
	m_vpcInactiveIceTraps.clear();
	m_vpcActiveIceTraps.clear();
	m_vpcInactiveSnowballs.clear();
	m_vpcActiveSnowballs.clear();
	m_vpcInactivePortals.clear();
	m_vpcActivePortals.clear();
	m_vpcInactivePickups.clear();
	m_vpcActivePickups.clear();
	m_vpcActiveSpawners.clear();
	m_vpcInactiveSpawners.clear();
	m_vpcActiveSceneryObjects.clear();
	m_vpcInactiveSceneryObjects.clear();
	m_vpcActiveSignPosts.clear();
	m_vpcInactiveSignPosts.clear();
}

bool CObjectManager::CheckActiveEnemies()
{
	//int nNumActiveEnemies = 0;
	/*if(this->m_vpcActiveExplosiveEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveExplosiveEnemies.size();i++)
		{
			if(m_vpcActiveExplosiveEnemies[i]->GetIsGuardian())
				return true;
		}
	}
	if(this->m_vpcActiveFireEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveFireEnemies.size();i++)
		{
			if(m_vpcActiveFireEnemies[i]->GetIsGuardian())
				return true;
		}
	}
	if(this->m_vpcActiveIceEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveIceEnemies.size();i++)
		{
			if(m_vpcActiveIceEnemies[i]->GetIsGuardian())
			{
				return true;
			}
		}
	}
	if(this->m_vpcActiveMeleeEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveMeleeEnemies.size();i++)
		{
			if(m_vpcActiveMeleeEnemies[i]->GetIsGuardian())
				return true;
		}
	}
	if(this->m_vpcActiveRangedEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveRangedEnemies.size();i++)
		{
			if(m_vpcActiveRangedEnemies[i]->GetIsGuardian())
				return true;
		}
	}*/

	/*if(this->m_vpcActiveExplosiveEnemies.size())
		return true;
	if(this->m_vpcActiveFireEnemies.size())
		return true;
	if(this->m_vpcActiveIceEnemies.size())
		return true;
	if(this->m_vpcActiveMeleeEnemies.size())
		return true;
	if(this->m_vpcActiveRangedEnemies.size())
		return true;
		*/

	if(m_pcWizardEnemy->GetInitialized())
		return true;

	return false;
}
CEnemy * CObjectManager::GetCrystalGuardian()
{
	//CEnemy * pEnemy = nullptr;

	if(this->m_vpcActiveExplosiveEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveExplosiveEnemies.size();i++)
		{
			if(m_vpcActiveExplosiveEnemies[i]->GetIsGuardian())
				return m_vpcActiveExplosiveEnemies[i];
		}
	}
	if(this->m_vpcActiveFireEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveFireEnemies.size();i++)
		{
			if(m_vpcActiveFireEnemies[i]->GetIsGuardian())
				return m_vpcActiveFireEnemies[i];
		}
	}
	if(this->m_vpcActiveIceEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveIceEnemies.size();i++)
		{
			if(m_vpcActiveIceEnemies[i]->GetIsGuardian())
			{
				return m_vpcActiveIceEnemies[i];
			}
		}
	}
	if(this->m_vpcActiveMeleeEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveMeleeEnemies.size();i++)
		{
			if(m_vpcActiveMeleeEnemies[i]->GetIsGuardian())
				return m_vpcActiveMeleeEnemies[i];
		}
	}
	if(this->m_vpcActiveRangedEnemies.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveRangedEnemies.size();i++)
		{
			if(m_vpcActiveRangedEnemies[i]->GetIsGuardian())
				return m_vpcActiveRangedEnemies[i];
		}
	}
	return nullptr;
}

void CObjectManager::ClearScenery(void)
{
	if(this->m_vpcActiveSceneryObjects.size())
	{
		for(unsigned int i = 0; i < m_vpcActiveSceneryObjects.size();i++)
		{
			m_vpcActiveSceneryObjects[i]->Uninitialize();
			m_vpcInactiveSceneryObjects.push_back(m_vpcActiveSceneryObjects[i]);
		}
		m_vpcActiveSceneryObjects.clear();
	}
}

void CObjectManager::ClearPickups()
{
	if(this->m_vpcActivePickups.size())
	{
		for(unsigned int i = 0; i < m_vpcActivePickups.size();i++)
		{
			m_vpcActivePickups[i]->Uninitialize();
			m_vpcInactivePickups.push_back(m_vpcActivePickups[i]);
		}
		m_vpcActivePickups.clear();
	}
}

void CObjectManager::ClearEnvironmental()
{
	if (m_vpcActiveEnvironmentObjects.size())
	{
		unsigned int arrSize = m_vpcActiveEnvironmentObjects.size();
		for (unsigned int i = 0; i < arrSize; ++i)
		{
			m_vpcActiveEnvironmentObjects[i]->Uninitialize();
			m_vpcInactiveEnvironmentObjects.push_back(m_vpcActiveEnvironmentObjects[i]);
		}
		m_vpcActiveEnvironmentObjects.clear();
	}
}

void CObjectManager::ClearFireTraps(void)
{
	if (m_vpcActiveFireTraps.size())
	{
		unsigned int arrSize = m_vpcActiveFireTraps.size();
		for (unsigned int index = 0; index < arrSize; index++)
		{
			m_vpcActiveFireTraps[index]->Uninitialize();
			m_vpcInactiveFireTraps.push_back(m_vpcActiveFireTraps[index]);
		}
		m_vpcActiveFireTraps.clear();
	}
}

void CObjectManager::ClearIceTraps(void)
{
	if (m_vpcActiveIceTraps.size())
	{
		unsigned int arrSize = m_vpcActiveIceTraps.size();
		for (unsigned int index = 0; index < arrSize; index++)
		{
			m_vpcActiveIceTraps[index]->Uninitialize();
			m_vpcInactiveIceTraps.push_back(m_vpcActiveIceTraps[index]);
		}
		m_vpcActiveIceTraps.clear();
	}
}

void CObjectManager::ClearSnowballs(void)
{
	if (m_vpcActiveSnowballs.size())
	{
		unsigned int arrSize = m_vpcActiveSnowballs.size();
		for (unsigned int index = 0; index < arrSize; index++)
		{
			m_vpcActiveSnowballs[index]->Uninitialize();
			m_vpcInactiveSnowballs.push_back(m_vpcActiveSnowballs[index]);
		}
		m_vpcActiveSnowballs.clear();
	}
}

void CObjectManager::UninitializeAllObjects()
{
	if(m_pcPlayer != nullptr)
	{
		this->m_pcPlayer->Uninitialize();
	}
	if(this->m_pcWizardEnemy != nullptr)
	{
		this->m_pcWizardEnemy->Uninitialize();
	}

	for(unsigned int i = 0;i < m_vpcActiveAttackObjects.size();i++)
	{
		m_vpcActiveAttackObjects[i]->Uninitialize();
		m_vpcInactiveAttackObjects.push_back(m_vpcActiveAttackObjects[i]);
	}

	

	for(unsigned int i = 0;i < m_vpcActiveEnvironmentObjects.size();i++)
	{
		m_vpcActiveEnvironmentObjects[i]->Uninitialize();
		m_vpcInactiveEnvironmentObjects.push_back( m_vpcActiveEnvironmentObjects[i]);
	}
	
	for(unsigned int i = 0;i < m_vpcActiveProjectiles.size();i++)
	{
		m_vpcActiveProjectiles[i]->Uninitialize();
		m_vpcInactiveProjectiles.push_back( m_vpcActiveProjectiles[i]);
	}
	
	for(unsigned int i = 0;i < m_vpcActiveExplosiveEnemies.size();i++)
	{
		m_vpcActiveExplosiveEnemies[i]->Uninitialize();
		m_vpcInactiveExplosiveEnemies.push_back( m_vpcActiveExplosiveEnemies[i]);
	}
	
	for(unsigned int i = 0;i < m_vpcActiveMeleeEnemies.size();i++)
	{
		m_vpcActiveMeleeEnemies[i]->Uninitialize();
		m_vpcInactiveMeleeEnemies.push_back( m_vpcActiveMeleeEnemies[i]);
	}
	
	for(unsigned int i = 0;i < m_vpcActiveRangedEnemies.size();i++)
	{
		m_vpcActiveRangedEnemies[i]->Uninitialize();
		m_vpcInactiveRangedEnemies.push_back( m_vpcActiveRangedEnemies[i]);
	}
	
	for(unsigned int i = 0;i < m_vpcActiveFireEnemies.size();i++)
	{
		m_vpcActiveFireEnemies[i]->Uninitialize();
		m_vpcInactiveFireEnemies.push_back( m_vpcActiveFireEnemies[i]);
	}
	
	for(unsigned int i = 0;i < m_vpcActiveIceEnemies.size();i++)
	{
		m_vpcActiveIceEnemies[i]->Uninitialize();
		m_vpcInactiveIceEnemies.push_back( m_vpcActiveIceEnemies[i]);
	}
	
	//for(unsigned int i = 0;i < m_vpcActiveCollidableObjects.size();i++)
	//{
	//	m_vpcActiveCollidableObjects[i]->Uninitialize();
	//	m_vpcInactiveCollidableObjects.push_back( m_vpcActiveCollidableObjects[i]);
	//}

	for(unsigned int i = 0;i < m_vpcActivePickups.size();i++)
	{
		m_vpcActivePickups[i]->Uninitialize();
		m_vpcInactivePickups.push_back( m_vpcActivePickups[i]);
	}
	for(unsigned int i = 0;i < m_vpcActiveFireTraps.size();i++)
	{
		m_vpcActiveFireTraps[i]->Uninitialize();
		m_vpcInactiveFireTraps.push_back( m_vpcActiveFireTraps[i]);
	}

	for(unsigned int i = 0;i < m_vpcActiveIceTraps.size();i++)
	{
		m_vpcActiveIceTraps[i]->Uninitialize();
		m_vpcInactiveIceTraps.push_back( m_vpcActiveIceTraps[i]);
	}

	for(unsigned int i = 0;i < m_vpcActiveWalls.size();i++)
	{
		m_vpcActiveWalls[i]->Uninitialize();
		m_vpcInactiveWalls.push_back( m_vpcActiveWalls[i]);
	}
	for(unsigned int i = 0;i < m_vpcActiveSnowballs.size();i++)
	{
		m_vpcActiveSnowballs[i]->Uninitialize();
		m_vpcInactiveSnowballs.push_back( m_vpcActiveSnowballs[i]);
	}

	for(unsigned int i = 0;i < m_vpcActivePortals.size();i++)
	{
		m_vpcActivePortals[i]->Uninitialize();
		m_vpcInactivePortals.push_back( m_vpcActivePortals[i]);
	}

	for(unsigned int i = 0;i < m_vpcActiveSpawners.size();i++)
	{
		m_vpcActiveSpawners[i]->Uninitialize();
		m_vpcInactiveSpawners.push_back( m_vpcActiveSpawners[i]);
	}


	for(unsigned int i = 0;i < m_vpcActiveSceneryObjects.size();i++)
	{
		m_vpcActiveSceneryObjects[i]->Uninitialize();
		m_vpcInactiveSceneryObjects.push_back( m_vpcActiveSceneryObjects[i]);
	}


	m_vpcActiveEnvironmentObjects.clear();
	m_vpcActiveProjectiles.clear();
	m_vpcActiveExplosiveEnemies.clear();
	m_vpcActiveMeleeEnemies.clear();
	m_vpcActiveRangedEnemies.clear();
	m_vpcActiveFireEnemies.clear();
	m_vpcActiveIceEnemies.clear();
//	m_vpcActiveCollidableObjects.clear();
	m_vpcActiveAttackObjects.clear();
	m_vpcActiveFireTraps.clear();
	m_vpcActiveIceTraps.clear();
	m_vpcActiveSnowballs.clear();
	m_vpcActivePortals.clear();
	m_vpcActivePickups.clear();
	m_vpcActiveSpawners.clear();
	m_vpcActiveSceneryObjects.clear();
}

