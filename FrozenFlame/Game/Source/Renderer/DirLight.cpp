#include "DirLight.h"
#include "AssetManager.h"

const float CDirLight::sm_fShadowDepthOffset = 0.05f;
const float CDirLight::sm_fShadowDirOffset = 1.0f / (CDirLight::sm_nShadowSize * 1.0f);

CDirLight::CDirLight(bool bEnabled, const D3DXVECTOR3& d3dDirection,
		  const D3DXVECTOR4& d3dAmbient, const D3DXVECTOR4& d3dDiffuse, const D3DXVECTOR4& d3dSpecular,
		  float fSpecPower, float fSpecIntensity)
		  : CLight(bEnabled, d3dAmbient, d3dDiffuse, d3dSpecular, fSpecPower, fSpecIntensity)
{
	m_d3dDirection	= d3dDirection;
	m_hDirection	= nullptr;
	m_hViewProj		= nullptr;

	SetShader(CAssetManager::LoadShader("DirectionalLight.fx"));
	LoadHandles();
}

void CDirLight::LoadHandles(void)
{
	SetLightHandle(CAssetManager::GetShaderParameter(GetShader(), nullptr, "gDirLight"));

	CLight::LoadHandles();
	
	SetShadowMap(CAssetManager::GetShaderParameter(GetShader(), nullptr, "dirLightShadowMap"));
	
	m_hViewProj			 = CAssetManager::GetShaderParameter(GetShader(), GetLightHandle(), "viewProj");
	m_hDirection		 = CAssetManager::GetShaderParameter(GetShader(), GetLightHandle(), "direction");

	m_hShadowDepthOffset = CAssetManager::GetShaderParameter(GetShader(), nullptr, "gShadowDepthOffset");
	CAssetManager::ShaderSetValue(GetShader(), m_hShadowDepthOffset, &sm_fShadowDepthOffset, sizeof(float));

	m_hShadowDirOffset	 = CAssetManager::GetShaderParameter(GetShader(), nullptr, "gShadowDirOffset");
	CAssetManager::ShaderSetValue(GetShader(), m_hShadowDirOffset, &sm_fShadowDirOffset, sizeof(float));	
}

void CDirLight::BuildInitialMatrices(float fSphereRadius, const D3DXVECTOR3& d3dSphereCenter)
{
	D3DXVECTOR3 d3dExtent(fSphereRadius, fSphereRadius, fSphereRadius);
	D3DXVECTOR3 d3dBoxMin = d3dSphereCenter - d3dExtent;
	D3DXVECTOR3 d3dBoxMax = d3dSphereCenter + d3dExtent;

	//	Build the projection matrix based on this box
	float fWidth = d3dBoxMax.x - d3dBoxMin.x;
	float fHeight = d3dBoxMax.y - d3dBoxMin.y;
	D3DXMatrixOrthoLH(&m_d3dProj, fWidth, fHeight, d3dBoxMin.z, d3dBoxMax.z);
	
	//	Build the view matrix
	D3DXMatrixIdentity(&m_d3dView);
	//		Set our Z axis to equal DirectionalLight's Z axis
	//		Normalize our Z axis
	D3DXVECTOR3 d3dAxisZNorm;
	D3DXVec3Normalize(&d3dAxisZNorm, &m_d3dDirection);

	m_d3dView._31 = d3dAxisZNorm.x;
	m_d3dView._32 = d3dAxisZNorm.y;
	m_d3dView._33 = d3dAxisZNorm.z;
	m_d3dView._34 = 0.0f;
		
	//		Set our X axis to the World Up cross our Z axis
	D3DXVECTOR3 d3dAxisX, d3dAxisXNorm;
	D3DXVec3Cross(&d3dAxisX, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &d3dAxisZNorm);
	//		Normalize our X axis
	D3DXVec3Normalize(&d3dAxisXNorm, &d3dAxisX);

	m_d3dView._11 = d3dAxisXNorm.x;
	m_d3dView._12 = d3dAxisXNorm.y;
	m_d3dView._13 = d3dAxisXNorm.z;
	m_d3dView._14 = 0.0f;

	//		Set our Y axis to our Z axis cross our X axis
	D3DXVECTOR3 d3dAxisY, d3dAxisYNorm;
	D3DXVec3Cross(&d3dAxisY, &d3dAxisZNorm, &d3dAxisXNorm);
	//		Normalize our Y axis
	D3DXVec3Normalize(&d3dAxisYNorm, &d3dAxisY);

	m_d3dView._21 = d3dAxisYNorm.x;
	m_d3dView._22 = d3dAxisYNorm.y;
	m_d3dView._23 = d3dAxisYNorm.z;
	m_d3dView._24 = 0.0f;

	// Now we need to invert the view matrix
	std::swap(m_d3dView._12, m_d3dView._21);
	std::swap(m_d3dView._13, m_d3dView._31);
	std::swap(m_d3dView._23, m_d3dView._32);

	d3dAxisXNorm = D3DXVECTOR3(m_d3dView._11, m_d3dView._12, m_d3dView._13);
	d3dAxisYNorm = D3DXVECTOR3(m_d3dView._21, m_d3dView._22, m_d3dView._23);
	d3dAxisZNorm = D3DXVECTOR3(m_d3dView._31, m_d3dView._32, m_d3dView._33);
	D3DXVECTOR3 d3dAxisW(m_d3dView._41, m_d3dView._42, m_d3dView._43);

	m_d3dView._14 = -D3DXVec3Dot(&d3dAxisXNorm, &d3dAxisW);
	m_d3dView._24 = -D3DXVec3Dot(&d3dAxisYNorm, &d3dAxisW);
	m_d3dView._34 = -D3DXVec3Dot(&d3dAxisZNorm, &d3dAxisW);
}

void CDirLight::BuildViewProjection(float fSphereRadius, const D3DXVECTOR3& d3dSphereCenter)
{
	// 1. Reposition the light View Matrix
	D3DXVECTOR3 d3dNewPos = d3dSphereCenter - m_d3dDirection * fSphereRadius;

	D3DXVECTOR3 d3dLightAxisInv;
	d3dLightAxisInv = D3DXVECTOR3(m_d3dView._11, m_d3dView._21, m_d3dView._31) * -1.0f;
	m_d3dView._41 = D3DXVec3Dot(&d3dLightAxisInv, &d3dNewPos);
	d3dLightAxisInv = D3DXVECTOR3(m_d3dView._12, m_d3dView._22, m_d3dView._32) * -1.0f;
	m_d3dView._42 = D3DXVec3Dot(&d3dLightAxisInv, &d3dNewPos);
	d3dLightAxisInv = D3DXVECTOR3(m_d3dView._13, m_d3dView._23, m_d3dView._33) * -1.0f;
	m_d3dView._43 = D3DXVec3Dot(&d3dLightAxisInv, &d3dNewPos);

	// 2. Build the m_DirViewProjection Matrix
	D3DXMATRIX d3dDirViewProj;
	D3DXMatrixMultiply(&d3dDirViewProj, &m_d3dView, &m_d3dProj);

	// 3. Cancel out any sub-texel translation
	D3DXVECTOR3 lightSpacePoint;
	D3DXVec3TransformCoord(&lightSpacePoint, &D3DXVECTOR3(0,0,0), &d3dDirViewProj);

	D3DXVECTOR2 d3dNearTxl;
	d3dNearTxl.x = lightSpacePoint.x * sm_nShadowSize * 0.5f;
	d3dNearTxl.y = lightSpacePoint.y * sm_nShadowSize * 0.5f;

	D3DXVECTOR2 d3dNearTxlRound;
	d3dNearTxlRound.x = floor(d3dNearTxl.x + 0.5f);
	d3dNearTxlRound.y = floor(d3dNearTxl.y + 0.5f);

	float dX = (d3dNearTxlRound.x - d3dNearTxl.x) / (sm_nShadowSize * 0.5f);
	float dY = (d3dNearTxlRound.y - d3dNearTxl.y) / (sm_nShadowSize * 0.5f);

	D3DXMATRIX mRound;
	D3DXMatrixTranslation(&mRound, dX, dY, 0);
	D3DXMatrixMultiply(&d3dDirViewProj, &d3dDirViewProj, &mRound);

	m_d3dViewProj = d3dDirViewProj;
}

void CDirLight::ApplyLight(void) const
{
	CLight::ApplyLight();

	CAssetManager::ShaderSetValue(GetShader(), m_hDirection, m_d3dDirection, sizeof(m_d3dDirection));
	CAssetManager::ShaderSetMatrix(GetShader(), m_hViewProj, &m_d3dViewProj);
}