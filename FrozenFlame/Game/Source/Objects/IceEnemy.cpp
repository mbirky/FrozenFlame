/***********************************************
* Filename:  		IceEnemy.cpp
* Date:      		10/30/2012
* Mod. Date: 		10/30/2012
* Mod. Initials:	CM
* Author:    		Charles Meade
* Purpose:   		Function definitions for the ice enemy object
************************************************/

#include "IceEnemy.h"
#include "../Renderer/View.h"
#include "../AI/Header/CFSM.h"
#include "../CGame.h"
#include "../AI/Header/HomingBehavior.h"
#include "../Core/CWorldManager.h"
#include "../Renderer/CEmitterManager.h"

CIceEnemy::CIceEnemy(void)
	: m_pSmokeEmitter(nullptr), m_pIceEmitter(nullptr)
{
	SetType(ICE_OBJECT);
	m_nLightID = -1;
	m_bIceBreathEnabled = false;
}

CIceEnemy::~CIceEnemy(void)
{
	// Clean up emitters
	if(m_pSmokeEmitter){
		CView::GetEmitterManager().KillEmitter(m_pSmokeEmitter);
	}
	m_pSmokeEmitter = nullptr;

	if(m_pIceEmitter){
		CView::GetEmitterManager().KillEmitter(m_pIceEmitter);
	}
	m_pIceEmitter = nullptr;
}

void CIceEnemy::Initialize(void)
{
	CEnemy::Initialize();

	CFSM *_pcBehavior = new CFSM();
	_pcBehavior->SetAgent(this);
	_pcBehavior->AddState((CBaseBehavior*)(new CHomingBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CWanderBehavior()), true);
	_pcBehavior->AddState((CBaseBehavior*)(new CMeleeBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CSnowballBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CBreathBehavior()));

	SetFSM(_pcBehavior);

	SetMoveSpeed(6.5);
	SetAttackElement(ICE);
	SetAttackSpeed(1.5f);
	m_nBaseAttackDamage = -1;

	m_pIceEmitter = m_pSmokeEmitter = nullptr;

	m_nPrevAttackedByElement = NEUTRAL;
	m_nNumContinuousSameElementAttacks = 0;

	switch(CGame::GetInstance()->GetDifficultyLevel()){
	case DIFF_EASY:{
			SetElementalState(NEUTRAL);
		}
		break;
	case DIFF_MEDIUM:{
			SetElementalState(COLD);
		}
		break;
	case DIFF_HARD:{
			SetElementalState(FROZEN);
			// set these a little higer on hard mode
			SetMoveSpeed(7.5);
			SetAttackSpeed(2.5f);
		}
		break;
	}

	LoadLuaValues();

	// Load the model and push it to the View
	if(GetRenderID() == -1){
		SetRenderID(CView::Push(this, "IceBeast", RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
		m_nIceBreathAttID = CView::PushAttachment(GetRenderID(), "IceBreathNew", "Jaw_End");
		CView::SetAttachmentEnabled(m_nIceBreathAttID, false);
	}

	// Load all the animations
	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));

	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "IceBeast/IceBeast_Punch.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "IceBeast/IceBeast_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "IceBeast/IceBeast_Movement.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "IceBeast/IceBeast_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "IceBeast/IceBeast_Damaged.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_A, "IceBeast/IceBeast_Throw.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_B, "IceBeast/IceBeast_Breath.anm");

	// If we don't have an ice emitter, load it in
	if(!m_pIceEmitter){
		m_pIceEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/IceMist.txt");
	}
	// If we don't have a smoke emitter, load it in
	if(!m_pSmokeEmitter){
		m_pSmokeEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/ReverseSmoke.txt");
	}

	// New light system - Dan
	if(m_nLightID == -1){
		m_nLightID = CView::MakeLight(GetRenderID(), true, vec3f(0, 4, 0), vec4f(0.0f, 0.0f, 1.0f, 1.0f), 30.0f);
	}

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
	CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_frozenmax.png"), -1);

	//for(int x = 0; x < 10000; ++x)
	//{
	//
	//}

	//float fOffset = float( rand()%51 / 100.0f);
	//this->SetTimerOffset(fOffset);
	SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
}

void CIceEnemy::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CIceEnemy::Update(float fElapsedTime)
{
	if(!GetInitialized()){
		return;
	}

	// Call the parent's update first
	CEnemy::Update(fElapsedTime);
	if(!GetInitialized())
		return;

	// If we're dead remove the dynamic shape
	if(GetHealth() <= 0){
		CView::DynamicShapeRemove(m_nShapeID);
		m_nShapeID = -1;
		SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
		return;
	}

	// Set his attack damage to scale up as he sizes up
	if(GetElementalState() >= NEUTRAL){
		SetDamage(m_nBaseAttackDamage);
	}
	else if(GetElementalState() == COLD){
		SetDamage((int)(m_nBaseAttackDamage * 1.5f));
	}
	else if(GetElementalState() == FROZEN){
		SetDamage((int)(m_nBaseAttackDamage * 2.0f));
	}

	switch (GetElementalState()){
	case FROZEN:{
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
				SetHealth(GetMaxHealth());
				matrix4f worldMatrix = *GetWorldMatrix();
				worldMatrix.scale_post(1.25f, 1.25f, 1.25f);
				SetWorldMatrix(worldMatrix);
				SetIsDebuffSet(true);
			}
		}
		break;
	case HOT:{
			if (!GetIsDebuffSet()){
				if (GetMoveSpeed() >= 1.5f){
					SetMoveSpeed(GetMoveSpeed() / 2);
				}
				SetIsDebuffSet(true);
			}
		}
		break;
	}

	// Update the emitters
	if(GetElementalState() >= HOT){
		m_pIceEmitter->SetActive(false);

		if(!m_pSmokeEmitter->GetActive()){
			m_pSmokeEmitter->SetActive(true);
		}
	}
	else if(!m_pIceEmitter->GetActive()){
		m_pSmokeEmitter->SetActive(false);
		m_pIceEmitter->SetActive(true);
	}

	// Update the emitter's position
	if(m_pIceEmitter && m_pIceEmitter->GetActive()){
		m_pIceEmitter->SetEmittPos(GetPos().x, GetPos().y + 2.5f * (-GetElementalState() + 1), GetPos().z);
	}
	if(m_pSmokeEmitter && m_pSmokeEmitter->GetActive()){
		m_pSmokeEmitter->SetEmittPos(GetPos().x, GetPos().y + 2.5f * (GetElementalState() + 1), GetPos().z);
	}
}

void CIceEnemy::UpdateAnimations(float fElapsedTime)
{
	CAnimPlayer* pAnim = GetAnimationPlayer();
	if(!pAnim) return;

	// Check if we should play death animation
	if(GetHealth() <= 0){
		// If the last played animation was a death animation, uninitialize
		if(pAnim->GetLastPlayedAnimType() == ANIM_DEATH){
			pAnim->SetAnimationSpeed(1.0f);
			SpawnPickups();
			Uninitialize();
		}
		// if we're not dying, die
		else if(!pAnim->IsPlaying(ANIM_DEATH)){
			SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			pAnim->PlayAnimation(ANIM_DEATH);
			SetIsDying(true);
		}
	}
	
	if(!pAnim->IsPlaying()){
		if(GetIsMoving() && !GetIsAttacking()){
			pAnim->PlayAnimation(ANIM_WALK, true);
		}
		else if(GetIsAttacking() && !GetIsMoving()){
			pAnim->PlayAnimation(ANIM_ATTACK, false);
		}
		else{
			pAnim->PlayAnimation(ANIM_IDLE, true);
		}
	}
}

void CIceEnemy::OnAttack(int nDamage,int nElementType)
{
	// Return if already dead
	if (GetIsDying()){
		return;
	}

	// I wrote this line of code, and I'm proud of it - Dan
	CView::FlashRenderInstance(GetRenderID(), nElementType == FIRE ? vec4f(1, 0.25f, 0, 1) : nElementType == ICE ? vec4f(0, 0.8f, 1, 1) : vec4f(1, 1, 1, 1));
	
	// If Ice beast can take damage
	if (GetAttackedTimer() >= DAMAGE_COOLDOWN){	//keep track of the number of times we have been attacked by the same element	
		if(m_nPrevAttackedByElement == nElementType){
			++m_nNumContinuousSameElementAttacks;
		}		
		else{	// Set to 1 because we have now been attacked by the new element
			m_nNumContinuousSameElementAttacks = 1;
		}
		//Store what previos attack element was
		m_nPrevAttackedByElement = nElementType;

		// As long as it is not an ice attack do damage
		if (nElementType >= 0){
			if (GetElementalState() > NEUTRAL)
				SetHealth(GetHealth() - nDamage);
		}
		// store the previos elemental state
		SetPreviousElement(GetElementalState());

		// Switch to know what to do based on what elemental state Ice beast currently in
		// Reset the number of continuous attacks everytime a state is changed
		switch(GetElementalState()){
		case BURNING:
			SetElementalState(HOT);
			break;
		case HOT:{	// If Ice Attack and attacked enought times go to neutral state			
				if(nElementType <= COLD){
					if(m_nHotToNeutralHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(NEUTRAL);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		case NEUTRAL:{	// Enough continues HOT attacks will lead to beast going to hot state			
				if(nElementType >= HOT){
					if(m_nNeutralToHotHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(HOT);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}			
				else if(nElementType <= COLD){	// Too many COLD attacks leads to going to COLD state
					if(m_nNeutralToColdHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(COLD);
						CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_frozenmax.png"), -1);
						CView::SetLightEnabled(m_nLightID, true);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		case COLD:{	// Enough continuous HOT attacks will lead to beast going to Neutral state
				if(nElementType >= HOT){
					if(m_nColdToNeutralHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(NEUTRAL);
						m_nNumContinuousSameElementAttacks = 0;
						CView::SetLightEnabled(m_nLightID, false);
					}
				}			
				else if(nElementType <= COLD){	// Enought cold hits will make the beast go to the frozen state
					if(m_nColdToFrozenHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(FROZEN);
						CView::SetLightEnabled(m_nLightID, true);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		case FROZEN:{	// Hit the beast with HOT enough times and it will go to the COLD state
				if(nElementType == HOT){
					if(m_nFrozenToColdHitCount <= m_nNumContinuousSameElementAttacks){
						SetElementalState(COLD);
						m_nNumContinuousSameElementAttacks = 0;
					}
				}
			}
			break;
		}

		if ((GetPreviousElement() == FROZEN && GetElementalState() == COLD) || (GetPreviousElement() == COLD && GetElementalState() == NEUTRAL)){
			matrix4f worldMatrix = *GetWorldMatrix();
			worldMatrix.scale_post(0.8f, 0.8f, 0.8f);
			SetWorldMatrix(worldMatrix);
		}
		if (GetPreviousElement() == COLD && GetElementalState() == NEUTRAL){	// when the ice golem reaches the neutral state, set his health to about 5 hits from the player
			CPlayerObject* player = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer();
		}
		if ((GetPreviousElement() == COLD && GetElementalState() == FROZEN) || (GetPreviousElement() == NEUTRAL && GetElementalState() == COLD)){
			SetIsDebuffSet(false);
		}
		SetAttackedTimer(0.0);
	//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"FX_2D_MenuCancel");

		if(NEUTRAL <= nElementType)	{
			if(GetHealth() > GetMaxHealth() * 0.75f){
				CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_frozenmax.png"), -1);
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

void CIceEnemy::LoadLuaValues()
{
	// Read in lua file variables
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBHealthPoints");
	vector<int> vHealth;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vHealth);
	
	SetMaxHealth((float)vHealth[CGame::GetInstance()->GetDifficultyLevel()]);
	SetHealth(GetMaxHealth());

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBDamage");
	vector<int> vDamage;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vDamage);
	m_nBaseAttackDamage = vDamage[CGame::GetInstance()->GetDifficultyLevel()];
	
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBAmmountToDrop");
	vector<int> vDropAmount;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vDropAmount);
	SetAmountToDrop(vDropAmount[CGame::GetInstance()->GetDifficultyLevel()]);

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBPercnetageHealthChance");
	vector<int> vHealthChance;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vHealthChance);
	SetHealthChance(vHealthChance[CGame::GetInstance()->GetDifficultyLevel()]);
	
	// The state change date
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBHotToNeutral");
	m_nHotToNeutralHitCount		= static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBNeutralToHot");
	m_nNeutralToHotHitCount		= static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBNeutralToCold");
	m_nNeutralToColdHitCount	= static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBColdToNeutral");
	m_nColdToNeutralHitCount	= static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBColdToFrozen");
	m_nColdToFrozenHitCount		= static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "IBFrozenToCold");
	m_nFrozenToColdHitCount		= static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1));

	// Pop the lua stack
	lua_pop(CWorldManager::GetInstance()->GetluaState(), lua_gettop(CWorldManager::GetInstance()->GetluaState()));
}

void CIceEnemy::Uninitialize(void)
{
	// only play this sound when the golem dies and not when we exit the game or stage
	if(GetHealth() <= 0)
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_ICEGOLEM_DEATH, GetSoundID());

	if(m_nLightID != -1){
		CView::DestroyLight(m_nLightID);
		m_nLightID = -1;
	}
	
	if(GetRenderID() != -1){
		CView::DynamicShapeRemove(m_nShapeID);
		m_nShapeID = -1;
	}

	// Clean up emitters
	if(m_pSmokeEmitter){
		CView::GetEmitterManager().KillEmitter(m_pSmokeEmitter);
	}
	m_pSmokeEmitter = nullptr;

	if(m_pIceEmitter){
		CView::GetEmitterManager().KillEmitter(m_pIceEmitter);
	}
	m_pIceEmitter = nullptr;
	
	CEnemy::Uninitialize();
	SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
}

void CIceEnemy::PrepareToThrow(void)
{
	GetAnimationPlayer()->PlayAnimationImmediate(ANIM_SPECIAL_A, false);
	SetIsMoving(false);
	SetVelocity(vec3f(0, 0, 0));
}
void CIceEnemy::PrepareToBlow(void)
{
	GetAnimationPlayer()->PlayAnimationImmediate(ANIM_SPECIAL_B, false);
	SetIsMoving(false);
	SetVelocity(vec3f(0, 0, 0));
}
void CIceEnemy::StartRunning(void)
{
	GetAnimationPlayer()->PlayAnimationImmediate(ANIM_WALK, true);
	SetIsMoving(true);
	SetIsAttacking(false);
}

void CIceEnemy::SetIceBreathEnabled(bool bEnabled)
{
	m_bIceBreathEnabled = bEnabled;
	CView::SetAttachmentEnabled(m_nIceBreathAttID, bEnabled);
}

void CIceEnemy::Punch(void)
{
	GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK, false);
	SetIsMoving(false);
	SetIsAttacking(true);
}
