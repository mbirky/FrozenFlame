/***********************************************
 * Filename:		Wall.cpp 		
 * Date:      		11/13/2012
 * Mod. Date: 		11/13/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		Wall object definitions
 ************************************************/
#include "Wall.h"

/********************************
* Public Functions
*********************************/

CWall::CWall(void)
{
	SetType(WALL_OBJECT);
}

CWall::~CWall(void)
{

}

void CWall::Initialize(void)
{
	CEnvironmentObject::Initialize();

	SetRenderID(CView::Push(this, "LevelWall"));
	// CView::RotateInstance(GetRenderID(),false,1,90);
	// CView::ScaleInstance(GetRenderID(),1.0f,1.0f,2.0f);
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_y_pre(D3DXToRadian(90.0f));
	worldMatrix.scale_post(1.0f, 1.0f, 2.0f);
	SetWorldMatrix(worldMatrix);
//	GetMatrix()->rotate_y_pre(D3DXToRadian(90.0f));
//	GetMatrix()->scale_post(1.0f, 1.0f, 2.0f);


	SetHealth(25);
}

void CWall::Reinitialize(void)
{
	if(!GetInitialized())
	{
		Initialize();
	}
}

void CWall::Update(float fElapsedTime)
{
	UpdateCollisionData();
	SetAttackedTimer(GetAttackedTimer() + fElapsedTime);

	if(GetHealth() <= 0)
	{
		Uninitialize();
	}
}