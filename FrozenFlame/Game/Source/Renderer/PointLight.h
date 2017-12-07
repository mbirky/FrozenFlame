#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Light.h"
#include "RenderThing.h" 
class CPointLight : public CLight
{
	D3DXVECTOR3 m_d3dPosition;
	D3DXVECTOR3 m_d3dAttenuation;
	float		m_fRange;

	D3DXMATRIX  m_d3dProjection;
	D3DXMATRIX	m_d3dViewProj[6];

	D3DXHANDLE	m_hPosition;
	D3DXHANDLE	m_hAttenuation;
	D3DXHANDLE	m_hRange;
	D3DXHANDLE  m_hViewProj;

	void MakeViewProj(UINT uiIndex, const D3DXVECTOR3& d3dAxisX, const D3DXVECTOR3& d3dAxisY, const D3DXVECTOR3& d3dAxisZ);

public:
	CPointLight(bool bEnabled, const D3DXVECTOR3& d3dPosition, float fRange, const D3DXVECTOR3& d3dAttenuation,
		const D3DXVECTOR4& d3dAmbient, const D3DXVECTOR4& d3dDiffuse, const D3DXVECTOR4& d3dSpecular,
		float fSpecPower, float fSpecIntensity);

	void LoadHandles(void);
	void ApplyLight(void) const;

	void UpdateMatrices(void);

	void SetPosition(const D3DXVECTOR3& d3dPos) { m_d3dPosition = d3dPos; }
	const D3DXVECTOR3& GetPosition(void) const	{ return m_d3dPosition; }
	
	const D3DXMATRIX GetWorldMatrix(void) const
	{
		D3DXVECTOR3 d3dPosition = m_d3dPosition;
		if(GetOwner())
			d3dPosition += D3DXVECTOR3(GetOwner()->GetWorldTransform()._41, GetOwner()->GetWorldTransform()._42, GetOwner()->GetWorldTransform()._43);

		D3DXMATRIX d3dMatrix;
		D3DXMatrixTranslation(&d3dMatrix, d3dPosition.x, d3dPosition.y, d3dPosition.z);
		return d3dMatrix;
	}
};

#endif