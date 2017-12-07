#ifndef _RENDER_INSTANCE_ATTACHED_H_
#define _RENDER_INSTANCE_ATTACHED_H_

#include "RenderInstance.h"

class CRenderInstanceAttached : public CRenderInstance
{
	// Pointer to the RenderInstance this is attached to
	const CRenderInstance* m_pOwner;

	// ID of the attachment joint on this Render Instance
	int m_nAnchorJointID;

	// Is this visible?
	bool m_bEnabled;

public:
	// Constructor
	CRenderInstanceAttached(int nModelID, const CRenderInstance* pOwner, const string& szAttachmentJoint);
	// Destructor
	~CRenderInstanceAttached(void) {}
	// Updates the Render Instance
	void Update(float fElapsedTime);

	// Returns true if this render instance should be rendered
	bool IsRenderable(bool bCollisionRenderer) const;

	D3DXVECTOR4 GetEffectColor(void) const;
	
	// Accessors
	inline int GetAnchorJointID(void) const				{ return m_nAnchorJointID; }
	inline bool GetEnabled(void) const					{ return m_bEnabled; }
	inline const CRenderInstance* GetOwner(void) const  { return m_pOwner; }
	virtual inline EType GetType(void) const			{ return ATTACHED; }

	// Mutators
	inline void SetEnabled(bool bEnabled)				{ m_bEnabled = bEnabled; }
};


#endif