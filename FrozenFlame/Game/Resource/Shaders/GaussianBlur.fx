#ifndef _GAUSSIAN_BLUR_FX_
#define _GAUSSIAN_BLUR_FX_

#include "Globals.fx"

// Those values are very hard coded and gross, but you can always
// close your eyes and pretend they don't exist.
const shared float gCenterBlurWeights = 0.398942292f;
const shared float3 gBlurWeights = 
	float3(0.241970733f, 0.0539909750f, 0.00443184935f);
// Don't worry. There's nothing to be afraid.
// They don't exist.
// I promise.



struct TBlurOutput
{
	float4 position : POSITION0;
	float2 texCoords : TEXCOORD0;
	float2 positiveOffsets[3] : TEXCOORD1;
    float2 negativeOffsets[3] : TEXCOORD4;
};

TBlurOutput BlurVS(TGeneralInput input, uniform float2 gBlurDirection)
{
	TBlurOutput output;
	
	output.position = float4(input.locPos, 1.0f);	
	output.texCoords = input.texCoords;
	
	float2 offset = 2 * gHalfPixel * gBlurDirection;
	
	output.positiveOffsets[0] = input.texCoords + 1 * offset;
	output.negativeOffsets[0] = input.texCoords - 1 * offset;
	output.positiveOffsets[1] = input.texCoords + 2 * offset;
	output.negativeOffsets[1] = input.texCoords - 2 * offset;
	output.positiveOffsets[2] = input.texCoords + 3 * offset;
	output.negativeOffsets[2] = input.texCoords - 3 * offset;
	
	return output;
}

float4 BlurPS(TBlurOutput input, uniform float gBlurScaleUp) : COLOR0
{
	// These are to store the offset sample results
	float3x4 positiveSamples, negativeSamples;
	// This will be the weighted sample for the center texel
	float4 color;
	
	positiveSamples[0] = tex2D(sourceTexSampler, input.positiveOffsets[0] + gHalfPixel);
	negativeSamples[0] = tex2D(sourceTexSampler, input.negativeOffsets[0] - gHalfPixel);
	positiveSamples[1] = tex2D(sourceTexSampler, input.positiveOffsets[1] + gHalfPixel);
	negativeSamples[1] = tex2D(sourceTexSampler, input.negativeOffsets[1] - gHalfPixel);
	positiveSamples[2] = tex2D(sourceTexSampler, input.positiveOffsets[2] + gHalfPixel);
	negativeSamples[2] = tex2D(sourceTexSampler, input.negativeOffsets[2] - gHalfPixel);
	
	color = tex2D(sourceTexSampler, input.texCoords + gHalfPixel) * gCenterBlurWeights;
	
	int isAfterTopLeft = int(input.texCoords.x > 4 * gHalfPixel.x && input.texCoords.y > 4 * gHalfPixel.y);
	int isBeforeBotRight = int(input.texCoords.x < 1.0f - 4 * gHalfPixel.x && input.texCoords.y < 1.0f - 4 * gHalfPixel.y);

	color += mul(gBlurWeights, negativeSamples) * gBlurScaleUp * isAfterTopLeft;
	color += mul(gBlurWeights, positiveSamples) * gBlurScaleUp * isBeforeBotRight;
	
	return color;
}

technique GaussianBlurH
{
	pass P0
	{
		VertexShader = compile vs_3_0 BlurVS(float2(1, 0));
		PixelShader = compile ps_3_0 BlurPS(1);
        ZEnable = false;
	}		
}
technique GaussianBlurV
{
	pass P0
	{
		VertexShader = compile vs_3_0 BlurVS(float2(0, 1));
		PixelShader = compile ps_3_0 BlurPS(1);
        ZEnable = false;
	}		
}
technique BloomH
{
	pass P0
	{
		VertexShader = compile vs_3_0 BlurVS(float2(1, 0));
		PixelShader = compile ps_3_0 BlurPS(2);
        ZEnable = false;
	}		
}
technique BloomV
{
	pass P0
	{
		VertexShader = compile vs_3_0 BlurVS(float2(0, 1));
		PixelShader = compile ps_3_0 BlurPS(2);
        ZEnable = false;
	}		
}
technique UltraBlurH
{
	pass P0
	{
		VertexShader = compile vs_3_0 BlurVS(float2(0, 5));
		PixelShader = compile ps_3_0 BlurPS(1);
        ZEnable = false;
	}		
}
technique UltraBlurV
{
	pass P0
	{
		VertexShader = compile vs_3_0 BlurVS(float2(5, 0));
		PixelShader = compile ps_3_0 BlurPS(1);
        ZEnable = false;
	}		
}



#endif