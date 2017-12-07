#include "AssetManager.h"
#include "View.h"

#include "Model.h"
#include "../Animations/Animation.h"

const string CAssetManager::sm_szTexturesLoc	= "Resource/Textures/";
const string CAssetManager::sm_szSpritesLoc		= "Resource/Sprites/";
const string CAssetManager::sm_szShadersLoc		= "Resource/Shaders/";
const string CAssetManager::sm_szAnimationsLoc	= "Resource/Animations/";

vector<TTexture>	CAssetManager::sm_vtTextures;
vector<TShader>		CAssetManager::sm_vtShaders;
vector<CModel*>		CAssetManager::sm_vpModels;
vector<CAnimation*> CAssetManager::sm_vpAnimations;

int CAssetManager::LoadTexture(string szFilename, bool bSprite, int* pnWidth, int* pnHeight, TTexture::EType type)
{
	// First let's check if this texture hasn't been loaded already
	for(int i = 0; i < (int)sm_vtTextures.size(); ++i)
	{
		if(sm_vtTextures[i].m_szName == szFilename)
		{
			// If this texture has already been loaded, return its location settings it's width and height
			D3DSURFACE_DESC d3dSurfDesc;
			ZeroMemory(&d3dSurfDesc, sizeof(D3DSURFACE_DESC));
			sm_vtTextures[i].m_pHandle->GetLevelDesc(0, &d3dSurfDesc);
			if(pnWidth) *pnWidth = d3dSurfDesc.Width;
			if(pnHeight) *pnHeight = d3dSurfDesc.Height;
			return i;
		}
	}

	// If this texture hasn't been loaded yet let's do it!
	TTexture newTex;
	string szTexturePath = bSprite ? sm_szSpritesLoc : sm_szTexturesLoc;
	szTexturePath.append(szFilename);
	newTex.m_pHandle = CView::sm_cRenderer.LoadTexture(szTexturePath.c_str());

	// If we failed return -1
	if(!newTex.m_pHandle)
		return -1;

	// Set the name for the texture
	newTex.m_szName = szFilename;

	// Get the texture width and height
	D3DSURFACE_DESC d3dSurfDesc;
	ZeroMemory(&d3dSurfDesc, sizeof(D3DSURFACE_DESC));
	newTex.m_pHandle->GetLevelDesc(0, &d3dSurfDesc);
	newTex.m_eType = type;
	if(pnWidth) *pnWidth = d3dSurfDesc.Width;
	if(pnHeight) *pnHeight = d3dSurfDesc.Height;

	// Push the texture to the vector and return its ID
	sm_vtTextures.push_back(newTex);
	return sm_vtTextures.size() - 1;
}

int CAssetManager::LoadShader(string szFilename)
{
	for(int i = 0; i < (int)sm_vtShaders.size(); ++i)
	{
		if(sm_vtShaders[i].m_szName == szFilename)
		{
			// If this shader has already been loaded, return its location
			return (int)i;
		}
	}

	TShader newShader;
	newShader.m_szName = szFilename;

	string szShaderPath = sm_szShadersLoc;
	szShaderPath.append(szFilename);
	newShader.m_pEffect = CView::sm_cRenderer.LoadEffect(szShaderPath.c_str());

	if(!newShader.m_pEffect)
		return -1;

	sm_vtShaders.push_back(newShader);
	return sm_vtShaders.size() - 1;
}

int CAssetManager::LoadModel(string szFilename, bool bBoundsAround)
{
	for(int i = 0; i < (int)sm_vpModels.size(); ++i)
	{
		if(sm_vpModels[i]->m_szModelName == szFilename)
		{
			// If this model has already been loaded, return its location
			return i;
		}
	}

	sm_vpModels.push_back(new CModel(szFilename, bBoundsAround));

	return sm_vpModels.size() - 1;
}

int CAssetManager::LoadAnimation(string szFilename)
{
	for(int i = 0; i < (int)sm_vpAnimations.size(); ++i)
	{
		if(sm_vpAnimations[i]->m_szName == szFilename)
		{
			// If this animation has already been loaded, return its location
			return i;
		}
	}

	string szAnimPath = sm_szAnimationsLoc;
	szAnimPath.append(szFilename);

	CAnimation* pAnim = new CAnimation;
	pAnim->Load(szAnimPath.c_str());
	pAnim->m_szName = szFilename;

	int index = szAnimPath.find_last_of('.');
	if(index != string::npos)
	{
		szAnimPath.replace(index + 1, 3, "txt");
		pAnim->LoadEvents(szAnimPath.c_str());
	}

	sm_vpAnimations.push_back(pAnim);
	return sm_vpAnimations.size() - 1;
}

void CAssetManager::ApplyTexture(int nShaderID, int nTextureID)
{
	if(!AssertTexture(nTextureID))
		return;

	TTexture& curTex = sm_vtTextures[nTextureID];

	if(!curTex.m_pHandle)
		return;

	switch(curTex.m_eType)
	{
	case TTexture::TEX_DIFFUSE:
		ShaderSetTexture(nShaderID, "diffuseMap", curTex.m_pHandle);
		break;
	case TTexture::TEX_SPECULAR:
		ShaderSetTexture(nShaderID, "specularMap", curTex.m_pHandle);
		break;
	}
}

void CAssetManager::Draw(int nTextureID, float fPosX, float fPosY, float fScaleX, float fScaleY, RECT* pSection,
							   float fRotCenterX, float fRotCenterY, float fRotation, DWORD dwColor)
{
	if(!AssertTexture(nTextureID))
		return;

	D3DXMATRIX scale;
	D3DXMATRIX rotation;
	D3DXMATRIX translate;
	D3DXMATRIX combined;

	// Initialize the Combined matrix.
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&translate);
	D3DXMatrixIdentity(&combined);

	// Scale the sprite.
	D3DXMatrixScaling(&scale, fScaleX, fScaleY, 1.0f);
	combined *= scale;

	// Rotate the sprite.
	D3DXMatrixTranslation(&translate, -fRotCenterX * fScaleX, -fRotCenterY * fScaleY, 0.0f);
	combined *= translate;
	D3DXMatrixRotationZ(&rotation, fRotation);
	combined *= rotation;
	D3DXMatrixTranslation(&translate, fRotCenterX * fScaleX, fRotCenterY * fScaleY, 0.0f);
	combined *= translate;

	// Translate the sprite
	D3DXMatrixTranslation(&translate, fPosX, fPosY, 0.0f);
	combined *= translate;

	// Apply the transform.
	CView::sm_cRenderer.SetSpriteTransform(&combined);

	CView::sm_cRenderer.Draw(sm_vtTextures[nTextureID].m_pHandle, pSection, NULL, NULL, dwColor);

	// Move the world back to identity.
	D3DXMatrixIdentity(&combined);
	CView::sm_cRenderer.SetSpriteTransform(&combined);
}
void CAssetManager::GetModelBounds(int nModelID, vec3f& vCenter, vec3f& vExtent)
{
	if(!AssertModel(nModelID))
		return;

	vCenter = sm_vpModels[nModelID]->GetCenter();
	vExtent = sm_vpModels[nModelID]->GetExtent();
}

void CAssetManager::Shutdown(void)
{
	for(size_t i = 0; i < sm_vtTextures.size(); ++i)
	{
		if(sm_vtTextures[i].m_pHandle)
		{
			sm_vtTextures[i].m_pHandle->Release();
			sm_vtTextures[i].m_pHandle = nullptr;
		}
	}
	for(size_t i = 0; i < sm_vtShaders.size(); ++i)
	{
		if(sm_vtShaders[i].m_pEffect)
		{
			sm_vtShaders[i].m_pEffect->Release();
			sm_vtShaders[i].m_pEffect = nullptr;
		}
	}
	for(size_t i = 0; i < sm_vpModels.size(); ++i)
	{
		if(sm_vpModels[i])
		{
			delete sm_vpModels[i];
			sm_vpModels[i] = nullptr;
		}
	}
	for(size_t i = 0; i < sm_vpAnimations.size(); ++i)
	{
		if(sm_vpAnimations[i])
		{
			delete sm_vpAnimations[i];
			sm_vpAnimations[i] = nullptr;
		}
	}

	sm_vtTextures.clear();
	sm_vtShaders.clear();
	sm_vpModels.clear();
	sm_vpAnimations.clear();
}