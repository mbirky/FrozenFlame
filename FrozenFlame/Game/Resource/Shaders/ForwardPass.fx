#ifndef _FORWARD_PASS_FX_
#define _FORWARD_PASS_FX_

#include "Globals.fx"

// This is going to be the background of the scene
// Wherever nothing would be rendered we'll render the background
texture backgroundMap;
sampler backgroundSampler =
sampler_state
{
    Texture = <backgroundMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

texture sceneMap;
sampler sceneSampler =
sampler_state
{
    Texture = <sceneMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

// Vertex shader
TFinalOutput FinalVertex(TGeneralInput input)
{
	TFinalOutput output;
	
	output.position = float4(input.locPos, 1.0f);
	output.screenPos = float4(input.locPos, 1.0f);
	output.texCoords = input.texCoords;
	
	return output;
}

// Pixel shader
float4 FinalPixel(TFinalOutput input) : COLOR
{
	// Get the texture coords, the lit color and the sampled depth
	float2 texCoord = input.texCoords + gHalfPixel;	
	float4 litColor = tex2D(sceneSampler, texCoord);
	float sampledDepth = tex2D(depthGBufferSampler, texCoord).r;
	
	// Get texture coords for the background based on the player position
	float2 bgTexCoord = texCoord;
	bgTexCoord.x -= gPlayerPos.z / 500.0f;
	bgTexCoord.y -= gPlayerPos.x / 500.0f;
	
	// This is the sampled background color
	float4 bgColor = tex2D(backgroundSampler, bgTexCoord);

	// We use the sampled depth to figure out how much of the background should be displayed
	float4 finalColor = int(1.05f - sampledDepth) * bgColor + litColor;

	// Apply gamma
	float power = 1.0f;
	float gammaPower = power /(gGammaValue);
	finalColor.rgb = pow(finalColor, float3(gammaPower, gammaPower, gammaPower));
	
	//return tex2D(diffuseGBufferSampler, texCoord);

	return finalColor;
}

float4 SimplerPS(TFinalOutput input) : COLOR
{
	return tex2D(diffuseGBufferSampler, input.texCoords + gHalfPixel);
}


technique Final
{
	pass P0
	{
		vertexShader = compile vs_3_0 FinalVertex();
		pixelShader = compile ps_3_0 FinalPixel();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
technique Debug
{
	pass P0
	{
		vertexShader = compile vs_3_0 FinalVertex();
		pixelShader = compile ps_3_0 SimplerPS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
#endif