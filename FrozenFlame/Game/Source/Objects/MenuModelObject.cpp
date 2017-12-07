#include "MenuModelObject.h"
#include "../Animations/AnimPlayer.h"

CMenuModelObject::CMenuModelObject(const char* szModelName, const vec3f& vInitialRelativePosition)
	: m_vRelativePosition(vInitialRelativePosition)
{
	// Call the parent's Initialize
	CBaseObject::Initialize();

	// Push this object to the view
	SetRenderID(CView::Push(this, szModelName, RENDER_MENU_MODEL));
}

CMenuModelObject::~CMenuModelObject(void)
{
	CBaseObject::Uninitialize();

	// Pop this object's render instance from the view
	if(GetRenderID() != -1) {
		CView::Pop(GetRenderID());
		SetRenderID(-1);
	}
}

void CMenuModelObject::Update(float fElapsedTime)
{
	// Update the world matrix of this object by gettins the world position in front of the camera
	// with the correct offset
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.axis_pos = CView::GetCamera().GetPositionInFrontOfCamera(m_vRelativePosition.x, m_vRelativePosition.y, m_vRelativePosition.z);

	// Get this object's animation player
	CAnimPlayer* pAnimPlayer = CView::GetAnimPlayer(GetRenderID());
	if(pAnimPlayer) {
		// If we're not playing any animation play Idle
		if(!pAnimPlayer->IsPlaying()) {
			pAnimPlayer->PlayAnimation(ANIM_IDLE, true);
		}
	}

	// After modifying the world matrix let's set it back
	SetWorldMatrix(worldMatrix);
}

bool CMenuModelObject::LoadAnimation(EAnimType eType, const char* szFile)
{
	// Get this object's animation player
	CAnimPlayer* pAnimPlayer = CView::GetAnimPlayer(GetRenderID());
	if(pAnimPlayer) {
		// If it has an animation player, load the animation
		pAnimPlayer->LoadAnimation(eType, szFile);
		return true;
	}
	return false;
}
bool CMenuModelObject::PlayAnimation(EAnimType eType, bool bLoop)
{
	// Get this object's animation player
	CAnimPlayer* pAnimPlayer = CView::GetAnimPlayer(GetRenderID());
	if(pAnimPlayer) {
		// If it has an animation player, play the animation
		pAnimPlayer->PlayAnimationImmediate(eType, bLoop);
		return true;
	}
	return false;
}

void CMenuModelObject::RotateX(float fRadians)
{
	// Rotate this object on the X axis by fRadians
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_x_post(fRadians);
	SetWorldMatrix(worldMatrix);
}
void CMenuModelObject::RotateY(float fRadians)
{
	// Rotate this object on the Y axis by fRadians
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_y_post(fRadians);
	SetWorldMatrix(worldMatrix);
}
void CMenuModelObject::RotateZ(float fRadians)
{
	// Rotate this object on the Z axis by fRadians
	matrix4f worldMatrix = *GetWorldMatrix();
	worldMatrix.rotate_z_post(fRadians);
	SetWorldMatrix(worldMatrix);
}