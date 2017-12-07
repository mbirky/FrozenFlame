#ifndef _MANAGERS_H_
#define _MANAGERS_H_

#include "RenderDefinitions.h"

class CModel;
class CAnimation;
class CAssetManager
{
	static vector<TTexture>		sm_vtTextures;
	static vector<TShader>		sm_vtShaders;
	static vector<CModel*>		sm_vpModels;
	static vector<CAnimation*>	sm_vpAnimations;

	static const string sm_szTexturesLoc;
	static const string sm_szSpritesLoc;
	static const string sm_szShadersLoc;
	static const string sm_szAnimationsLoc;

	__forceinline static bool AssertTexture(int nTexID)		{ return (nTexID	>= 0 &&	nTexID		< (int)sm_vtTextures.size()); }
	__forceinline static bool AssertShader(int nShaderID)	{ return (nShaderID >= 0 &&	nShaderID	< (int)sm_vtShaders.size()); }
	__forceinline static bool AssertModel(int nModelID)		{ return (nModelID	>= 0 &&	nModelID	< (int)sm_vpModels.size()); }

public:
	__forceinline static bool AssertAnimation(int nAnimID)	{ return (nAnimID	>= 0 && nAnimID		< (int)sm_vpAnimations.size()); }

	static int LoadTexture(string szFilename, bool bSprite = false, int* pnWidth = nullptr, int* pnHeight = nullptr, TTexture::EType type = TTexture::TEX_DIFFUSE);
	static int LoadShader(string szFilename);
	static int LoadModel(string szFilename, bool bBoundsAround);
	static int LoadAnimation(string szFilename);

	static void ApplyTexture(int nShaderID, int nTextureID);
	
	static void ApplyTexture(int nShaderID, D3DXHANDLE hTexture, int nTextureID)
	{
		if(AssertTexture(nTextureID))
		{
			ShaderSetTexture(nShaderID, hTexture, sm_vtTextures[nTextureID].m_pHandle);
		}
	}
	
	static void Draw(int nTextureID, float fPosX, float fPosY, float fScaleX, float fScaleY, RECT* pSection,
		float fRotCenterX, float fRotCenterY, float fRotation, DWORD dwColor);
	static void GetModelBounds(int nModelID, vec3f& vCenter, vec3f& vExtent);

	__forceinline static CModel* GetModel(int nModelID)	{ if(AssertModel(nModelID)) return sm_vpModels[nModelID]; return nullptr; }
	__forceinline static CAnimation* GetAnimation(int nAnimID) { if(AssertAnimation(nAnimID)) return sm_vpAnimations[nAnimID]; return nullptr; }

	static UINT ShaderBegin(int nShaderID)
	{
		if(AssertShader(nShaderID))
		{
			UINT numPasses;
			sm_vtShaders[nShaderID].m_pEffect->Begin(&numPasses, 0);
			return numPasses;
		}
		return 0;
	}
	static void ShaderEnd(int nShaderID) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->End(); } 
	static void ShaderBeginPass(int nShaderID, UINT uiPass) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->BeginPass(uiPass); }
	static void ShaderEndPass(int nShaderID) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->EndPass(); }
	static void ShaderSetTechnique(int nShaderID, const char* szTechnique) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->SetTechnique(szTechnique); }
	static void ShaderSetMatrix(int nShaderID, D3DXHANDLE hParam, const D3DXMATRIX* pMatrix) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->SetMatrix(hParam, pMatrix); }
	static void ShaderSetMatrixArray(int nShaderID, D3DXHANDLE hParam, const D3DXMATRIX* pMatrices, UINT uiSize)
	{
		if(AssertShader(nShaderID))
		{
			HRESULT hr = sm_vtShaders[nShaderID].m_pEffect->SetMatrixArray(hParam, pMatrices, uiSize);
			int x = 0;
		}
	}
	static void ShaderSetValue(int nShaderID, D3DXHANDLE hParam, LPCVOID pData, UINT uiBytes) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->SetValue(hParam, pData, uiBytes); }
	static void ShaderSetTexture(int nShaderID, D3DXHANDLE hParam, IDirect3DTexture9* pTexture) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->SetTexture(hParam, pTexture); }
	static void ShaderCommitChanges(int nShaderID) {
		if(AssertShader(nShaderID)) sm_vtShaders[nShaderID].m_pEffect->CommitChanges(); }
	static D3DXHANDLE GetShaderParameter(int nShaderID, D3DXHANDLE hParent, char* szName) {
		if(AssertShader(nShaderID)) return sm_vtShaders[nShaderID].m_pEffect->GetParameterByName(hParent, szName); return nullptr; }

	static void Shutdown(void);
};


#endif