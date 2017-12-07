#ifndef _RENDER_INSTANCE_ANIMATED_H_
#define _RENDER_INSTANCE_ANIMATED_H_

#include "RenderInstance.h"

class CRenderInstanceAnimated : public CRenderInstance
{
	// Animation player that controls which animations are playing
	CAnimPlayer					m_cAnimPlayer;

	// Joint on this render instance to which other objects can be attached
	int							m_nWeaponAnchorJoint;

	// Array of pre-calculated matrices used by the shader
	const static unsigned int	sm_nMaxJoints = 32;
	D3DXMATRIX					m_rgPreCalculatedPose[sm_nMaxJoints];

public:
	// Constructor
	CRenderInstanceAnimated(int nModelID, CBaseObject* pObject);
	// Destructor
	~CRenderInstanceAnimated(void) {}
	// Updates the Render Instance
	void Update(float fElapsedTime);
	// Render the instance with the passed in shader
	void Render(int nCurShader, UINT uiPass);

	// Accessors
	inline CAnimPlayer*			GetAnimPlayer(void)			{ return &m_cAnimPlayer; }
	inline const CAnimPlayer*	GetAnimPlayer(void) const	{ return &m_cAnimPlayer; }
	inline int					GetAnchorJoint(void) const	{ return m_nWeaponAnchorJoint; }
	virtual inline EType		GetType(void) const			{ return ANIMATED; }

	// Mutators
	inline void					SetAnchorJoint(int nAnchorJoint)	{ m_nWeaponAnchorJoint = nAnchorJoint; }
};

#endif