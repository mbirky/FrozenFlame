#include "WizardEnemy.h"
#include "../Renderer/View.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RenderInstance.h"
#include "../AI/Header/CFSM.h"
#include "../AI/Header/CBehaviors.h"
#include "../Globals.h"
#include "../Core/CWorldManager.h"
#include "../Renderer/CEmitterManager.h"
#include "../Achievements/Header/CAchievementManager.h"
#include "Spawner.h"
#include "../States/CGameState.h"

/*****************************************************************
*	CWizardEnemy():		Default constructor for the wizard enemy object
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
CWizardEnemy::CWizardEnemy(void)
{
	SetType(WIZARD_OBJECT);
	SetWizardSubtype(FINAL_BOSS);
	SetFSM(nullptr);
	this->m_pcBubble = nullptr;
	m_tWizTimer.Reset();
	this->m_pcCurrLevel = nullptr;
	m_pcBubbleBurstEmitter  = nullptr;
	m_pcBubbleSheenEmitter = nullptr;
	m_pcCrashEmitter = nullptr;
	m_pcDeathBurstEmitter = nullptr;
	m_pcDeathFlowEmitter = nullptr;
	m_pcSmokeEmitter = nullptr;
	m_pcTakeoffEmitter = nullptr;
	m_pcSpawnEmitter = nullptr;
	m_fDeathTimeLimit = FLT_MAX- 100.0f;

}

/*****************************************************************
*	~CWizardEnemy():	Default destructor for the wizard enemy object
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
CWizardEnemy::~CWizardEnemy(void)
{
	if (GetFSM())
	{
		CFSM* tempBehavior = GetFSM();
		delete tempBehavior;
		SetFSM(nullptr);
		tempBehavior = nullptr;
	}

	if(m_pcBubbleBurstEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcBubbleBurstEmitter);
		m_pcBubbleBurstEmitter = nullptr;
	}
	if(m_pcBubbleSheenEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcBubbleSheenEmitter);
		m_pcBubbleSheenEmitter = nullptr;
	}
	if(m_pcCrashEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcCrashEmitter);
		m_pcCrashEmitter = nullptr;
	}
	if(m_pcDeathBurstEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcDeathBurstEmitter);
		m_pcDeathBurstEmitter = nullptr;
	}
	if(m_pcDeathFlowEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcDeathFlowEmitter);
		m_pcDeathFlowEmitter = nullptr;
	}
	if(m_pcSmokeEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcSmokeEmitter);
		m_pcSmokeEmitter = nullptr;
	}
	if(m_pcTakeoffEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcTakeoffEmitter);
		m_pcTakeoffEmitter = nullptr;
	}
	if(m_pcSpawnEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcSpawnEmitter);
		m_pcSpawnEmitter = nullptr;
	}

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
* Mod. Date:		    11/30/2012
* Mod. Initials:	    JM
*****************************************************************/
void CWizardEnemy::Initialize(void)
{
	CEnemy::Initialize();

	m_tWizTimer.Start();
	m_fTrapSpawnTimer = 0.0f;
	m_fFinalDeathTimer = 0.0f;
	m_bDeathCharging = false;
	m_bDeathExloding = false;
	m_bDeathExploded = false;

	m_bEscaped = false;
	if(GetRenderID() == -1)
	{
		SetRenderID(CView::Push(this, "Wizard", RENDER_NOT_SHADOWCASTER | RENDER_RAGE_AFFECTED));
		CView::PushAttachment(GetRenderID(), "WizardStaff", "weapon_anchor", 
			"", RENDER_NOT_SHADOWCASTER);

		// Set up Shadow for wizard
		m_tShadow.m_nImageID = CView::LoadSprite("WizardShadow.png", &m_tShadow.m_nImageWidth, &m_tShadow.m_nImageHeight);
		m_nShadowRenderID = CView::DynamicShapeCreate(GetRenderID(), true);
		CView::DynamicShapeMakePlane(m_nShadowRenderID, 3, 3);
		CView::DynamicShapeSetTextures(m_nShadowRenderID, m_tShadow.m_nImageID, -1);
	}

	if(m_pcBubble == nullptr)
	{
		m_pcBubble = (CSceneryObject*) CWorldManager::GetInstance()->GetObjectManager()->NewObject(SCENERY_OBJECT, 0, "WizardShield");
		CView::SetRenderInstanceFlags(m_pcBubble->GetRenderID(), RENDER_FULLY_TRANSLUCENT | RENDER_NOT_SHADOWCASTER | RENDER_SCROLLING_UVS | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED | RENDER_INVISIBLE); // Now this is real code!

		m_pcBubble->SetTransparency(0.25f);
		m_pcBubble->SetColor(vec3f(1.0f,0.0f,1.0f));
		matrix4f mMat = *m_pcBubble->GetWorldMatrix();//->scale_pre(vec3f(1.5f,1.5f,1.5f));
		mMat.scale_pre( 0.5f, 0.5f, 0.5f );
		m_pcBubble->SetWorldMatrix(mMat);
	}

	if(GetFSM() != nullptr)
	{
		CFSM* tempBehavior = GetFSM();
		delete tempBehavior;
		SetFSM(nullptr);
	}

	SetWizardForm(1);

	if(GetFSM() == nullptr)
	{
		CFSM* _pcBehavior = new CFSM();
		_pcBehavior->SetAgent(this);
		_pcBehavior->AddState((CBaseBehavior*)(new CGloatBehavior()), true);
		if(this->GetWizardSubtype() == FINAL_BOSS)
		{
			//CFSM* _pcBehavior = new CFSM();
			//_pcBehavior->SetAgent(this);
			//_pcBehavior->AddState((CBaseBehavior*)(new CRangedBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CFloatBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CTrapSpawnBehavior())/*,true*/);
			_pcBehavior->AddState((CBaseBehavior*)(new CExhaustBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CHyperOrbBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CHyperSnowballBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CHyperdashBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CAlignBehavior()));//,true);

			//_pcBehavior->GetIsChasing() = false;
			//SetFSM(_pcBehavior);

			SetMaxHealth(400);
			SetWizardForm(1);

		}
		else if(this->GetWizardSubtype() == FIRST_STAGE)
		{
			//CFSM* _pcBehavior = new CFSM();
			//_pcBehavior->SetAgent(this);
			_pcBehavior->AddState((CBaseBehavior*)(new CFloatBehavior())/*, true*/);
			_pcBehavior->AddState((CBaseBehavior*)(new CExhaustBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CResetBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CEscapeBehavior()));

			//_pcBehavior->GetIsChasing() = false;
			//SetFSM(_pcBehavior);

			SetMaxHealth(50);
		}
		else if(this->GetWizardSubtype() == SECOND_STAGE)
		{
			//CFSM* _pcBehavior = new CFSM();
			//_pcBehavior->SetAgent(this);
			_pcBehavior->AddState((CBaseBehavior*)(new CFloatBehavior())/*, true*/);
			_pcBehavior->AddState((CBaseBehavior*)(new CExhaustBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CRangedBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CResetBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CEscapeBehavior()));

			//_pcBehavior->GetIsChasing() = false;
			//SetFSM(_pcBehavior);

			SetMaxHealth(50);
			//SetWizardForm(2);

		}
		else if(this->GetWizardSubtype() == THIRD_STAGE)
		{
			//CFSM* _pcBehavior = new CFSM();
			//_pcBehavior->SetAgent(this);

			_pcBehavior->AddState((CBaseBehavior*)(new CAlignBehavior())/*, true*/);
			_pcBehavior->AddState((CBaseBehavior*)(new CHyperdashBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CResetBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CEscapeBehavior()));
			//_pcBehavior->AddState((CBaseBehavior*)(new CRangedBehavior()));

			//_pcBehavior->GetIsChasing() = false;
			//SetFSM(_pcBehavior);

			SetMaxHealth(50);
		}
		else
		{
			//CFSM* _pcBehavior = new CFSM();
			//_pcBehavior->SetAgent(this);

			_pcBehavior->AddState((CBaseBehavior*)(new CFloatBehavior())/*, true*/);
			_pcBehavior->AddState((CBaseBehavior*)(new CExhaustBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CHyperOrbBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CResetBehavior()));
			_pcBehavior->AddState((CBaseBehavior*)(new CEscapeBehavior()));

			//_pcBehavior->GetIsChasing() = false;
			//SetFSM(_pcBehavior);

			SetMaxHealth(200);
		}
		_pcBehavior->GetIsChasing() = false;
		SetFSM(_pcBehavior);
	}

	if(this->GetWizardSubtype() != FINAL_BOSS)
	{
		SetHealth(GetMaxHealth());
	}
	else
	{
		m_fTotalHealth = GetMaxHealth();
		SetHealth(100);
	}
	
	SetAttackSpeed(1.25);
	SetMoveSpeed(7.5f);
	SetDamage(30);
	SetExhausted(false);

	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));
	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "Wizard/Wizard_Cast.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Wizard/Wizard_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "Wizard/Wizard_Rest.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "Wizard/Wizard_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_A, "Wizard/Wizard_TrapSummon.anm");
	
	GetAnimationPlayer()->PlayAnimation(ANIM_IDLE, true); // PLAY IDLE

	//GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_B, );
	//GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "Wizard/Wizard_Movement.anm");

	vec3f pos = this->GetPos();
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_y_pre(D3DXToRadian(180));
	worldMatrix.scale_pre(GetScale().x * 1.25f, GetScale().y * 1.25f, GetScale().z * 1.25f);
	SetWorldMatrix(worldMatrix);
	SetPos(pos);
	CView::AttachLight(GetRenderID(), vec4f(1, 1, 1, 1), 4.0f, vec4f(0.25f, 0.25f, 0.25f, 1.0f),
		vec4f(0, 0, 0, 0), 0, 0, vec3f(0.1f, 2.5f, 0.0f), vec3f(-0.2f, 3, 0.0f));

	m_bIsAlive = true;
	m_bDefeated = false;

	// Create the Health Display
	m_nShapeID = CView::DynamicShapeCreate(GetRenderID(), false, vec3f(1.5f, 1.0f, 1.5f));
	CView::DynamicShapeMakePlane(m_nShapeID, 3.0f, 3.0f);
	matrix4f position;

	position.make_identity();
	position.rotate_y_post(3.0f * D3DX_PI / 4.0f);
	position.scale_post(vec3f(2.0f, 0.0f, 2.0f));
	position.translate_post(0.0f, 1.0f, 0.0f);

	CView::DynamicShapeSetLocalMatrix(m_nShapeID, position);
	CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_4.png"), -1);

	this->m_pcCurrLevel = nullptr;

	m_vpcWizardSpawners.clear();

	if(!m_pcBubbleBurstEmitter)
	{
		m_pcBubbleBurstEmitter = CView::GetEmitterManager().AddEmitter(false,false,GetPos(),"Resource/Particles/WizardBubbleBurst.txt",false,this);
	}
	if(!m_pcBubbleSheenEmitter)
	{
		m_pcBubbleSheenEmitter = CView::GetEmitterManager().AddEmitter(true,false,GetPos(),"Resource/Particles/WizardBubbleSheen.txt",false,this);
	}
	if(!m_pcCrashEmitter)
	{
		m_pcCrashEmitter = CView::GetEmitterManager().AddEmitter(false,false,GetPos(),"Resource/Particles/WizardCrashLand.txt",false,this);
	}
	if(GetWizardSubtype() == FINAL_BOSS)
	{
		//if(!m_pcDeathBurstEmitter)
		//{
		//	m_pcDeathBurstEmitter = CView::GetEmitterManager().AddEmitter(false,true,GetPos(),"Resource/Particles/WizardDeath.txt",true);
		//}
		if(!m_pcDeathFlowEmitter)
		{
			m_pcDeathFlowEmitter = CView::GetEmitterManager().AddEmitter(true,false,GetPos(),"Resource/Particles/WizardDeath.txt",false,this);
		}
	}
	else
	{
		m_pcDeathBurstEmitter = nullptr;
		m_pcDeathFlowEmitter = nullptr;
	}
	if(!m_pcSmokeEmitter)
	{
		m_pcSmokeEmitter = CView::GetEmitterManager().AddEmitter(true,false,GetPos(),"Resource/Particles/WizardSmokeTrail.txt",false,this);
	}
	if(!m_pcTakeoffEmitter)
	{
		m_pcTakeoffEmitter = CView::GetEmitterManager().AddEmitter(false,false,GetPos(),"Resource/Particles/WizardTakeOff.txt",false,this);
	}
	if(!m_pcSpawnEmitter)
	{
		m_pcSpawnEmitter = CView::GetEmitterManager().AddEmitter(false,false,GetPos(),"Resource/Particles/SpawnerBlocks.txt",false,this);
	}
	this->m_pcFireSpawned = nullptr;
	this->m_pcIceSpawned = nullptr;
	m_fDeathTimeLimit = FLT_MAX-100.0f;
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
void CWizardEnemy::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();

	//SetHealth(GetMaxHealth());
	SetMoveSpeed(7.5f);
	SetDamage(20);
	SetWizardForm(1);
	SetExhausted(false);
	m_bEscaped = false;
	m_bDefeated = false;
	this->m_pcCurrLevel = nullptr;
	m_fFinalDeathTimer = 0.0f;
	m_bDeathCharging = false;
	m_bDeathExloding = false;
	m_bDeathExploded = false;
	this->m_pcFireSpawned = nullptr;
	this->m_pcIceSpawned = nullptr;
	m_fDeathTimeLimit = FLT_MAX-100.0f;
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
* Mod. Date:		    12/1/2012
* Mod. Initials:	    JM
*****************************************************************/
void CWizardEnemy::Update(float fElapsedTime)
{
	if(!GetInitialized()){		
		return;
	}

	if(this->m_nWizardSubType == FINAL_BOSS && !GetIsDying())
	{
		if(GetHealth() <= 0)
		{
			SetHealth(100);
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_4.png"), -1);
			IncrementPhase();
		}		
	}
	
	if(GetFSM() && (GetFSM()->GetCurrBehavior()->GetBehaviorType() == ESCAPE_BEHAVIOR || GetFSM()->GetCurrBehavior()->GetBehaviorType() == GLOAT_BEHAVIOR ))
	{
		fElapsedTime = m_tWizTimer.GetElapsedTime();
	}

	if (m_pcBubble)
	{
		m_pcBubble->SetTransparency((GetHealth() / GetMaxHealth()) * 0.34f + 0.1f);
	}

	if (GetIsShaking())
	{
		SetShakeTimer(GetShakeTimer() + fElapsedTime);
		if (GetShakeTimer() >= 2.0f)
		{
			SetIsShaking(false);
			SetShakeTimer(0.0f);
		}
		else
			ShakeObject();
	}

	if(m_bEscaped)
	{
		m_bEscaped = false;
		m_bDefeated = false;
		Uninitialize();
	}

	if(this->GetHitCooldown() >= 0.7f)
	{
		this->SetIsHit(false);
		this->SetHitCooldown(0.0f);
	}

	if(this->GetIsHit())
	{
		this->SetHitCooldown(this->GetHitCooldown() + fElapsedTime);
	}

	if(m_pcBubble)
	{
		vec3f vPos = GetPos();
		vPos.y += 3.0f;
		m_pcBubble->SetPos(vPos);
	}

	m_pPlayer = CWorldManager::GetInstance()->GetObjectManager()->GetPlayer();

	CEnemy::Update(fElapsedTime);
	if(!GetInitialized())
		return;

	if(GetFSM()/*->GetCurrBehavior()->GetBehaviorType() == ESCAPE_BEHAVIOR*/)
	{
		GetFSM()->SetCurTime(fElapsedTime);
	}

	if(!this->GetInitialized())
		return;

	if (GetElementalState() >= HOT || GetElementalState() <= COLD)
	{
		SetElementalState(NEUTRAL);
	}

	int nRand = (rand() % 100) + 1;
	//m_fAttackCooldown += fElapsedTime;

	SetPos(GetPos() + GetVelocity() * fElapsedTime);
	if(!GetIsDying())
		UpdateCollisionData();

	if(!GetFSM()->GetIsChasing())
		return;

	

	//CFireTrap* pcFire = nullptr;
	//CIceTrap* pcIce = nullptr;
	//
	//vec3f vPlayerPos = m_pPlayer->GetPos();
	//
	//if(this->GetWizardSubtype() == FINAL_BOSS)//GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERORB_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != EXHAUST_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != ALIGN_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != RANGED_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERDASH_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERORB_BEHAVIOR)
	//{
	//	m_fTrapSpawnTimer += fElapsedTime;
	//}
	//else
	//{
	//	m_fTrapSpawnTimer = 0.0f;
	//}
	//
	//
	////depricated for beta turn in
	//if( m_fTrapSpawnTimer > 5.0f && this->GetWizardSubtype() == FINAL_BOSS && this->GetWizardForm() == 1)// (GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERORB_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != EXHAUST_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != ALIGN_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != RANGED_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERDASH_BEHAVIOR && GetFSM()->GetCurrBehavior()->GetBehaviorType() != HYPERORB_BEHAVIOR))
	//{
	//	vec4f vDiff;
	//	int nLightID = -1;
	//	//vec4f vDiff;
	//	int nTrapType = nRand % 2;
	//
	//	if(nTrapType)
	//	{
	//		vDiff = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	//		//pcFire = (CFireTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
	//	}
	//
	//	else
	//	{
	//		vDiff = vec4f(0.0f, 0.0f, 1.0f, 1.0f);
	//		//pcIce = (CIceTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
	//	}
	//
	//	nLightID = CView::CreateWorldLight(vPlayerPos, vDiff);
	//	vPlayerPos.y -= 3.0f;
	//
	//	if(nTrapType)
	//	{
	//		pcFire = (CFireTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
	//		pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(-2.0f,0,-2.0f), nLightID);
	//		pcFire = (CFireTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
	//		pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(2.0f,0,-2.0f), nLightID);
	//		pcFire = (CFireTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
	//		pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(2.0f,0,2.0f), nLightID);
	//		pcFire = (CFireTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(FIRETRAP_OBJECT);
	//		pcFire->SummonTrap(1.0f, vPlayerPos + vec3f(-2.0f,0,2.0f), nLightID);
	//	}
	//
	//	else// if(pcIce)
	//	{
	//		pcIce = (CIceTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
	//		pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(-1,0,-1), nLightID);
	//		pcIce = (CIceTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
	//		pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(1,0,-1), nLightID);
	//		pcIce = (CIceTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
	//		pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(1,0,1), nLightID);
	//		pcIce = (CIceTrap*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(ICETRAP_OBJECT);
	//		pcIce->SummonTrap(0.5f, vPlayerPos + vec3f(-1,0,1), nLightID);
	//	}
	//
	//
	//
	//	m_fTrapSpawnTimer = 0.0f;
	//	//m_pPlayer->SetIdleTime(0.0f);
	//}

	// Update the Shadow
	if(GetRenderID() != -1 && m_nShadowRenderID != -1)
	{
		matrix4f mShadowMatrix;
		mShadowMatrix.make_identity();
		mShadowMatrix.axis_pos.y -= GetPos().y - 0.75f;
		//mShadowMatrix.make_translation(GetPos().x,CWorldManager::GetInstance()->GetGroundHeight() + 0.5f, GetPos().z);
		CView::DynamicShapeSetLocalMatrix(m_nShadowRenderID, mShadowMatrix);
	}

	if(m_fTotalHealth <=0 && this->m_nWizardSubType == FINAL_BOSS)
	{
		if(m_fFinalDeathTimer == 0.0f){
			CView::GetCamera().SetPlayer(this);
			CInput::GetInstance()->DisablePlayerInput();
			CView::DynamicShapeRemove(m_nShapeID);
		}

		if(m_fFinalDeathTimer >= m_fDeathTimeLimit +1.0f && !m_bDeathExploded)
		{
			m_bDeathExploded = true;
			//if (m_pcBubble)
			//{
			//	CView::Pop(m_pcBubble->GetRenderID());
			//}
			//if (m_nShapeID)
			//{
				
			//}
			
			return;
		}
		else if(m_fFinalDeathTimer >= m_fDeathTimeLimit && !m_bDeathExloding)
		{
			//m_pcDeathBurstEmitter->SetPos(GetPos());
			//m_pcDeathBurstEmitter->SetEmittPos(GetPos());
			//m_pcDeathBurstEmitter->SetActive(true);
			GetAnimationPlayer()->PlayAnimationToEnd(ANIM_DEATH);
			CView::GetEmitterManager().AddEmitter(false,true,GetPos(),"Resource/Particles/WizardDeath.txt",true);
			m_pcDeathFlowEmitter->SetActive(false);
			m_bDeathExloding = true;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARD_EXPLOSION, GAME_OBJECT_ID_PLAYER);
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_DEATH, GAME_OBJECT_ID_PLAYER);
			CAchievementManager::GetInstance()->ActivateAchievement("Champion");
			//CView::SetIsVisible(GetRenderID(),false);
			m_bIsAlive = false;
			CWorldManager::GetInstance()->SetSectionComplete(true);
			Uninitialize();
			CGame::GetInstance()->SetVictory(true);
			CGameState::GetInstance()->SetPortalZoomOn(true);
		}
		else if(!m_bDeathCharging)
		{
			//this->m_pcDeathFlowEmitter->SetActive(true);
			m_bDeathCharging = true;
			CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_WIZARTTAUNT_IMPOSSIBLE, GAME_OBJECT_ID_PLAYER);
			GetFSM()->TransitionTo(EXHAUST_BEHAVIOR);
			// record the time required to explode all the enemies on screen
			m_fDeathTimeLimit = (float)(CWorldManager::GetInstance()->PopCurrentLevelEnemies(0.2f) * 0.2f);
			// the death taunt takes 4 seconds, so if the deathTimeLimit is less than 4 seconds, extend to 4 seconds
			if(m_fDeathTimeLimit < 4.0f)
			{
				m_fDeathTimeLimit = 4.0f;
			}
		}
		m_fFinalDeathTimer += fElapsedTime;

	}
	else if(this->GetWizardSubtype() != FINAL_BOSS && GetHealth() <= 0 && !GetIsDying())
	{
		m_bIsAlive = false;
	}

}

void CWizardEnemy::Uninitialize(void)
{
	if(GetInitialized())
	{
		this->m_pcCurrLevel = nullptr;

		if(GetRenderID() != -1)
		{
			CView::DynamicShapeRemove(m_nShadowRenderID);
			m_nShadowRenderID = -1;

			CView::DynamicShapeRemove(m_nShapeID);
			m_nShapeID = -1;
		}

		CCollidableObject::Uninitialize();

		if (GetFSM())
		{
			CFSM* tempBehavior = GetFSM();
			delete tempBehavior;
			SetFSM(nullptr);
			tempBehavior = nullptr;
		}

		if(this->m_pcBubble != nullptr)
		{
			m_pcBubble->Uninitialize();
			this->m_pcBubble = nullptr;
		}
	}
	this->SetInitialized(false);
	m_vpcWizardSpawners.clear();

	if(m_pcBubbleBurstEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcBubbleBurstEmitter);
		m_pcBubbleBurstEmitter = nullptr;
	}
	if(m_pcBubbleSheenEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcBubbleSheenEmitter);
		m_pcBubbleSheenEmitter = nullptr;
	}
	if(m_pcCrashEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcCrashEmitter);
		m_pcCrashEmitter = nullptr;
	}
	if(m_pcDeathBurstEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcDeathBurstEmitter);
		m_pcDeathBurstEmitter = nullptr;
	}
	if(m_pcDeathFlowEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcDeathFlowEmitter);
		m_pcDeathFlowEmitter = nullptr;
	}
	if(m_pcSmokeEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcSmokeEmitter);
		m_pcSmokeEmitter = nullptr;
	}
	if(m_pcTakeoffEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcTakeoffEmitter);
		m_pcTakeoffEmitter = nullptr;
	}
	if(m_pcSpawnEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcSpawnEmitter);
		m_pcSpawnEmitter = nullptr;
	}
	this->m_pcFireSpawned = nullptr;
	this->m_pcIceSpawned = nullptr;

}

void CWizardEnemy::OnAttack(int nDamage,int nElementType)
{
	CEnemy::OnAttack(nDamage, nElementType);
	CView::FlashRenderInstance( ((CWizardEnemy*)this)->GetBubble()->GetRenderID(), vec4f(1,1,1,1));
	this->m_fTotalHealth -= nDamage;

	if(GetWizardSubtype() != FINAL_BOSS)
	{
		if(GetHealth() > GetMaxHealth() * 0.75f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_4.png"), -1);
		}
		else if(GetHealth() > GetMaxHealth() * 0.5f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_3.png"), -1);
		}
		else if(GetHealth() > GetMaxHealth() * 0.25f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_2.png"), -1);
		}
		else if(GetHealth() > 0.0f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_1.png"), -1);
		}
		else
		{
			if(GetRenderID() != -1)
			{
				CView::DynamicShapeRemove(m_nShapeID);
				m_nShapeID = -1;
			}
		}
	}
	else
	{
		if(GetHealth() > 75.0f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_4.png"), -1);
		}
		else if(GetHealth() > 50.0f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_3.png"), -1);
		}
		else if(GetHealth() > 25.0f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_2.png"), -1);
		}
		else if(GetHealth() > 0.0f)
		{
			CView::DynamicShapeSetTextures(m_nShapeID, CView::LoadSprite("pip_placeholder_1.png"), -1);
		}
	}
}


void CWizardEnemy::SpawnEnemies()
{
	//return;
	if(this->GetWizardSubtype() != FINAL_BOSS)
		return;
	
	this->SetSpawnEmitterActive(true);

	CObjectManager * pObjectMan = GetFSM()->GetObjectManager();

	if(!pObjectMan)
		return;

	if(!m_pcCurrLevel)
	{
		CWorldManager * pWorldMan = CWorldManager::GetInstance();
		m_pcCurrLevel = pWorldMan->GetLevel(pWorldMan->GetLevelForObject(this));
	}

	CEnemy * pEnemy = nullptr;
	vec3f levelPos = vec3f(m_pcCurrLevel->GetCenterX(), 0.0, m_pcCurrLevel->GetCenterZ());
	vec3f localPos = GetPos();
	vec3f finalPos = vec3f(localPos.x, localPos.y, localPos.z - levelPos.z);

	vec3f posModifiers[8];
	posModifiers[0] = vec3f(3.0f,0.0f,0.0f);
	posModifiers[1] = vec3f(-3.0f,0.0f,0.0f);
	posModifiers[2] = vec3f(0.0f,0.0f,3.0f);
	posModifiers[3] = vec3f(0.0f,0.0f,-3.0f);
	posModifiers[4] = vec3f(-1.7f,0.0f, 1.7f);
	posModifiers[5] = vec3f(-1.7f,0.0f,-1.7f);
	posModifiers[6] = vec3f( 1.7f,0.0f,-1.7f);
	posModifiers[7] = vec3f( 1.7f,0.0f, 1.7f);

	switch (this->GetWizardForm())
	{
	case 1: // spawn 8 melee enemies
		{
			for(unsigned int i = 0; i < 8;i++)
			{
				pEnemy = (CEnemy*)m_pcCurrLevel->PushEnemy(finalPos+posModifiers[i],MELEE_OBJECT);
				if(pEnemy)
				{
					pEnemy->GetFSM()->GetIsChasing() = true;
				}
				else
				{
					return;
				}
			}
			break;
		}
	case 2: // spawn 8 melee and 4 explosive
		{
			for(unsigned int i = 0; i < 8;i++)
			{
				pEnemy = (CEnemy*)m_pcCurrLevel->PushEnemy(finalPos+posModifiers[i],MELEE_OBJECT);
				if(pEnemy)
				{
					pEnemy->GetFSM()->GetIsChasing() = true;
				}
				else
				{
					return;
				}
			}

			//for(unsigned int i = 0; i < 4; i ++)
			//{
			//	pEnemy = (CEnemy*)m_pcCurrLevel->PushEnemy(finalPos+posModifiers[i+4],EXPLOSIVE_OBJECT,((rand()%2)*2)-1);
			//	if(pEnemy)
			//	{
			//		pEnemy->GetFSM()->GetIsChasing() = true;
			//	}
			//	else
			//	{
			//		return;
			//	}
			//}

			break;
		}
	case 3: // spawn 1 fire and 1 ice
		{
			if(!m_pcFireSpawned || !m_pcFireSpawned->GetInitialized())
			{
				m_pcFireSpawned = (CFireEnemy*) m_pcCurrLevel->PushEnemy(finalPos+vec3f(0.0f,0.0f,-4.0f),FIRE_OBJECT);
				if(m_pcFireSpawned)
				{
					m_pcFireSpawned->GetFSM()->GetIsChasing() = true;
				}
			}
			if(!m_pcIceSpawned || !m_pcIceSpawned->GetInitialized())
			{
				m_pcIceSpawned = (CIceEnemy*)m_pcCurrLevel->PushEnemy(finalPos+vec3f(4.0f,0.0f,0.0f),ICE_OBJECT);
				if(m_pcIceSpawned)
				{
					m_pcIceSpawned->GetFSM()->GetIsChasing() = true;
				}
			}
			break;
		}
	case 4: // spawn 1 fire and 1 ice and 8 melee
		{
			for(unsigned int i = 0; i < 8;i++)
			{
				pEnemy = (CEnemy*)m_pcCurrLevel->PushEnemy(finalPos+posModifiers[i],MELEE_OBJECT);
				if(pEnemy)
				{
					pEnemy->GetFSM()->GetIsChasing() = true;
				}
				else
				{
					return;
				}
			}

			if(!m_pcFireSpawned || !m_pcFireSpawned->GetInitialized())
			{
				m_pcFireSpawned = (CFireEnemy*) m_pcCurrLevel->PushEnemy(finalPos+vec3f(0.0f,0.0f,-4.0f),FIRE_OBJECT);
				if(m_pcFireSpawned)
				{
					m_pcFireSpawned->GetFSM()->GetIsChasing() = true;
				}
			}
			if(!m_pcIceSpawned || !m_pcIceSpawned->GetInitialized())
			{
				m_pcIceSpawned = (CIceEnemy*)m_pcCurrLevel->PushEnemy(finalPos+vec3f(4.0f,0.0f,0.0f),ICE_OBJECT);
				if(m_pcIceSpawned)
				{
					m_pcIceSpawned->GetFSM()->GetIsChasing() = true;
				}
			}

			break;
		}

	};
}

void CWizardEnemy::CrystalDisabled()
{
	this->GetFSM()->TransitionTo(EXHAUST_BEHAVIOR);
	for(unsigned int i = 0; i < m_vpcWizardSpawners.size();i++)
	{
		if(m_vpcWizardSpawners[i] == nullptr){
			continue;
		}
		//CView::SetRenderInstanceFlags(m_vpcWizardSpawners[i]->GetBubble()->GetRenderID(), RENDER_FULLY_TRANSLUCENT | RENDER_NOT_SHADOWCASTER | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		CView::SetIsVisible(m_vpcWizardSpawners[i]->GetBubble()->GetRenderID(), true);
		m_vpcWizardSpawners[i]->m_bIsDestructable = false;
	}
}

void CWizardEnemy::ReactivateCrystals()
{
	for(unsigned int i = 0; i < m_vpcWizardSpawners.size();i++)
	{
		if(m_vpcWizardSpawners[i] == nullptr)
			continue;

		//CView::SetRenderInstanceFlags(m_vpcWizardSpawners[i]->GetBubble()->GetRenderID(),RENDER_INVISIBLE);
		//m_vpcWizardSpawners[i]->m_bIsDestructable = true;
		m_vpcWizardSpawners[i]->m_bShieldCooldown = true;		
		m_vpcWizardSpawners[i]->SetHealth(m_vpcWizardSpawners[i]->GetMaxHealth());
	}

	// reset phase to what it was in last
	if(GetWizardForm() >= 5)
	{
		SetIsDying(true);
		SetHealth(0);
	}
	else if( GetFSM()->GetCurrBehavior()->GetBehaviorType() != GLOAT_BEHAVIOR)
	{
		GetFSM()->TransitionTo(FLOAT_BEHAVIOR);
	}
}

void CWizardEnemy::IncrementPhase()
{
	// should be called from wizard's update if he was dealth enough damage

	// uninit one that is dead and set to nullptr
	for(unsigned int i = 0; i < m_vpcWizardSpawners.size();i++)
	{
		if(m_vpcWizardSpawners[i] == nullptr)
			continue;
		if(m_vpcWizardSpawners[i]->GetHealth() <= 0)
		{
			CView::GetEmitterManager().AddEmitter( false, true, m_vpcWizardSpawners[i]->GetPos(), "Resource/Particles/SpawnerBlocks.txt", true);
			CView::GetEmitterManager().AddEmitter(false, true, m_vpcWizardSpawners[i]->GetPos(), "Resource/Particles/SpawnerShatter.txt", true);
			m_vpcWizardSpawners[i]->Uninitialize();
			m_vpcWizardSpawners[i] = nullptr;
		}
	}
	// set wizard to move on to next stage
	this->m_fTotalHealth = 400.0f - (GetWizardForm() * 100.0f);
	this->SetWizardForm(GetWizardForm() + 1);
	ReactivateCrystals();

	//GetFSM()->TransitionTo(FLOAT_BEHAVIOR);
}

void CWizardEnemy::PushBackWizardSpawner(CSpawner * pcSpawner)
{
	this->m_vpcWizardSpawners.push_back(pcSpawner);
}

void CWizardEnemy::ClearWizardSpawners()
{
	this->m_vpcWizardSpawners.clear();
}


void CWizardEnemy::SetBubbleBurstEmitterActive(bool bActive)
{
	if(this->m_pcBubbleBurstEmitter)
	{
		m_pcBubbleBurstEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetBubbleSheenEmitterActive(bool bActive)
{
	if(this->m_pcBubbleSheenEmitter)
	{
		m_pcBubbleSheenEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetCrashEmitterActive(bool bActive)
{
	if(this->m_pcCrashEmitter)
	{
		m_pcCrashEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetDeathBurstEmitterActive(bool bActive)
{
	if(this->m_pcDeathBurstEmitter)
	{
		m_pcDeathBurstEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetDeathFlowEmitterActive(bool bActive)
{
	if(this->m_pcDeathFlowEmitter)
	{
		m_pcDeathFlowEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetSmokeEmitterActive(bool bActive)
{
	if(this->m_pcSmokeEmitter)
	{
		m_pcSmokeEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetTakeoffEmitterActive(bool bActive)
{
	if(this->m_pcTakeoffEmitter)
	{
		m_pcTakeoffEmitter->SetActive(bActive);
	}
}

void CWizardEnemy::SetSpawnEmitterActive(bool bActive)
{
	if(this->m_pcSpawnEmitter)
	{
		m_pcSpawnEmitter->SetActive(bActive);
	}
}

