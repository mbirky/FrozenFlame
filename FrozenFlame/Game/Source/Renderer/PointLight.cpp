#include "PointLight.h"
#include "AssetManager.h"
#include "View.h"
#include "RenderThing.h"

CPointLight::CPointLight(bool bEnabled, const D3DXVECTOR3& d3dPosition, float fRange, const D3DXVECTOR3& d3dAttenuation,
			const D3DXVECTOR4& d3dAmbient, const D3DXVECTOR4& d3dDiffuse, const D3DXVECTOR4& d3dSpecular,
			float fSpecPower, float fSpecIntensity)
		  : CLight(bEnabled, d3dAmbient, d3dDiffuse, d3dSpecular, fSpecPower, fSpecIntensity)
{
	m_d3dPosition = d3dPosition;
	m_fRange = fRange;
	m_d3dAttenuation = d3dAttenuation;

	m_hPosition		= nullptr;
	m_hAttenuation	= nullptr;
	m_hRange		= nullptr;
	m_hViewProj		= nullptr;

	SetShader(CAssetManager::LoadShader("PointLight.fx"));
	LoadHandles();

	SetBoundingMesh(CView::sm_cRenderer.CreateSphere(m_fRange));
//	CreateRenderTarget(1024, 768, D3DFMT_R32F);
}

void CPointLight::LoadHandles(void)
{
	SetLightHandle(CAssetManager::GetShaderParameter(GetShader(), nullptr, "gPointLight"));

	CLight::LoadHandles();

	SetShadowMap(CAssetManager::GetShaderParameter(GetShader(), nullptr, "pointLightShadowMap"));

	m_hPosition =		CAssetManager::GetShaderParameter(GetShader(), GetLightHandle(), "position");
	m_hAttenuation =	CAssetManager::GetShaderParameter(GetShader(), GetLightHandle(), "attenuation");
	m_hRange =			CAssetManager::GetShaderParameter(GetShader(), GetLightHandle(), "range");

	m_hViewProj =		CAssetManager::GetShaderParameter(GetShader(), GetLightHandle(), "viewProj");

	D3DXMatrixPerspectiveFovLH(&m_d3dProjection, D3DXToRadian(90.0f), 1.0f, 0.1f, 100.0f);
}

void CPointLight::ApplyLight(void) const
{
	CLight::ApplyLight();

	D3DXVECTOR3 d3dPosition = m_d3dPosition;
	if(GetOwner())
		d3dPosition += D3DXVECTOR3(GetOwner()->GetWorldTransform()._41, GetOwner()->GetWorldTransform()._42, GetOwner()->GetWorldTransform()._43);

	CAssetManager::ShaderSetValue(GetShader(), m_hPosition,		d3dPosition,		sizeof(d3dPosition));
	CAssetManager::ShaderSetValue(GetShader(), m_hRange,		&m_fRange,			sizeof(m_fRange));
	CAssetManager::ShaderSetValue(GetShader(), m_hAttenuation,	m_d3dAttenuation,	sizeof(m_d3dAttenuation));

	CAssetManager::ShaderSetMatrixArray(GetShader(), m_hViewProj, m_d3dViewProj, 6);
}

void CPointLight::UpdateMatrices(void)
{
	// Right
	MakeViewProj(0, D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(1, 0, 0));
	// Left
	MakeViewProj(1, D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(-1, 0, 0));
	// Up
	MakeViewProj(2, D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 1, 0));
	// Down
	MakeViewProj(3, D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, -1, 0));
	// Forward
	MakeViewProj(4, D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 1));
	// Backward
	MakeViewProj(5, D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, -1));
}

void CPointLight::MakeViewProj(UINT uiIndex, const D3DXVECTOR3& d3dAxisX,
							   const D3DXVECTOR3& d3dAxisY, const D3DXVECTOR3& d3dAxisZ)
{
	if(uiIndex >= 0 && uiIndex < 6)
	{
		D3DXMATRIX d3dWorld;
		D3DXMatrixIdentity(&d3dWorld);

		d3dWorld._11 = d3dAxisX.x;
		d3dWorld._12 = d3dAxisX.y;
		d3dWorld._13 = d3dAxisX.z;

		d3dWorld._21 = d3dAxisY.x;
		d3dWorld._22 = d3dAxisY.y;
		d3dWorld._23 = d3dAxisY.z;

		d3dWorld._31 = d3dAxisZ.x;
		d3dWorld._32 = d3dAxisZ.y;
		d3dWorld._33 = d3dAxisZ.z;

		d3dWorld._41 = m_d3dPosition.x;
		d3dWorld._42 = m_d3dPosition.y;
		d3dWorld._43 = m_d3dPosition.z;

		D3DXMATRIX d3dInverse = d3dWorld;
		// Transpose 3x3
		std::swap(d3dInverse._12, d3dInverse._21);
		std::swap(d3dInverse._13, d3dInverse._31);
		std::swap(d3dInverse._23, d3dInverse._32);

		D3DXVECTOR3 d3dWorldAxisX(d3dWorld._11, d3dWorld._12, d3dWorld._13);
		D3DXVECTOR3 d3dWorldAxisY(d3dWorld._21, d3dWorld._22, d3dWorld._23);
		D3DXVECTOR3 d3dWorldAxisZ(d3dWorld._31, d3dWorld._32, d3dWorld._33);
		D3DXVECTOR3 d3dWorldAxisW(d3dWorld._41, d3dWorld._42, d3dWorld._43);

		d3dInverse._41 = -D3DXVec3Dot(&d3dWorldAxisX, &d3dWorldAxisW);
		d3dInverse._42 = -D3DXVec3Dot(&d3dWorldAxisY, &d3dWorldAxisW);
		d3dInverse._43 = -D3DXVec3Dot(&d3dWorldAxisZ, &d3dWorldAxisW);

		D3DXMatrixMultiply(&m_d3dViewProj[uiIndex], &d3dInverse, &m_d3dProjection);
	}
}
