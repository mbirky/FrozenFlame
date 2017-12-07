/***********************************************
* Filename:  		CollidableObject.h
* Date:      		10/31/2012
* Mod. Date: 		10/31/2012
* Mod. Initials:	CM
* Author:    		Charles Meade
* Purpose:   		Function declarations for the collidable object
************************************************/

#include "CollidableObject.h"
#include "../Globals.h"
#include "../CGame.h"
#include "ProjectileObject.h"
#include "../Core/CWorldManager.h"
#include "Enemy.h"
#include "IceEnemy.h"
#include "PlayerObject.h"


CCollidableObject::CCollidableObject(void) : m_fMaxHealth(0)
{
	SetType(COLLIDABLE_OBJECT);
}


CCollidableObject::~CCollidableObject(void)
{
	SetInitialized(false);
}


void CCollidableObject::Initialize(void)
{
	CBaseObject::Initialize();

	m_fSphereRad = -1.0f;
	m_fHealth = 0;
	m_fMaxHealth = 0;
	m_fElementalTimer = 0.0f;
	m_fDamageTimer = 0.0f;
	m_fAttackedTimer = 0.0f;
	m_fAttackSpeed = 1.0f;
	m_vScale = vec3f(0.8f, 0.8f, 0.8f);

	m_nElementalState = NEUTRAL;
	m_nAttackElement = NORMAL;
	m_nDamage = 5;

	m_cSpherePos = vec3f(-1,-1,-1);
	m_cAABBMin = vec3f(-1,-1,-1);
	m_cAABBMax = vec3f(-1,-1,-1);

	SetAmountToDrop(2);
	SetHealthChance(70);

	m_vKnockbackVelocity.make_zero();

	m_bDebuffSet = false;
	m_bAttacking = false;
	m_bMoving = false;
	m_bDying = false;
	m_bDashedInto = false;
	m_bCanPlayDeathSFX = true;
	
	m_pAniPlayer = nullptr;

	if(GetType() == ATTACK_OBJECT)
	{
		SetRenderID(CView::Push(this, "AttackCube"));
	}
}


void CCollidableObject::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CCollidableObject::UpdateAnimations(float fElapsedTime)
{
	// It will make things easier if we get a pointer to our animation player
	CAnimPlayer* pAnim = GetAnimationPlayer();
	
	// If we don't have an animation player we shouldn't even be in here
	if(!pAnim) return;

	// Check if we should play death animation
	if(GetHealth() <= 0)
	{
		if(GetType() == WIZARD_OBJECT)
		{
			if( ((CWizardEnemy*)(this))->GetWizardSubtype() == FINAL_BOSS || CWorldManager::GetInstance()->GetTotalCurrentLevels() == 1)
			{
				if(!pAnim->IsPlaying(ANIM_DEATH))
				{
					m_bDying = true;
					SetVelocity(vec3f(0.0f,0.0f,0.0f));
					pAnim->PlayAnimation(ANIM_DEATH);
				}

				if(pAnim->GetLastPlayedAnimType() == ANIM_DEATH)
				{
					CWorldManager::GetInstance()->SetSectionComplete(true);
					SpawnPickups();
					Uninitialize();
					return;
				}
			}
			else
			{
				if(pAnim->GetLastPlayedAnimType() != ANIM_DAMAGED)
				{
					((CWizardEnemy*)(this))->GetAnimationPlayer()->PlayAnimation(ANIM_DAMAGED);
				}
				else
				{
					((CWizardEnemy*)(this))->SetIsDefeated(true);
					m_bDying = false;
					if(((CWizardEnemy*)this)->GetFSM()->GetCurrBehavior()->GetBehaviorType() != ESCAPE_BEHAVIOR)
						((CWizardEnemy*)this)->GetFSM()->TransitionTo(ESCAPE_BEHAVIOR);

				}
				
			}
		}
		
		else
		{
			// If the last played animation was a death animation, uninitialize
			if(pAnim->GetLastPlayedAnimType() == ANIM_DEATH)
			{
				if(GetType() == ICE_OBJECT)
				{
					//this->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
					
				}
				if(GetType() == FIRE_OBJECT)
				{
					((CFireEnemy*)(this))->RemoveEmitters();
					//this->GetAnimationPlayer()->SetAnimationSpeed(1.0f);
					if (m_bCanPlayDeathSFX)
					{
						m_bCanPlayDeathSFX = false;
						CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_FIREGOLEM_DEATH, GetSoundID());
					}					
				}
				

				SpawnPickups();
				Uninitialize();
				return;
			}

			// if we're not dying, die
			else if(!pAnim->IsPlaying(ANIM_DEATH))
			{
				m_bDying = true;
				SetVelocity(vec3f(0.0f,0.0f,0.0f));
				pAnim->PlayAnimation(ANIM_DEATH);
				GetAnimationPlayer()->SetAnimationSpeed(1.0f);

				if(GetType() == RANGED_OBJECT)
				{
					CSoundManager::GetInstance()->Playsound(AK::EVENTS::PLAY_FX_MELEEMINION_DEATH, GetSoundID());
				}
			}
		}
	
	}
	// Check if we should play attack animation
	else if(m_bAttacking && !m_bMoving)
	{
			pAnim->PlayAnimationImmediate(ANIM_ATTACK, false);
	}
	// Check if we should play movement animation
	else if(m_bMoving && !m_bAttacking)
	{
		if(this->GetType() == FIRE_OBJECT && !((CFireEnemy*)this)->GetIsRolling() && !((CFireEnemy*)this)->GetIsWarmingUp())
		{

			pAnim->PlayAnimation(ANIM_WALK, true);
		}
		else if(this->GetType() != FIRE_OBJECT)
		{
			pAnim->PlayAnimation(ANIM_WALK, true);
		}
	}
	else
	{
		if(this->GetType() == FIRE_OBJECT && !((CFireEnemy*)this)->GetIsRolling() && !((CFireEnemy*)this)->GetIsWarmingUp())
		{
			pAnim->PlayAnimation(ANIM_IDLE, true);
		}
		else if(this->GetType() != FIRE_OBJECT && this->GetType() != WIZARD_OBJECT)
		{
			pAnim->PlayAnimation(ANIM_IDLE, true);
		}
	}

	// By the end, if we're not playing anything, we should play idle
	if(!pAnim->IsPlaying())
	{
		pAnim->PlayAnimation(ANIM_IDLE, true);
	}
}

void CCollidableObject::Update(float fElapsedTime)
{
	// udpate this object's sound position, but only if it actually has a sound ID
	if (-1 != GetSoundID())
	{
		SetSoundPos(GetPos());
	}

	// Update position
	SetPos(GetPos() + GetVelocity() * fElapsedTime + GetKnockbackVelocity() * fElapsedTime);

	// Update variables
	m_vKnockbackVelocity *= 0.75f;
	m_fAttackedTimer += fElapsedTime;

	// Update collision bounding volumes
	UpdateCollisionData();

	// Update animations
	UpdateAnimations(fElapsedTime);

	// This will cause the object to translate based on its animation events
	if(GetAnimationPlayer())
	{
		matrix4f matrix = *GetWorldMatrix();
		matrix.translate_post(GetAnimationPlayer()->GetEventTranslation());
		SetWorldMatrix(matrix);
	}
}

void CCollidableObject::UpdateCollisionData()
{
	if(GetRenderID() != -1)
	{
		vec3f vAABBMin = GetAABBMin(), vAABBMax = GetAABBMax();
		CView::GetCollisionAabb(GetRenderID(), vAABBMin, vAABBMax);
		SetAABBMin(vAABBMin);
		SetAABBMax(vAABBMax);

		vec3f vSpherePos = GetSpherPos();
		float fSphereRad = GetSphereRad();
		CView::GetCollisionSphere(GetRenderID(), vSpherePos, fSphereRad);
		SetSpherePos(vSpherePos);
		SetSphereRad(fSphereRad);
	}
}


void CCollidableObject::GetCollisionAABB(vec3f& vfMin,vec3f& vfMax)
{
	vfMin = GetAABBMin();
	vfMax = GetAABBMax();
}

void CCollidableObject::GetCollisionSphere(vec3f& vfPos,float& fRadius)
{
	vfPos= GetSpherPos();
	fRadius = GetSphereRad();
}

void CCollidableObject::OnAttack(int nDamage,int nElementType)
{
	if (GetIsDying())
		return;

	// I wrote this line of code, and I'm proud of it - Dan
	CView::FlashRenderInstance(GetRenderID(), nElementType == FIRE ? vec4f(1, 0.25f, 0, 1) : nElementType == ICE ? vec4f(0, 0.8f, 1, 1) : vec4f(1, 1, 1, 1));

	if (m_fAttackedTimer >= DAMAGE_COOLDOWN)
	{
		//if(GetType() >= ENEMY_OBJECT && GetType() <= WIZARD_OBJECT)	{
		//	CSoundManager::GetInstance()->Playsound(CGame::GetInstance()->GetSoundBank2(),"FX_2D_MenuCancel");
		//}

		SetHealth(GetHealth() - nDamage);
		m_nElementalState += nElementType;

		if (m_nElementalState >= BURNING)
		{
			m_nElementalState = BURNING;
		}
		else if (m_nElementalState <= FROZEN)
		{
			m_nElementalState = FROZEN;
		}

		m_fElementalTimer = 0.0f;
		m_fAttackedTimer = 0.0f;			
	}

	// This wasn't getting properly set. A frame could go by w/o game knowing someone is dying. -JC
	if(GetHealth() <= FLT_EPSILON && !(GetType() == WIZARD_OBJECT && ((CWizardEnemy*)this)->GetWizardForm() < 4 &&  ((CWizardEnemy*)this)->GetWizardSubtype() == FINAL_BOSS)  )
		SetIsDying(true);
}

void CCollidableObject::SetHealth(float _fHealth)
{
	if(GetIsDying())
		return;

	m_fHealth = _fHealth;
	if(m_fHealth > m_fMaxHealth)
	{
		m_fHealth = m_fMaxHealth;
	}
	else if(m_fHealth < 0)
	{
		m_fHealth = 0;
	}
}

void CCollidableObject::Uninitialize(void)
{
	CBaseObject::Uninitialize();
	m_pAniPlayer = nullptr;
}

void CCollidableObject::SpawnPickups(void)
{
	for(int i = 0; i < m_nAmountToDrop; i++)
	{
		int nSubtype = -1;
		int chance = rand()% 100;

		if(chance <= m_nHeatlhChance)
		{
			nSubtype = HEALTH_PICKUP;
		}
		else
		{
			nSubtype = RAGE_PICKUP;
		}

		vec3f curPos = this->GetPos();

		switch(i)
		{
		case 1:
			{
				curPos.x -= 1.75f;
			}
			break;
		case 2:
			{
				curPos.z -= 1.75f;
			}
			break;
		case 3:
			{
				curPos.x += 1.75f;
			}
			break;
		case 4:
			{
				curPos.z += 1.75f;
			}
			break;
		};
		
		curPos.y += 1.5f;
		CPickupObject* pickup = (CPickupObject*)CWorldManager::GetInstance()->MakeObject(curPos, PICKUP_OBJECT, nSubtype);
		CWorldManager::GetInstance()->BoundsKeeping(pickup);
	}

	//int toRand = -1;
	//toRand = rand()% 25;
	//if(toRand % 2 == 0 || GetType() == FIRE_OBJECT || GetType() == ICE_OBJECT)
	//{
	//	for(int i = 0; i < 5; ++i)
	//	{
	//		int nSubtype = -1;
	//		if(i < 3)
	//		{
	//			nSubtype = HEALTH_PICKUP;
	//			
	//		}
	//		else
	//		{
	//			nSubtype = RAGE_PICKUP;
	//		}
	//		//CPickupObject* pPickup = (CPickupObject*)CWorldManager::GetInstance()->GetObjectManager()->NewObject(PICKUP_OBJECT, nSubtype);
	//		vec3f curPos = this->GetPos();
	//		//curPos.y += 1.0f;
	//		//if(i == 0)
	//		//{
	//		//	pPickup->SetPos(curPos);
	//		//}
	//		if(i == 1)
	//		{
	//			curPos.x -= 1.75f;
	//			//pPickup->SetPos(curPos);
	//		}
	//		else if(i == 2)
	//		{
	//			curPos.z -= 1.75f;
	//			//pPickup->SetPos(curPos);
	//		}
	//		else if(i == 3)
	//		{
	//			curPos.x += 1.75f;
	//			//pPickup->SetPos(curPos);
	//		}
	//		else if(i == 4)
	//		{
	//			curPos.z += 1.75f;
	//			//pPickup->SetPos(curPos);
	//		}
	//		curPos.y += 1.5f;
	//		CWorldManager::GetInstance()->MakeObject(curPos, PICKUP_OBJECT, nSubtype);
	//	}
	//}
}
