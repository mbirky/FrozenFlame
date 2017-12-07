/***********************************************
 * Filename:  		RangedEnemy.cpp
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the ranged enemy object
 ************************************************/

#include "RangedEnemy.h"
#include "../Renderer/View.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RenderInstance.h"
#include "../AI/Header/CFSM.h"
#include "../AI/Header/CBehaviors.h"
#include "../Globals.h"


/*****************************************************************
*	CRangedEnemy():		Default constructor for the ranged enemy object
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
CRangedEnemy::CRangedEnemy(void)
{
	SetType(RANGED_OBJECT);
}

/*****************************************************************
*	~CRangedEnemy():		Default destructor for the ranged enemy object
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
CRangedEnemy::~CRangedEnemy(void)
{

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
void CRangedEnemy::Initialize(void)
{
	CEnemy::Initialize();

	if(GetRenderID() == -1)
	{
		SetRenderID(CView::Push(this, "Minion", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	}

	if(GetFSM() == nullptr)
	{
		CFSM* _pcBehavior = new CFSM();
		_pcBehavior->SetAgent(this);
		_pcBehavior->AddState((CBaseBehavior*)(new CRangedBehavior()));
		_pcBehavior->AddState((CBaseBehavior*)(new CWanderBehavior()),true);

		SetFSM(_pcBehavior);
	}

	SetMaxHealth(10);
	SetHealth(GetMaxHealth());
	SetAttackSpeed(3);
	// ranged minions will shoot frequently on hard mode
	if (CGame::GetInstance()->GetDifficultyLevel() == DIFF_HARD)
	{
		SetAttackSpeed(1);
	}
	else
	{
		SetAttackSpeed(3);
	}
	SetMoveSpeed(5.0);
	SetDamage(10);

	SetAmountToDrop(3);
	SetHealthChance(70);

	m_bobbingData.fBobbingSpeed = 0.8f;
	m_bobbingData.fBobbingDistance = 1.2f;
	m_bobbingData.fCurrentFloatDistance = 0.0f + float( rand()% 5 ) * 0.2f;
	m_bobbingData.bBobbingUp = true;

	SetAnimPlayer(CView::GetAnimPlayer(GetRenderID()));

	GetAnimationPlayer()->LoadAnimation(ANIM_IDLE, "Minion/Minion_Idle.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DEATH, "Minion/Minion_Death.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_ATTACK, "Minion/Minion_RangedAttack.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_WALK, "Minion/Minion_Movement.anm");
	GetAnimationPlayer()->LoadAnimation(ANIM_DAMAGED, "Minion/Minion_Damaged.anm");

	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.scale_pre(GetScale().x * 0.75f, GetScale().y * 0.75f, GetScale().z * 0.75f);
	worldMatrix.axis_pos.y += 3.0f; // raise the minion into the air a little
	SetWorldMatrix(worldMatrix);

	this->GetAnimationPlayer()->SetAnimationSpeed(this->GetAnimationPlayer()->GetAnimationSpeed() * (this->GetTimerOffset() + 1.0f));
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
void CRangedEnemy::Reinitialize(void)
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
void CRangedEnemy::Update(float fElapsedTime)
{	
	SetVelocity(vec3f(0.0, 0.0, 0.0));

	CCollidableObject::Update(fElapsedTime);
	if(!GetInitialized())
		return;

	switch (GetElementalState())
	{
	case FROZEN:
		{
			//m_pcBehavior->TransitionTo();
			SetColor(vec3f(0.0f, 0.0f, 1.0f));
		}
		break;

	case COLD:
		{
			if (!GetIsDebuffSet())
			{
				SetMoveSpeed(GetMoveSpeed() / 2);
				SetIsDebuffSet(true);
			}
			SetColor(vec3f(0.2f, 0.2f, 0.5f));
		}
		break;

	case HOT:
		{
			if (GetDamageTimer() >= DAMAGE_COOLDOWN * 3.0f)
			{
				SetHealth(GetHealth() - 1);
				// do not want damage images in right now
				//AddDamageImage(1);
				SetDamageTimer(0.0);
			}

			if (!GetIsDebuffSet())
			{
				SetMoveSpeed(GetMoveSpeed() * 2);
				SetIsDebuffSet(true);
			}
			SetColor(vec3f(0.5f, 0.2f, 0.2f));
		}
		break;

	case BURNING:
		{
			if (GetDamageTimer() >= DAMAGE_COOLDOWN * 3.0f)
			{
				SetHealth(GetHealth() - 2);
				// do not want damage images in right now
				//AddDamageImage(2);
				SetDamageTimer(0.0);
			}
			SetColor(vec3f(1.0f, 0.0f, 0.0f));
			GetFSM()->TransitionTo(WANDER_BEHAVIOR);
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
		SetIsDebuffSet(false);
		if(GetAttackElement() == FIRE)
		{
			SetColor(vec3f(1.0f, 0.2f, 0.2f));
		}
		else if(GetAttackElement() == ICE)
		{
			SetColor(vec3f(0.2f, 0.2f, 1.0f));
		}
		else
		{
			SetColor(vec3f(1.0f, 1.0f, 1.0f));
		}
	}

	static float floatDistance = m_bobbingData.fBobbingSpeed * fElapsedTime;
	vec3f currentPos = GetPos();
	if (m_bobbingData.bBobbingUp)
	{
		// float the object up
		m_bobbingData.fCurrentFloatDistance += floatDistance;
		currentPos.y += floatDistance;
		this->SetPos(currentPos);
		// once it has floated as high as it will, float it back down
		if (m_bobbingData.fCurrentFloatDistance > m_bobbingData.fBobbingDistance)
		{
			m_bobbingData.bBobbingUp = false;
			m_bobbingData.fCurrentFloatDistance = m_bobbingData.fBobbingDistance;
		}
	}
	else
	{
		m_bobbingData.fCurrentFloatDistance -= floatDistance;
		currentPos.y -= floatDistance;
		this->SetPos(currentPos);
		if (m_bobbingData.fCurrentFloatDistance < 0)
		{
			m_bobbingData.bBobbingUp = true;
			m_bobbingData.fCurrentFloatDistance = 0;
		}
	}

}

// This will not only set the attack element for the ranged minion but will
// also load his staff based on the element.
void CRangedEnemy::SetAttackElement(int _nAttackElement)
{
	// Call the parent's SetAttackElement() function to set the variable
	CEnemy::SetAttackElement(_nAttackElement);

	// Depending on the attack element let's push a fire or ice staff
	// as an attachment to this enemy's render instance
	switch(_nAttackElement)
	{
	case ICE:
		CView::PushAttachment(GetRenderID(), "MinionStaffIce", "Weapon_Anchor_r", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		break;
	case FIRE:
		CView::PushAttachment(GetRenderID(), "MinionStaffFire", "Weapon_Anchor_r", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
		break;
	default:
		CView::PushAttachment(GetRenderID(), "MinionStaff", "Weapon_Anchor_r", "", RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
	}
}

void CRangedEnemy::Uninitialize(void)
{
	// if this minion has just been killed, increment the kill count for the credit screen
	if (GetHealth() <= 0)
	{
		CGame::GetInstance()->IncrementMinionKillCount();
	}

	CEnemy::Uninitialize();
}