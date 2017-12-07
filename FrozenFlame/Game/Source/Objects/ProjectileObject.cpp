/***********************************************
 * Filename:  		ProjectileObject.cpp
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the projectile object
 ************************************************/

#include "ProjectileObject.h"
#include "../Renderer/View.h"
#include "../Renderer/Emitter.h"
#include "../Renderer/CEmitterManager.h"


/*****************************************************************
*	CProjectileObject():	Default constructor for the projectile object
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
CProjectileObject::CProjectileObject(void)
{
	SetType(PROJECTILE_OBJECT);
}

/*****************************************************************
*	~CProjectileObject():	Default destructor for the enemy base object
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
CProjectileObject::~CProjectileObject(void)
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
void CProjectileObject::Initialize(void)
{
	CCollidableObject::Initialize();

	if(GetRenderID() == -1)
	{
		// We'll initialize this to a fire orb but anyway it's going to be overwritten when we set its attack element
		SetRenderID(CView::Push(this, "FireOrb"));

		// Let's set the FIXED_COLLISION flag so that the AABB of this model doesn't scale with it - Dan
		unsigned short curFlags = CView::GetRenderInstance(GetRenderID())->GetFlags();
		CView::SetRenderInstanceFlags(GetRenderID(), curFlags | RENDER_FIXED_COLLISION | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
	}

	m_pEmitterTrail = nullptr;

	m_pcOwner = nullptr;
	m_fLifeTimer = 0.0f;
	m_fSpeed = 20.0f;
	m_bDeflected = false;
}

void CProjectileObject::SetAttackElement(int _nAttackElement)
{
	if(GetAttackElement() == _nAttackElement)
		return;

	if(GetRenderID() != -1)
	{
		CView::Pop(GetRenderID());
	}

	CCollidableObject::SetAttackElement(_nAttackElement);

	if(_nAttackElement == FIRE)
		SetRenderID(CView::Push(this, "FireOrb"));
	else
		SetRenderID(CView::Push(this, "IceOrb"));

	// Let's set the FIXED_COLLISION flag so that the AABB of this model doesn't scale with it - Dan
	unsigned short curFlags = CView::GetRenderInstance(GetRenderID())->GetFlags();
	CView::SetRenderInstanceFlags(GetRenderID(), curFlags | RENDER_FIXED_COLLISION | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED);
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
void CProjectileObject::Reinitialize()
{
	if(!GetInitialized())
	{
		Initialize();
	}
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
void CProjectileObject::Update(float fElapsedTime)
{
	SetPos(GetPos() + GetVelocity() * fElapsedTime * m_fSpeed);
	m_fLifeTimer+= fElapsedTime;

	if(!m_pEmitterTrail){
		if( GetAttackElement() == FIRE ){
			m_pEmitterTrail =  CView::GetEmitterManager().AddEmitter("Resource/Particles/ProjectileFireTrail.txt",vec3f(0,0,0),vec3f(0,0,0),
				EMIT_STARTACTIVE | EMIT_LOOP , this);
		}
		else{
			m_pEmitterTrail =  CView::GetEmitterManager().AddEmitter("Resource/Particles/ProjectileIceTrail.txt",vec3f(0,0,0),vec3f(0,0,0),
				EMIT_STARTACTIVE | EMIT_LOOP , this);	
		}
	}	

	if(m_bDeflected){
		m_fLifeTimer = 0.0f;
	}

	if(m_fLifeTimer >= 5.0f || GetPos().y <= 0.0f){
		Uninitialize();
		return;
	}	

	UpdateCollisionData();
}


void CProjectileObject::Uninitialize()
{

	if(this->GetAttackElement() == FIRE){
		CView::GetEmitterManager().AddEmitter(false, true, GetPos(), "Resource/Particles/FireExplosion.txt", true);
	}
	else{
		CView::GetEmitterManager().AddEmitter(false, true, GetPos(), "Resource/Particles/IceExplosion.txt", true);
	}
	if(m_pEmitterTrail){
		CView::GetEmitterManager().KillEmitter(m_pEmitterTrail);
	}

	CCollidableObject::Uninitialize();
}

void CProjectileObject::OnAttack(int nDamage,int nElementType)
{
	if(this->GetType() != PROJECTILE_OBJECT)
		return;

	if(!m_bDeflected){
		vec3f toTarget = (m_pcOwner->GetPos() + vec3f(0.0f, 2.0f, 0.0f)) - GetPos();
		toTarget.normalize();
		SetVelocity(toTarget);
		SetDeflected(true);
		matrix4f projMatrix = *GetWorldMatrix();
		projMatrix.scale_post( 2.0f, 2.0f, 2.0f );
		SetWorldMatrix(projMatrix);
	}
}