/********************************************************************
* Filename:			CLevel.cpp
* Date:				11/8/2012
* Mod. Date:		11/27/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This is a level in the game
********************************************************************/

#include "CLevel.h"
#include "CWorldManager.h"

#include "../Sound/CSoundManager.h"
#include "../CGame.h"
#include "../Objects/EnvironmentObject.h"
#include "../Objects/SceneryObject.h"
#include "../Objects/Signpost.h"

//int CLevel::sm_nSoundCount = 3;
CLevel::CLevel(void)
{
	m_pWall		= nullptr;
	m_nLevelID	= -1;
	m_fPosZ		= 0.0f;
	m_fExtentZ	= 0.0f;
	m_fExtentX	= 0.0f;
	m_fPosY		= 0.0f;
	m_fCenterX	= 0.0f;
	m_bActive	= false;
	m_nBackgroundID = -1;
	m_bIntroDone	= false;
	//m_nSoundCount = 3;

	m_fPoppingEnemiesTimer = -1.0f;
	m_fPoppingEnemiesCooldown = 0.0f;
}

CLevel::~CLevel(void)
{

}

void CLevel::MakeGround(void)
{
	// Create the ground piece
	CEnvironmentObject* pGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pGround = pGround;
	m_pvEnviromentObjects.push_back(pGround);

	//pGround->GetMatrix()->scale_post(5.0f, 1.0f, 5.0f);
	pGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	// Get the min and the max of the ground piece
	vec3f vMin, vMax;
	CView::GetCollisionAabb(pGround->GetRenderID(), vMin, vMax);

	// Get the offset for this level based on the id
	m_fExtentZ = -(vMax.z - vMin.z);
	m_fExtentX = vMax.x - vMin.x;
	m_fPosZ = vMin.z + m_fExtentZ * m_nLevelID;
	m_fPosY = vMax.y;
	m_fCenterX = (vMax.x + vMin.x) / 2.0f;

	// Change the ground position to match the correct offset and set the bound variables
	matrix4f worldMatrix = *(pGround->GetWorldMatrix());
	worldMatrix.axis_pos.z = GetCenterZ();
	pGround->SetWorldMatrix(worldMatrix);

	// Create the lower ground piece
	CEnvironmentObject* pLowerGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pLowerGround);
	pLowerGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGround->SetPos(vec3f((pGround->GetPos().x - m_fExtentX) + 2.5f, pGround->GetPos().y - 20, pGround->GetPos().z));

	// make the walls for the level
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 20.0f, GetCenterZ()), 90);			// Very Top
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 0.0f, GetCenterZ()), 90);			// Top
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f) + 3.0f, -22.0f, GetCenterZ()), 90);	// Bottom
	MakeWall(vec3f(m_fCenterX + 3.0f, -20.0f, GetCenterZ() - m_fExtentZ / 2), 180);		// Front

	// If the first level need to make some terrain to block players view
	if(0 == m_nLevelID)
	{
		// Creat the pice before the level
		CEnvironmentObject* pBeforeGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
		m_pvEnviromentObjects.push_back(pBeforeGround);
		pBeforeGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
		pBeforeGround->SetPos(vec3f(pGround->GetPos().x, pGround->GetPos().y, pGround->GetPos().z - m_fExtentZ));

		// Create the lower before piece
		CEnvironmentObject* pBeforeLowerGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
		m_pvEnviromentObjects.push_back(pBeforeLowerGround);
		pBeforeLowerGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
		pBeforeLowerGround->SetPos(vec3f((pGround->GetPos().x - m_fExtentX)+2.0f, pGround->GetPos().y - 20, pGround->GetPos().z - m_fExtentZ));

		// Make the walls for the level
		MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 20.0f, GetCenterZ() - m_fExtentZ), 90);			// Very Top
		MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 0.0f, GetCenterZ() - m_fExtentZ), 90);				// Top
		MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f) + 3.0f, -22.0f, GetCenterZ() - m_fExtentZ), 90);	// Bot
	}

	// Load the sky texture to the background
	m_nBackgroundID = CAssetManager::LoadTexture("Mountain.png");
}

void CLevel::MakeWizardGround(void)
{
	// Create the ground piece
	CEnvironmentObject* pGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pGround = pGround;
	m_pvEnviromentObjects.push_back(pGround);	
	pGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	// Get the min and the max of the ground piece
	vec3f vMin, vMax;
	CView::GetCollisionAabb(pGround->GetRenderID(), vMin, vMax);

	// Get the offset for this level based on the id
	m_fExtentZ = -(vMax.z - vMin.z);
	m_fExtentX = vMax.x - vMin.x;
	m_fPosZ = vMin.z + m_fExtentZ /** m_nLevelID*/;
	m_fPosY = vMax.y;
	m_fCenterX = (vMax.x + vMin.x) / 2.0f;

	// Change the ground position to match the correct offset and set the bound variables
	matrix4f worldMatrix = *(pGround->GetWorldMatrix());
	worldMatrix.axis_pos.z = GetCenterZ();
	pGround->SetWorldMatrix(worldMatrix);

	// Create the ground piece
	CEnvironmentObject* pLowerGroundOne = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundOne);	
	pLowerGroundOne->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundOne->SetPos(vec3f((pGround->GetPos().x - m_fExtentX), pGround->GetPos().y - 8, pGround->GetPos().z));

	// Create the ground piece
	CEnvironmentObject* pLowerGroundTwo = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundTwo);	
	pLowerGroundTwo->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundTwo->SetPos(vec3f((pGround->GetPos().x - m_fExtentX), pGround->GetPos().y - 8, pGround->GetPos().z - m_fExtentZ));

	// Create the ground piece
	CEnvironmentObject* pLowerGroundThree = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundThree);	
	pLowerGroundThree->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundThree->SetPos(vec3f(pGround->GetPos().x, pGround->GetPos().y - 8, pGround->GetPos().z - m_fExtentZ));

	// Create the ground piece
	CEnvironmentObject* pLowerGroundFour = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundFour);	
	pLowerGroundFour->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundFour->SetPos(vec3f((pGround->GetPos().x + m_fExtentX), pGround->GetPos().y - 8, pGround->GetPos().z));

	// Create the ground piece
	CEnvironmentObject* pLowerGrounFive = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGrounFive);	
	pLowerGrounFive->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGrounFive->SetPos(vec3f((pGround->GetPos().x + m_fExtentX), pGround->GetPos().y - 8, pGround->GetPos().z + m_fExtentZ));

	// Create the ground piece
	CEnvironmentObject* pLowerGroundSix = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundSix);	
	pLowerGroundSix->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundSix->SetPos(vec3f(pGround->GetPos().x, pGround->GetPos().y - 8, pGround->GetPos().z + m_fExtentZ));

	// Create the ground piece
	CEnvironmentObject* pLowerGroundSeven = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundSeven);	
	pLowerGroundSeven->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundSeven->SetPos(vec3f((pGround->GetPos().x - m_fExtentX), pGround->GetPos().y - 8, pGround->GetPos().z + m_fExtentZ));

	// Create the ground piece
	CEnvironmentObject* pLowerGroundEight = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WIZARDGROUND);
	m_pvEnviromentObjects.push_back(pLowerGroundEight);	
	pLowerGroundEight->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pLowerGroundEight->SetPos(vec3f((pGround->GetPos().x + m_fExtentX), pGround->GetPos().y - 8, pGround->GetPos().z - m_fExtentZ));

	// Make walls
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f), -22.0f, GetCenterZ() + (m_fExtentZ / 4)), 90);
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f), -22.0f, GetCenterZ() - (m_fExtentZ / 4)), 90);

	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 4.0f), -22.0f, GetCenterZ() - (m_fExtentZ / 2)), 180);
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 4.0f), -22.0f, GetCenterZ() - (m_fExtentZ / 2)), 180);

	// Load the sky texture to the background
	m_nBackgroundID = CAssetManager::LoadTexture("SkyTexture.jpg");
}

void CLevel::MakeBridge(void)
{
	// Create the ground piece
	CEnvironmentObject* pGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_BRIDGE);
	m_pGround = pGround;
	m_pvEnviromentObjects.push_back(pGround);
	pGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	// Get the min and the max of the ground piece
	vec3f vMin, vMax;
	CView::GetCollisionAabb(pGround->GetRenderID(), vMin, vMax);

	// Get the offset for this level based on the id
	m_fExtentZ = -(vMax.z - vMin.z);
	m_fExtentX = vMax.x - vMin.x;
	m_fPosZ = vMin.z + m_fExtentZ * m_nLevelID;
	m_fPosY = vMax.y;
	m_fCenterX = (vMax.x + vMin.x) / 2.0f;

	// Change the ground position to match the correct offset and set the bound variables
	matrix4f worldMatrix = *(pGround->GetWorldMatrix());
	worldMatrix.axis_pos.z = GetCenterZ();
	pGround->SetWorldMatrix(worldMatrix);

	// Create the plane below the bridge
	CEnvironmentObject* pBelowGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pBelowGround);

	pBelowGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	pBelowGround->SetPos(vec3f(pGround->GetPos().x, pGround->GetPos().y - 20, pGround->GetPos().z));

	// Create the lower piece that is farther out
	CEnvironmentObject* pBeforeLowerGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pBeforeLowerGround);
	pBeforeLowerGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pBeforeLowerGround->SetPos(vec3f((pGround->GetPos().x - m_fExtentX * 2.0f) + 2.7f, pGround->GetPos().y - 20, pGround->GetPos().z));

	// Make the walls for the level
	// This magic number (17.6) is about half the width of a regular level - this will make so that the wall is on the right position regardless of the bridge size
	MakeWall(vec3f(17.6f, 20.0f, GetCenterZ() - 0.17f), 90);	// Very Top
	MakeWall(vec3f(17.6f, 0.0f, GetCenterZ() - 0.17f), 90);		// Top
	MakeWall(vec3f(17.6f, -20.0f, GetCenterZ()), 90);			// Bot

	// Make the lava for under the bridge
	CEnvironmentObject* pLava = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(
	vec3f(pGround->GetPos().x - 0.5f, pGround->GetPos().y - 19, pGround->GetPos().z), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_LAVA);
	m_pvEnviromentObjects.push_back(pLava);
	worldMatrix = *pLava->GetWorldMatrix();
	worldMatrix.rotate_y_post(D3DXToRadian(90));
	pLava->SetWorldMatrix(worldMatrix);

	// Load the sky texture to the background
	m_nBackgroundID = CAssetManager::LoadTexture("Mountain.png");
}

void CLevel::MakeCliff(void)
{
	// Create the ground piece
	CEnvironmentObject* pGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(8.75, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_CLIFF);
	m_pGround = pGround;
	m_pvEnviromentObjects.push_back(pGround);

	pGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	// Get the min and the max of the ground piece
	vec3f vMin, vMax;
	CView::GetCollisionAabb(pGround->GetRenderID(), vMin, vMax);

	// Get the offset for this level based on the id
	m_fExtentZ = -(vMax.z - vMin.z);
	m_fExtentX = vMax.x - vMin.x;
	m_fPosZ = vMin.z + m_fExtentZ * m_nLevelID;
	m_fPosY = vMax.y;
	m_fCenterX = (vMax.x + vMin.x) / 2.0f;

	// Change the ground position to match the correct offset and set the bound variables
	matrix4f worldMatrix = *(pGround->GetWorldMatrix());
	worldMatrix.axis_pos.z = GetCenterZ();
	pGround->SetWorldMatrix(worldMatrix);
//	pGround->GetMatrix()->axis_pos.z = GetCenterZ();

	// Creat the pice before the level
	CEnvironmentObject* pBeforeGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pBeforeGround);

	pBeforeGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	pBeforeGround->SetPos(vec3f(pGround->GetPos().x - m_fExtentX / 2, pGround->GetPos().y - 20, pGround->GetPos().z));

	// Create the lower before piece
	CEnvironmentObject* pBeforeLowerGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pBeforeLowerGround);

	pBeforeLowerGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));

	pBeforeLowerGround->SetPos(vec3f((pGround->GetPos().x - m_fExtentX * 2.25f) + 2.3f, pGround->GetPos().y - 20, pGround->GetPos().z));

	// Make the walls for the level
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f) - 1.0f, 20.0f, GetCenterZ()), 90);	// Very Top
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f) - 1.0f, 0.0f, GetCenterZ()), 90);	// Top
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f) + 1.0f, -20.0f, GetCenterZ()), 90);	// Bot
	
	// Load the sky texture to the background
	m_nBackgroundID = CAssetManager::LoadTexture("Mountain.png");
}

void CLevel::MakeEndGround(void)
{
	// Creat the pice After the level
	CEnvironmentObject* pAfterGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pAfterGround);
	pAfterGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pAfterGround->SetPos(vec3f(m_pGround->GetPos().x, m_pGround->GetPos().y, m_pGround->GetPos().z + m_fExtentZ));

	// Create the lower after piece
	CEnvironmentObject* pAfterLowerGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pAfterLowerGround);
	pAfterLowerGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pAfterLowerGround->SetPos(vec3f(m_pGround->GetPos().x - m_fExtentX  + 3.0f, m_pGround->GetPos().y - 20, m_pGround->GetPos().z + m_fExtentZ));

	// Creat the second pice After the level
	CEnvironmentObject* pSecondAfterGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pSecondAfterGround);
	pSecondAfterGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pSecondAfterGround->SetPos(vec3f(m_pGround->GetPos().x, m_pGround->GetPos().y, m_pGround->GetPos().z + (m_fExtentZ * 2.0f)));

	// Create the second lower after piece
	CEnvironmentObject* pSecondAfterLowerGround = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(vec3f(0, 0, 0), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_GROUND);
	m_pvEnviromentObjects.push_back(pSecondAfterLowerGround);
	pSecondAfterLowerGround->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	pSecondAfterLowerGround->SetPos(vec3f(m_pGround->GetPos().x - m_fExtentX  + 3.0f, m_pGround->GetPos().y - 20, m_pGround->GetPos().z + (m_fExtentZ * 2.0f)));

	// Make Walls
	// After section
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 20.0f, GetCenterZ() + m_fExtentZ), 90);			// Very Top
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 0.0f, GetCenterZ() + m_fExtentZ), 90);				// Top
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f) + 3.0f, -22.0f, GetCenterZ() + m_fExtentZ), 90);	// Bot

	// Second after section
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 20.0f, GetCenterZ() + (m_fExtentZ * 2)), 90);			// Very Top
	MakeWall(vec3f(m_fCenterX + (m_fExtentX / 2.0f), 0.0f, GetCenterZ() + (m_fExtentZ * 2)), 90);			// Top
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f) + 3.0f, -22.0f, GetCenterZ() + (m_fExtentZ * 2)), 90);	// Bot

	// Make end of level
	MakeWall(vec3f(m_fCenterX, 0.0f, GetCenterZ() + (m_fExtentZ * 2.5f)), 180);							// End of level
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f), 0.0f, GetCenterZ() + (m_fExtentZ * 2.5f)), 180);	// Side end
	MakeWall(vec3f(m_fCenterX - (m_fExtentX / 2.0f), -20.0f, GetCenterZ() + (m_fExtentZ * 2.5f)), 180);	// Side bottom
}

void CLevel::MakeWall(vec3f vPos, float fAngle)
{
	if(CEnvironmentObject* pWall =
		dynamic_cast<CEnvironmentObject*>(CWorldManager::GetInstance()->MakeObject(vPos, ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_WALL)))
	{
		m_pvEnviromentObjects.push_back(pWall);
		matrix4f worldMatrix = *(pWall->GetWorldMatrix());
		worldMatrix.rotate_y_post(D3DXToRadian(fAngle));
		pWall->SetWorldMatrix(worldMatrix);
		pWall->SetColor(vec3f(1.0f, 1.0f, 1.0f));
	}

	/* MB: This is for if we want to make all walls scenery objects
	// Note: need to make this relative position but are passed in absolute position
	CSceneryObject* pWall = dynamic_cast<CSceneryObject*>(PushScenery(vPos, fAngle, 1.0, false, "MountainWall"));
	*/
}

void CLevel::Update(float fElapsedTime)
{
	// Intro actions
	DoIntroActions(fElapsedTime);
	DoBossLevelIntroActions(fElapsedTime);
	CleanupIntroActions(fElapsedTime);

	if(m_fPoppingEnemiesTimer >= 0.0f)
	{
		// If there are no more active enemies stop this
		if(m_pvActiveEnemies.empty())
			m_fPoppingEnemiesTimer = -1.0f;
		// Else let's increase the timer
		else
			m_fPoppingEnemiesTimer += fElapsedTime;
	}

	for(size_t i = 0; i < m_pvActiveEnemies.size(); ++i)
	{
		CEnemy* pEnemy = m_pvActiveEnemies[i];
				
		if(!pEnemy)
			continue;

		if(m_fPoppingEnemiesTimer >= m_fPoppingEnemiesCooldown && pEnemy->GetType() != WIZARD_OBJECT)
		{
			m_fPoppingEnemiesTimer = 0.0f;
			CView::GetEmitterManager().AddEmitter(false, true, pEnemy->GetPos(), "Resource/Particles/Explosion.txt", true); 
			pEnemy->Uninitialize();
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_SMALL_FLESHY_EXPLOSION, pEnemy->GetSoundID());
		}

		// HACK
		if(!pEnemy->GetInitialized())
			m_vDeleteVector.push_back(i);
		else
		{
			if(m_bActive)
				CWorldManager::GetInstance()->BoundsKeeping(pEnemy);
			else
				KeepInside(pEnemy);
		}
	}
	
	// if any objects need to be deleted
	// Signed int because otherwise WTFBBQPWNSTARS
	for(int index = m_vDeleteVector.size() - 1; index >= 0; --index)
	{
		m_pvActiveEnemies.erase(m_pvActiveEnemies.begin() + m_vDeleteVector[index]);
	}
	m_vDeleteVector.clear();

	for(size_t i = 0; i < m_pvActiveBeasts.size(); ++i)
	{
		CEnemy* pEnemy = m_pvActiveBeasts[i];
				
		if(!pEnemy)
		{
			continue;
		}

		// HACK
		if(!pEnemy->GetInitialized())
		{
			m_vDeleteVector.push_back(i);
		}
	}
	
	// if any objects need to be deleted
	// Signed int because otherwise WTFBBQPWNSTARS
	for(int index = m_vDeleteVector.size() - 1; index >= 0; --index)
	{
		m_pvActiveBeasts.erase(m_pvActiveBeasts.begin() + m_vDeleteVector[index]);
	}
	m_vDeleteVector.clear();
}

void CLevel::DoIntroActions(float fElapsedTime)
{	
	// if we're doing a boss intro, don't set introDone to true before returning -SMS
	if (CWorldManager::GetInstance()->GetCurrentSection() == 4)
		return;
	// for any other level...
	if((!CView::GetCamera().IsTransitioning()) || (CView::GetCamera().GetCenterPos().z > m_fPosZ - m_fExtentZ )) {
		m_bIntroDone = true;
		return; }

	// Do stuff you want as an intro to this particular level here. -JC 
	// Anything you do will be visible around the time the camera hits about 1/4
	// through the level when it's panning for the intro.
	if(!m_bIntroDone && CView::GetCamera().IsTransitioning() &&
		( CView::GetCamera().GetCenterPos().z > GetCenterZ() + m_fExtentZ * 1.5f )) {
		
		// LOW ORBIT CRYSTAL CANNON ENGAGE! #YOLO -JC
		unsigned int enviroSize = m_pvEnviromentObjects.size();
		for(unsigned int i = 0; i < enviroSize; ++i) {
			if(m_pvEnviromentObjects[i]->GetType() == SPAWNER_OBJECT) {
				// Make 'em visible
				CView::SetIsVisible( m_pvEnviromentObjects[i]->GetRenderID(), true );
				// Set 'em high on Y
				vec3f newPos = m_pvEnviromentObjects[i]->GetPos();
				newPos.y += 60.0f;
				m_pvEnviromentObjects[i]->SetPos(newPos);
				// Send 'em blazing in from orbit! - JC
				m_pvEnviromentObjects[i]->SetVelocity(vec3f(0.0f,-80.0f,0.0f)); }
			}

			m_bIntroDone = true; }
	}

void CLevel::DoBossLevelIntroActions(float fElapsedTime)
{
	if(CWorldManager::GetInstance()->GetCurrentSection() != 4 || m_bIntroDone)
		return;

	// LOW ORBIT CRYSTAL CANNON ENGAGE! #YOLO -JC
	unsigned int enviroSize = m_pvEnviromentObjects.size();
	for(unsigned int i = 0; i < enviroSize; ++i) 
	{
		if(m_pvEnviromentObjects[i]->GetType() == SPAWNER_OBJECT) 
		{
			// Make 'em visible
			CView::SetIsVisible( m_pvEnviromentObjects[i]->GetRenderID(), true );
			// Set 'em high on Y
			vec3f newPos = m_pvEnviromentObjects[i]->GetPos();
			newPos.y += 70.0f;
			m_pvEnviromentObjects[i]->SetPos(newPos);
			// Send 'em blazing in from orbit! - JC
			m_pvEnviromentObjects[i]->SetVelocity(vec3f(0.0f,-80.0f,0.0f)); 
		}
	}

	m_bIntroDone = true;
}

void CLevel::CleanupIntroActions(float fElapsedTime)
{
	// Clean up anything you did in intro here, if you need to. - JC
	if(m_bIntroDone && !CView::GetCamera().IsTransitioning()) {

	}
}

bool CLevel::ActiveEnemies(void)
{
	for(size_t i = 0; i < m_pvActiveEnemies.size(); ++i)
	{
		if(m_pvActiveEnemies[i])
		{
			return true;
		}
	}

	return false;
}

void CLevel::UnloadLevel(void)
{
	m_fPoppingEnemiesTimer = -1.0f;
	m_fPoppingEnemiesCooldown = 0.0f;

	for(size_t i = 0; i < m_pvActiveEnemies.size(); ++i)
	{
		if(m_pvActiveEnemies[i])
		{
			m_pvActiveEnemies[i]->Uninitialize();
			m_pvActiveEnemies[i] = nullptr;
		}
	}
	m_pvActiveEnemies.clear();

	// Added clearing of beasts, because uhhhh it wasn't there. - JC
	for(size_t i = 0; i < m_pvActiveBeasts.size(); ++i) {
		if(m_pvActiveBeasts[i]) {		
			m_pvActiveBeasts[i]->Uninitialize();
			m_pvActiveBeasts[i] = nullptr; }
	}	
	m_pvActiveBeasts.clear();

	for(size_t i = 0; i < m_pvEnviromentObjects.size(); ++i)
	{
		// Removed unnessecary gross checks that served no purpose. - JC
		if(m_pvEnviromentObjects[i]) {
		m_pvEnviromentObjects[i]->Uninitialize();
		m_pvEnviromentObjects[i] = nullptr; }
	}
	m_pvEnviromentObjects.clear();

	if(m_pWall)
	{
		m_pWall->Uninitialize();
		m_pWall = nullptr;
	}
	

	CWorldManager::GetInstance()->GetObjectManager()->ClearScenery();
}

CBaseObject* CLevel::PushEnemy(vec3f vPos, int nObjType, int _nAttackElement, int nSubType)
{
	CBaseObject* pEnemy = CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), nObjType, nSubType);
	
	if(pEnemy)
	{
		static_cast<CEnemy*>(pEnemy)->SetAttackElement(_nAttackElement);
	}

	if(pEnemy && pEnemy->GetType() == MELEE_OBJECT)
	{
		if(pEnemy->GetSoundID() && CSoundManager::GetInstance()->GetInit())
		{
			pEnemy->RegisterWithSoundManager();
		}
	}

	if(pEnemy && pEnemy->GetType() == RANGED_OBJECT)
	{
		if(pEnemy->GetSoundID() && CSoundManager::GetInstance()->GetInit())
		{
			pEnemy->RegisterWithSoundManager();
		}
	}

	if(pEnemy && pEnemy->GetType() == EXPLOSIVE_OBJECT)
	{
		if(pEnemy->GetSoundID() && CSoundManager::GetInstance()->GetInit())
		{
			pEnemy->RegisterWithSoundManager();
		}
	}

	if(pEnemy && pEnemy->GetType() == ICE_OBJECT)
	{
		m_pvActiveBeasts.push_back((CEnemy*)pEnemy);

		if(pEnemy->GetSoundID() && CSoundManager::GetInstance()->GetInit())
		{
			pEnemy->RegisterWithSoundManager();
		}
	}

	if(pEnemy && pEnemy->GetType() == FIRE_OBJECT)
	{
		if(pEnemy->GetSoundID() && CSoundManager::GetInstance()->GetInit())
		{
			pEnemy->RegisterWithSoundManager();
		}

		m_pvActiveBeasts.push_back((CEnemy*)pEnemy);
	}

	if(pEnemy)
	{
		m_pvActiveEnemies.push_back((CEnemy*)pEnemy);
	}

	/*if(sm_nSoundCount >= 10000)
	{
		LOG("Something has gone terribly wrong D=");
	}*/

	return pEnemy;
}

CBaseObject* CLevel::PushEnvironment(vec3f vPos, int nObjType, float _fElapsedTime, int nSubType, const char* szText)
{
	if(nObjType == WALL_OBJECT)
	{
		m_pWall = (CWall*)CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), nObjType, nSubType);
		if (m_pWall)
		{
			m_pWall->SetAttackElement(FIRE);
			m_pWall->SetPos(vPos);
		}
		return nullptr;
	}

	CBaseObject* pEnvironment = nullptr;

	if(nObjType == PORTAL_OBJECT)
	{
		pEnvironment = CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z/* + GetCenterZ()*/), nObjType, nSubType);
		// this is where the guardian object is being added and set as a guardian
		// to change the type of enemy, simply change the enum that is passed into the makeobject call
		//CEnemy * pEnemy = (CEnemy*)PushEnemy(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z - 5.0f),ICE_OBJECT,-1);// = (CEnemy*)CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z - 5.0f), ICE_OBJECT , 0);
		//pEnemy->SetGuardian(true);
		CWorldManager::GetInstance()->GetObjectManager()->SetPortal((CPortal*)pEnvironment);

		if(((CPortal*)pEnvironment)->GetSoundID() && CSoundManager::GetInstance()->GetInit())
		{
			((CPortal*)pEnvironment)->RegisterWithSoundManager();
		}
	}
	else
	{
		if(nSubType == CEnvironmentObject::ENVOBJ_SIGN)
		{
			pEnvironment = (CSignpost*)CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), SIGN_OBJECT, -1, szText);
		}
		else
		{
			pEnvironment = CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), nObjType, nSubType);
		}
	}

	if(pEnvironment)
	{
		m_pvEnviromentObjects.push_back((CEnvironmentObject*)pEnvironment);

		if(pEnvironment->GetType() == ICETRAP_OBJECT)
		{
			((CIceTrap*)pEnvironment)->SetModelPosition(pEnvironment->GetPos());
			((CIceTrap*)pEnvironment)->SetPos(vec3f(pEnvironment->GetPos().x, pEnvironment->GetPos().y - 3.0f, pEnvironment->GetPos().z));
			
			if(pEnvironment->GetSoundID() && CSoundManager::GetInstance()->GetInit())
			{
				pEnvironment->RegisterWithSoundManager();
			}
		}

		if(pEnvironment->GetType() == FIRETRAP_OBJECT)
		{
			// I made the Fire Trap class follow const correctness and cleaned this up - Dan
			CFireTrap* pFireTrap = dynamic_cast<CFireTrap*>(pEnvironment);

			if(pFireTrap->GetSoundID() && CSoundManager::GetInstance()->GetInit())
			{
				pFireTrap->RegisterWithSoundManager();
			}

			pFireTrap->SetElapsedTime(_fElapsedTime);
			pFireTrap->SetBaseObjPosition(vec3f(
				pFireTrap->GetPos().x,
				pFireTrap->GetPos().y + 3.0f,
				pFireTrap->GetPos().z));
			// This function sets the position of BOTH emitters and calls reset particles on BOTH of them
			pFireTrap->SetEmittersPosition(vec3f(
				pFireTrap->GetPos().x,
				pFireTrap->GetPos().y + 2.5f, // set the emitter up a little
				pFireTrap->GetPos().z));
		}

		if (pEnvironment->GetType() == SPAWNER_OBJECT)
		{
			CSpawner* pSpawner = dynamic_cast<CSpawner*>(pEnvironment);

			if(pSpawner->GetSoundID() && CSoundManager::GetInstance()->GetInit())
			{
				pSpawner->RegisterWithSoundManager();
			}
		}

		//if(nSubType == CEnvironmentObject::ENVOBJ_SIGN)
		//{
			//CSignpost* pSign = (CSignpost*)(pEnvironment);
			//CSignpost* pSign = (CSignpost*)CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), nObjType, nSubType, szText);
			//pSign->Initialize();
			//pSign->SetDescription(const_cast<char*>(szText));
			//pSign->SetLevel(this->m_nLevelID);
			//pEnvironment = (CSignpost*)CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), SCENERY_OBJECT, -1, szText);
		//}

		
	}

	/*if(sm_nSoundCount >= 10000)
	{
		LOG("Something has gone terribly wrong D=");
	}*/

	return pEnvironment;
}

CBaseObject* CLevel::PushScenery(vec3f vPos, float fYRot, float fScale, bool bUpdatable, const char* szModelName)
{

	CBaseObject* pScenery = nullptr;

	pScenery = (CSceneryObject*)CWorldManager::GetInstance()->MakeObject(vec3f(vPos.x, vPos.y + m_fPosY, vPos.z + GetCenterZ()), SCENERY_OBJECT, -1, szModelName);
	matrix4f worldMatrix = *pScenery->GetWorldMatrix();
	worldMatrix.rotate_y_post(D3DXToRadian(fYRot));
	worldMatrix.scale_pre(vec3f(fScale, fScale, fScale));
	pScenery->SetWorldMatrix(worldMatrix);
	((CSceneryObject*)pScenery)->SetIsUpdatable(bUpdatable);
	
	return pScenery;
}

void CLevel::ActivateEnemies(void)
{
	if(m_bActive)
		return;
	bool bAnyEnemy = false;
	for(size_t i = 0; i < m_pvActiveEnemies.size(); ++i)
	{
		if(m_pvActiveEnemies[i] && m_pvActiveEnemies[i]->GetFSM())
		{
			m_pvActiveEnemies[i]->GetFSM()->GetIsChasing() = true;
			bAnyEnemy = true;
		}
	}

	ActivateSpawners();

	m_bActive = true;
}

bool CLevel::IsInside(CBaseObject* pObj) const
{
	vec3f vPos = pObj->GetWorldMatrix()->axis_pos;

	float fObjExtX = 0.0f;
	float fObjExtZ = 0.0f;

	if(CCollidableObject* obj = dynamic_cast<CCollidableObject*>(pObj))
	{
		fObjExtX = (obj->GetAABBMax().x - obj->GetAABBMin().x) / 2.0f;
		fObjExtZ = (obj->GetAABBMax().z - obj->GetAABBMin().z) / 2.0f;
	}

	if( (vPos.x < (m_fCenterX+m_fExtentX *0.5f) - fObjExtX) &&
		(vPos.x > (m_fCenterX-m_fExtentX *0.5f) + fObjExtX) &&
		(vPos.z < (m_fPosZ + fObjExtZ)) &&
		(vPos.z > (m_fPosZ + m_fExtentZ) + fObjExtZ) )
	{
		return true;
	}
	else
		return false;
}

void CLevel::KeepInside(CBaseObject* pObj)
{
	//matrix4f worldMatrix = *(pObj->GetWorldMatrix());
	vec3f vPos = pObj->GetPos();

	float fObjExtX = 0.0f;
	float fObjExtZ = 0.0f;
	
	if(CCollidableObject* obj = dynamic_cast<CCollidableObject*>(pObj))
	{
		fObjExtX = (obj->GetAABBMax().x - obj->GetAABBMin().x) * 0.5f;
		fObjExtZ = (obj->GetAABBMax().z - obj->GetAABBMin().z) * 0.5f;
	}

	if(vPos.z > m_fPosZ)
	{
		vPos.z = vPos.z - (vPos.z - m_fPosZ);
	}
	else if(vPos.z < (m_fPosZ + m_fExtentZ) + fObjExtZ)
	{
		vPos.z = m_fPosZ + m_fExtentZ + fObjExtZ;
	}

	if(vPos.x > (m_fCenterX + m_fExtentX * 0.5f) - fObjExtX)
	{
		vPos.x = m_fCenterX + m_fExtentX * 0.5f - fObjExtX;
	}
	else if(vPos.x < (m_fCenterX - m_fExtentX * 0.5f) + fObjExtX)
	{
		vPos.x = m_fCenterX - m_fExtentX * 0.5f + fObjExtX;
	}
	pObj->SetPos(vPos);

	if(pObj->GetType() == FIRE_OBJECT)
	{
		if(((CFireEnemy*)pObj)->GetIsRolling())
		{
			((CFireEnemy*)pObj)->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
		}
	}
}

void CLevel::PushEnemyIntoActiveVector(CEnemy* pEnemy)
{
	if(pEnemy->GetType() == ICE_OBJECT || pEnemy->GetType() == FIRE_OBJECT)
	{
		m_pvActiveBeasts.push_back(pEnemy);
	}
	m_pvActiveEnemies.push_back(pEnemy);
}

void CLevel::StopSpawners()
{
	for(unsigned int i = 0; i < m_pvEnviromentObjects.size(); i++)
	{
		if(m_pvEnviromentObjects[i]->GetType() == SPAWNER_OBJECT)
		{
			((CSpawner*)(m_pvEnviromentObjects[i]))->m_bIsSpawningEnemies = false;
			((CSpawner*)(m_pvEnviromentObjects[i]))->m_fCurrentDelay = 0.0f;
			((CSpawner*)(m_pvEnviromentObjects[i]))->m_cEmit->SetActive(false);
			//((CSpawner*)(m_pvEnviromentObjects[i]))->m_cEmit->ResetParticles();
		}
	}
}

void CLevel::ActivateSpawners()
{
	for(unsigned int i = 0; i < m_pvEnviromentObjects.size(); i++)
	{
		if(m_pvEnviromentObjects[i]->GetType() == SPAWNER_OBJECT)
		{
			((CSpawner*)(m_pvEnviromentObjects[i]))->m_bIsSpawningEnemies = true;
		}
	}
}

int CLevel::PopEnemies(float fPopCooldown)
{
	if(m_fPoppingEnemiesTimer < 0)
	{
		m_fPoppingEnemiesTimer = 0.0f;
		m_fPoppingEnemiesCooldown = fPopCooldown;
		return (int)m_pvActiveEnemies.size();
	}
	return -1;
}
