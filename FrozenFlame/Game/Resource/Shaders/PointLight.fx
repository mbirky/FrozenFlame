#ifndef _POINT_LIGHT_FX_
#define _POINT_LIGHT_FX_

#include "Globals.fx"

// The output of the point light vertex shader
struct TPointLightVertOut
{
	float4 position		: POSITION0;
	float4 screenPos	: TEXCOORD0;
};

TPointLightVertOut PointLightVS(float3 position : POSITION0)
{
	TPointLightVertOut output;

	float4 screenPos = mul(float4(position, 1.0f), gMVP);
	screenPos.z = min(screenPos.z, screenPos.w);
	
	output.screenPos = screenPos;
	output.position = screenPos;
	return output;
}

float4 PointLightPS(TPointLightVertOut input) : COLOR0
{
	float3 clipPos = input.screenPos;
	clipPos.xy /= input.screenPos.w;
	
	float2 texCoord = float2(clipPos.x, -clipPos.y) * 0.5f + 0.5f;
	
	// Fragment position
	float depth = tex2D(depthGBufferSampler, texCoord).r;
	float4 fragViewPos = float4(clipPos.x, clipPos.y, depth, 1.0f);
	float4 fragWorldPos = mul(fragViewPos, gInvViewProj);
	fragWorldPos /= fragWorldPos.w;
	
	// Attenuation
	float4 toLight = float4(gPointLight.position, 1.0f) - fragWorldPos;
	float lightDistance = length(toLight);
	toLight = normalize(toLight);
	
	float attenuation = 1.0f / (
		gPointLight.attenuation.x +
		gPointLight.attenuation.y * lightDistance +
		gPointLight.attenuation.z * pow(lightDistance, 2));
		
	attenuation = saturate(attenuation);
	float alpha = lightDistance / gPointLight.range;
	float damping = saturate(1.0f - alpha * alpha);
	attenuation *= damping;
	
	// Final ambient
	float4 diffuseColor = tex2D(diffuseGBufferSampler, texCoord);
	float4 finalAmbient = gPointLight.ambientColor * diffuseColor * diffuseColor.a;
	
	// Final diffuse
	float4 normal = tex2D(normalGBufferSampler, texCoord) * 2 - 1;
	float nDotL = saturate(dot(normal, toLight));
	
	// Toon
	nDotL = int(nDotL * 12) / 12.0f;

	float4 finalDiffuse = diffuseColor * nDotL * gPointLight.diffuseColor;
	
	// Final specular
	float4 specularColor = tex2D(specularGBufferSampler, texCoord);
	
	float3 toCamera = normalize(gCameraPos - fragWorldPos.xyz);
	float3 reflection = normalize(reflect(-toLight, normal));
	float reflectDotCam = saturate(dot(reflection, toCamera));
	float specMod = gPointLight.specularIntensity *
		pow(reflectDotCam, gPointLight.specularPower);
		
	float4 finalSpecular = specularColor * specMod * gPointLight.specularColor;
	
	float4 finalColor = finalAmbient + finalDiffuse + finalSpecular;	
	
	return attenuation * finalColor;
}


technique PointLight
{
	pass P0
	{
		vertexShader = compile vs_3_0 PointLightVS();
        pixelShader  = compile ps_3_0 PointLightPS();
        
		CullMode = CCW;
		
		
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;
		
		
		ZWriteEnable = FALSE;
		ZEnable = FALSE;

		COLORWRITEENABLE = 0x0000000F;
    }
}


#endif