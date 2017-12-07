#ifndef HEAT_DISTORTION_FX
#define HEAT_DISTORTION_FX

#include "Globals.fx"

// heat distortion variables
//float3 heatColor = float3(1.0f, 1.0f, 1.0f);
float  bumpyness = 0.1f;
float  HeatIntensity = 1.0f;
// the bumpyness and the heat intensity will change the "warblyness" of the effect
// though the bumpyness modifier affects it at a faster rate

TFinalOutput PostProcessVS(TGeneralInput input)
{
	TFinalOutput output;
	
	output.position = float4(input.locPos, 1.0f);
	output.screenPos = float4(input.locPos, 1.0f);
	output.texCoords = input.texCoords;
	
	return output;
}

/* WHOLE SCENE DISTORTION */
float4 FullScreenDistortionPS(TFinalOutput input) : COLOR
{
	float2 texCoord = input.texCoords + gHalfPixel;

	float4 distort = tex2D(gHeatNormalMapSampler, texCoord + float2(heatOffsetX, heatOffsetY)) * 2.0 - 1.0;
	distort = normalize(distort) * bumpyness;
	
	float4 Refract = tex2D(sourceTexSampler, texCoord + (distort.xy * 0.5f));
	
	float4 Scene = tex2D(sourceTexSampler, texCoord);
	
	float4 Mask = tex2D(gHeatMaskSampler, texCoord);
	
	float Alpha = clamp((Mask.x + Mask.y + Mask.z) / 3.0f * HeatIntensity, 0, 1);
	
	float4 output = (float4)0;
	
	if (Mask.x != 0 || Mask.y != 0 || Mask.z != 0)
	{
		output = Refract * Alpha + Scene * (1 - Alpha);
	}
	else
	{
		output = Scene;
	}
	
	return float4(output.xyz, 1.0f);
}

/* DISTORTION ONLY ON WARM COLORS */
float4 WarmColorDistortionPS(TFinalOutput input) : COLOR
{
	float2 texCoord = input.texCoords + gHalfPixel;

	float4 distort = tex2D(gHeatNormalMapSampler, texCoord + float2(heatOffsetX, heatOffsetY)) * 2.0 - 1.0;
	distort = normalize(distort) * bumpyness;
	
	float4 Refract = tex2D(sourceTexSampler, texCoord + (distort.xy * 0.5f));
	
	float4 Scene = tex2D(sourceTexSampler, texCoord);
	
	float4 Mask = tex2D(gHeatMaskSampler, texCoord);
	
	float4 output = (float4)0;

	// add distortion only to warm colors, the redder the color, the higher the heat intensity
	// the second part encourages bright yellows, but tries to reduce greens that are given distortion
	if ((Scene.r > 0.7f && Scene.b < 0.3f) || (Scene.g > 0.7f && Scene.r >= Scene.g && Scene.b < 0.3f))
	{
		float Alpha = clamp((Mask.x + Mask.y + Mask.z) / 3.0f * (HeatIntensity * max(Scene.r, Scene.g)), 0, 1);	
	
		output = Refract * Alpha + Scene * (1 - Alpha);
	}
	else
	{
		output = Scene;
	}
	
	return float4(output.xyz, 1.0f);
}

technique FullScreenHeatDistortion
{
	pass FirstPass
	{
		 vertexShader = compile vs_2_0 PostProcessVS();
		 pixelShader  = compile ps_2_0 FullScreenDistortionPS();
		 ALPHABLENDENABLE=TRUE;
	}
}

technique WarmColorHeatDistortion
{
	pass FirstPass
	{
		 vertexShader = compile vs_2_0 PostProcessVS();
		 pixelShader  = compile ps_2_0 WarmColorDistortionPS();
		 ALPHABLENDENABLE=TRUE;
	}
}

#endif