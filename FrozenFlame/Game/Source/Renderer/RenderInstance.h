#ifndef _RENDER_INSTANCE_H_
#define _RENDER_INSTANCE_H_

#include "../Animations/AnimPlayer.h"
#include "../Objects/BaseObject.h"
#include "RenderDefinitions.h"
#include "Light.h"
#include "RenderThing.h"

class CBaseObject;
class CPlayerObject;
class CCollidableObject;


class CRenderInstance : public IRenderThing
{
	// Had to friend this function for dashing collision purposes. - JC
	friend void AverageCollisionReaction(CPlayerObject* pObjOne, CCollidableObject* pObjTwo, vec3f& vCorrection);

	// object this instance is attached to
	CBaseObject*	m_pObject;
	// model id this instance is currently using
	int				m_nModelID;
	
	// collision info
	vec3f m_vCenter;
	vec3f m_vExtent;

	// recalculates the collision bounds information
	void RecalculateBounds(void) const;

public:
	// initializes data members based on the passed in parameters 
	CRenderInstance(int nModelID, CBaseObject* pObject);
	// updates the Render Instance according to a certain behavior
	void Update(float fElapsedTime);
	// clean up
	~CRenderInstance(void);
	// render the instance with the passed in shader
	virtual void Render(int nCurShader, UINT uiPass);

	// Returns the color of the game object this instance is attached to
	D3DXVECTOR4 GetEffectColor(void) const;

	// Returns the position of the game object this instance is attached to
	const vec3f GetPosition(void) const;

	// Returns true if this render instance should be rendered
	virtual bool IsRenderable(bool bCollisionRenderer) const;

	// Accessors
	// This returns the type of the object this instance is rendering - JC
	inline int GetObjectType(void) const		{ return m_pObject ? m_pObject->GetType() : -1; }
	inline int GetModelID(void) const			{ return m_nModelID; }
	CBaseObject* GetObject()					{ return m_pObject; }
	virtual inline EType GetType(void) const	{ return INSTANCE; }
};

#endif // _RENDER_INSTANCE_H_