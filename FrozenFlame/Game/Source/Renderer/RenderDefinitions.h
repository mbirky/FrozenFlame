#ifndef _RENDER_DEFINES_H_
#define _RENDER_DEFINES_H_

// Direct3D9 libraries
#include <d3d9.h>
#include <d3dx9.h>

#include "../Animations/KeyFrame.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "../Util/vec3.h"
#include "../Util/Matrix4x4.h"

/*
#ifdef _DEBUG	
	#define CHEAP_RENDERING
#endif
	*/

// Renderer and View return codes
enum EViewCodes { VIEW_INIT_FAILED, VIEW_SUCCEEDED, VIEW_LOAD_FAILED };

enum EAxis { AXIS_X, AXIS_Y, AXIS_Z };

// Those are for Render Instances
enum ERenderFlags {
	RENDER_FIXED_COLLISION		= 0x1,
	RENDER_ANIMATED				= 0x2,
	RENDER_SCROLLING_UVS		= 0x4,
	RENDER_TRANSPARENT			= 0x8,
	RENDER_FULLY_TRANSLUCENT	= 0x10,
	RENDER_EFFECT				= 0x20,
	RENDER_TWOD_PARTICLE		= 0x40,
	RENDER_INVISIBLE			= 0x80,
	RENDER_NOT_SHADOWCASTER		= 0x100,
	RENDER_RAGE_AFFECTED		= 0x200,
	RENDER_RAGE_BRIGHTENED		= 0x400,
	RENDER_MENU_MODEL			= 0x800,
};


struct TJoint
{
	//D3DXMATRIX	m_d3dLocalTransform;
	string		m_szName;
	vector<CKeyFrame> m_vKeyFrames;
};

struct TInfluence
{
	UINT32	m_uiJointIndex; 
	FLOAT	m_fWeight;
};

struct TRawVert
{
	D3DXVECTOR3 m_d3dPosition;
	D3DXVECTOR3 m_d3dNormal;
	D3DXVECTOR2 m_d3dTexCoords;
};

struct TParticleVert
{
	D3DXVECTOR3 m_d3dPosition;
	D3DXVECTOR4 m_d3dColor;
	D3DXVECTOR2 m_d3dScaleAndRot;
};

struct TVertex
{
	TRawVert	m_tVertData;
	float		m_rgWeights[3];
	float		m_rgIndices[3];
};

struct TPrimitive
{
	UINT32 m_uiIndices[3];
};

struct TMesh
{
	// Name of the mesh
	char* m_szMeshName;
	// Pointer to vertex buffer
	IDirect3DVertexBuffer9* m_pVertexBuff;
	// Pointer to index buffer
	IDirect3DIndexBuffer9* m_pIndexBuff;
	// Number of primitives (triangles) in this mesh
	UINT m_uiNumTris;
	// Array of texture IDs used by this mesh
	int* m_rgTextures;
	// Number of textures
	UINT m_uiTexCount;
	// Array of unique verts used by this mesh
	TVertex* m_rgUniqueVerts;
	// Number of vertices in this mesh
	UINT m_uiNumVerts;

	TMesh(void)
		: m_szMeshName(0), m_pVertexBuff(0), m_pIndexBuff(0), m_uiNumTris(0), m_rgTextures(0), m_uiTexCount(0), m_rgUniqueVerts(0), m_uiNumVerts(0)
	{}
};

struct TTexture
{
	enum EType { TEX_DIFFUSE, TEX_SPECULAR };

	string				m_szName;
	IDirect3DTexture9*	m_pHandle;
	EType				m_eType;
};

struct TShader
{
	string				m_szName;
	ID3DXEffect*		m_pEffect;
};

struct TAttackCone
{
	matrix4f			m_tMatrix;
	bool				m_bFire;
	bool				m_bInvert;
};


#endif