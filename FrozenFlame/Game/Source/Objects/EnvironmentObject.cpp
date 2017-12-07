/***********************************************
 * Filename:  		EnvironmentObject.cpp
 * Date:      		10/30/2012
 * Mod. Date: 		10/30/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function definitions for the environment object
 ************************************************/

#include "EnvironmentObject.h"
#include "../Renderer/View.h"

/*****************************************************************
*	CEnvironmentObject():	Default constructor for the environment object
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
CEnvironmentObject::CEnvironmentObject(void)
{
	SetType(ENVIRONMENT_OBJECT);
	m_pcOwner = nullptr;
}

/*****************************************************************
*	~CEnvironmentObject():	Default destructor for the envirnoment object
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
CEnvironmentObject::~CEnvironmentObject(void)
{
	SetInitialized(false);
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
void CEnvironmentObject::Initialize(void)
{
	CCollidableObject::Initialize();
	
	m_nSubType = ENVOBJ_NULL;
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
void CEnvironmentObject::Reinitialize()
{
	if(GetInitialized()){
		Uninitialize();
	}
	Initialize();
}

void CEnvironmentObject::Uninitialize()
{
	CCollidableObject::Uninitialize();
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
void CEnvironmentObject::Update(float fElapsedTime)
{
	//SetPos(GetPos() + GetVelocity() * fElapsedTime);
	UpdateCollisionData();

	// udpate this object's sound position, but only if it actually has a sound ID
	if (-1 != GetSoundID())
		SetSoundPos(GetPos());
}

void CEnvironmentObject::SetSubType(int nSubType)
{
	m_nSubType = nSubType;

	if(m_nSubType == ENVOBJ_GROUND)
	{
		SetRenderID(CView::Push(this, "MountainTile1"));
	}
	else if(m_nSubType == ENVOBJ_WIZARDGROUND)
	{
		SetRenderID(CView::Push(this, "MountainTop"));
	}
	else if(m_nSubType == ENVOBJ_BRIDGE)
	{
		SetRenderID(CView::Push(this, "Bridge"));
	}
	else if(m_nSubType == ENVOBJ_CLIFF)
	{
		SetRenderID(CView::Push(this, "Cliff"));
	}
	else if(m_nSubType == ENVOBJ_WALL)
	{
		SetRenderID(CView::Push(this, "MountainWall"));
	}
	else if(m_nSubType == ENVOBJ_ROCK1)
	{
		SetRenderID(CView::Push(this, "Rock1"));
	}
	else if(m_nSubType == ENVOBJ_ROCK2)
	{
		SetRenderID(CView::Push(this, "Rock2"));
	}
	else if(m_nSubType == ENVOBJ_ROCK3)
	{
		SetRenderID(CView::Push(this, "Rock3"));
	}
	else if(m_nSubType == ENVOBJ_DEADTREE1)
	{
		SetRenderID(CView::Push(this, "DeadTree1"));
	}
	else if(m_nSubType == ENVOBJ_DEADTREE2)
	{
		SetRenderID(CView::Push(this, "DeadTree2", RENDER_TRANSPARENT));
	}
	else if(m_nSubType == ENVOBJ_LIVINGTREE)
	{
		SetRenderID(CView::Push(this, "LivingTree", RENDER_TRANSPARENT));
	}
	else if(m_nSubType == ENVOBJ_PINETREE)
	{
		SetRenderID(CView::Push(this, "PineTree", RENDER_TRANSPARENT));
	}
	else if(m_nSubType == ENVOBJ_SHRUB)
	{
		SetRenderID(CView::Push(this, "SnowShrub", RENDER_TRANSPARENT));
	}
	else if(m_nSubType == ENVOBJ_SIGN)
	{
		SetRenderID(CView::Push(this, "SignPost"));
	}
	else if (m_nSubType == ENVOBJ_ICECUBE)
	{
		SetRenderID(CView::Push(this, "EffectFrozen", RENDER_FULLY_TRANSLUCENT | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
		matrix4f matrix = *GetWorldMatrix();
		matrix.scale_pre(0.75f, 0.75f, 0.75f);
		SetWorldMatrix(matrix);
	}
	else if(m_nSubType == ENVOBJ_LAVA)
	{
		SetRenderID(CView::Push(this, "LavaFlowExtended", RENDER_SCROLLING_UVS | RENDER_RAGE_AFFECTED | RENDER_RAGE_BRIGHTENED));
	}
	else
	{
		printf("Enviroment Object: %i", nSubType);
	}
}
