#ifndef _MENU_MODEL_OBJECT_H_
#define _MENU_MODEL_OBJECT_H_

#include "BaseObject.h" 
#include "../Animations/AnimPlayer.h"

class CMenuModelObject : public CBaseObject
{
	// This vector is the position relative to a position right in front
	// of the camera, that would normally show up in the middle of the screen.
	vec3f			m_vRelativePosition;

	// Those flags will affect how the model will react while it's updating
	unsigned char	m_ucModelFlags;

	// Disable SetWorldMatrix() and SetPos() to the overall user by making them
	// private to this class
	void SetWorldMatrix(const matrix4f& _matrix)	{ CBaseObject::SetWorldMatrix(_matrix); }
	void SetPos(vec3f _vPos)						{ CBaseObject::SetPos(_vPos); }

	// Disable those inherited functions
	void Initialize(void) { }
	void Reinitialize(void) { }
	void Uninitialize(void) { }

public:
	// Constructor
	CMenuModelObject(const char* szModelName, const vec3f& vInitialRelativePosition = vec3f(0.0f, 0.0f, 0.0f));

	// Destructor
	~CMenuModelObject(void);

	// Update
	void Update(float fElapsedTime);

	void RotateX(float fRadians);
	void RotateY(float fRadians);
	void RotateZ(float fRadians);

	bool LoadAnimation(EAnimType eType, const char* szFile);
	bool PlayAnimation(EAnimType eType, bool bLoop);

	// Accessors
	inline const vec3f& GetRelativePosition(void) const { return m_vRelativePosition; }

	// Mutators
	inline void SetRelativePosition(const vec3f& vRelativePosition) { m_vRelativePosition = vRelativePosition; }
};

#endif