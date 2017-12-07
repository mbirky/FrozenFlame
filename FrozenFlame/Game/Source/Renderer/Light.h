#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "RenderDefinitions.h"
#include "RenderTarget.h"
#include "../Util/vec4.h"

class IRenderThing;
class CLight
{
	// This light could be attached to something that is rendered
	IRenderThing* m_pOwner;

	// values
	bool		m_bEnabled;
	D3DXVECTOR4 m_d3dAmbientColor;
	D3DXVECTOR4 m_d3dDiffuseColor;
	D3DXVECTOR4 m_d3dSpecularColor;
	float		m_fSpecPower;
	float		m_fSpecIntensity;
	int			m_nShader;

	// this is the "parent" handle (in shader) for all our light handles
	D3DXHANDLE	m_hLight;

	// handles on the shader
	D3DXHANDLE	m_hAmbientColor;
	D3DXHANDLE	m_hDiffuseColor;
	D3DXHANDLE	m_hSpecularColor;
	D3DXHANDLE	m_hSpecPower;
	D3DXHANDLE	m_hSpecIntensity;
	D3DXHANDLE	m_hShadowMap;

	CRenderTarget	m_cShadowTarget;

	ID3DXMesh*	m_pBoundingMesh;

protected:

	inline void SetShadowMap(D3DXHANDLE hShadowMap) { m_hShadowMap = hShadowMap; }
	inline void	SetLightHandle(D3DXHANDLE hLight)	{ m_hLight = hLight; }
	inline D3DXHANDLE	GetLightHandle(void)		{ return m_hLight; }
	inline void SetBoundingMesh(ID3DXMesh* pMesh)	{ m_pBoundingMesh = pMesh; }

public:
	CLight(bool bEnabled, const D3DXVECTOR4& d3dAmbient, const D3DXVECTOR4& d3dDiffuse, const D3DXVECTOR4& d3dSpecular,
		float fSpecPower, float fSpecIntensity);
	virtual ~CLight(void) = 0 {}

	virtual void LoadHandles(void);

	void CreateRenderTarget(UINT uiWidth, UINT uiHeight, D3DFORMAT d3dFormat);
	void ActivateRenderTarget(DWORD dwTargetNo);
	void RevertRenderTarget(void);

	virtual void BuildInitialMatrices(float fSphereRadius, const D3DXVECTOR3& d3dSphereCenter) {}
	virtual void BuildViewProjection(float fSphereRadius, const D3DXVECTOR3& d3dSphereCenter) {}

	virtual void ApplyLight(void) const;
	
	inline int			GetShader(void)	const		{ return m_nShader; }
	inline bool			GetEnabled(void) const		{ return m_bEnabled; }

	inline void	SetShader(int nShader)			{ m_nShader = nShader; }
	inline void	SetEnabled(bool bEnabled)		{ m_bEnabled = bEnabled; }

	inline ID3DXMesh* GetBoundingMesh(void)	const	{ return m_pBoundingMesh; }

	virtual void SetPosition(const D3DXVECTOR3& d3dPos) {}

	void SetDiffuseColor(D3DXCOLOR color);
	void SetDiffuseColor(vec4f color);
	void SetDiffuseColor(D3DXVECTOR4 color);

	inline void SetOwner(IRenderThing* pOwner)			{ m_pOwner = pOwner; }
	inline const IRenderThing* GetOwner(void) const		{ return m_pOwner; }
};

#endif