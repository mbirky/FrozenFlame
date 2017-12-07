#include "Light.h"
#include "AssetManager.h"


CLight::CLight(bool bEnabled, const D3DXVECTOR4& d3dAmbient, const D3DXVECTOR4& d3dDiffuse, const D3DXVECTOR4& d3dSpecular,
	   float fSpecPower, float fSpecIntensity)
{
	m_pOwner			= nullptr;
	m_pBoundingMesh		= nullptr;

	m_bEnabled			= bEnabled;
	m_d3dAmbientColor	= d3dAmbient;
	m_d3dDiffuseColor	= d3dDiffuse;
	m_d3dSpecularColor	= d3dSpecular;
	m_fSpecPower		= fSpecPower;
	m_fSpecIntensity	= fSpecIntensity;

	m_hAmbientColor		= nullptr;
	m_hDiffuseColor		= nullptr;
	m_hSpecularColor	= nullptr;
	m_hSpecPower		= nullptr;
	m_hSpecIntensity	= nullptr;
	m_hShadowMap		= nullptr;
	m_hLight			= nullptr;
}


void CLight::LoadHandles(void)
{
	m_hAmbientColor		= CAssetManager::GetShaderParameter(GetShader(), m_hLight, "ambientColor");
	m_hDiffuseColor		= CAssetManager::GetShaderParameter(GetShader(), m_hLight, "diffuseColor");
	m_hSpecularColor	= CAssetManager::GetShaderParameter(GetShader(), m_hLight, "specularColor");
	m_hSpecPower		= CAssetManager::GetShaderParameter(GetShader(), m_hLight, "specularPower");
	m_hSpecIntensity	= CAssetManager::GetShaderParameter(GetShader(), m_hLight, "specularIntensity");
}

void CLight::CreateRenderTarget(UINT uiWidth, UINT uiHeight, D3DFORMAT d3dFormat)
{
	m_cShadowTarget.Create(uiWidth, uiHeight, d3dFormat);

	CAssetManager::ShaderSetTexture(m_nShader, m_hShadowMap, m_cShadowTarget.GetTexture());
}

void CLight::ActivateRenderTarget(DWORD dwTargetNo)
{
	m_cShadowTarget.ActivateTarget(dwTargetNo);
}

void CLight::RevertRenderTarget(void)
{
	m_cShadowTarget.RevertTarget();
}

void CLight::ApplyLight(void) const
{
	CAssetManager::ShaderSetValue(m_nShader, m_hAmbientColor,	m_d3dAmbientColor,	sizeof(m_d3dAmbientColor));
	CAssetManager::ShaderSetValue(m_nShader, m_hDiffuseColor,	m_d3dDiffuseColor,	sizeof(m_d3dDiffuseColor));
	CAssetManager::ShaderSetValue(m_nShader, m_hSpecularColor,	m_d3dSpecularColor,	sizeof(m_d3dSpecularColor));
	CAssetManager::ShaderSetValue(m_nShader, m_hSpecPower,		&m_fSpecPower,		sizeof(m_fSpecPower));
	CAssetManager::ShaderSetValue(m_nShader, m_hSpecIntensity,	&m_fSpecIntensity,	sizeof(m_fSpecIntensity));

	CAssetManager::ShaderSetTexture(m_nShader, m_hShadowMap, m_cShadowTarget.GetTexture());
}

void CLight::SetDiffuseColor(D3DXCOLOR color)
{
	m_d3dDiffuseColor.x = color.r;
	m_d3dDiffuseColor.y = color.g;
	m_d3dDiffuseColor.z = color.b;
	m_d3dDiffuseColor.w = color.a;
}

void CLight::SetDiffuseColor(vec4f color)
{
	m_d3dDiffuseColor.x = color.x;
	m_d3dDiffuseColor.y = color.y;
	m_d3dDiffuseColor.z = color.z;
	m_d3dDiffuseColor.w = color.w;
}

void CLight::SetDiffuseColor(D3DXVECTOR4 color)
{
	m_d3dDiffuseColor.x = color.x;
	m_d3dDiffuseColor.y = color.y;
	m_d3dDiffuseColor.z = color.z;
	m_d3dDiffuseColor.w = color.w;
}