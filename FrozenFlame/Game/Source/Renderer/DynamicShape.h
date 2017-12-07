// DynamicShape
// This class will be used to represent 3d objects that are not loaded from
// a model and may have custom behavior (like multiple textures).
// It will be used for the Beasts' health bars, among other things.

#ifndef DYNAMIC_SHAPE_H_
#define DYNAMIC_SHAPE_H_

#include <d3dx9.h>
#include "../Util/vec3.h"
#include "RenderThing.h"

// Forward declarations
class CRenderInstance;

// DynamicShape class
class CDynamicShape : public IRenderThing
{
	// This Shape could be attached to a Render Instance so it moves with it.
	// If it is, m_pParent points to that Render Instance
	const CRenderInstance* m_pParent;

	// This is the transform relative to the parent. If there's no parent
	// this is just the transform of the shape in world space.
	D3DXMATRIX m_d3dLocalTransform;

	// Vertex buffer for this shape
	IDirect3DVertexBuffer9* m_pVertexBuffer;

	// Index buffer for this shape
	IDirect3DIndexBuffer9* m_pIndexBuffer;

	// Information of the mesh
	UINT32 m_uiNumVerts;
	UINT32 m_uiNumPrimitives;
	UINT32 m_uiVertSize;

	// Current diffuse texture being used by the shape
	int m_nCurDiffuseTex;

	// Current specular texture being used by the shape
	int m_nCurSpecularTex;

	// Color to be applied over the diffuse texture
	D3DXVECTOR4 m_d3dColor;

	// holds a vector to the scale of the render shape
	D3DXMATRIX m_LocalScale;

	// Should we consider the rotation when multiplying this matrix by its parent?
	bool m_bAttachedRotation;

	void RecalculateBounds(void) const;


public:
	// Default constructor
	CDynamicShape(const CRenderInstance* pParent = nullptr, bool bAttachedRotation = false);

	// Make this into a plane segment (basically a square in 3D space)
	void MakePlane(float fWidth, float fDepth);

	// Make this into a triangle
	void MakeTriangle(float fWidth, float fDepth);

	// Draws the dynamic shape with the passed in shader
	void Render(int nCurShader, UINT uiPass);

	// Updates this object's world transform based on the parent's matrix
	void Update(float fElapsedTime);

	// Apply the current textures for the passed in shader
	void ApplyTextures(int nShader);

	// Returns true if this render instance should be rendered
	bool IsRenderable(bool bCollisionRenderer) const;

	// Modify the local matrix
	void Translate(bool bPost, float fX, float fY, float fZ);
	void Rotate(bool bPost, float fX, float fY, float fZ);
	void Scale(bool bPost, float fX, float fY, float fZ);

	// Accessors
	inline int GetCurDiffuseTex(void) const					{ return m_nCurDiffuseTex; }
	inline int GetCurSpecularTex(void) const				{ return m_nCurSpecularTex; }
	virtual D3DXVECTOR4 GetEffectColor(void) const			{ return m_d3dColor; }
	
	// Mutators
	inline void SetLocalMatrix(const D3DXMATRIX& d3dMatrix)	{ m_d3dLocalTransform = d3dMatrix; }
	inline void SetCurDiffuseTex(int nCurDiffuseTex)		{ m_nCurDiffuseTex = nCurDiffuseTex; }
	inline void SetCurSpecularTex(int nCurSpecularTex)		{ m_nCurSpecularTex = nCurSpecularTex; }
	inline void SetColor(const D3DXVECTOR4& d3dColor)		{ m_d3dColor = d3dColor; }
	void SetLocalScale(const vec3f& scale);

	virtual inline EType GetType(void) const { return SHAPE; }

};

#endif