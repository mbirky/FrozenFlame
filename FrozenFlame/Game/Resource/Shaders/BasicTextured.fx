#ifndef BASIC_TEXTURED_FX
#define BASIC_TEXTURED_FX

#include "Globals.fx"

struct TBasicTexInput
{
	float3 locPos		: POSITION0;
	float2 texCoords	: TEXCOORD0;
};
struct TBasicTexOutput
{
	float4 clipPos		: POSITION0;
	float2 texCoords	: TEXCOORD0;
};

TBasicTexOutput BasicTexturedVS(TBasicTexInput input)
{
	TBasicTexOutput output;
	
	output.clipPos = mul(float4(input.locPos, 1.0f), gMVP);
	output.texCoords = input.texCoords;
	
	return output;
}

float4 BasicTexturedPS(TBasicTexOutput input) : COLOR0
{
	return tex2D(diffuseSampler, input.texCoords) * gEffectColor;
}

technique BasicTextured
{
	pass P0
	{
		vertexShader = compile vs_3_0 BasicTexturedVS();
		pixelShader = compile ps_3_0 BasicTexturedPS();
		
		cullMode = None;
		ZEnable = true;
		ZWriteEnable = true;
		
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
	}
}

#endif