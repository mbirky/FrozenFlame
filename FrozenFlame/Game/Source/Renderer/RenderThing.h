#ifndef _RENDER_THING_H_
#define _RENDER_THING_H_

#include "View.h"

// IRenderThing
// This is an abstract base class that RenderInstance and DynamicShape
// will inherit from. The only purpose of this function is to be able
// to sort dynamic shapes and render instances with each other in an
// organized manner. Well, they're all render things anyway.
class IRenderThing
{
	// World transformation (updated every frame and sent to shader)
	D3DXMATRIX m_d3dWorldTransform;

	// Flags for this render thing
	unsigned short	m_ucFlags;

	// Color to be ADDED to the object's diffuse before rendering
	D3DXVECTOR4		m_d3dAddedColor;

	// Is this flashing?
	bool							m_bIsFlashing;
	float							m_fFlashTimer;

	// Collision info
	mutable vec3f m_vWorldMin;
	mutable vec3f m_vWorldMax;


	virtual void RecalculateBounds(void) const = 0 {}

protected:
	inline void SetWorldTransform(const D3DXMATRIX& d3dWorldTransform)	{ m_d3dWorldTransform = d3dWorldTransform; }
	inline void SetWorldMin(const vec3f& vWorldMin)	const				{ m_vWorldMin = vWorldMin; }
	inline void SetWorldMax(const vec3f& vWorldMax)	const				{ m_vWorldMax = vWorldMax; }

public:
	enum EType { INSTANCE, ANIMATED, PARTICLES, ATTACHED, SHAPE };

	IRenderThing(void);
	virtual ~IRenderThing(void) = 0 {}
	virtual void Render(int nCurShader, UINT uiPass) = 0 {}

	// This is used for sorting - returns true if this thing is behind the other
	bool IsBehind(const IRenderThing* pOtherInstance) const;
	bool operator<(const IRenderThing& rhs) const;

	virtual void Update(float fElapsedTime) = 0;

	// This will set this render instance to flash in d3dFlashColor for the passed in duration
	void Flash(const D3DXVECTOR4& d3dFlashColor, float fFlashDuration);
		
	
	// Returns true if this render instance should be rendered
	virtual bool IsRenderable(bool bCollisionRenderer) const = 0 {}


	// gets the pre-calculated collision bounds
	void GetCollisionBounds(vec3f& vWorldMin, vec3f& vWorldMax) const;

	// Accessors
	inline const D3DXMATRIX& GetWorldTransform(void) const			{ return m_d3dWorldTransform; }
	virtual inline EType GetType(void) const = 0 {}
	virtual D3DXVECTOR4 GetEffectColor(void) const = 0 {}
	inline bool IsVisible(void) const								{ return (m_ucFlags & RENDER_INVISIBLE) ? false : true; }
	inline bool IsTransparent(void) const							{ return (m_ucFlags & RENDER_TRANSPARENT) ? true : false; }
	inline bool IsShadowCaster(void) const							{ return (m_ucFlags & RENDER_NOT_SHADOWCASTER) ? false : true; }
	inline unsigned short GetFlags(void) const						{ return m_ucFlags; }
	inline const D3DXVECTOR4& GetAddedColor(void) const				{ return m_d3dAddedColor; }
	inline const vec3f& GetWorldMin(void) const						{ return m_vWorldMin; }
	inline const vec3f& GetWorldMax(void) const						{ return m_vWorldMax; }

	// Mutators
	inline void SetFlags(unsigned short ucFlags)					{ m_ucFlags = ucFlags; }
	inline void SetAddedColor(const D3DXVECTOR4& d3dAddedColor)		{ m_d3dAddedColor = d3dAddedColor; }

};

#endif