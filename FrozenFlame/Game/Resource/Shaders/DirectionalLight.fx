#ifndef _DIR_LIGHT_FX_
#define _DIR_LIGHT_FX_

#include "Globals.fx"

struct TDirLightVertOut
{
	float4 position		: POSITION0;
	float2 texCoord		: TEXCOORD1;
	float4 screenPos	: TEXCOORD2;
};

float DepthComparison(float2 texCoord, float depth);

TDirLightVertOut DirLightVS(TGeneralInput input)
{
	TDirLightVertOut output;

	//float4 screenPos = mul(float4(input.locPos, 1.0f), gMVP);
	//screenPos.z = min(screenPos.z, screenPos.w);
	
	output.screenPos = float4(input.locPos, 1.0f);
	output.position = float4(input.locPos, 1.0f);
	output.texCoord = input.texCoords;
	return output;
}

float4 DirLightPS(TDirLightVertOut input) : COLOR0
{
	float3 clipPos = input.screenPos;
	clipPos.xy /= input.screenPos.w;
	
	float2 texCoord = float2(clipPos.x, -clipPos.y) * 0.5f + 0.5f;
	
	// Final Ambient
	float4 diffuseColor = tex2D(diffuseGBufferSampler, texCoord);
	float4 finalAmbient = gDirLight.ambientColor * diffuseColor * diffuseColor.a;
	
	// Final Diffuse
	float4 normal = tex2D(normalGBufferSampler, texCoord) * 2 - 1;
	float nDotL = saturate(dot(normal, -gDirLight.direction));
	
	// TOON
	nDotL = int(nDotL * 5) / 5.0f;

	float4 finalDiffuse = diffuseColor * nDotL * gDirLight.diffuseColor;
	
	// Fragment Position
	float depth = tex2D(depthGBufferSampler, texCoord).r;
	float4 fragViewPos = float4(clipPos.x, clipPos.y, depth, 1.0f);
	float4 fragWorldPos = mul(fragViewPos, gInvViewProj);
	fragWorldPos /= fragWorldPos.w;
	
	// Specular color
	float4 specularColor = tex2D(specularGBufferSampler, texCoord);
	
	float3 toCamera = normalize(gCameraPos - fragWorldPos.xyz);
	float3 reflection = normalize(reflect(gDirLight.direction, normal));
	float reflectDotCam = saturate(dot(reflection, toCamera));
	float specMod = gDirLight.specularIntensity *
		pow(reflectDotCam, gDirLight.specularPower);
		
	float4 finalSpecular = specularColor * specMod * gDirLight.specularColor;
	
	// Shadow
	float4 lightSpacePos = mul(fragWorldPos, gDirLight.viewProj);
	lightSpacePos.xyz /= lightSpacePos.w;
	
	float2 shadowTexCoord = lightSpacePos.xy;
	shadowTexCoord.x = (shadowTexCoord.x + 1) / 2;
	shadowTexCoord.y = 1 - ((shadowTexCoord.y + 1) / 2);
	float fragDepth = min(lightSpacePos.z, 0.99f);
	
	float shadowFactor = 0.0f;		
	float3 offsets = float3(-gShadowDirOffset, 0, gShadowDirOffset);
	for(int xOffs=0; xOffs < 3; xOffs++)
	{
		for(int yOffs=0; yOffs < 3; yOffs++)
		{
			float2 thisTexCoord = shadowTexCoord;
			thisTexCoord.x += offsets[xOffs];
			thisTexCoord.y += offsets[yOffs];
			
			shadowFactor += DepthComparison(thisTexCoord, fragDepth);
		}
	}
	shadowFactor /= 9;
	
	float4 finalColor = finalAmbient + shadowFactor * (finalDiffuse + finalSpecular);
	
	return finalColor;
}

float DepthComparison(float2 texCoord, float depth)
{
	float4 shadowSample = tex2D(dirLightShadowSampler, texCoord);
	float sampledDepth = shadowSample.r;
	return 1.0f * (depth < sampledDepth);
}

TDepthOutput DirLightDepthVS(TGeneralInput input)
{
	TDepthOutput output;
	
	float4 worldPos = mul(float4(input.locPos, 1.0f), gWorld);
	output.clipPos = mul(worldPos, gDirLight.viewProj);
	output.depth = output.clipPos.zw;
	
	return output;
}

TDepthOutput DirLightDepthAnimatedVS(TAnimatedInput input)
{
	TDepthOutput output;
	
	float4 locPos = float4(input.locPos, 1.0f);
	float4 animPos = mul(locPos, gCurPoseMats[input.indices[0]]) * input.weights[0];
	animPos += mul(locPos, gCurPoseMats[input.indices[1]]) * input.weights[1];
	animPos += mul(locPos, gCurPoseMats[input.indices[2]]) * input.weights[2];
	
	float4 worldPos = mul(animPos, gWorld);
	output.clipPos = mul(worldPos, gDirLight.viewProj);
	output.depth = output.clipPos.zw;
	
	return output;
}


float4 DirLightDepthPS(TDepthOutput input) : COLOR0
{
	return input.depth.x / input.depth.y;
}

technique DirectionalLight
{
	pass P0
	{
		vertexShader = compile vs_3_0 DirLightVS();
        pixelShader  = compile ps_3_0 DirLightPS();

		CullMode = CCW;
		
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;
		
		ZWriteEnable = FALSE;
		ZEnable = FALSE;
    }
}


technique DirectionalDepth
{
	pass P0
	{
		vertexShader = compile vs_3_0 DirLightDepthVS();
		pixelShader = compile ps_3_0 DirLightDepthPS();
		
		cullMode = CW;
		ZWriteEnable = TRUE;
		ZEnable = TRUE;
		AlphaBlendEnable = FALSE;
	}
}

technique DirectionalDepthAnimated
{
	pass P0
	{
		vertexShader = compile vs_3_0 DirLightDepthAnimatedVS();
		pixelShader = compile ps_3_0 DirLightDepthPS();
		
		cullMode = CW;
		ZWriteEnable = TRUE;
		ZEnable = TRUE;
		AlphaBlendEnable = FALSE;
	}
}
#endif