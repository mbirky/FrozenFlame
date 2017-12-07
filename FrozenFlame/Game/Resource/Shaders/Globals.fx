#ifndef _GLOBALS_FX_
#define _GLOBALS_FX_

shared float4x4	gWorld 			: WORLD;
shared float4x4	gViewProj 		: VIEWPROJECTION;
shared float4x4 gMVP			: WORLDVIEWPROJECTION;
shared float4x4 gInvViewProj;
shared float2 gHalfPixel;
shared float4 gEffectColor;
shared float  gParticleScale;
shared float    gGammaValue;

shared float3 gCameraPos;
shared float3 gPlayerPos;

shared float	gTime;

float4x4 gCurPoseMats[32];
float4 gAddedColor = float4(0, 0, 0, 0);

shared float heatOffsetX = 0.0f;
shared float heatOffsetY = 0.0f;

/// An offset to be used when comparing depths for shadow calculations
shared float gShadowDepthOffset;
/// An offset to be used to find the texels in the directional light shadow 
/// map around the one being sampled.
shared float gShadowDirOffset;

struct TPointLight
{
	float3 position;
	
	float4x4 viewProj[6];
	
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	
	float3 attenuation;
	float range;
	
	float specularPower;
	float specularIntensity;
};
shared TPointLight gPointLight;

struct TDirectionalLight
{
	float4x4 viewProj;
	
	float3 direction;
	
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	
	float specularPower;
	float specularIntensity;
};
shared TDirectionalLight gDirLight;


struct TGeneralInput
{
	float3 locPos		: POSITION0;
	float3 locNormal	: NORMAL0;
	float2 texCoords	: TEXCOORD0;
};

struct TAnimatedInput
{
	float3 locPos		: POSITION0;
	float3 locNormal	: NORMAL0;
	float2 texCoords	: TEXCOORD0;
	float3 weights		: BLENDWEIGHT;
	float3 indices		: BLENDINDICES;
};

struct TParticleInput
{
	float3 locPos		: POSITION0;
	float4 partColor	: COLOR0;
	float2 scaleRot		: TEXCOORD0;
};

struct TDepthOutput
{
	float4 clipPos		: POSITION0;
	float2 depth		: TEXCOORD0;
};

struct TFinalOutput
{
	float4 position		: POSITION0;
	float2 texCoords	: TEXCOORD1;
	float4 screenPos	: TEXCOORD2;
};

struct TGeomVertOutput
{
	float4 clipPos		: POSITION0;
	float2 texCoords	: TEXCOORD0;
	float4 worldNormal	: TEXCOORD1;
	float2 depth		: TEXCOORD2;
};

struct TGeomFragOutput
{
	float4 diffuse : COLOR0;
	float4 normal : COLOR1;
	float4 specular : COLOR2;
	float4 depth : COLOR3;
};

struct TGFragOutTransparent
{
	float4 diffuse : COLOR0;
	float4 normal : COLOR1;
	float4 specular : COLOR2;	
};

struct TParticleVertOutput
{
	float4 clipPos		: POSITION0; // Holds particle positions in world space. -JC
	float2 texCoords	: TEXCOORD0; // These are interpolated for point sprites. -JC
	float4 colorMod		: COLOR0; // Holds the color modification for sprites. -JC
	float  rotation		: COLOR1;			
	float  partScale	: PSIZE; 	 // Holds scale of particles - JC
};

// Source texture for post process effects
texture sourceTexture;
sampler sourceTexSampler =
sampler_state
{
    Texture = <sourceTexture>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

shared texture diffuseMap;
sampler diffuseSampler =
sampler_state
{
    Texture = <diffuseMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

shared texture specularMap;
sampler specularSampler =
sampler_state
{
    Texture = <specularMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

shared texture depthGBuffer;
sampler depthGBufferSampler =
sampler_state
{
	Texture = <depthGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture lightBuffer;
sampler lightBufferSampler =
sampler_state
{
	Texture = <lightBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture diffuseGBuffer;
sampler diffuseGBufferSampler =
sampler_state
{
	Texture = <diffuseGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture normalGBuffer;
sampler normalGBufferSampler =
sampler_state
{
	Texture = <normalGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture specularGBuffer;
sampler specularGBufferSampler =
sampler_state
{
	Texture = <specularGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture effectGBuffer;
sampler effectGBufferSampler =
sampler_state
{
	Texture = <effectGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture dirLightShadowMap;
sampler dirLightShadowSampler =
sampler_state
{
	Texture = <dirLightShadowMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	
	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture pointLightShadowMap;
sampler pointLightShadowSampler =
sampler_state
{
	Texture = <pointLightShadowMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
	
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

texture gHeatMaskTexture;
sampler gHeatMaskSampler =
sampler_state
{
    Texture = <gHeatMaskTexture>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

texture gHeatNormalTexture;
sampler gHeatNormalMapSampler =
sampler_state
{
    Texture = <gHeatNormalTexture>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

#endif