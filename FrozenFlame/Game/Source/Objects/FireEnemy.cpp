/***********************************************
* Filename:  		FireEnemy.cpp
* Date:      		10/30/2012
* Mod. Date: 		10/30/2012
* Mod. Initials:	CM
* Author:    		Charles Meade
* Purpose:   		Function definitions for the fire enemy object
************************************************/

#include "FireEnemy.h"
#include "../Renderer/View.h"
#include "../AI/Header/CFSM.h"
#include "../CGame.h"
#include "../AI/Header/HomingBehavior.h"
#include "../Core/CWorldManager.h"
#include "../Achievements/Header/CAchievementManager.h"
#include "../Renderer/CEmitterManager.h"
#include "../Renderer/RenderInstanceAnimated.h"

/*****************************************************************
*	CFireEnemy():		Default constructor for the fire enemy object
*						
*
* Ins:					void
*
* Outs:					void
*
* Returns:				n/a
*
* Mod. Date:		    10/30/2012
* Mod. Initials:	    CM
*****************************************************************/
CFireEnemy::CFireEnemy(void)
{
	SetType( FIRE_OBJECT );
	SetColor( vec3f( 1.0f, 0.6f, 0.29f ) );
	/***************************************/
	m_bIsRolling = false;

	for( int i = 0; i < 3; i++ ){
		m_cEmit[i] = nullptr;
	}

	m_nLightID = -1;
	m_pcCube = nullptr;
	m_pcDest = nullptr;
}

/*****************************************************************
*	~CFireEnemy():		Default destructor for the fire enemy object
*						
*
* Ins:					void
*
* Outs:					void
*
* Returns:				n/a
*
* Mod. Date:		    10/30/2012
* Mod. Initials:	    CM
*****************************************************************/
CFireEnemy::~CFireEnemy(void)
{
	RemoveEmitters();
}

/*****************************************************************
*	Initialize():		The default funciton that sets all starting values for
*						this instance of the class
*
* Ins:					void
*
* Outs:					void
*
* Returns:				void
*
* Mod. Date:		    10/30/2012
* Mod. Initials:	    CM
*****************************************************************/
void CFireEnemy::Initialize(void)
{
	CEnemy::Initialize();

	m_bIsRolling = false;
	m_bIsRollUp = false;
	m_bPlayRollUpSound = true;

	m_vOldPos = 0.0f;
	m_fStuckTimer = 0.0f;
	m_fWarmUpTimer = 0.0f;
	m_bWarmingUp = false;
	m_nBaseAttackDamage = -1;

	if(GetRenderID() == -1){
		SetRenderID(CView::Push(this, "FireBeast", RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	}
	if(!m_pcCube)
		m_pcCube = new CSceneryObject();

	if(!m_pcDest)
		m_pcDest = new CSceneryObject();

	m_pcCube->Initialize();
	m_pcDest->Initialize();

	m_nCubeEmitID = CView::PushAttachment(GetRenderID(), "AttackCube", "joint5");
	m_pcCube->SetRenderID(CView::Push(m_pcCube, "AttackCube", RENDER_INVISIBLE));
	m_pcDest->SetRenderID(CView::Push(m_pcDest, "AttackCube", RENDER_INVISIBLE));

	const CRenderInstance* pRenderInst = CView::GetRenderInstance(GetRenderID());

	if(pRenderInst->GetType() == IRenderThing::ANIMATED)
	{
		CView::SetAttachmentEnabled(m_nCubeEmitID, false);
		matrix4f destMat = *m_pcDest->GetWorldMatrix();
		destMat = *this->GetWorldMatrix();
		destMat.axis_pos.y += 10.0f;
		m_pcDest->SetWorldMatrix(destMat);
	}
	
	for(int i = 0; i < 3; i++){		
		m_cEmit[i] = nullptr;		
	}
	

	if(!m_cEmit[0]){
		m_cEmit[0] = CView::GetEmitterManager().AddEmitter(true, true, GetPos(), "Resource/Particles/Fire1.txt", false, m_pcCube, m_pcDest);
		m_cEmit[0]->ActivateDestination(true);
		m_cEmit[0]->SetDestBlend(D3DBLEND_DESTCOLOR);
		m_cEmit[0]->SetSourceBlend(D3DBLEND_DESTALPHA);
	}
	if(!m_cEmit[1]){
		m_cEmit[1] = CView::GetEmitterManager().AddEmitter(true, true, GetPos(), "Resource/Particles/Fire2.txt", false, m_pcCube, m_pcDest);
		m_cEmit[1]->ActivateDestination(true);
		m_cEmit[1]->SetDestBlend(D3DBLEND_DESTCOLOR);
		m_cEmit[1]->SetSourceBlend(D3DBLEND_SRCALPHA);
	}
	if(!m_cEmit[2]){
		m_cEmit[2] = CView::GetEmitterManager().AddEmitter(true, true, GetPos(), "Resource/Particles/RageFireTrail.txt");
		m_cEmit[2]->SetDestBlend(D3DBLEND_DESTCOLOR);
		m_cEmit[2]->SetSourceBlend(D3DBLEND_SRCALPHA);
	}
	
	CFSM *_pcBehavior = new CFSM();
	_pcBehavior->SetAgent(this);
	_pcBehavior->AddState((CBaseBehavior*)(new CHomingBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CWanderBehavior()), true);
	_pcBehavior->AddState((CBaseBehavior*)(new CMeleeBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CAlignBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CRollBehavior()));

	SetFSM(_pcBehavior);

	
	// New light system - Dan
	if(m_nLightID == -1){
		CView::MakeLight(GetRenderID(), true, vec3f(0, 4, 0), vec4f(0.7f, 0.5f, 0.2f, 1.0f), 25.0f);
	}

	m_nPrevAttackedByElement = NEUTRAL;
	m_nNumContinuousSameElementAttacks = 0;


	LoadLuaValues();

	SetMoveSpeed(6.5);
	SetAttackSpeed(1.5);
	SetAttackElement(FIRE);
	m_fOldSpeed = GetMoveSpeed();
	// initialize which state/size the beast starts
	switch(CGame::GetInstance()->GetDifficultyLevel()){
	case DIFF_EASY:{
			SetElementalState(NEUTRAL);
		}
		break;
	case DIFF_MEDIUM:{
			SetElementalState(HOT);
		}
		break;
	case DIFF_HARD:{
			SetElementalState(BURNING);
			// set these a little higer on hard mode
			m_fOldSpeed = 10;
			SetAttackSpeed(0.75f);
		}
		break;
	}

	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));

	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "FireBeast/FireBeast_Punch.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "FireBeast/FireBeast_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "FireBeast/FireBeast_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "FireBeast/FireBeast_Damaged.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "FireBeast/FireBeast_Movement.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_A, "FireBeast/FireBeast_Roll.anm"); // Setting up to roll
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_B, "FireBeast/FireBeast_Roll2.anm"); // Rolling

	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.scale_pre(GetScale().x * 1.5f, GetScale().y * 1.5f, GetScale().z * 1.5f);
	SetWorldMatrix(worldMatrix);

	// Create the Health Display
	m_nShapeID = CView::DynamicShapeCreate(GetRenderID(), false, vec3f(1.5f, 1.0f, 1.5f));
	CView::DynamicShapeMakePlane(m_nShapeID, 3.0f, 3.0f);
	matrix4f position;

	position.make_identity();
	position.rotate_y_post(3.0f * D3DX_PI / 4.0f);
	position.translate_post(0.0f, 1.0f, 0.0f);

	CView::DynamicShapeSetLocalMatrix(m_nShapeID, position);
	CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_firemax.png"), -1);

	//for(int x = 0; x < 100000; ++x)
	//{
	//
	//}
	//
	//float fOffset = float( rand()%51 / 100.0f);
	//this->SetTimerOffset(fOffset);

}

/*****************************************************************
*	Reinitialize():		The default funciton that resets all starting values for
*						this instance of the class
*
* Ins:					void
*
* Outs:					void
*
* Returns:				void
*
* Mod. Date:		    10/30/2012
* Mod. Initials:	    CM
*****************************************************************/
void CFireEnemy::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

/*****************************************************************
*	Update():			Updates all data for the class based off of time
*
* Ins:					fElapsedTime
*
* Outs:					void
*
* Returns:				void
*
* Mod. Date:		    10/30/2012
* Mod. Initials:	    CM
*****************************************************************/
void CFireEnemy::Update(float fElapsedTime)
{
	CEnemy::Update(fElapsedTime);
	if(!GetInitialized())
		return;

	//mat4f mEmitMat = *m_cEmit[0]->GetWorldMatrix();
	//mat4f mBeastMat = *this->GetWorldMatrix();
	//mBeastMat.axis_pos.y += 2.0f;
	//
	//mEmitMat = mBeastMat * mEmitMat;
	//m_cEmit[0]->SetWorldMatrix(mEmitMat);
	
	//const vec3f emitterVec = pRenderInst->GetPosition();
	// Up vector of Fire Beast
	//vec3f emitterY(emitterMat->_21,emitterMat->_22,emitterMat->_23);
	//vec3f emitterZ(emitterMat->_31,emitterMat->_32,emitterMat->_33);
	//
	//vec3f emitterPos = vec3f(emitterMat->_41,emitterMat->_42 - (2.0f * this->GetWorldMatrix()->yy),emitterMat->_43);
	//vec3f emitterVec = vec3f(emitterVec.x,emitterVec.y,emitterVec.z);

	// Deposit particles along Fire Beast
	float modifier = float((rand() % 1000) * 0.001f);
	//for(int i = 0; i < 2; ++i)
	//{
	//	matrix4f mEmitter = *m_cEmit[i]->GetWorldMatrix();
	//	emitterY *= 2.0f * modifier * this->GetWorldMatrix()->yy;
	//		for(int x = 0; x < 4; ++x)
	//		{
	//			for(int y = 0; y < 4; ++y)
	//			{
	//				mEmitter.m[x][y] = emitterMat->m[x][y];
	//			}
	//		}
	//	m_cEmit[i]->SetWorldMatrix(mEmitter);
	//}
	//emitterPos -= emitterY;
	//emitterPos -= emitterZ;
	//vec3f offset = emitterPos - GetPos();

	//vec3f offset = m_cEmit[0]->GetWorldMatrix()->axis_pos - m_pcCube->GetPos();
	//m_cEmit[0]->SetOffset(offset);
	//m_cEmit[1]->SetOffset(offset);
	//m_cEmit[0]->
	//m_cEmit[0]->SetPos(emitterVec);

	if(GetHealth() > 0){
		if(!m_bIsRolling && !m_bWarmingUp){
			SetMoveSpeed(m_fOldSpeed);
		}

		else if(m_bWarmingUp && !m_bIsRolling){
			SetMoveSpeed(0.0f);
			m_fWarmUpTimer += fElapsedTime;
			
			if(m_fWarmUpTimer >= 1.2f)/*  * (GetTimerOffset() + 1.0f))*/{
				m_fWarmUpTimer = 0.0f;
				m_bWarmingUp = false;
				m_bIsRolling = true;
				SetMoveSpeed(m_fOldSpeed * 5.25f);
			}
		}

		switch (GetElementalState()){
		case BURNING:{
				if (!GetIsDebuffSet()){
					SetHealth(GetMaxHealth());
					matrix4f worldMatrix = *GetWorldMatrix();
					worldMatrix.scale_post(1.25f, 1.25f, 1.25f);
					SetWorldMatrix(worldMatrix);
					SetIsDebuffSet(true);
					CAchievementManager::GetInstance()->ActivateAchievement("Well, that Escalated Quickly");
				}
			}
			break;
		case HOT:{
				if (!GetIsDebuffSet()){
					SetHealth(GetMaxHealth());
					matrix4f worldMatrix = *GetWorldMatrix();
					worldMatrix.scale_post(1.25f, 1.25f, 1.25f);
					SetWorldMatrix(worldMatrix);
					SetIsDebuffSet(true);
				}
			}
			break;
		case COLD:{
				if (!GetIsDebuffSet()){
					if (GetMoveSpeed() >= 1.5f){
						SetMoveSpeed(GetMoveSpeed() / 2);
					}
					SetIsDebuffSet(true);
				}
			}
			break;
		}

		if (GetInitialized()){

			const CRenderInstance* pRenderInst = CView::GetRenderInstance(GetRenderID());

			if(pRenderInst->GetType() == IRenderThing::ANIMATED)
			{
				const CRenderInstance* pAttach = CView::GetAttachmentInstance(-1, m_nCubeEmitID);
				const D3DXMATRIX* emitterMat = &pAttach->GetWorldTransform();

				matrix4f mEmitter;
				for(int x = 0; x < 4; ++x)
				{
					for(int y = 0; y < 4; ++y)
					{
						mEmitter.m[x][y] = emitterMat->m[x][y];
					}
				}
				m_pcCube->SetWorldMatrix(mEmitter);
			}
			matrix4f destMat = *m_pcDest->GetWorldMatrix();
			destMat = *this->GetWorldMatrix();
			destMat.axis_pos.y += 10.0f;
			m_pcDest->SetWorldMatrix(destMat);

			vec3f tempPos = GetPos();

			if (GetElementalState() <= COLD){
				for(int i = 0; i < 3; ++i){
					m_cEmit[i]->SetActive(false);
				}
			}
			else{
				for(int i = 0; i < 3; ++i){
					if(m_cEmit[i]->GetActive() == false){
						m_cEmit[i]->SetActive(true);
					}
				}
			}
			for(int i = 0; i < 3; i++){
				switch(i){
				case 0:{
					m_cEmit[i]->SetEmittPos(GetPos().x - 0.5f,GetPos().y + (3.5f * (GetElementalState() + 1)),GetPos().z);
					}
					break;

				case 1:{
						m_cEmit[i]->SetEmittPos(GetPos().x + 0.5f,GetPos().y + (3.5f * (GetElementalState() + 1)),GetPos().z);
					}
					break;
				}
			}
		}
	}
	else{
		
		m_bIsRolling = false;
		m_bIsRollUp = false;

		if(GetAnimationPlayer())
		GetAnimationPlayer()->SetAnimationSpeed(1.0f);

		CView::DynamicShapeRemove(m_nShapeID);
		m_nShapeID = -1;
	}
}

void CFireEnemy::OnAttack(int nDamage,int nElementType)
{
	if (GetIsDying()){
		m_bIsRolling = false;
		m_bIsRollUp = false;

		if(GetAnimationPlayer())
		GetAnimationPlayer()->SetAnimationSpeed(1.0f);

		return;
	}

	// I wrote this line of code, and I'm proud of it - Dan
	CView::FlashRenderInstance(GetRenderID(), nElementType == FIRE ? vec4f(1, 0.25f, 0, 1) : nElementType == ICE ? vec4f(0, 0.8f, 1, 1) : vec4f(1, 1, 1, 1));

	// set his attack damage to scale up as he sizes up
	if (GetElementalState() <= NEUTRAL){
		SetDamage(m_nBaseAttackDamage);
	}
	else if (GetElementalState() == HOT){
		SetDamage((int)(m_nBaseAttackDamage * 1.5f));
	}
	else{ // if state == BURNING	
		SetDamage((int)(m_nBaseAttackDamage * 2.0f));
	}

	if (GetAttackedTimer() >= DAMAGE_COOLDOWN){
		//keep track of the number of times we have been attacked by the same element
		if(m_nPrevAttackedByElement == nElementType){
			++m_nNumContinuousSameElementAttacks;
		}
		else{
			// Set to 1 because we have now been attacked by the new element
			m_nNumContinuousSameElementAttacks = 1;
		}

		//Store what previos attack element was
		m_nPrevAttackedByElement = nElementType;

		if (nElementType <= 0){
			if (GetElementalState() < NEUTRAL)
				SetHealth(GetHealth() - nDamage);
		}

		SetPreviousElement(GetElementalState());
		switch(GetElementalState()){
		case BURNING:{
				if(nElementType <= COLD){
					if(m_nBurningToHotHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(HOT);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		case HOT:{
				if(nElementType >= HOT){
					if(m_nHotToBurningHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(BURNING);				
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
				else if(nElementType <= COLD){
					if(m_nHotToNeutralHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(NEUTRAL);						
						// New light system - Dan
						CView::SetLightEnabled(m_nLightID, false);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		case NEUTRAL:{
				if(nElementType >= HOT){
					if(m_nNeutralToHotHitCount <= m_nNumContinuousSameElementAttacks){
						CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_firemax.png"), -1);
						SetElementalState(HOT);					
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
				else if(nElementType <= COLD){
					if(m_nNeutralToColdHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(COLD);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;

		case COLD:{
				if(nElementType >= HOT){
					if(m_nColdToNeutralHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(NEUTRAL);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		case FROZEN:
			SetElementalState(COLD);
			break;
		}

		if ((GetPreviousElement() == BURNING && GetElementalState() == HOT) || (GetPreviousElement() == HOT && GetElementalState() == NEUTRAL)){
			matrix4f worldMatrix = *GetWorldMatrix();
			worldMatrix.scale_post(0.8f, 0.8f, 0.8f);
			SetWorldMatrix(worldMatrix);
		}
		if (GetPreviousElement() == HOT && GetElementalState() == NEUTRAL){
			// when the fire golem reaches the neutral state, set his health to about 5 hits from the player
			CPlayerObject* player = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer();
		}
		if ((GetPreviousElement() == HOT && GetElementalState() == BURNING) || (GetPreviousElement() == NEUTRAL && GetElementalState() == HOT)){
			SetIsDebuffSet(false);
			
			// New light system - Dan
			CView::SetLightEnabled(m_nLightID, true);
		}
		if (GetPreviousElement() > COLD && GetElementalState() <= COLD){
			SetMoveSpeed(GetMoveSpeed() / 2);
			m_fOldSpeed /= 2.0f;
		}
		else if (GetPreviousElement() == COLD && GetElementalState() > COLD){
			SetMoveSpeed(GetMoveSpeed() * 2);
			m_fOldSpeed *= 2.0f;
		}
		SetAttackedTimer(0.0);
	//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_2D_MenuCancel");

		if( NEUTRAL >= nElementType){
			if(GetHealth() > GetMaxHealth() * 0.75f){
				CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_firemax.png"), -1);
			}
			else if(GetHealth() > GetMaxHealth() * 0.5f){
				CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_3.png"), -1);
			}
			else if(GetHealth() > GetMaxHealth() * 0.25f){
				CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_2.png"), -1);
			}
			else if(GetHealth() > 0.0f){
				CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_1.png"), -1);
			}
			else{
				CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_0.png"), -1);
			}
		}

	}
}

void CFireEnemy::ResetParticles(void)
{
	for(int i = 0; i < 3; i++)
	{
		if(m_cEmit[i])
		{
			switch(i)
			{
			case 0:
				{
					m_cEmit[i]->SetEmittPos(GetPos().x - 0.5f,GetPos().y,GetPos().z);
				}
				break;

			case 1:
				{
					m_cEmit[i]->SetEmittPos(GetPos().x + 0.5f,GetPos().y,GetPos().z);
				}
				break;
			}
		}
	}
}

void CFireEnemy::RemoveEmitters(void)
{
	for(int i = 0; i < 3; i++)
	{
		CView::GetEmitterManager().KillEmitter(m_cEmit[i]);
		m_cEmit[i] = nullptr;
	}
}

void CFireEnemy::CollisionReaction(CCollidableObject* pObj)
{
	if(pObj && pObj->GetInitialized()){
		vec3f vecToBeast = pObj->GetPos() - this->GetPos();
		vec3f beastFacing = this->GetWorldMatrix()->axis_z;
		vecToBeast.normalize();
		beastFacing.normalize();

		//std::cout << "Dot Product Output From Roll Check ";
		float dProd;
		dProd = dot_product(vecToBeast, beastFacing);

		//std::cout << dProd << std::endl;

		if(dProd <= 1.0f && dProd >= 0.65f){
			SetMoveSpeed(0.0f);
			this->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
		}
		else if(dProd >= 0.0f && dProd < 0.40f){
			SetMoveSpeed(0.0f);
			this->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
		}
		else if(dProd >= -1.0f && dProd < -0.55f){
			SetMoveSpeed(0.0f);
			this->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
		}
		else if(dProd <= 0.0f && dProd > -0.25f){
			SetMoveSpeed(0.0f);
			this->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
		}
		else{
			SetMoveSpeed(GetMoveSpeed()/2.0f);
		}
	}
}

void CFireEnemy::LoadLuaValues()
{
	// Read in lua variables
	// Set Max Health
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBHealthPoints");
	vector<int> vHealth;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vHealth);

	SetMaxHealth((float)vHealth[CGame::GetInstance()->GetDifficultyLevel()]);
	SetHealth(GetMaxHealth());

	// Set the Damage
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBDamage");
	vector<int> vDamage;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vDamage);
	m_nBaseAttackDamage = vDamage[CGame::GetInstance()->GetDifficultyLevel()];

	// Set the Roll Damage
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBRollDamage");
	vector<int> vRollDamage;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vRollDamage);

	m_nRollDamage = vDamage[CGame::GetInstance()->GetDifficultyLevel()];

	// Set the drop variables
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBAmmountToDrop");
	vector<int> vDropAmount;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vDropAmount);
	SetAmountToDrop(vDropAmount[CGame::GetInstance()->GetDifficultyLevel()]);

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBPercnetageHealthChance");
	vector<int> vHealthChance;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vHealthChance);
	SetHealthChance(vHealthChance[CGame::GetInstance()->GetDifficultyLevel()]);
	
	// Set the number of hits it takes to change for each of the state changes
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBColdToNeutralHit");
	m_nColdToNeutralHitCount = static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBNeutralToColdHit");
	m_nNeutralToColdHitCount = static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBNeutralToHotHit");
	m_nNeutralToHotHitCount = static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBHotToNeutralHit");
	m_nHotToNeutralHitCount = static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBHotToBurningHit");
	m_nHotToBurningHitCount = static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "FBBurningToHotHit");
	m_nBurningToHotHitCount = static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));

	// pop all the items on the stack
	lua_pop(CWorldManager::GetInstance()->GetluaState(), lua_gettop(CWorldManager::GetInstance()->GetluaState()));
}

void CFireEnemy::Uninitialize(void)
{
	if(m_nLightID != -1){
		CView::DestroyLight(m_nLightID);
		m_nLightID = -1;
	}

	RemoveEmitters();

	if(GetRenderID() != -1){
		CView::DynamicShapeRemove(m_nShapeID);
		m_nShapeID = -1;
	}

	if(m_pcCube)
	{
		m_pcCube->Uninitialize();
		delete m_pcCube;
		m_pcCube = nullptr;
	}

	if(m_pcDest)
	{
		m_pcDest->Uninitialize();
		delete m_pcDest;
		m_pcDest = nullptr;
	}

	CEnemy::Uninitialize();
}
