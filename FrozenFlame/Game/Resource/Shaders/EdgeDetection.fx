#ifndef _EDGE_DETECTION_FX_
#define _EDGE_DETECTION_FX_

#include "Globals.fx"

// Settings controlling the edge detection filter.
shared float gEdgeWidth = 0.8f;
shared float gEdgeIntensity = 1.5f;

// How sensitive should the edge detection be to tiny variations in the input data?
// Smaller settings will make it pick up more subtle edges, while larger values get
// rid of unwanted noise.
shared float NormalThreshold = 0.25;
shared float DepthThreshold = 0.005;

// How dark should the edges get in response to changes in the input data?
shared float NormalSensitivity = 1;
shared float DepthSensitivity = 10;


// Vertex shader
TFinalOutput EdgeDetectionVS(TGeneralInput input)
{
	TFinalOutput output;
	
	output.position = float4(input.locPos, 1.0f);
	output.screenPos = float4(input.locPos, 1.0f);
	output.texCoords = input.texCoords;
	
	return output;
}

float4 EdgeDetectionPS(TFinalOutput input) : COLOR0
{
	float4 normal1, normal2, normal3, normal4, diagonalDeltaNorm;
	float2 edgeOffset, texCoord;
	float depth1, depth2, depth3, depth4, normalDelta, depthDelta, edgeAmount;

	texCoord = input.texCoords + gHalfPixel;
    edgeOffset = gEdgeWidth * (gHalfPixel * 2);

	normal1 = tex2D(normalGBufferSampler, texCoord + float2(1,1) * edgeOffset);
	normal2 = tex2D(normalGBufferSampler, texCoord + float2(-1,-1) * edgeOffset);
	normal3 = tex2D(normalGBufferSampler, texCoord + float2(1,-1) * edgeOffset);
	normal4 = tex2D(normalGBufferSampler, texCoord + float2(-1,1) * edgeOffset);
	depth1 	= tex2D(depthGBufferSampler, texCoord + float2(1,1) * edgeOffset);
	depth2	= tex2D(depthGBufferSampler, texCoord + float2(-1,-1) * edgeOffset);
	depth3 	= tex2D(depthGBufferSampler, texCoord + float2(1,-1) * edgeOffset);
	depth4	= tex2D(depthGBufferSampler, texCoord + float2(-1,1) * edgeOffset);
	
	diagonalDeltaNorm = abs(normal1 - normal2) + abs(normal3 - normal4);
	normalDelta = dot(diagonalDeltaNorm.xyz, 1);
	normalDelta = saturate((normalDelta - NormalThreshold) * NormalSensitivity);
	
	depthDelta = abs(depth1 - depth2) + abs(depth3 - depth4);
	depthDelta = saturate((depthDelta - DepthThreshold) * DepthSensitivity);
	edgeAmount = (depthDelta + normalDelta) * gEdgeIntensity * 0.5;
	
	float4 sourceColor = tex2D(sourceTexSampler, texCoord);
	return sourceColor * (1 - edgeAmount);
}

technique EdgeDetection
{
	pass P0
	{
		vertexShader = compile vs_3_0 EdgeDetectionVS();
		pixelShader = compile ps_3_0 EdgeDetectionPS();
		
		cullMode = CCW;
		
		ZWriteEnable = FALSE;
	}
}
#endif