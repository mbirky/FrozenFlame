/***********************************************
 * Filename:  		MeleeEnemy.h
 * Date:      		10/29/2012
 * Mod. Date: 		11/02/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the melee enemy object
 ************************************************/

#include "MeleeEnemy.h"
#include "../Renderer/View.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RenderInstance.h"
#include "../AI/Header/CFSM.h"
#include "../AI/Header/CBehaviors.h"
#include "../Globals.h"
#include "../Core/CWorldManager.h"
#include "../AI/Header/HomingBehavior.h"
#include "../AI/Header/MeleeBehavior.h"
#include "../Renderer/CEmitterManager.h"

#define AFTER_EFFECT_TIMER 2.5f


CMeleeEnemy::CMeleeEnemy(void)
{
	SetType(MELEE_OBJECT);

	m_pcEffectEmitter1 = nullptr;
	m_pcEffectEmitter2 = nullptr;
	m_pcEffectEmitter3 = nullptr;
	m_pcAmbientColdEmitter = nullptr;
	m_pcAmbientFireEmitter = nullptr;
	m_pcIceBlock = nullptr;

	m_nLightID = -1;
}

CMeleeEnemy::~CMeleeEnemy(void)
{
	// Stop any effects from happening since we're dead - JC
	if(m_pcEffectEmitter1)
	{
		m_pcEffectEmitter1->SetActive(false);
		CView::GetEmitterManager().KillEmitter(m_pcEffectEmitter1);
		m_pcEffectEmitter1 = nullptr;

		LOG("m_pcEffectEmitter1 cleaned up!");
	}
	
	if(m_pcEffectEmitter2)
	{
		CView::GetEmitterManager().KillEmitter(m_pcEffectEmitter2, false);
		m_pcEffectEmitter2 = nullptr;

		LOG("m_pcEffectEmitter2 cleaned up!");
	}
}

void CMeleeEnemy::Initialize(void)
{
	CEnemy::Initialize();

	CFSM *_pcBehavior = new CFSM();
	_pcBehavior->SetAgent(this);
	_pcBehavior->AddState((CBaseBehavior*)(new CHomingBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CWanderBehavior()), true);
	_pcBehavior->AddState((CBaseBehavior*)(new CMeleeBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CPanicBehavior()));
	
	SetFSM(_pcBehavior);

	m_pcIceBlock = nullptr;

	if(GetRenderID() == -1)
	{
		SetRenderID(CView::Push(this, "Minion", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	}

	SetMoveSpeed(7.5);
	SetAttackSpeed(0.5);

	LoadLuaValues();
	//SetHealth(8.0f);

	SetEffectTimer(0.0f);
	SetAfterEffectActive(false);
	SetStunDuration(0.5f);

	m_pcEffectEmitter1 = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/FireModel.txt");

	m_pcIceBlock = nullptr;
	
	if(GetRenderID() != -1)
	{
		SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));
	}
	else
	{
		SetAnimPlayer(nullptr);
	}

	if (!m_pcAmbientColdEmitter)
	{
		m_pcAmbientColdEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/IceMist.txt");
	}

	if (!m_pcAmbientFireEmitter)
	{
		m_pcAmbientFireEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/Fire2.txt");
	}

	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Minion/Minion_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "Minion/Minion_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "Minion/Minion_MeleeAttack.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "Minion/Minion_Movement.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "Minion/Minion_Damaged.anm");

	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_y_pre(D3DXToRadian(180));
	worldMatrix.scale_pre(GetScale().x * 0.65f, GetScale().y * 0.65f, GetScale().z * 0.65f);
	SetWorldMatrix(worldMatrix);

	// New light system - Dan
	if(m_nLightID == -1)
		CView::MakeLight(GetRenderID(), false, vec3f(0, 0, 0), vec4f(0, 0, 1, 1), 10.0f);

	 m_bCanPlayHurtSound = true;	
	 m_bCanPlayDeathSound = true;
	 m_fHurtSoundTimer = 0.0f;	
	 m_fHurtSoundResetTime = 0.3f;

	// for(int x = 0; x < 10000; ++x)
	//{
	//
	//}

	//float fOffset = float( rand()%51 / 100.0f);
	//this->SetTimerOffset(fOffset);
}


void CMeleeEnemy::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}


void CMeleeEnemy::Update(float fElapsedTime)
{
	if(!GetInitialized())
	{
		return;
	}

	m_fHurtSoundTimer += fElapsedTime;
	if (m_fHurtSoundTimer > m_fHurtSoundResetTime)
	{	// reset ability to play hurt sound
		m_bCanPlayHurtSound = true;
		m_fHurtSoundTimer = 0.0f;
	}

	if (m_pcEffectEmitter1)
	{
		m_pcEffectEmitter1->SetEmittPos(GetPos());
	}
	if (m_pcAmbientColdEmitter)
	{
		m_pcAmbientColdEmitter->SetEmittPos(GetPos());
	}
	if (m_pcAmbientFireEmitter)
	{
		vec3f agentPos = GetPos();
		agentPos.y += 2.0f;
		m_pcAmbientFireEmitter->SetEmittPos(agentPos);
	}

	if (GetHealth() <= 0)
	{
		if (GetElementalState() == BURNING)
		{
			m_fEffectTimer += fElapsedTime;
			if (m_fEffectTimer >= AFTER_EFFECT_TIMER)
			{
				SetAfterEffectActive(false);
				vec3f agentPos = GetPos();
				CView::GetEmitterManager().AddEmitter(false, true, agentPos, "Resource/Particles/AshCloud.txt",true);
				
				m_pcEffectEmitter1->SetActive(false);

				// Get rid of the Ice Cube
				if (m_pcIceBlock)
				{
					m_pcIceBlock->Uninitialize();
					m_pcIceBlock = nullptr;
				}

				SpawnPickups();
				Uninitialize();
			}
			else
			{
				SetAfterEffectActive(true);
				SetPos(GetPos() + GetVelocity() * fElapsedTime);
				UpdateCollisionData();
			}
		}
		else if (GetElementalState() == FROZEN)
		{
			m_fEffectTimer += fElapsedTime;
			if (m_fEffectTimer >= AFTER_EFFECT_TIMER)
			{
				SetAfterEffectActive(false);
				vec3f agentPos = GetPos();
				agentPos.y += 2.0f;
				m_pcEffectEmitter2 = CView::GetEmitterManager().AddEmitter(false, true, agentPos, "Resource/Particles/IceShatter.txt", true);
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_ICEBLOCKSHATTER, m_pcIceBlock->GetSoundID());
				
				// Get rid of the Ice Cube
				if (m_pcIceBlock)
				{
					m_pcIceBlock->Uninitialize();
					m_pcIceBlock = nullptr;
				}

				SpawnPickups();
				Uninitialize();
			}
			else
			{
				SetAfterEffectActive(true);
				SetPos(GetPos() + GetVelocity() * fElapsedTime + GetKnockbackVelocity() * fElapsedTime);
				UpdateCollisionData();
			}
		}
		else
		{
			// Get rid of the Ice Cube
			if (m_pcIceBlock)
			{
				m_pcIceBlock->Uninitialize();
				m_pcIceBlock = nullptr;
			}
		}
	}
	else
	{
		if (m_pcIceBlock)
		{
			m_pcIceBlock->Uninitialize();
			m_pcIceBlock = nullptr;
		}
	}

	if (!GetAfterEffectActive())
		CEnemy::Update(fElapsedTime);

	if(!GetInitialized())
		return;

	if (m_pcIceBlock)
	{
		if (GetElementalState() == FROZEN)
		{
			m_pcIceBlock->SetPos(GetPos());
			//m_pcIceBlock->Update(fElapsedTime);
		}
	}

	switch (GetElementalState())
	{
	case FROZEN:
		{			
			SetColor(vec3f(0.0f, 0.0f, 1.0f));
			
			// New light system - Dan
			// CView::AttachLight(GetRenderID(), vec4f(0.0f, 0.0f, 1.0f, 1.0f), 10.0f);
			CView::SetLightEnabled(m_nLightID, true);
			CView::SetLightDiffuseColor(m_nLightID, vec4f(0, 0, 1, 1));

			SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			if (m_pcIceBlock == nullptr)
			{
				m_pcIceBlock = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(GetPos(), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_ICECUBE);
				m_pcIceBlock->SetOwner(this);
				m_pcIceBlock->RegisterWithSoundManager();

				// This will set the ice block flags to be fully translucent
				unsigned short curIceBlockFlags = CView::GetRenderInstance(m_pcIceBlock->GetRenderID())->GetFlags();
				CView::SetRenderInstanceFlags(m_pcIceBlock->GetRenderID(), curIceBlockFlags | RENDER_FULLY_TRANSLUCENT);
				m_pcIceBlock->SetTransparency(0.75f);
			}
			if (GetAnimationPlayer())
			{
				GetAnimationPlayer()->Stop();
			}
			if (m_pcAmbientColdEmitter)
			{
				m_pcAmbientColdEmitter->SetActive(true);
			}
		}
		break;

	case COLD:
		{
			if (!GetIsDebuffSet())
			{
				SetMoveSpeed(GetMoveSpeed() / 2);
				SetIsDebuffSet(true);
			}
			// The enemy is cold, which will cause the enemy movement to slow, and thus the associated animations
			if (GetAnimationPlayer())
			{
				this->GetAnimationPlayer()->SetAnimationSpeed(0.5f); // * this->GetTimerOffset());
			}

			SetColor(vec3f(0.2f, 0.2f, 0.5f));
			if (m_pcIceBlock)
			{
				m_pcIceBlock->SetPos(vec3f(-100.0f, -10000.0f, -1000.0f));
			}
			
			// New light system - Dan
			// CView::DisableAttachedLight(GetRenderID());
			CView::SetLightEnabled(m_nLightID, false);
			if (m_pcAmbientColdEmitter)
			{
				m_pcAmbientColdEmitter->SetActive(true);
			}
		}
		break;

	case NEUTRAL:
		{
			SetColor(vec3f(1.0f, 1.0f, 1.0f));

			// New light system - Dan
			// CView::DisableAttachedLight(GetRenderID());
			CView::SetLightEnabled(m_nLightID, false);

			if (m_pcEffectEmitter1)
				m_pcEffectEmitter1->SetActive(false);
			if (m_pcAmbientFireEmitter)
			{
				m_pcAmbientFireEmitter->SetActive(false);
			}
			if (m_pcAmbientColdEmitter)
			{
				m_pcAmbientColdEmitter->SetActive(false);
			}
		}
		break;

	case HOT:
		{
			// The enemy is hot, which will result in the enemy's movement to quicken, thus quickening the associated animations
			if (GetAnimationPlayer())
				this->GetAnimationPlayer()->SetAnimationSpeed(1.5f); // * this->GetTimerOffset());
			if (GetDamageTimer() >= DAMAGE_COOLDOWN * 3.0f)
			{
				SetHealth(GetHealth() - 1);
				SetDamageTimer(0.0);
			}

			
			SetColor(vec3f(0.5f, 0.2f, 0.2f));

			// CView::DisableAttachedLight(GetRenderID());
			CView::SetLightEnabled(m_nLightID, false);
			
			if (m_pcEffectEmitter1)
				m_pcEffectEmitter1->SetActive(false);
			if (m_pcAmbientFireEmitter)
			{
				m_pcAmbientFireEmitter->SetActive(true);
			}
		}
		break;

	case BURNING:
		{
			if (GetAnimationPlayer())
				this->GetAnimationPlayer()->SetAnimationSpeed(2.0f); // * this->GetTimerOffset());

			if (GetDamageTimer() >= DAMAGE_COOLDOWN * 3.0f)
			{
				SetHealth(GetHealth() - 2);
				SetDamageTimer(0.0);
			}
			SetColor(vec3f(1.0f, 0.0f, 0.0f));

			if (!GetIsDebuffSet())
			{
				SetMoveSpeed(GetMoveSpeed() * 2);
				SetIsDebuffSet(true);
			}

			if (GetFSM())
			{
				if (GetFSM()->GetCurrBehavior()->GetBehaviorType() != PANIC_BEHAVIOR)
					GetFSM()->TransitionTo(PANIC_BEHAVIOR);
			}

			// New light system - Dan
			// CView::AttachLight(GetRenderID(), vec4f(1.0f, 0.0f, 0.0f, 1.0f), 10.0f);
			CView::SetLightEnabled(m_nLightID, true);
			CView::SetLightDiffuseColor(m_nLightID, vec4f(1, 0, 0, 1));
			
			if (m_pcEffectEmitter1)
			{
				m_pcEffectEmitter1->SetActive(true);
				m_pcEffectEmitter1->SetEmittPos(GetPos());
			}
			if (m_pcAmbientFireEmitter)
			{
				m_pcAmbientFireEmitter->SetActive(true);
			}

		}
		break;
	}

	if (GetElementalState() != NEUTRAL && !GetAfterEffectActive())
	{
		SetElementalTimer(GetElementalTimer() + fElapsedTime);
		SetDamageTimer(GetDamageTimer() + fElapsedTime);

		if (GetElementalTimer() >= TIMER_COOLDOWN)
		{
			if (GetElementalState() > NEUTRAL)
			{
				SetElementalState(GetElementalState() - 1);
				if (GetElementalState() == NEUTRAL)
				{
					SetMoveSpeed(GetMoveSpeed() / 2);
				}
			}
			else if (GetElementalState() < NEUTRAL)
			{
				SetElementalState(GetElementalState() + 1);
				if (GetElementalState() == NEUTRAL)
				{
					SetMoveSpeed(GetMoveSpeed() * 2);
				}
			}
			SetElementalTimer(0.0);
		}
	}
	else
	{
		// If the enemy does not have any effect (i.e. burning, frozen) and is neutral, set the animation speed back to normal
		if (GetAnimationPlayer())
		{
			this->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
		}

		if (!GetAfterEffectActive())
		{
			SetIsDebuffSet(false);
			SetColor(vec3f(1.0f, 1.0f, 1.0f));
		}
	}

	// if the enemy is stunned
	UpdateStunTimer(fElapsedTime);
	if (GetIsStunned())
	{
		SetVelocity(vec3f(0, 0, 0));
		if (GetFSM() && GetFSM()->GetCurrBehavior())
		{
			if (GetFSM()->GetCurrBehavior()->GetBehaviorType() == MELEE_BEHAVIOR)
			{
				((CMeleeBehavior*)GetFSM()->GetCurrBehavior())->ResetAttackTimer();
				GetFSM()->GetCooldownTimer() = 0;
				GetFSM()->GetCooldown() = true;
				SetIsAttacking(false);
				GetAnimationPlayer()->Stop();
			}
			else
			{
				GetFSM()->GetCooldownTimer() = 0;
				GetFSM()->GetCooldown() = true;
				SetIsAttacking(false);
				GetAnimationPlayer()->Stop();
			}
		}
	}
}

void CMeleeEnemy::OnAttack(int nDamage,int nElementType)
{
	SetIsStunned(true);
	SetPreviousElement(GetElementalState());
	CCollidableObject::OnAttack(nDamage, nElementType);

	if ((GetPreviousElement() == HOT && GetElementalState() == BURNING) || (GetPreviousElement() == NEUTRAL && GetElementalState() == BURNING))
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MELEEMINION_SCREAMING, GetSoundID());
		if(GetFSM())
		{
			if (GetFSM()->GetCurrBehavior()->GetBehaviorType() != PANIC_BEHAVIOR)
				GetFSM()->TransitionTo(PANIC_BEHAVIOR);
		}
		// reaching the burning state will always kill the minion
		SetHealth(0);
	}
	else if ((GetPreviousElement() == COLD && GetElementalState() == FROZEN) || (GetPreviousElement() == NEUTRAL && GetElementalState() == FROZEN))
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_FREEZE, GetSoundID());
		if (m_pcIceBlock == nullptr)
		{
			m_pcIceBlock = (CEnvironmentObject*)CWorldManager::GetInstance()->MakeObject(GetPos(), ENVIRONMENT_OBJECT, CEnvironmentObject::ENVOBJ_ICECUBE);
			m_pcIceBlock->SetOwner(this);
			m_pcIceBlock->RegisterWithSoundManager();

			// This will set the ice block flags to be fully translucent
			unsigned short curIceBlockFlags = CView::GetRenderInstance(m_pcIceBlock->GetRenderID())->GetFlags();
			CView::SetRenderInstanceFlags(m_pcIceBlock->GetRenderID(), curIceBlockFlags | RENDER_FULLY_TRANSLUCENT);
			m_pcIceBlock->SetTransparency(0.70f);
			
			// New light system - Dan
			// CView::AttachLight(m_pcIceBlock->GetRenderID(), vec4f(0.65f, 0.65f, 0.8f, 1.0f), 7.0f);
			CView::MakeLight(m_pcIceBlock->GetRenderID(), true, vec3f(0, 0, 0), vec4f(0.65f, 0.65f, 0.8f, 1.0f), 7.0f);

			if (GetAnimationPlayer())
			{
				GetAnimationPlayer()->Stop();
			}
		}
		if (m_pcAmbientColdEmitter)
		{
			m_pcAmbientColdEmitter->SetActive(true);
		}
		SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
		// reaching the frozen state will always kill the minion
		SetHealth(0);
	}
	else
	{
		if (GetHealth() <= 0 && m_bCanPlayDeathSound) // death sound
		{
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MELEEMINION_DEATH, GetSoundID());
			m_bCanPlayDeathSound = false;
		}
		else // hurt sound
		{	// don't let the sound repeatedly play while stuck in a block of ice
			if (m_bCanPlayHurtSound && GetPreviousElement() != FROZEN)
			{
				CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MELEEMINION_HURT, GetSoundID());
				m_bCanPlayHurtSound = false;
			}
		}
	}

	
}

void CMeleeEnemy::LoadLuaValues()
{
	// Read in lua variables
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "MMHealthPoints");
	//float rndHealthValue = (float)(rand() % 11 + static_cast<float>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));
	vector<int> vRandomHealthValue;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vRandomHealthValue);
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		{
			SetMaxHealth((float)(rand() % 11 + vRandomHealthValue[DIFF_EASY]));
		}
		break;
	case DIFF_MEDIUM:
		{
			SetMaxHealth((float)(rand() % 11 + vRandomHealthValue[DIFF_MEDIUM]));
		}
		break;
	case DIFF_HARD:
		{
			SetMaxHealth((float)(rand() % 11 + vRandomHealthValue[DIFF_HARD]));
		}
		break;
	}
	SetHealth(GetMaxHealth());

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "MMDamage");
	//SetDamage(static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));
	vector<int> vDamage;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vDamage);
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		{
			SetDamage(vDamage[DIFF_EASY]);
		}
		break;
	case DIFF_MEDIUM:
		{
			SetDamage(vDamage[DIFF_MEDIUM]);
		}
		break;
	case DIFF_HARD:
		{
			SetDamage(vDamage[DIFF_HARD]);
		}
		break;
	}
	

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "MMAmmountToDrop");
	vector<int> vDropAmounts;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vDropAmounts);
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		{
			if (rand() % 3) 
			{
				SetAmountToDrop(vDropAmounts[DIFF_EASY]);
			}
			else // sometimes you get a bonus pickup
			{
				SetAmountToDrop(vDropAmounts[DIFF_EASY] + 1);
			}
		}
		break;
	case DIFF_MEDIUM:
		{
			SetAmountToDrop(vDropAmounts[DIFF_MEDIUM]);
		}
		break;
	case DIFF_HARD:
		{
			SetAmountToDrop(vDropAmounts[DIFF_HARD]);
		}
		break;
	}

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "MMPercnetageHealthChance");
	//SetHealthChance(static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));
	vector<int> vHealthChance;
	LoadVec(CWorldManager::GetInstance()->GetluaState(), vHealthChance);
	switch(CGame::GetInstance()->GetDifficultyLevel())
	{
	case DIFF_EASY:
		{
			SetHealthChance(vHealthChance[DIFF_EASY]);
		}
		break;
	case DIFF_MEDIUM:
		{
			SetHealthChance(vHealthChance[DIFF_MEDIUM]);
		}
		break;
	case DIFF_HARD:
		{
			SetHealthChance(vHealthChance[DIFF_HARD]);
		}
		break;
	}

	// Pop the lua stack
	lua_pop(CWorldManager::GetInstance()->GetluaState(), lua_gettop(CWorldManager::GetInstance()->GetluaState()));
}

void CMeleeEnemy::Uninitialize()
{
	// if this minion has just been killed, increment the kill count for the credit screen
	// only increase this count when it's killed, and not just left behind when you enter the portal
	if (GetHealth() <= 0)
	{
		CGame::GetInstance()->IncrementMinionKillCount();
	}

	// Stop any effects from happening since we're dead - JC
	if(m_pcEffectEmitter1)
	{
		m_pcEffectEmitter1->SetActive(false);
		CView::GetEmitterManager().KillEmitter(m_pcEffectEmitter1);
		m_pcEffectEmitter1 = nullptr;

		LOG("m_pcEffectEmitter1 cleaned up!");
	}
	
	if(m_pcEffectEmitter2)
	{
		CView::GetEmitterManager().KillEmitter(m_pcEffectEmitter2, false);
		m_pcEffectEmitter2 = nullptr;

		LOG("m_pcEffectEmitter2 cleaned up!");
	}

	if (m_pcAmbientColdEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcAmbientColdEmitter);
		m_pcAmbientColdEmitter = nullptr;
	}

	if (m_pcAmbientFireEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcAmbientFireEmitter);
		m_pcAmbientFireEmitter = nullptr;
	}

	//if (m_pcIceBlock)
	//{
	//	m_pcIceBlock->Uninitialize();
	//	m_pcIceBlock = nullptr;
	//}

	//if (m_pcEffectEmitter3)
	//{
	//	m_pcEffectEmitter3->SetActive(false);
	//}

	//cout << "Enemy uninit\n";

	// Now call parent
	CEnemy::Uninitialize();
}

