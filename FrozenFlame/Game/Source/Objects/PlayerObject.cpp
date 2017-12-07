/***********************************************
* Filename:  		PlayerObject.cpp
* Date:      		10/30/2012
* Mod. Date: 		11/30/2012
* Mod. Initials:	JM
* Author:    		Charles Meade
* Purpose:   		Function definitions for the player object
************************************************/

#include "PlayerObject.h"
#include "AttackObject.h"
#include "ObjectManager.h"
#include "../Core/CWorldManager.h"
#include "../CGame.h"
#include "../Animations/Animation.h"
#include "../Renderer/CEmitterManager.h"
#include "../Achievements/Header/CAchievementManager.h"
#include "SceneryObject.h"
#include "../Renderer/RenderInstanceAnimated.h"

#define STUN_RUMBLE_FADES 0	// on if the rumble fades over time, off if it abruptly turns off after the timer finishes



CPlayerObject::CPlayerObject(void) : m_fRageDecAmt(2.0f), m_fMaxAccumulatedRageValue(7.0f)
{
	SetType(PLAYER_OBJECT);
	m_pcFireEmitter = nullptr;
	m_pcIceEmitter = nullptr;
	//m_pcRageEmitter = nullptr;
	//m_pcRageEmitter2 = nullptr;
	m_nRageLight = -1;
}

CPlayerObject::~CPlayerObject(void)
{
	if (m_pcFireEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcFireEmitter);
		m_pcFireEmitter = nullptr;
	}

	if (m_pcIceEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcIceEmitter);
		m_pcIceEmitter = nullptr;
	}

	/*if (m_pcRageEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter);
		m_pcRageEmitter = nullptr;
	}

	if (m_pcRageEmitter2)
	{
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter2);
		m_pcRageEmitter2 = nullptr;
	}*/

	if (m_pcFireAttackEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcFireAttackEmitter);
		m_pcFireAttackEmitter = nullptr;
	}

	if (m_pcIceAttackEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcIceAttackEmitter);
		m_pcIceAttackEmitter = nullptr;
	}
	
	if (m_pcAmbientColdEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientColdEmitter);
		m_pcAmbientColdEmitter = nullptr;
	}
	if (m_pcAmbientFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientFireEmitter);
		m_pcAmbientFireEmitter = nullptr;
	}
}

void CPlayerObject::Initialize(void)
{
	CCollidableObject::Initialize();

	// Lovely - JC

	if (m_pcFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireEmitter);
		m_pcFireEmitter = nullptr;
	}
	if (m_pcIceEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceEmitter);
		m_pcIceEmitter = nullptr;
	}	
	/*if (m_pcRageEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter);
		m_pcRageEmitter = nullptr;
	}
	if (m_pcRageEmitter2){
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter2);
		m_pcRageEmitter2 = nullptr;
	}*/
	if (m_pcFireAttackEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireAttackEmitter);
		m_pcFireAttackEmitter = nullptr;
	}
	if (m_pcIceAttackEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceAttackEmitter);
		m_pcIceAttackEmitter = nullptr;
	}
	if (m_pcAmbientColdEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientColdEmitter);
		m_pcAmbientColdEmitter = nullptr;
	}
	if (m_pcAmbientFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientFireEmitter);
		m_pcAmbientFireEmitter = nullptr;
	}

	m_chInputBits = 0;

	m_fRage = 0.0f;
	m_fAttackCooldown = 1.0f;
	m_fRageDecreaseTimer = 0.0;
	m_fCurrRot = 0.0f;
	m_fRageCooldown = 0.0f;
	m_fAccumulatedRage = 0.0f;
	m_fDashCooldown = 0.0f;
	m_fDamageSoundTimer = 0.0f;	
	m_fDamageSoundBuffer = 0.3f;
	m_fAttackButtonTimer = 0.0f;	
	m_fAttackButtonBuffer = 0.15f;
	m_fIdleTimer = 0.0f;
	this->m_fCurrentSize = 1.0f;
	this->m_fGrowthTimer = 0.0f;
	this->m_fDashTimer = 0.0f;
	m_fStartRot = this->GetCurrRotation();
	m_fStunDuration = 0.75f;
	m_fRageSpeedFactor = 1.7f;
	m_fStunTimer = 0.0f;
	m_fKnockbackFactor = 2.0f;
	m_fFireTrapRumbleDuration = 0.4f;
	m_fFireTrapRumbleTimer = 0;	
	m_fIceTrapRumbleDuration = 0.2f; 
	m_fIceTrapRumbleTimer = 0;	
	m_fProjectileHitRumbleDuration = 0.13f;
	m_fProjectileHitRumbleTimer = 0;	
	m_fFireBeastRollRumbleDuration = 0.5f;
	m_fFireBeastRollRumbleTimer = 0;	
	m_FireBeastMeleeRumble.fDuration = 0.3f;
	m_FireBeastMeleeRumble.fTimer = 0;
	m_MeleeMinionMeleeRumble.fDuration = 0.16f;
	m_MeleeMinionMeleeRumble.fTimer = 0;

	m_bSpeedRage = true;
	m_bPrevRage = false;
	m_bStoppedRage = false;
	m_bAttackedLastFrame = false;
	m_bDashing = false;
	m_bCanGetRage = true;
	m_bRaging = false;
	m_bCanRage = false;
	m_bHitLastFrame = false;
	m_bEverlastingRage = false;
	m_bStunned = false;
	m_bFireTrapRumble = false;
	m_bIceTrapRumble = false;
	m_bActivateRage = false;
	m_bProjectileRumble = false;
	m_bFireBeastRollRumble = false;
	m_FireBeastMeleeRumble.bActive = false;
	m_MeleeMinionMeleeRumble.bActive = false;
	m_bMeleeMinionLeftSideRumble = true;
	m_nAmountofTimesRaged = 0;
	m_fSpeed = 1.0f;
	// rumble
	m_nStunRumbleInitial = 25000;
	m_nStunRumbleCurrent = 0;
	m_nFireTrapRumbleInitial = 25000;
	m_nFireTrapRumbleCurrent = 0;
	m_nIceTrapRumbleInitial = 15000;
	m_nIceTrapRumbleCurrent = 0;
	m_nProjectileRumbleInitial = 14000;
	m_nFireBeastRollRumbleInitial = 35000;
	m_nFireBeastRollRumbleCurrent = 0;
	m_FireBeastMeleeRumble.nInitialRumble = 25000;
	m_FireBeastMeleeRumble.nCurrentRumble = 0;
	m_MeleeMinionMeleeRumble.nInitialRumble = 15000;
	m_MeleeMinionMeleeRumble.nCurrentRumble = 0;
	m_bIsRageUsed = false;
	m_nMinDamage = 4;
	m_nMaxDamage = 7;

	m_cRageFlag = 0;
	m_cPrevRage = 0;

	m_pcFireEmitter = m_pcIceEmitter = nullptr; //m_pcRageEmitter = nullptr;
	m_pcFireAttackEmitter = m_pcIceAttackEmitter = nullptr;

	m_tLastValidVelocity.make_zero();

	SetMaxHealth(150.0f);
	SetHealth(GetMaxHealth());
	SetElementalState(NEUTRAL);

	if(GetRenderID() == -1)
	{
		SetRenderID(CView::Push(this, "Player", RENDER_RAGE_AFFECTED));
		m_nFireSwordAttID = CView::PushAttachment(GetRenderID(), "FireSword", "weapon_anchor", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		m_nIceSwordAttID = CView::PushAttachment(GetRenderID(), "IceSword", "weapon_anchor", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);

		CView::SetAttachmentEnabled(m_nFireSwordAttID, false);
		CView::SetAttachmentEnabled(m_nIceSwordAttID, true);
	}

	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));
	
	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Player/Player_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "Player/Player_Movement.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "Player/Player_AttackUp.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK2, "Player/Player_AttackDown.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK3, "Player/Player_AttackDiagonal.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "Player/Player_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "Player/Player_Damaged.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_A, "Player/Player_Dash.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_B, "Player/Player_ActivateRage.anm");

	m_pcFireEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageFireTrail.txt", GetPos(), vec3f(0,0,0),
		EMIT_LOOP, this, nullptr);

	m_pcIceEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageIceTrail.txt", GetPos(), vec3f(0,0,0),
		EMIT_LOOP, this, nullptr);

	//m_pcRageEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageEffect.txt", GetPos());
	//m_pcRageEmitter2 = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageEffect2.txt",GetPos());

	m_pcFireAttackEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/FireAttack.txt",GetPos(),vec3f(-100.0f,0,0.0f),
		EMIT_LOOP | EMIT_SPRAY ,this,nullptr);

	m_pcIceAttackEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/IceAttack.txt",GetPos(),vec3f(0,0,0),
		EMIT_LOOP,this,nullptr);

	m_pcAmbientFireEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/Fire2.txt");
	m_pcAmbientColdEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/IceMist.txt");
	
	if (CInput::GetInstance()->GetLeftMotorSpeed() || CInput::GetInstance()->GetRightMotorSpeed())
	{
		CInput::GetInstance()->SetRightMotorSpeed(0);
		CInput::GetInstance()->SetLeftMotorSpeed(0);
	}

	size_t qSize = m_qDashGhosts.size();
	for (size_t index = 0; index < qSize; index++)
	{
		m_qDashGhosts[index]->ghost->Uninitialize();
	}
	m_unMaxDashGhosts = 10;	
	m_fDashGhostTimer = 0.0f;		
	m_fDashGhostSpawnDelay = 0.005f;//0.01f;	
	m_fInitialDashGhostAlpha = 0.5f;			
	m_fGhostLifetime = 0.1f;//0.25f;//float(m_unMaxDashGhosts * m_fDashGhostSpawnDelay);			

	// Player base light - This was on World Manager before but has been brought here by Dan - Dan
	/*CView::AttachLight(m_cObjectManager.GetPlayer()->GetRenderID(), vec4f(1, 1, 1, 1), 5.0f, vec4f(0.25f, 0.25f, 0.25f, 1.0f),
		vec4f(0, 0, 0, 0), 0, 0, vec3f(0.05f, 2.0f, 0.0f), vec3f(-0.25f, 4, 0.0f));*/
	CView::MakeLight(GetRenderID(), true, vec3f(-0.25f, 2, 0), vec4f(1, 1, 1, 1), 2.0f, vec4f(0.25f, 0.25f, 0.25f, 1.0f));

	
	if(m_nRageLight == -1)
	{	
		// New light system - Dan
		/* m_pRageLight = CView::AttachLight(GetRenderID(),vec4f(1.0f,1.0f,1.0f,1.0f),20.0f,
		vec4f(0.1f, 0.1f, 0.1f, 0.1f),vec4f(1.0f, 1.0f, 1.0f, 1.0f),16.0f,3.0f,vec3f(0.1f, 0.015f, 0.035f),
		vec3f(GetPos().x,GetPos().y + 5.0f,GetPos().z)); */
		m_nRageLight = CView::MakeLight(GetRenderID(), false, vec3f(0, 5, 0), vec4f(1.0f,1.0f,1.0f,1.0f), 20.0f);
	}
}

void CPlayerObject::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();

	testEmitter = nullptr; /// BRG MUST REMOVE
}

void CPlayerObject::ReinitializeEmitters()
{
	if (m_pcFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireEmitter);
		m_pcFireEmitter = nullptr;
	}
	if (m_pcIceEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceEmitter);
		m_pcIceEmitter = nullptr;
	}	
	/*if (m_pcRageEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter);
		m_pcRageEmitter = nullptr;
	}
	if (m_pcRageEmitter2){
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter2);
		m_pcRageEmitter2 = nullptr;
	}*/
	if (m_pcFireAttackEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireAttackEmitter);
		m_pcFireAttackEmitter = nullptr;
	}
	if (m_pcIceAttackEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceAttackEmitter);
		m_pcIceAttackEmitter = nullptr;
	}
	if (m_pcAmbientColdEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientColdEmitter);
		m_pcAmbientColdEmitter = nullptr;
	}
	if (m_pcAmbientFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientFireEmitter);
		m_pcAmbientFireEmitter = nullptr;
	}

	m_pcFireEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageFireTrail.txt", GetPos(), vec3f(0,0,0),
		EMIT_LOOP, this, nullptr);

	m_pcIceEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageIceTrail.txt", GetPos(), vec3f(0,0,0),
		EMIT_LOOP, this, nullptr);

	//m_pcRageEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageEffect.txt", GetPos());
	//m_pcRageEmitter2 = CView::GetEmitterManager().AddEmitter("Resource/Particles/RageEffect2.txt",GetPos());

	m_pcFireAttackEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/FireAttack.txt",GetPos(),vec3f(-100.0f,0,0.0f),
		EMIT_LOOP | EMIT_SPRAY ,this,nullptr);

	m_pcIceAttackEmitter = CView::GetEmitterManager().AddEmitter("Resource/Particles/IceAttack.txt",GetPos(),vec3f(0,0,0),
		EMIT_LOOP,this,nullptr);

	m_pcAmbientFireEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/Fire2.txt");
	m_pcAmbientColdEmitter = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/IceMist.txt");
}

void CPlayerObject::Uninitialize()
{
	// This will make an ash cloud effect be created when the player dies. - Dan
	SetIsDying(false);
	CView::GetEmitterManager().AddEmitter(false, true, GetPos(), "Resource/Particles/AshCloud.txt", true);
	CView::GetCamera().RestoreDefaultCamera();

	CCollidableObject::Uninitialize();

	m_fRage = 0.0f;
	m_bCanRage = false;
	m_fAccumulatedRage= 0.0f;
	m_fCurrentSize = 0.0f;
	m_fGrowthTimer = 0.0f;
	m_bCanGetRage = true;
	m_bHitLastFrame = false;

		if (m_pcFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireEmitter);
		m_pcFireEmitter = nullptr;
	}
	if (m_pcIceEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceEmitter);
		m_pcIceEmitter = nullptr;
	}	
	/*if (m_pcRageEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter);
		m_pcRageEmitter = nullptr;
	}
	if (m_pcRageEmitter2){
		CView::GetEmitterManager().KillEmitter(m_pcRageEmitter2);
		m_pcRageEmitter2 = nullptr;
	}*/
	if (m_pcFireAttackEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcFireAttackEmitter);
		m_pcFireAttackEmitter = nullptr;
	}
	if (m_pcIceAttackEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcIceAttackEmitter);
		m_pcIceAttackEmitter = nullptr;
	}
	if (m_pcAmbientColdEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientColdEmitter);
		m_pcAmbientColdEmitter = nullptr;
	}
	if (m_pcAmbientFireEmitter){
		CView::GetEmitterManager().KillEmitter(m_pcAmbientFireEmitter);
		m_pcAmbientFireEmitter = nullptr;
	}

	if (CInput::GetInstance()->GetLeftMotorSpeed() || CInput::GetInstance()->GetRightMotorSpeed())
	{
		CInput::GetInstance()->SetRightMotorSpeed(0);
		CInput::GetInstance()->SetLeftMotorSpeed(0);
	}
}

void CPlayerObject::Update(float fElapsedTime)
{	
	// If we were not initialized, do nothing
	if(!GetInitialized())
	{
		return;
	}

	// If the player is dying, let's just update animations and return - Dan
	if(GetIsDying())
	{
		SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
		UpdateAnimations(fElapsedTime);
		return;
	}

	if (m_bStoppedRage)
	{
		if (m_fCurrentSize > 1.0f)
		{
			m_fCurrentSize -= 0.5f * fElapsedTime;
			if (m_fCurrentSize <= 1.0f)
			{
				m_fCurrentSize = 1.0f;
				m_bStoppedRage = false;
			}
		}
	}

	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.scale_pre(vec3f(GetScale().x, GetScale().y, GetScale().z));
	// Reset our attackedLastFrame bool
	if(m_bAttackedLastFrame == true)
	{
		m_bAttackedLastFrame = false;
	}

	if((!GetAnimationPlayer()->IsPlaying(ANIM_ATTACK) && !GetAnimationPlayer()->IsPlaying(ANIM_ATTACK2)) && !GetAnimationPlayer()->IsPlaying(ANIM_ATTACK3) /*&& GetIsAttacking()!m_bAttackedLastFrame*/)
	{
		SetIsAttacking(false);
		
		m_pcFireEmitter->SetActive(false);
		m_pcIceEmitter->SetActive(false);
		m_pcFireAttackEmitter->SetActive(false);
		m_pcIceAttackEmitter->SetActive(false);
		
		//m_pcFireEmitter->ResetParticles();
		//m_pcIceEmitter->ResetParticles();
	}

	playerPos = GetPos();
	vec3f agentPos = GetPos();
	agentPos.y += 2.0f;

	if (m_pcAmbientFireEmitter)
	{
		m_pcAmbientFireEmitter->SetEmittPos(agentPos);
	}
	if (m_pcAmbientColdEmitter)
	{
		m_pcAmbientColdEmitter->SetEmittPos(agentPos);
	}

	if (m_bSpeedRage)
	{
		if (IsRaging())
		{
			m_fAttackCooldown += fElapsedTime * m_fRageSpeedFactor;
		}
		else
		{
			m_fAttackCooldown += fElapsedTime;
		}
	}
	else
	{
		m_fAttackCooldown += fElapsedTime;
	}

	m_fAttackButtonTimer += fElapsedTime;
	// if you've taken more than m_fAttackButtonBuffer seconds to press an attack
	// button, stop attacking
	if (m_fAttackButtonTimer > m_fAttackButtonBuffer)
	{
		SetIsAttacking(false);
	}

	static bool check = false;
	if(m_fRage >= m_nMaxPlayerRage && check == false)
	{
		//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_Heartbeat");
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_HEARTBEAT, GAME_OBJECT_ID_PLAYER);
		check = true;
	}
	else if(m_fRage < m_nMaxPlayerRage && check == true)
	{
		//CSoundManager::GetInstance()->StopSound(CGame::GetInstance()->GetSoundBank(),"SFX_Heartbeat");
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::STOP_SFX_HEARTBEAT, GAME_OBJECT_ID_PLAYER);
		check = false;
	}

	if(m_bRaging)
	{
		// Turn on fire trail / ice trail emitters if needed - JC
		if(m_bDashing) 
		{
			CreateElementalDashGhosts();

			switch (GetAttackElement()) {
				case FIRE: {
					if(!m_pcFireEmitter->GetActive()) {
						m_pcFireEmitter->SetOffset(vec3f(0.0f,-2.5f,0.0f));
						m_pcFireEmitter->SetActive(true); }
					break; }
				case ICE: {
					if(!m_pcIceEmitter->GetActive()) {
						m_pcIceEmitter->SetOffset(vec3f(0.0f,-2.5f,0.0f));
						m_pcIceEmitter->SetActive(true); }
					break; }
			}
		}

		/*if (m_pcRageEmitter)
		{
			m_pcRageEmitter->SetEmittPos(GetPos());
			m_pcRageEmitter->SetActive(true);
		}
		if (m_pcRageEmitter2)
		{
			m_pcRageEmitter2->SetEmittPos(GetPos());
			m_pcRageEmitter2->SetActive(true);
		}*/
					
		if(m_fRage > 25.0f && m_fCurrentSize < 1.5f)
		{
			m_fCurrentSize += 0.5f * fElapsedTime;
			m_fSpeed = 1.5f;
			//m_vOldVel = GetVelocity();
			//SetVelocity(vec3f(GetVelocity().x * 1.5f, GetVelocity().y, GetVelocity().z * 1.5f));
		}
		else if(m_fRage < 25.0f && m_fRage > 0.0f /*&& m_fCurrentSize > 1.25f*/)
		{
			if(m_fCurrentSize > 1.25f)
			{
				m_fCurrentSize -= 0.5f * fElapsedTime;
				m_fSpeed = 1.25f;
				//SetVelocity(vec3f(GetVelocity().x * 1.25f, GetVelocity().y, GetVelocity().z * 1.25f));
			}
		}
		else if(m_fRage <= 0.0f)
		{
			m_bRaging = false;
			CView::StopRaging();

			if(m_fCurrentSize > 1.0f)
			{
				m_fCurrentSize -= 0.5f * fElapsedTime;
				m_fSpeed = 1.0f;
				//SetVelocity(m_vOldVel);
			}
		}
	}

	UpdateElementalDashGhosts(fElapsedTime);

	if(m_fRage <= 0.0f)
	{
		m_bRaging = false;
		/*if (m_pcRageEmitter)
		{
			m_pcRageEmitter->SetActive(false);
		}
		if (m_pcRageEmitter2)
		{
			m_pcRageEmitter2->SetActive(false);
		}*/
		if(m_fCurrentSize > 1.0f)
		{
			m_fCurrentSize -= 0.5f * fElapsedTime;
			m_fSpeed = 1.0f;
		}
	}

	int nXRot = 0;
	int nYRot = 0;
	
	if(!m_bDashing)
	{
		SetVelocity(vec3f(0, 0, 0));
		if(!m_bActivateRage && !this->GetIsAttacking())
		{
			// to buffer the walking sound - Roberto
			// static int buffer = 0;

			if(m_chInputBits & CH_UP)
			{
				SetIsMoving(true);
				SetIsAttacking(false);
				SetVelocity(vec3f(GetVelocity().x + (1), GetVelocity().y, GetVelocity().z - (1)));
				
				nXRot += 1;
			}

			if(m_chInputBits & CH_DOWN)
			{
				SetIsMoving(true);
				SetIsAttacking(false);
				SetVelocity(vec3f(GetVelocity().x - (1), GetVelocity().y, GetVelocity().z + (1)));
				
				nXRot += -1;
			}
			if(m_chInputBits & CH_LEFT)
			{
				SetIsMoving(true);
				SetIsAttacking(false);
				SetVelocity(vec3f(GetVelocity().x + (1), GetVelocity().y, GetVelocity().z + (1)));
				
				nYRot += -1;
			}
			if(m_chInputBits & CH_RIGHT)
			{
				SetIsMoving(true);
				SetIsAttacking(false);
				SetVelocity(vec3f(GetVelocity().x - (1), GetVelocity().y, GetVelocity().z - (1)));
				
				nYRot += 1;
			}
		}

		if( (m_chInputBits & CH_UP) == 0 && (m_chInputBits & CH_DOWN) == 0 && (m_chInputBits & CH_LEFT) == 0 && (m_chInputBits & CH_RIGHT) == 0)
		{
			SetIsMoving(false);
		}

		int nDegreeRot = 0;
		int nDivideByNum = 0;
		if(nXRot)
		{
			if(nXRot == -1)
			{
				nDegreeRot += 180;
			}
			else if(nYRot == -1)
			{
				nDegreeRot += 360;
			}
			nDivideByNum++;
		}
		if(nYRot)
		{
			if(nYRot == 1)
				nDegreeRot += 90;
			else
				nDegreeRot += 270;
			nDivideByNum++;
		}

		if(nDivideByNum)
		{
			m_fCurrRot = (float)(nDegreeRot/nDivideByNum)-45;

			vec3f pos = GetPos();
			
			matrix4f worldMatrix = *GetWorldMatrix();
			worldMatrix.make_rotation_y(D3DXToRadian(m_fCurrRot));
			SetWorldMatrix(worldMatrix);
			SetPos(pos, true);
		}

		vec3f scale;

		scale.x = 1.0f/sqrt(dot_product(GetWorldMatrix()->axis_x,GetWorldMatrix()->axis_x));
		scale.y = 1.0f/sqrt(dot_product(GetWorldMatrix()->axis_y,GetWorldMatrix()->axis_y));
		scale.z = 1.0f/sqrt(dot_product(GetWorldMatrix()->axis_z,GetWorldMatrix()->axis_z));

		matrix4f worldMatrix = *GetWorldMatrix();
		worldMatrix.scale_pre(scale);
		SetWorldMatrix(worldMatrix);

		worldMatrix.scale_pre(vec3f(GetScale().x * m_fCurrentSize, GetScale().y * m_fCurrentSize, GetScale().z * m_fCurrentSize));
		SetWorldMatrix(worldMatrix);

		if(!m_bDashing && GetVelocity().magnitude())
		{
			float fTotal = abs(GetVelocity().x) + abs(GetVelocity().z);
			SetVelocity(vec3f((GetVelocity().x / fTotal) * m_fSpeed, GetVelocity().y, (GetVelocity().z / fTotal) * m_fSpeed));
			m_tLastValidVelocity = GetVelocity();
		}

		
		if(m_chInputBits & CH_DASH && m_fDashCooldown > 0.31f && !this->GetIsAttacking())
		{
			if(!m_bActivateRage && !this->GetIsAttacking())
			{
				if(!m_bDashing)
				{
					SetIsAttacking(false);
					m_bAttackedLastFrame = false;
					m_fAttackCooldown = 1.0f;

					SetVelocity(m_tLastValidVelocity * 4);
				
					m_bDashing = true;
					m_fDashCooldown = 0.0f;
					//CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank(),"SFX_Whoosh");
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WHOOSH,GAME_OBJECT_ID_PLAYERDASH);
					GetAnimationPlayer()->PlayAnimationImmediate(ANIM_SPECIAL_A);
				}
			}
		}
		else
		{
			// By Dan
			// This hardcoded terrible code is completely mine, but it was necessary for the attack to work fine and feel good
			float fWaitTimeBetweenAttacks;
			if(GetAnimationPlayer()->IsPlaying(ANIM_ATTACK3))
				fWaitTimeBetweenAttacks = GetAnimationPlayer()->GetAnimDuration(ANIM_ATTACK3) / 3.5f;
			else if(GetAnimationPlayer()->IsPlaying(ANIM_ATTACK2))
				fWaitTimeBetweenAttacks = GetAnimationPlayer()->GetAnimDuration(ANIM_ATTACK2) / 2.0f;
			else
				fWaitTimeBetweenAttacks = GetAnimationPlayer()->GetAnimDuration(ANIM_ATTACK) / 2.0f;

			if(m_chInputBits & CH_FIRE_ATK)
			{
				if(!m_bAttackedLastFrame)
				{
					if(m_fAttackCooldown >= fWaitTimeBetweenAttacks)
					{
						// GETTO PARTICLE EDITOR
						// UNCOMMENT AND CHANGE EMITTER FILE TO WHAT YOU WANT TO TEST AND FIRE ATTACK WITH PLAYER TO TEST
						// YOU CAN CHANGE THE FILE THEN SAVE IT AND ATTACK AGAIN TO RELOAD THE NEW EMITTER
						//if(testEmitter)
						//{
						//	CView::GetEmitterManager().KillEmitter(testEmitter);
						//}
						//testEmitter = CView::GetEmitterManager().AddEmitter(true,true,GetPos(),"Resource/Particles/WizardDeath.txt",false,this);

						SetIsAttacking(true);
						SetIsMoving(false);
						m_fAttackCooldown = 0.0f;
						m_fAttackButtonTimer = 0.0f;
						SetAttackElement(HOT);
						
						//****The on/off for fire/ice hit emitters has been moved to ObjectManager -JC 						
						m_pcFireAttackEmitter->SetActive(true);
						m_pcIceAttackEmitter->SetActive(false);

						m_bAttackedLastFrame = true;

						// Disable the Ice Sword and enable the Fire Sword
						CView::SetAttachmentEnabled(m_nIceSwordAttID, false);
						CView::SetAttachmentEnabled(m_nFireSwordAttID, true);


						//LOG("FIRE ATTACK");
					}
				}

			}
			else if(m_chInputBits & CH_ICE_ATK)
			{
				if(!m_bAttackedLastFrame)
				{
					if(m_fAttackCooldown >= fWaitTimeBetweenAttacks)
					{
						SetIsAttacking(true);
						SetIsMoving(false);
						m_fAttackCooldown = 0.0f;
						m_fAttackButtonTimer = 0.0f;
						SetAttackElement(COLD);
						//****The on/off for fire/ice hit emitters has been moved to ObjectManager -JC						
						m_pcIceAttackEmitter->SetActive(true);
						m_pcFireAttackEmitter->SetActive(false);						

						m_bAttackedLastFrame = true;

						// Disable the Ice Sword and enable the Fire Sword
						CView::SetAttachmentEnabled(m_nIceSwordAttID, true);
						CView::SetAttachmentEnabled(m_nFireSwordAttID, false);


						//LOG("ICE ATTACK");
					}
				}
			}
			else
			{
				m_bHitLastFrame = false;
			}

			if(m_chInputBits & CH_RAGE)
			{
				if (!m_bPrevRage)
				{
					m_bPrevRage = true;
					if(!m_bActivateRage && !this->m_bRaging && this->m_bCanRage)
					{
						SetIsMoving(false);
						GetAnimationPlayer()->PlayAnimationImmediate(ANIM_SPECIAL_B);
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_DX_PLAYER_RAGEROAR, GAME_OBJECT_ID_PLAYER);
						CView::StartRaging();

						m_bActivateRage = true;
						m_bRaging = true;
						CAchievementManager::GetInstance()->SetHasPlayerRaged(true);
						m_nAmountofTimesRaged += 1;
						if (m_nAmountofTimesRaged == 5)
							CAchievementManager::GetInstance()->ActivateAchievement("My Secret Is...");
					}
					else
					{
						CView::StopRaging();
						//m_bActivateRage = false;
						m_bRaging = false;
						m_bStoppedRage = true;
						/*if (m_pcRageEmitter)
						{
							m_pcRageEmitter->SetActive(false);
						}
						if (m_pcRageEmitter2)
						{
							m_pcRageEmitter2->SetActive(false);
						}*/
						//m_fCurrentSize = 1.0f;
					}
				}
				
			}
			else
			{
				m_bPrevRage = false;
			}
		}

		if(m_bActivateRage && !this->GetAnimationPlayer()->IsPlaying(ANIM_SPECIAL_B))
		{
			m_bActivateRage = false;
		}

		if(!m_bDashing && m_fAttackCooldown <= 0.3f)
		{
			SetVelocity(vec3f(0, 0, 0));
		}
	}
	
	if(m_bDashing)
	{
		this->SetIsMoving(false);
		this->GetAnimationPlayer()->PlayAnimation(ANIM_SPECIAL_A);
		m_fDashTimer += fElapsedTime;

		//float fDur;
		//fDur = this->GetAnimationPlayer()->GetCurAnimation()->GetDuration() / 2;
		//fDur = 0.3f;
		if(this->m_fDashTimer >= this->GetAnimationPlayer()->GetCurrentAnimDuration() * 0.5f)
		{
			//m_fDashCooldown = 0.0f;
			m_bDashing = false;
			//GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true);
			this->SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			m_fDashTimer = 0.0f;

			// turn off any fire/ice trail emitters - JC
			if(m_pcFireEmitter->GetActive()) {		
				m_pcFireEmitter->SetOffset(vec3f(0.0f,0.0f,0.0f));
				m_pcFireEmitter->SetActive(false);
				//m_pcFireEmitter->ResetParticles(); 
			}

			if(m_pcIceEmitter->GetActive()) {
				m_pcIceEmitter->SetOffset(vec3f(0.0f,0.0f,0.0f));
				m_pcIceEmitter->SetActive(false); 
				//m_pcIceEmitter->ResetParticles();
			}
		}
	}
	else
	{
		m_fDashCooldown += fElapsedTime;
	}
	m_fIdleTimer += fElapsedTime;

	if(GetIsMoving() || m_bDashing)
	{
		m_fIdleTimer = 0.0f;
	}

	if(!m_bCanGetRage || m_fAccumulatedRage >= 1)
	{
		m_fRageCooldown += fElapsedTime;
	}

	if(m_fRageCooldown >= 0.5f)
	{
		m_bCanGetRage = true;
		m_fRageCooldown = 0.0f;
	}

	if(GetElementalState() > HOT)
	{
		SetElementalState(HOT);
	}
	else if(GetElementalState() < COLD)
	{
		SetElementalState(COLD);
	}

	switch (GetElementalState())
	{
	case COLD:
		{
			this->GetAnimationPlayer()->SetAnimationSpeed(0.7f);
			SetColor(vec3f(0.2f, 0.2f, 0.5f));
			if (m_pcAmbientColdEmitter)
			{
				m_pcAmbientColdEmitter->SetActive(true);
			}
		}
		break;

	case NEUTRAL:
		{
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
			if (GetDamageTimer() >= DAMAGE_COOLDOWN)
			{
				SetHealth(GetHealth() - 1);
				SetDamageTimer(0.0);
			}
			SetColor(vec3f(0.5f, 0.2f, 0.2f));
			if (m_pcAmbientFireEmitter)
			{
				m_pcAmbientFireEmitter->SetActive(true);
			}
		}
		break;
	}

	if (GetElementalState() != NEUTRAL)
	{
		SetElementalTimer(GetElementalTimer() + fElapsedTime);
		SetDamageTimer(GetDamageTimer() + fElapsedTime);

		if (GetElementalTimer() >= TIMER_COOLDOWN)
		{
			if (GetElementalState() > NEUTRAL)
			{
				SetElementalState(GetElementalState() - 1);
			}
			else if (GetElementalState() < NEUTRAL)
			{
				SetElementalState(GetElementalState() + 1);
				if (GetElementalState() == NEUTRAL)
				{
					SetVelocity(GetVelocity() * 2.0f);
				}
			}
			SetElementalTimer(0.0);
		}
	}
	else
	{
		SetIsDebuffSet(false);
		SetColor(vec3f(1.0f, 1.0f, 1.0f));
		if (m_bSpeedRage){
			if (IsRaging()){
				if (GetAnimationPlayer()){
					GetAnimationPlayer()->SetAnimationSpeed(m_fRageSpeedFactor);
				}
			}
			else
				this->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
		}
		else
		{
			if (GetAnimationPlayer()){
				GetAnimationPlayer()->SetAnimationSpeed(1.0f);
			}
		}
	}

	vec3f objVel = GetVelocity();
	if (GetElementalState() <= COLD)
		objVel /= 2.0f;
	SetPos(GetPos() + objVel * 10.0f * fElapsedTime, true);

	if (GetInitialized())
	{	
		// Fire / Ice SWIPE effect positions
		const CRenderInstance* pRenderInst = CView::GetRenderInstance(GetRenderID());
		
		if(pRenderInst->GetType() == IRenderThing::ANIMATED)
		{
			// const D3DXMATRIX* fireSwordMat = ((CRenderInstanceAnimated*)pRenderInst)->GetD3DAttachmentTransform(m_nFireSwordAttID);
			// const D3DXMATRIX* iceSwordMat = ((CRenderInstanceAnimated*)pRenderInst)->GetD3DAttachmentTransform(m_nIceSwordAttID);
			const D3DXMATRIX* fireSwordMat = &CView::GetAttachmentInstance(-1, m_nFireSwordAttID)->GetWorldTransform();
			const D3DXMATRIX* iceSwordMat = &CView::GetAttachmentInstance(-1, m_nIceSwordAttID)->GetWorldTransform();

			// Forward vector of sword
			vec3f fireSwordZ(fireSwordMat->_31,fireSwordMat->_32,fireSwordMat->_33);
			vec3f iceSwordZ(iceSwordMat->_31,iceSwordMat->_32,iceSwordMat->_33);
			vec3f iceSwordY(iceSwordMat->_21,iceSwordMat->_22,iceSwordMat->_23);

			// Deposit particles along length of sword
			/*float modifier = float((rand() % 1000) * 0.001f);

			fireSwordZ *= 3.0f * modifier;
			iceSwordZ *= 3.0f * modifier;*/

			//iceSwordZ *= 3.0f;
			iceSwordY *= 3.0f;

			vec3f fireSwordPos (fireSwordMat->_41,fireSwordMat->_42,fireSwordMat->_43);
			vec3f iceSwordPos(iceSwordMat->_41,iceSwordMat->_42,iceSwordMat->_43);

			fireSwordPos -= fireSwordZ;
			iceSwordPos -= iceSwordY;

			// Offset
			vec3f offset = fireSwordPos - GetPos();
			m_pcFireAttackEmitter->SetOffset(offset);			
			m_pcIceAttackEmitter->SetOffset(offset);

			m_pcFireAttackEmitter->SetDestinationPos(iceSwordPos);
		}
	}

	if(this->m_bRaging)
	{
		m_bCanGetRage = false;

		// New light system - Dan
		// CView::EnableAttachedLight(GetRenderID());
		CView::SetLightEnabled(m_nRageLight, true);

		if(m_chInputBits & CH_FIRE_ATK && m_nRageLight != -1)
		{
			// m_pRageLight->SetDiffuseColor(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
			CView::SetLightDiffuseColor(m_nRageLight, vec4f(1, 0, 0, 1));
		}
		else if(m_chInputBits & CH_ICE_ATK && m_nRageLight != -1)
		{
			// m_pRageLight->SetDiffuseColor(D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));
			CView::SetLightDiffuseColor(m_nRageLight, vec4f(0, 0, 1, 1));
		}

		this->m_fRageDecreaseTimer += fElapsedTime;
		if(this->m_fRageDecreaseTimer >= 0.5f)
		{
			if( this->m_fRage > 0)
			{
				this->m_fRage -= (m_fRageDecAmt * 4.0f * fElapsedTime);
				if(GetHealth() < 150)
				{
					SetHealth(GetHealth() + 10 * fElapsedTime);
				}
			}
		}
	}
	else
	{
		// New light system - Dan
		// CView::DisableAttachedLight(GetRenderID());
		CView::SetLightEnabled(m_nRageLight, false);
	}

	if(GetHealth() > 150.0f)
	{
		SetHealth(150.0);
	}

	if(m_fRage == 100.0f)
	{
		m_bCanRage = true;
	}

	else
	{
		m_bCanRage = false;
	}

	if(m_bEverlastingRage)
	{
		m_fRage = 100.0f;
	}

	if(m_fRage > 100)
	{
		m_fRage = 100;
	}

	if(m_fRage <= 0)
	{
		m_fRage = 0;
	}

	if(m_fRage == 0)
	{
		m_bCanRage = false;
	}

	// Saving current position - JC
	m_vLastPosition = GetWorldMatrix()->axis_pos;

	//m_bAttackedLastFrame = GetIsAttacking();
	if(m_bAttackedLastFrame)
	{
		//LOG("ATTACKED LAST FRAME");
	}


	// You want to build your AABBs and such AFTER player input.
	// Otherwise your collision data will be a frame off. - JC
	CCollidableObject::Update(fElapsedTime);

	if(!GetInitialized())
		return;

	UpdateBufferTimers(fElapsedTime);

	CSoundManager::GetInstance()->SetListenerPosition((GetWorldMatrix()->axis_pos));
}

void CPlayerObject::UpdateAnimations(float fElapsedTime)
{
	// It will make things easier if we get a pointer to our animation player
	CAnimPlayer* pAnim = GetAnimationPlayer();
	
	// If we don't have an animation player we shouldn't even be in here
	if(!pAnim) return;

	// Check if we should play death animation
	if(GetHealth() <= 0)
	{
		// If the last played animation was a death animation, uninitialize
		//if(pAnim->GetLastPlayedAnimType() == ANIM_DEATH)
		//{
		//	Uninitialize();
		//}
		// if we're not dying, die
		/*else*/if(!pAnim->IsPlaying(ANIM_DEATH))
		{
			SetIsDying(true);
			SetVelocity(vec3f(0.0f,0.0f,0.0f));

			// If we're not yet playing the death animation, let's do it - Dan
			pAnim->PlayAnimationToEnd(ANIM_DEATH);

			// Just in case the player is ragin this will make the rage effect be removed - Dan
			CView::StopRaging();

			// When the player is dying let's make the camera zoom - Dan
			CView::GetCamera().ZoomCamera(15.0f, pAnim->GetAnimDuration(ANIM_DEATH), false);
		}
		return;
	}
	// Check if we should play attack animation
	else if(GetIsAttacking() && !GetIsMoving() && m_bAttackedLastFrame)
	{
		// If we're playing the attack animation then play attack 2
		if(GetAnimationPlayer()->IsPlaying(ANIM_ATTACK))
		{
			SetDamage(m_nMaxDamage);
			GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK2);
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WHOOSH,GAME_OBJECT_ID_PLAYER);
		}
		else if(GetAnimationPlayer()->IsPlaying(ANIM_ATTACK2))
		{
			SetDamage(m_nMinDamage);
			if(GetAnimationPlayer()->GetLastPlayedAnimType() == ANIM_ATTACK3)
				GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK);
			else
				GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK3);
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WHOOSH, GAME_OBJECT_ID_PLAYER);
		}
		else if(GetAnimationPlayer()->IsPlaying(ANIM_ATTACK3))
		{
			SetDamage(m_nMaxDamage);
			GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK2);
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_WHOOSH,GAME_OBJECT_ID_PLAYER);
		}
		else
		{
			SetDamage(m_nMinDamage);
			GetAnimationPlayer()->PlayAnimationImmediate(ANIM_ATTACK);
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_SFX_SWORDSWING,GAME_OBJECT_ID_PLAYER);
		}
	}
	// Check if we should play movement animation
	else if(GetIsMoving() && !GetIsAttacking() && !GetIsActivatingRage() && !IsDashing())
	{
		pAnim->PlayAnimation(ANIM_WALK, true);
	}
	else if(!GetIsAttacking())
	{
		pAnim->PlayAnimation(ANIM_IDLE, true);
	}

	// By the end, if we're not playing anything, we should play idle
	if(!pAnim->IsPlaying())
	{
		pAnim->PlayAnimation(ANIM_IDLE, true);
	}
}

void CPlayerObject::ResetRotation(void)
{
	m_fCurrRot = 0.0f;
	vec3f pos = GetPos();
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.make_rotation_y(D3DXToRadian(m_fCurrRot));
	SetWorldMatrix(worldMatrix);
	SetPos(pos);
}

void CPlayerObject::ResetSize(void)
{
	vec3f pos = GetPos();
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.make_rotation_y(D3DXToRadian(m_fCurrRot));
	worldMatrix.scale_pre(vec3f(m_fCurrentSize,m_fCurrentSize,m_fCurrentSize));
	SetWorldMatrix(worldMatrix);
	SetPos(pos);
}

void CPlayerObject::UpdateInput(char chInputBits)
{
	// If we're dying let's not care about input - Dan
	if(GetIsDying())
		m_chInputBits = 0;
	else
		m_chInputBits = chInputBits;
}

void CPlayerObject::SetRage (float	fRage)
{
	m_fRage = fRage;
	if(fRage > m_nMaxPlayerRage)
	{
		m_fRage = m_nMaxPlayerRage;
	}
	else if(fRage < 0)
	{
		m_fRage = 0;
	}
}

void CPlayerObject::StunThePlayer()
{
	if (!m_bStunned)
	{
		m_bStunned = true;
		CInput::GetInstance()->DisablePlayerInput();
		GetAnimationPlayer()->PlayAnimation(ANIM_DAMAGED, false);

		// knock the player back a small amount
		matrix4f playerMatrix = *GetWorldMatrix();
		vec3f forwardVector = playerMatrix.axis_z;
		playerMatrix.axis_pos += forwardVector.normalize() * m_fKnockbackFactor;
		SetWorldMatrix(playerMatrix);

		CInput::GetInstance()->SetRightMotorSpeed((int)m_nStunRumbleInitial);
		CInput::GetInstance()->SetLeftMotorSpeed((int)m_nStunRumbleInitial);
	}											
}

void CPlayerObject::InitiateFireTrapRumble()
{
	m_bFireTrapRumble = true;
	m_nFireTrapRumbleCurrent = m_nFireTrapRumbleInitial;
	m_fFireTrapRumbleTimer = 0;
}

void CPlayerObject::InitiateIceTrapRumble()
{
	m_bIceTrapRumble = true;
	m_nIceTrapRumbleCurrent = m_nIceTrapRumbleInitial;
	m_fIceTrapRumbleTimer = 0;
}

void CPlayerObject::InitiateProjectileRumble()
{
	m_bProjectileRumble = true;
	m_fProjectileHitRumbleTimer = 0;
}

void CPlayerObject::InitiateFireBeastRollRumble(int elementalState)
{
	m_bFireBeastRollRumble = true;
	m_fFireBeastRollRumbleTimer = 0;
	switch(abs(elementalState))
	{
	case 2: // fire beast is huge/in a burning elemental state, provide a huge rumble strength
		{
			m_nFireBeastRollRumbleInitial = 65000;
		}
		break;

	case 1: // fire beast is large/in a hot elemental state, provide a high rumble strength
		{
			m_nFireBeastRollRumbleInitial = 45000;
		}
		break;

	case 0: // fire beast is normal/in a neutral or colder state, provide the default rumble strength
	default:
		{
			m_nFireBeastRollRumbleInitial = 35000;
		}
		break;
	}
}

void CPlayerObject::InitiateBeastMeleeRumble(int damageToPlayer)
{
	m_FireBeastMeleeRumble.bActive = true;
	m_FireBeastMeleeRumble.fTimer = 0;
	m_FireBeastMeleeRumble.nInitialRumble = 30000 + (damageToPlayer * 1000);
	if (m_FireBeastMeleeRumble.nInitialRumble > 65535)
		m_FireBeastMeleeRumble.nInitialRumble = 65535;
}

void CPlayerObject::InitiateMeleeMinionMeleeRumble(int damageToPlayer, bool minionIsOnPlayersLeft)
{
	m_MeleeMinionMeleeRumble.bActive = true;
	m_MeleeMinionMeleeRumble.fTimer = 0;
	m_MeleeMinionMeleeRumble.nInitialRumble = 15000 + (damageToPlayer * 1000);
	if (m_MeleeMinionMeleeRumble.nInitialRumble > 65535)
		m_MeleeMinionMeleeRumble.nInitialRumble = 65535;

	if (minionIsOnPlayersLeft)
		m_bMeleeMinionLeftSideRumble = true;
	else
		m_bMeleeMinionLeftSideRumble = false;
}

void CPlayerObject::UpdateBufferTimers(float fElapsedTime)
{
	// if the player is stunned
	if (m_bStunned)
	{
		
		SetColor(vec3f(0.0f, 0.0f, 0.0f));
		m_fStunTimer += fElapsedTime;
#if STUN_RUMBLE_FADES
		// the current rumble strength equals the ratio of the remaining stun time over the total stun duration
		m_nStunRumbleCurrent = (1 - m_fStunTimer/m_fStunDuration) * m_fStunRumbleInitial;
		if (m_nStunRumbleCurrent < 1)
			m_nStunRumbleCurrent = 0;
		CInput::GetInstance()->SetRightMotorSpeed(m_nStunRumbleCurrent);
		CInput::GetInstance()->SetLeftMotorSpeed(m_nStunRumbleCurrent);
#endif
		if (m_fStunTimer >= m_fStunDuration)
		{
			CInput::GetInstance()->EnablePlayerInput();
			m_bStunned = false;
			m_fStunTimer = 0.0f;
			SetColor(vec3f(1.0f, 1.0f, 1.0f));
#if !STUN_RUMBLE_FADES	// turn off rumble abruptly
			m_nStunRumbleCurrent = 0;
			CInput::GetInstance()->SetRightMotorSpeed(m_nStunRumbleCurrent);
			CInput::GetInstance()->SetLeftMotorSpeed(m_nStunRumbleCurrent);
#endif
		}
	}

	if (m_bFireTrapRumble)
	{
		// update the current timer
		m_fFireTrapRumbleTimer += fElapsedTime;
		// the current rumble strength equals the ratio of the remaining time over the total duration
		m_nFireTrapRumbleCurrent = (int)((1 - m_fFireTrapRumbleTimer/m_fFireTrapRumbleDuration) * m_nFireTrapRumbleInitial);
		if (m_nFireTrapRumbleCurrent < 1)
			m_nFireTrapRumbleCurrent = 0;
		CInput::GetInstance()->SetRightMotorSpeed(m_nFireTrapRumbleCurrent);
		CInput::GetInstance()->SetLeftMotorSpeed(m_nFireTrapRumbleCurrent);
		if (m_fFireTrapRumbleTimer >= m_fFireTrapRumbleDuration)
		{
			// reset 
			m_bFireTrapRumble = false;
			m_fFireTrapRumbleTimer = 0;
			CInput::GetInstance()->SetRightMotorSpeed(m_nFireTrapRumbleCurrent);
			CInput::GetInstance()->SetLeftMotorSpeed(m_nFireTrapRumbleCurrent);
		}
	}

	if (m_bIceTrapRumble)
	{
		// update the current timer
		m_fIceTrapRumbleTimer += fElapsedTime;
		// the current rumble strength equals the ratio of the remaining time over the total duration
		m_nIceTrapRumbleCurrent = (int)((1 - m_fIceTrapRumbleTimer/m_fIceTrapRumbleDuration) * m_nIceTrapRumbleInitial);
		if (m_nIceTrapRumbleCurrent < 1)
			m_nIceTrapRumbleCurrent = 0;
		CInput::GetInstance()->SetRightMotorSpeed((int)(m_nIceTrapRumbleCurrent * 0.4f));
		CInput::GetInstance()->SetLeftMotorSpeed(m_nIceTrapRumbleCurrent);
		if (m_fIceTrapRumbleTimer >= m_fIceTrapRumbleDuration)
		{
			// reset
			m_bIceTrapRumble = false;
			m_fIceTrapRumbleTimer = 0;
			CInput::GetInstance()->SetRightMotorSpeed((int)(m_nIceTrapRumbleCurrent * 0.4f));
			CInput::GetInstance()->SetLeftMotorSpeed(m_nIceTrapRumbleCurrent);
		}
	}

	if (m_bProjectileRumble)
	{
		// update the current timer
		m_fProjectileHitRumbleTimer += fElapsedTime;
		CInput::GetInstance()->SetRightMotorSpeed(m_nProjectileRumbleInitial);
		CInput::GetInstance()->SetLeftMotorSpeed(m_nProjectileRumbleInitial);
		if (m_fProjectileHitRumbleTimer >= m_fProjectileHitRumbleDuration)
		{
			// reset
			m_bProjectileRumble = false;
			m_fProjectileHitRumbleTimer = 0;
			CInput::GetInstance()->SetRightMotorSpeed(0);
			CInput::GetInstance()->SetLeftMotorSpeed(0);
		}
	}

	if (m_bFireBeastRollRumble)
	{
		// update the current timer
		m_fFireBeastRollRumbleTimer += fElapsedTime;
		// the current rumble strength equals the ratio of the remaining time over the total duration
		m_nFireBeastRollRumbleCurrent = (int)((1 - m_fFireBeastRollRumbleTimer/m_fFireBeastRollRumbleDuration) * m_nFireBeastRollRumbleInitial);
		if (m_nFireBeastRollRumbleCurrent < 1)
			m_nFireBeastRollRumbleCurrent = 0;
		CInput::GetInstance()->SetRightMotorSpeed((int)(m_nFireBeastRollRumbleCurrent * 0.80f));
		CInput::GetInstance()->SetLeftMotorSpeed(m_nFireBeastRollRumbleCurrent);
		if (m_fFireBeastRollRumbleTimer >= m_fFireBeastRollRumbleDuration)
		{
			// reset 
			m_bFireBeastRollRumble = false;
			m_fFireBeastRollRumbleTimer = 0;
			CInput::GetInstance()->SetRightMotorSpeed((int)(m_nFireBeastRollRumbleCurrent * 0.80f));
			CInput::GetInstance()->SetLeftMotorSpeed(m_nFireBeastRollRumbleCurrent);
		}
	}

	if (m_FireBeastMeleeRumble.bActive)
	{
		// update the current timer
		m_FireBeastMeleeRumble.fTimer += fElapsedTime;
		// the current rumble strength equals the ratio of the remaining time over the total duration
		m_FireBeastMeleeRumble.nCurrentRumble = (int)((1 - m_FireBeastMeleeRumble.fTimer/m_FireBeastMeleeRumble.fDuration) * m_FireBeastMeleeRumble.nInitialRumble);
		if (m_FireBeastMeleeRumble.nCurrentRumble < 1)
			m_FireBeastMeleeRumble.nCurrentRumble = 0;
		CInput::GetInstance()->SetRightMotorSpeed(m_FireBeastMeleeRumble.nCurrentRumble);
		CInput::GetInstance()->SetLeftMotorSpeed(m_FireBeastMeleeRumble.nCurrentRumble);
		if (m_FireBeastMeleeRumble.fTimer >= m_FireBeastMeleeRumble.fDuration)
		{
			// reset 
			m_FireBeastMeleeRumble.bActive = false;
			m_FireBeastMeleeRumble.fTimer = 0;
			CInput::GetInstance()->SetRightMotorSpeed(m_FireBeastMeleeRumble.nCurrentRumble);
			CInput::GetInstance()->SetLeftMotorSpeed(m_FireBeastMeleeRumble.nCurrentRumble);
		}
	}

	if (m_MeleeMinionMeleeRumble.bActive)
	{
		// update the timer
		m_MeleeMinionMeleeRumble.fTimer += fElapsedTime;
		// the current rumble strength equals the ratio of the remaining time over the total duration
		m_MeleeMinionMeleeRumble.nCurrentRumble = (int)((1 - m_MeleeMinionMeleeRumble.fTimer/m_MeleeMinionMeleeRumble.fDuration) * m_MeleeMinionMeleeRumble.nInitialRumble);
		if (m_MeleeMinionMeleeRumble.nCurrentRumble < 1)
			m_MeleeMinionMeleeRumble.nCurrentRumble = 0;
		// if the minion is attack from the left, use the left motor
		if (m_bMeleeMinionLeftSideRumble)
			CInput::GetInstance()->SetLeftMotorSpeed(max((m_MeleeMinionMeleeRumble.nCurrentRumble - 2000), 0));
		else	// else use the right motor
			CInput::GetInstance()->SetRightMotorSpeed(m_MeleeMinionMeleeRumble.nCurrentRumble);
		if (m_MeleeMinionMeleeRumble.fTimer >= m_MeleeMinionMeleeRumble.fDuration)
		{
			// reset
			m_MeleeMinionMeleeRumble.bActive = false;
			m_MeleeMinionMeleeRumble.fTimer = 0;
			m_MeleeMinionMeleeRumble.nCurrentRumble = 0;
			// if the minion is attack from the left, use the left motor
			if (m_bMeleeMinionLeftSideRumble)
				CInput::GetInstance()->SetLeftMotorSpeed(m_MeleeMinionMeleeRumble.nCurrentRumble);
			else	// else use the right motor
				CInput::GetInstance()->SetRightMotorSpeed(m_MeleeMinionMeleeRumble.nCurrentRumble);
		}
	}

	m_fDamageSoundTimer += fElapsedTime;	
}

void CPlayerObject::SetDashing(bool bDashing)
{
	if(!bDashing) {
		// turn off any fire/ice trail emitters - JC
		if(m_pcFireEmitter->GetActive()) {		
			m_pcFireEmitter->SetOffset(vec3f(0.0f,0.0f,0.0f));
			m_pcFireEmitter->SetActive(false);
			//m_pcFireEmitter->ResetParticles(); 
		}

		if(m_pcIceEmitter->GetActive()) {
			m_pcIceEmitter->SetOffset(vec3f(0.0f,0.0f,0.0f));
			m_pcIceEmitter->SetActive(false); 
			//m_pcIceEmitter->ResetParticles(); 
		}
	}

	m_bDashing = bDashing; 
}

void CPlayerObject::CreateElementalDashGhosts()
{
	// create a new ghost where the player currently is

	// I've commented out the time delay here so that it produces ghosts as fast as it can
	// the delay timer produces more delay than I want -Steven
//	if (m_fDashGhostTimer >= m_fDashGhostSpawnDelay)
	{
		m_fDashGhostTimer = 0.0f; // reset the timer

		tDashGhost* newQueueGhost = new tDashGhost;

		matrix4f playerMatrix = *(GetWorldMatrix());
		CSceneryObject* newGhost = (CSceneryObject*)CWorldManager::GetInstance()->MakeObject(playerMatrix.axis_pos, SCENERY_OBJECT, -1, "Player");
		CView::SetRenderInstanceFlags(newGhost->GetRenderID(), RENDER_TRANSPARENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		newGhost->SetWorldMatrix(playerMatrix);
		switch (GetAttackElement())
		{
			case FIRE:	// make a red ghost if player last used fire
				{
					newGhost->SetColor(vec3f(1.0f, 0.25f, 0.1f));
					CView::MakeLight(newGhost->GetRenderID(), true, vec3f(0, 3, 0), vec4f(0.4f, 0.0f, 0.0f, 1.0f), 20.0f);
				}
				break;

			case ICE:	// make a blue ghost if player last used ice
				{
					newGhost->SetColor(vec3f(0.3f, 0.3f, 10.0f));
					CView::MakeLight(newGhost->GetRenderID(), true, vec3f(0, 3, 0), vec4f(0.2f, 0.2f, 1.0f, 1.0f), 20.0f);
				}
				break;
		}
		
		newGhost->SetTransparency(m_fInitialDashGhostAlpha);
	

		newQueueGhost->ghost = newGhost;
		newQueueGhost->fAge = 0.0f;

		// This wasn't really doing anything ultimately so I took it out - Dan
		// newQueueGhost->ghost->SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));

		CAnimPlayer* ghostAnimPlayer = CView::GetAnimPlayer(newGhost->GetRenderID());
		CAnimPlayer* playerAnimPlayer = GetAnimationPlayer();

		*ghostAnimPlayer = *playerAnimPlayer;

		//*(CView::GetAnimPlayer(newGhost->GetRenderID())) = *(CView::GetAnimPlayer(GetRenderID()));
		
		//newQueueGhost->nRenderID = CView::Push(newQueueGhost->ghost, "Player", RENDER_TRANSPARENT);
		//newQueueGhost->ghost->SetAnimPlayer(CView::GetAnimPlayer(newQueueGhost->nRenderID));

		// push this ghost into the queue
		m_qDashGhosts.push_back(newQueueGhost);
	}
}

void CPlayerObject::UpdateElementalDashGhosts(float fElapsedTime)
{
	m_fDashGhostTimer += fElapsedTime;	

	// update the current ghosts in the queue
	size_t queueSize = m_qDashGhosts.size();
	for (size_t index = 0; index < queueSize; index++)
	{
		// each ghost becomes more transparent over time
		m_qDashGhosts[index]->ghost->SetTransparency( (1 - m_qDashGhosts[index]->fAge / m_fGhostLifetime) * m_fInitialDashGhostAlpha);
		if (m_qDashGhosts[index]->ghost->GetTransparency() < 0.0f)
			m_qDashGhosts[index]->ghost->SetTransparency(0.0f);
 		m_qDashGhosts[index]->fAge += fElapsedTime;	
	}
	

	if (m_qDashGhosts.size() && m_qDashGhosts.front()->fAge > m_fGhostLifetime)
	{
			m_qDashGhosts.front()->ghost->Uninitialize();
			tDashGhost* deleteGhost = m_qDashGhosts.front();
			m_qDashGhosts.pop_front();
			delete deleteGhost; deleteGhost = nullptr;
	}
}

void CPlayerObject::OnAttack(int nDamage,int nElementType)
{
	// If the player is dying we shouldn't take damage as well
	if(GetIsDying())
		return;

	CCollidableObject::OnAttack( nDamage, nElementType );	
	// For alternating the sounds
	
	if (m_fDamageSoundTimer > m_fDamageSoundBuffer)
	{
		CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_DX_PLAYERHURT,GAME_OBJECT_ID_PLAYER_HURT1);
		m_fDamageSoundTimer = 0.0f;
	}

	CView::GetCamera().Shake(nDamage / 2.0f, nDamage / 20.0f);
	CView::SetDisplayDamage(true);
}
