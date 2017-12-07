#ifndef _GBUFFERS_PASS_FX_
#define _GBUFFERS_PASS_FX_

#include "Globals.fx"

TGeomVertOutput GBuffersVS(TGeneralInput input)
{
	TGeomVertOutput output;

	// Put the position in clip space
	output.clipPos = mul(float4(input.locPos, 1.0f), gMVP);

	// Put the normal in world space
	output.worldNormal = mul(float4(input.locNormal, 0.0f), gWorld);
	
	// Pass the texture coordinates
	output.texCoords = input.texCoords;
	
	// Pass the depth
	output.depth = output.clipPos.zw;
	return output;
}

TGeomVertOutput GBuffersScrollingVS(TGeneralInput input, uniform float2 direction)
{
	TGeomVertOutput output;

	// Put the position in clip space
	output.clipPos = mul(float4(input.locPos, 1.0f), gMVP);

	// Put the normal in world space
	output.worldNormal = mul(float4(input.locNormal, 0.0f), gWorld);
	
	// Pass the texture coordinates
	output.texCoords = input.texCoords + gTime * direction;
	
	// Pass the depth
	output.depth = output.clipPos.zw;
	return output;
}

TGeomVertOutput GBuffersAnimatedVS(TAnimatedInput input)
{
	TGeomVertOutput output;
	
	// Change the vert based on the current pose
	float4 locPos = float4(input.locPos, 1.0f);
	float4 animPos = mul(locPos, gCurPoseMats[input.indices[0]]) * input.weights[0];
	animPos += mul(locPos, gCurPoseMats[input.indices[1]]) * input.weights[1];
	animPos += mul(locPos, gCurPoseMats[input.indices[2]]) * input.weights[2];
	
	float remainderWeight = max(0.0f, 1.0f - input.weights[0] - input.weights[1] - input.weights[2]);
	animPos += locPos * remainderWeight;
	
	// Put the position in clip space
	output.clipPos = mul(animPos, gMVP);
	
	// Put the normal in world space
	output.worldNormal = mul(float4(input.locNormal, 0.0f), gWorld);
	
	// Pass the texture coordinates
	output.texCoords = input.texCoords;
	
	// Pass the depth
	output.depth = output.clipPos.zw;
	return output;
}

TGeomFragOutput GBuffersPS(TGeomVertOutput input)
{
	TGeomFragOutput output;
	
	// Depth
	output.depth = input.depth.x / input.depth.y;
	// Normal
	float4 normal = normalize(input.worldNormal);
	output.normal = (normal + 1.0f) * 0.5f;	
	// Diffuse
	float4 diffuse = tex2D(diffuseSampler, input.texCoords);
	float4 addedColor = float4(gAddedColor.rgb, 1.0f) * gAddedColor.a;
	diffuse = saturate((diffuse * gEffectColor) + addedColor);
	// This is for clipping out alpha values.
	// It's more efficient than a conditional and discard. -JC
	clip(diffuse.a < 0.05f ? -1:1);
	output.diffuse = diffuse;
	// Specular
	output.specular = tex2D(specularSampler, input.texCoords);
	
	return output;
}

float4 GBuffersRagePS(TGeomVertOutput input) : COLOR0
{
	// Diffuse
	float4 diffuse = tex2D(diffuseSampler, input.texCoords);
	float4 addedColor = float4(gAddedColor.rgb, 1.0f) * gAddedColor.a;
	diffuse = saturate((diffuse * gEffectColor) + addedColor);
	// This is for clipping out alpha values.
	// It's more efficient than a conditional and discard. -JC
	clip(diffuse.a < 0.05f ? -1:1);
	
	return saturate(diffuse * 0.25f + gEffectColor * 0.75f);		
}

float4 GBuffersBlackPS(TGeomVertOutput input) : COLOR0
{
	return 0;
}

TGFragOutTransparent GBuffersNoDepthPS(TGeomVertOutput input)
{
	TGFragOutTransparent output;
	
	// Normal
	float4 normal = normalize(input.worldNormal);
	output.normal = (normal + 1.0f) * 0.5f;	
	// Diffuse
	float4 diffuse = tex2D(diffuseSampler, input.texCoords);
	float4 addedColor = float4(gAddedColor.rgb, 1.0f) * gAddedColor.a;
	diffuse = saturate((diffuse * gEffectColor) + addedColor);
	// This is for clipping out alpha values.
	// It's more efficient than a conditional and discard. -JC
	clip(diffuse.a < 0.05f ? -1:1);
	output.diffuse = diffuse;
	// Specular
	output.specular = tex2D(specularSampler, input.texCoords);
	
	return output;
}

TGFragOutTransparent GBuffersEffectsPS(TGeomVertOutput input)
{
	TGFragOutTransparent output;
	
	// Normal
	float4 normal = normalize(input.worldNormal);
	output.normal = (normal + 1.0f) * 0.5f;	
	// Specular
	output.specular = tex2D(specularSampler, input.texCoords);
	// Diffuse
	float4 diffuse = tex2D(diffuseSampler, input.texCoords);
	float4 addedColor = float4(gAddedColor.rgb, 1.0f) * gAddedColor.a;
	diffuse = saturate((diffuse * gEffectColor * 1.2f) + addedColor);

	// This is for clipping out alpha values.
	// It's more efficient than a conditional and discard. -JC
	clip(diffuse.a < 0.2f ? -1:1);
	output.diffuse = diffuse;
	
	return output;	
}

technique GBuffersDefault
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersPS();
		
		cullMode 			= CCW;		
		AlphaBlendEnable 	= FALSE;		
		ZWriteEnable 		= TRUE;
	}
}

technique GBuffersScrollingUVs
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersScrollingVS(float2(0, -0.15f));
		pixelShader = compile ps_3_0 GBuffersPS();
		
		cullMode 			= CCW;		
		AlphaBlendEnable 	= FALSE;		
		ZWriteEnable 		= TRUE;
	}
}

technique GBuffersScrollingUVsAlpha
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersScrollingVS(float2(0, -0.15f));
		pixelShader = compile ps_3_0 GBuffersPS();
		
		cullMode = CCW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = TRUE;		
	}
}

technique GBuffersScrollingUVsFullyTranslucent
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersScrollingVS(float2(0, -0.15f));
		pixelShader = compile ps_3_0 GBuffersNoDepthPS();
		
		cullMode = CW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = FALSE;		
	}
	pass P1
	{
		vertexShader = compile vs_3_0 GBuffersScrollingVS(float2(0, -0.15f));
		pixelShader = compile ps_3_0 GBuffersNoDepthPS();
		
		cullMode = CCW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = FALSE;		
	}	
}

technique GBuffersFullyTranslucent
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersNoDepthPS();
		
		cullMode = CW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = FALSE;		
	}
	pass P1
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersNoDepthPS();
		
		cullMode = CCW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = FALSE;		
	}	
}

technique GBuffersAlpha
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersPS();
		
		cullMode = CCW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = TRUE;		
	}
}

technique GBuffersEffects
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersEffectsPS();
		
		cullMode = None;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = One;
		ZWriteEnable = FALSE;		
	}
}

////////////////////////////////////////////////////
// ANIMATED TECHNIQUES
////////////////////////////////////////////////////
technique GBuffersDefaultAnimated
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersAnimatedVS();
		pixelShader = compile ps_3_0 GBuffersPS();
		
		cullMode 			= CCW;		
		AlphaBlendEnable 	= FALSE;		
		ZWriteEnable 		= TRUE;
	}
}

technique GBuffersFullyTranslucentAnimated
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersAnimatedVS();
		pixelShader = compile ps_3_0 GBuffersNoDepthPS();
		
		cullMode = CW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = FALSE;		
	}
	pass P1
	{
		vertexShader = compile vs_3_0 GBuffersAnimatedVS();
		pixelShader = compile ps_3_0 GBuffersNoDepthPS();
		
		cullMode = CCW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = FALSE;		
	}	
}

technique GBuffersAlphaAnimated
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersAnimatedVS();
		pixelShader = compile ps_3_0 GBuffersPS();
		
		cullMode = CCW;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = TRUE;		
	}
}

technique GBuffersEffectsAnimated
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersAnimatedVS();
		pixelShader = compile ps_3_0 GBuffersEffectsPS();
		
		cullMode = None;
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		SrcBlend = SrcColor;
		DestBlend = One;
		ZWriteEnable = FALSE;		
	}
}

technique GBuffersRage
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersRagePS();
		
		cullMode 			= CCW;		
		AlphaBlendEnable 	= FALSE;		
		ZWriteEnable 		= TRUE;
	}
}

technique GBuffersRageAnimated
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersAnimatedVS();
		pixelShader = compile ps_3_0 GBuffersRagePS();
		
		cullMode 			= CCW;		
		AlphaBlendEnable 	= FALSE;		
		ZWriteEnable 		= TRUE;
	}
}

technique GBuffersBlack
{
	pass P0
	{
		vertexShader = compile vs_3_0 GBuffersVS();
		pixelShader = compile ps_3_0 GBuffersBlackPS();
		
		cullMode 			= CCW;		
		AlphaBlendEnable 	= FALSE;		
		ZWriteEnable 		= TRUE;
	}
}

#endif