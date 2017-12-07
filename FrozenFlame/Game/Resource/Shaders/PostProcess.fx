#ifndef _POST_PROCESS_FX_
#define _POST_PROCESS_FX_

#include "Globals.fx"

// Vertex shader
TFinalOutput PostProcessVS(TGeneralInput input)
{
	TFinalOutput output;
	
	output.position = float4(input.locPos, 1.0f);
	output.screenPos = float4(input.locPos, 1.0f);
	output.texCoords = input.texCoords;
	
	return output;
}

float4 InversionPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	return float4(1, 1, 1, 1) - sampledColor;
}

float4 CombineAvgPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float4 litColor = tex2D(lightBufferSampler, texCoord);
	return (sampledColor + litColor) * 0.5f;
}
float4 CombineMaxPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float4 litColor = tex2D(lightBufferSampler, texCoord);
	return max(sampledColor, litColor);
}
float4 CombineBloomPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float4 litColor = tex2D(lightBufferSampler, texCoord);
	return saturate(sampledColor + litColor);
	//float avgBloom = (sampledColor.r + sampledColor.g + sampledColor.b) / 3.0f;
	//return saturate(sampledColor * avgBloom + litColor);
}
float4 BlackAndWhitePS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float avgColor = (sampledColor.r + sampledColor.g + sampledColor.b + sampledColor.a)/4;
	return float4(avgColor, avgColor, avgColor, sampledColor.a);	
}

float4 SepiaScalePS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float avgColor = (sampledColor.r + sampledColor.g + sampledColor.b + sampledColor.a)/4;
	return float4(avgColor * 1.18f, avgColor, avgColor * 0.8f, sampledColor.a);
}

float4 IntenseColorPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float4 intenseColor = sampledColor;
	if(sampledColor.r > sampledColor.g)
		intenseColor.r *= 1.4f;
	if(sampledColor.r > sampledColor.b)
		intenseColor.r *= 1.4f;
	if(sampledColor.g > sampledColor.r)
		intenseColor.g *= 1.4f;
	if(sampledColor.g > sampledColor.b)
		intenseColor.g *= 1.4f;
	if(sampledColor.b > sampledColor.g)
		intenseColor.b *= 1.4f;
	if(sampledColor.b > sampledColor.r)
		intenseColor.b *= 1.4f;
	
	return saturate(intenseColor);
}

float4 FrozenFlamePS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 color = tex2D(sourceTexSampler, texCoord);
	/*
	finalColor.r *= (finalColor.r > finalColor.b + 0.5f) ? 1.1f : 0.9f;
	finalColor.b *= (finalColor.b > finalColor.r + 0.5f) ? 1.1f : 0.9f;
	*/
	float redOffset = max(color.r - color.b, 0) + min(color.r - color.b, 0);
	float blueOffset = max(color.b - color.r, 0) + min(color.b - color.r, 0);
	
	color.g += (redOffset * color.r + blueOffset * color.b) * color.g / 2.0f;
	color.r += redOffset * color.r;
	color.b += blueOffset * color.b;

	return saturate(color);
}

float4 RageEffectPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 sampledColor = tex2D(sourceTexSampler, texCoord);
	float4 effectColor = tex2D(effectGBufferSampler, texCoord);
	float4 litColor = tex2D(lightBufferSampler, texCoord);
	
	float avgColor = (sampledColor.r + sampledColor.g + sampledColor.b) / 3.0f;
	float avgEffect = (effectColor.r + effectColor.g + effectColor.b) / 3.0f;
	
	return saturate(avgColor * (avgEffect == 0) + sampledColor * effectColor * 1.25f);
}

technique Inversion
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 InversionPS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
technique BlackAndWhite
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 BlackAndWhitePS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
technique SepiaScale
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 SepiaScalePS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
technique IntenseColor
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 IntenseColorPS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
technique CombineAvg
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 CombineAvgPS();
		cullMode = CCW;
		ZWriteEnable = FALSE;
	}
}
technique CombineMax
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 CombineMaxPS();
		cullMode = CCW;
		ZWriteEnable = FALSE;
	}
}
technique CombineBloom
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 CombineBloomPS();
		cullMode = CCW;
		ZWriteEnable = FALSE;
	}
}

technique FrozenFlame
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 FrozenFlamePS();
		cullMode = CCW;
		ZWriteEnable = FALSE;
	}
}

technique RageEffect
{
	pass P0
	{
		vertexShader = compile vs_3_0 PostProcessVS();
		pixelShader = compile ps_3_0 RageEffectPS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}

#endif