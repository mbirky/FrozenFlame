#ifndef _DIR_LIGHT_H_
#define _DIR_LIGHT_H_

#include "Light.h"

class CDirLight : public CLight
{
	static const int sm_nShadowSize = 2048;
	static const float sm_fShadowDepthOffset;
	static const float sm_fShadowDirOffset;

	D3DXVECTOR3 m_d3dDirection;
	D3DXHANDLE	m_hDirection;

	D3DXMATRIX	m_d3dView;
	D3DXMATRIX	m_d3dProj;
	D3DXMATRIX	m_d3dViewProj;

	D3DXHANDLE	m_hShadowDepthOffset;
	D3DXHANDLE	m_hShadowDirOffset;
	D3DXHANDLE	m_hViewProj;


public:
	CDirLight(bool bEnabled, const D3DXVECTOR3& d3dDirection,
		const D3DXVECTOR4& d3dAmbient, const D3DXVECTOR4& d3dDiffuse, const D3DXVECTOR4& d3dSpecular,
		float fSpecPower, float fSpecIntensity);

	void BuildInitialMatrices(float fSphereRadius, const D3DXVECTOR3& d3dSphereCenter);
	void BuildViewProjection(float fSphereRadius, const D3DXVECTOR3& d3dSphereCenter);

	void LoadHandles(void);
	void ApplyLight(void) const;
};

#endif