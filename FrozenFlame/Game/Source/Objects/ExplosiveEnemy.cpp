/***********************************************
 * Filename:  		ExplosiveEnemy.cpp
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the explosive enemy object
 ************************************************/

#include "ExplosiveEnemy.h"
#include "../Renderer/View.h"
#include "../AI/Header/CFSM.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RenderInstanceAnimated.h"
#include "../AI/Header/HomingBehavior.h"
#include "../Core/CWorldManager.h"
#include "../Renderer/CEmitterManager.h"


CExplosiveEnemy::CExplosiveEnemy(void)
{
	SetType(EXPLOSIVE_OBJECT);
}


CExplosiveEnemy::~CExplosiveEnemy(void)
{
	if (m_pcEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcEmitter, false);
		m_pcEmitter = nullptr;
	}
	if (m_pcSmoke)
	{
		CView::GetEmitterManager().KillEmitter(m_pcSmoke, false);
		m_pcSmoke = nullptr;
	}
}


void CExplosiveEnemy::Initialize(void)
{
	CEnemy::Initialize();

	SetExploding(false);
	m_pcEmitter = nullptr;
	m_pcSmoke = nullptr;

	CFSM *_pcBehavior = new CFSM();
	_pcBehavior->SetAgent(this);
	_pcBehavior->AddState((CBaseBehavior*)(new CHomingBehavior()), true);
	_pcBehavior->AddState((CBaseBehavior*)(new CWanderBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CMeleeBehavior()));
	_pcBehavior->AddState((CBaseBehavior*)(new CRushBehavior()));

	SetFSM(_pcBehavior);

	if(GetRenderID() == -1)
	{
		SetRenderID(CView::Push(this, "Minion", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	}

	// Load lua variables
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "EMHealthPoints");
	SetMaxHealth(static_cast<float>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));
	SetHealth(GetMaxHealth());

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "EMDamage");
	SetDamage(static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));

	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "EMAmmountToDrop");
	SetAmountToDrop(static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));
	lua_getglobal(CWorldManager::GetInstance()->GetluaState(), "EMPercnetageHealthChance");
	SetHealthChance(static_cast<int>(lua_tointeger(CWorldManager::GetInstance()->GetluaState(), -1)));

	// Pop the lua stack
	lua_pop(CWorldManager::GetInstance()->GetluaState(), lua_gettop(CWorldManager::GetInstance()->GetluaState()));

	SetAttackSpeed(2.5);
	SetAttackElement(ICE);
	SetMoveSpeed(10.25);
	SetIsDamageDealt(false);

	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));

	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "Minion/Minion_ExplosiveAttack.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Minion/Minion_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "Minion/Minion_Movement.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "Minion/Minion_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "Minion/Minion_Damaged.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_SPECIAL_A, "Minion/Minion_ExplosiveExplode.anm");

	m_bScaleChanged = false;

	m_fExplosionScale = 1.0f;

	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_y_pre(D3DXToRadian(180));
	worldMatrix.scale_pre(GetScale().x * 0.75f, GetScale().y * 0.75f, GetScale().z * 0.75f);
	SetWorldMatrix(worldMatrix);

	if(!m_pcSmoke)
	{
		m_pcSmoke = CView::GetEmitterManager().AddEmitter(true, false, GetPos(), "Resource/Particles/Smoke1.txt",
		false, this, nullptr);
		m_pcSmoke->SetDestBlend(D3DBLEND_DESTCOLOR);
		m_pcSmoke->SetSourceBlend(D3DBLEND_SRCCOLOR);
	}

	//for(int x = 0; x < 10000; ++x)
	//{
	//
	//}
	
	//float fOffset = float( rand()%51 / 100.0f);
	//this->SetTimerOffset(fOffset);
}

void CExplosiveEnemy::SetAttackElement(int _nAttackElement)
{
	// Call the parent's SetAttackElement() function to set the variable
	//_nAttackElement = ICE;
	CEnemy::SetAttackElement(_nAttackElement);
	//CEnemy::SetAttackElement(FIRE);

	// Depending on the attack element let's push a fire or ice canister
	// as an attachment to this enemy's render instance
	switch(_nAttackElement)
	{
	case ICE:
		m_nBombID = CView::PushAttachment(GetRenderID(), "CanisterIce", "Back", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		break;
	case FIRE:
		m_nBombID = CView::PushAttachment(GetRenderID(), "CanisterFire", "Back", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		break;
	default:
		m_nBombID = CView::PushAttachment(GetRenderID(), "Canister", "Back", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
	}


}


void CExplosiveEnemy::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}


void CExplosiveEnemy::Update(float fElapsedTime)
{
	if(!GetInitialized())
	{
		return;
	}

	m_bScaleChanged = false;

	CEnemy::Update(fElapsedTime);
	if(!GetInitialized())
		return;

	if (GetInitialized())
	{
		const CRenderInstance* pRenderInst = CView::GetRenderInstance(GetRenderID());

		if(pRenderInst && pRenderInst->GetType() == IRenderThing::ANIMATED)
		{
			const D3DXMATRIX* bombMat = &CView::GetAttachmentInstance(-1, m_nBombID)->GetWorldTransform();
			//const D3DXMATRIX* fireSwordMat = 
			vec3f canisterPos = vec3f(bombMat->_41, bombMat->_42 + 3.2f, bombMat->_43);

			vec3f offset = canisterPos - GetPos();
			m_pcSmoke->SetOffset(offset);

		}
	}
	if(GetFSM() && GetFSM()->GetCurrBehavior() && GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
	{
		if(GetHealth() > 0.0f)
		{

			if(((CRushBehavior*)GetFSM()->GetCurrBehavior())->GetIsBombLit())
			{
				m_pcSmoke->SetActive(true);
				//m_pcEmitter->SetTransparency(
			}

			else
			{
				m_pcSmoke->SetActive(false);
			}

			if(((CRushBehavior*)GetFSM()->GetCurrBehavior())->GetIsBombLit() && m_fExplosionScale < 2.5f)
			{
				m_fExplosionScale += 0.0025f;//(0.3f * fElapsedTime);
				//matrix4f worldMatrix = *GetWorldMatrix();
				//worldMatrix.scale_post(m_fExplosionScale, m_fExplosionScale, m_fExplosionScale);
				//SetWorldMatrix(worldMatrix);
				m_bScaleChanged = true;
			}

			else if(!((CRushBehavior*)GetFSM()->GetCurrBehavior())->GetIsBombLit())
			{
				if(m_fExplosionScale > 1.0f)
				{
					m_fExplosionScale -= 0.01f;//(0.5f * fElapsedTime);
					//matrix4f worldMatrix = *GetWorldMatrix();
					//worldMatrix.scale_post(m_fExplosionScale, m_fExplosionScale, m_fExplosionScale);
					//SetWorldMatrix(worldMatrix);
					m_bScaleChanged = true;
				}

				if(m_fExplosionScale < 1.0f)
				{
					m_fExplosionScale = 1.0f;
					m_bScaleChanged = true;
				}
			}
		}

		else
		{
			if(m_fExplosionScale > 1.0f)
			{
				m_fExplosionScale -= 0.01f;//(0.5f * fElapsedTime);
				//matrix4f worldMatrix = *GetWorldMatrix();
				//worldMatrix.scale_post(m_fExplosionScale, m_fExplosionScale, m_fExplosionScale);
				//SetWorldMatrix(worldMatrix);
				m_bScaleChanged = true;
			}

			if(m_fExplosionScale < 1.0f)
			{
				m_fExplosionScale = 1.0f;
				m_bScaleChanged = true;
			}
		}
	}

	if(m_bScaleChanged)
	{
		vec3f scale;
		
		scale.x = 1.0f/sqrt(dot_product(GetWorldMatrix()->axis_x,GetWorldMatrix()->axis_x));
		scale.y = 1.0f/sqrt(dot_product(GetWorldMatrix()->axis_y,GetWorldMatrix()->axis_y));
		scale.z = 1.0f/sqrt(dot_product(GetWorldMatrix()->axis_z,GetWorldMatrix()->axis_z));

		matrix4f worldMatrix = *GetWorldMatrix();
		worldMatrix.scale_post(GetScale() * scale);
		SetWorldMatrix(worldMatrix);

		worldMatrix.scale_post(vec3f(GetScale().x * m_fExplosionScale, GetScale().y * m_fExplosionScale, GetScale().z * m_fExplosionScale));
		//worldMatrix.scale_post(m_fExplosionScale, m_fExplosionScale, m_fExplosionScale);
		SetWorldMatrix(worldMatrix);
	}

	
	switch (GetElementalState())
	{
	case FROZEN:
		{
			//m_pcBehavior->TransitionTo();
			if (GetAttackElement() == FIRE && GetFSM())
			{
				//SetAttackSpeed(0.0f);
				if (GetFSM()->GetCurrBehavior() && GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
				{
					((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
					((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(0.0f);
					//GetFSM()->TransitionTo(WANDER_BEHAVIOR);
					SetColor(vec3f(1.0f, 1.0f, 1.0f));
					GetFSM()->GetCooldown() = true;
				}
			}

			if (GetAttackElement() == ICE && GetFSM())
			{
				//SetAttackSpeed(0.0f);
				if (GetFSM()->GetCurrBehavior() && GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
				{
					//((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
					//GetFSM()->GetCooldown() = true;
					//SetExploding(true);
					((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(10.0f);
				}
			}
			//else
			//	SetColor(vec3f(0.0f, 0.0f, 1.0f));
		}
		break;

	case COLD:
		{
			if (GetAttackElement() == FIRE)
			{
				if (!GetIsDebuffSet())
				{
					SetMoveSpeed(GetMoveSpeed() / 2);
					SetIsDebuffSet(true);
				}
				if (GetFSM() && GetFSM()->GetCurrBehavior())
				{
					if (GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
					{
						((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
						((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(0.0f);
						//GetFSM()->TransitionTo(WANDER_BEHAVIOR);
						SetColor(vec3f(1.0f, 1.0f, 1.0f));
						GetFSM()->GetCooldown() = true;
					}
				}
				// SetColor(vec3f(0.2f, 0.2f, 0.5f));
			}
			else if (GetAttackElement() == ICE)
			{
				if (!GetIsDebuffSet())
				{
					SetMoveSpeed(GetMoveSpeed() * 2);
					SetIsDebuffSet(true);
				}

				if (GetFSM())
				{
					if (GetFSM()->GetCurrBehavior() && GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
					{
						((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(true);
						//GetFSM()->GetCooldown() = true;
					}
				}
			}
		}
		break;

	case HOT:
		{
			//if (GetDamageTimer() >= DAMAGE_COOLDOWN * 3.0f)
			//{
				if (GetAttackElement() == ICE)
				{
					if (GetFSM() && GetFSM()->GetCurrBehavior())
					{
						((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
						((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(0.0f);
						//GetFSM()->TransitionTo(WANDER_BEHAVIOR);
						SetColor(vec3f(1.0f, 1.0f, 1.0f));
						GetFSM()->GetCooldown() = true;
					}

					//SetHealth(GetHealth() - 1);
					// do not want damage images in right now
					//AddDamageImage(1);
					//SetDamageTimer(0.0);
				}
		//	}

			if (GetAttackElement() == ICE)
			{
				if (!GetIsDebuffSet())
				{
					SetMoveSpeed(GetMoveSpeed() / 2.0f);
					SetIsDebuffSet(true);
				}
				// SetColor(vec3f(0.5f, 0.2f, 0.0f));
			}
			else if (GetAttackElement() == FIRE)
			{
				if (!GetIsDebuffSet())
				{
					SetMoveSpeed(GetMoveSpeed() * 2.0f);
					//SetIsDebuffSet(true);
				}
				if (GetFSM())
				{
					if (GetFSM()->GetCurrBehavior() && GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
					{
						((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(true);
						//GetFSM()->GetCooldown() = true;
					}
				}
			}
		}
		break;

	case BURNING:
		{
			//SetExploding(true);
			if (GetAttackElement() == ICE && GetFSM())
			{
				//SetAttackSpeed(0.0f);
				if (GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
				{
					((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
					((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(0.0f);
					//GetFSM()->TransitionTo(WANDER_BEHAVIOR);
					SetColor(vec3f(1.0f, 1.0f, 1.0f));
					GetFSM()->GetCooldown() = true;
				}
			}

			if (GetAttackElement() == FIRE && GetFSM())
			{
				//SetAttackSpeed(0.0f);
				if (GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
				{
					//((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
					//GetFSM()->GetCooldown() = true;
					//SetExploding(true);
					((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(10.0f);
				}
			}
			//else
			//	SetColor(vec3f(1.0f, 0.0f, 0.0f));
		}
		break;

	//default:
	//	if (GetFSM())
	//	{
	//		if (GetFSM()->GetCurrBehavior()->GetBehaviorType() == RUSH_BEHAVIOR)
	//		{
	//			((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
	//			GetFSM()->GetCooldown() = true;
	//		}
	//	}
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
				if (GetElementalState() == NEUTRAL)
				{
					if (GetAttackElement() == FIRE)
						SetMoveSpeed(GetMoveSpeed() / 2);
					else
						SetMoveSpeed(GetMoveSpeed() * 2);
				}
			}
			else if (GetElementalState() < NEUTRAL)
			{
				SetElementalState(GetElementalState() + 1);
				if (GetElementalState() == NEUTRAL)
				{
					if (GetAttackElement() == FIRE)
						SetMoveSpeed(GetMoveSpeed() * 2);
					else
						SetMoveSpeed(GetMoveSpeed() / 2);
				}
			}
			SetElementalTimer(0.0);
		}
	}
	else
	{
		SetIsDebuffSet(false);
		if(GetAttackElement() == FIRE)
		{
			//SetColor(vec3f(0.5f, 0.0f, 0.8f));
		}
		else if(GetAttackElement() == ICE)
		{
			//SetColor(vec3f(0.0f, 1.0f, 1.0f));
		}
		else
		{
			//SetColor(vec3f(1.0f, 1.0f, 1.0f));
		}
		if(GetFSM() && GetFSM()->GetCurrBehavior() && GetFSM()->GetCurrBehavior()->GetBehaviorType() != WANDER_BEHAVIOR && ((CRushBehavior*)GetFSM()->GetCurrBehavior())->GetIsBombLit() && m_fExplosionScale > 1.0f)
		{
			((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetIsBombLit(false);
			((CRushBehavior*)GetFSM()->GetCurrBehavior())->SetBombTimer(0.0f);
			SetMoveSpeed(10.25);
			SetVelocity(vec3f(0.0f, 0.0f, 0.0f));
			GetFSM()->TransitionTo(WANDER_BEHAVIOR);
			SetColor(vec3f(1.0f, 1.0f, 1.0f));
			GetFSM()->GetCooldown() = true;
			m_pcSmoke->SetActive(false);
		}

	}

	if (GetInitialized())
	{
		if (GetExploding()/* && GetIsDamageDealt()*/)
		{
			string szEffectFilename;
			if (GetAttackElement() == FIRE)
				szEffectFilename = "Resource/Particles/Explosion.txt";
			else
				szEffectFilename = "Resource/Particles/ExplosionIce.txt";

			m_pcEmitter = CView::GetEmitterManager().AddEmitter(false, true, GetPos(), szEffectFilename);
			//m_pcEmitter = new CEmitter();
			//m_pcEmitter->SetLoop(false);
			//vec3f tempPos = GetPos();
			//m_pcEmitter->SetEmittPos(tempPos.x, tempPos.y, tempPos.z);
			//m_pcEmitter->SetActive(true);
			//if (GetAttackElement() == FIRE)
			//	m_pcEmitter->LoadParticle("Resource/Particles/Explosion.txt");
			//else
			//	m_pcEmitter->LoadParticle("Resource/Particles/ExplosionIce.txt");
			//CView::AddEmitter(m_pcEmitter);
			Uninitialize();
		}
	}

}

void CExplosiveEnemy::SetSubType(int _nSubType)
{
	if (_nSubType == -1)
	{
		SetAttackElement(ICE);
	}
	else if (_nSubType == 1)
	{
		SetAttackElement(FIRE);
	}
}

void CExplosiveEnemy::Uninitialize()
{
	// explosive minions were sometimes not stopping their beep sound when dying
	CSoundManager::GetInstance()->StopSound(AK::EVENTS::STOP_FX_EXPLOSIVEMINION_APPROACH,GetSoundID());

	if (m_pcEmitter)
	{
		CView::GetEmitterManager().KillEmitter(m_pcEmitter, false);
		m_pcEmitter = nullptr;
	}
	if (m_pcSmoke)
	{
		CView::GetEmitterManager().KillEmitter(m_pcSmoke, false);
		m_pcSmoke = nullptr;
	}
	CEnemy::Uninitialize();
}