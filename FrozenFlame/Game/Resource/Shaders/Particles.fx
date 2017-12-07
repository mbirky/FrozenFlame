#ifndef _PARTICLES_FX_
#define _PARTICLES_FX_

#include "Globals.fx"

int sBlend;
int dBlend;

TParticleVertOutput DiffuseParticleVertex(TParticleInput input)
{
	TParticleVertOutput output;
	
	output.clipPos   = mul(float4(input.locPos, 1.0f), gMVP);
	output.colorMod  = input.partColor; 
	output.partScale = (output.clipPos.z) * input.scaleRot.x;
	output.rotation	 = input.scaleRot.y;
	output.texCoords = float2(1.0f,1.0f);
	
	return output;
}

TGFragOutTransparent DiffuseParticlePixel(TParticleVertOutput input) : COLOR
{
	TGFragOutTransparent output;
	float4 normal, diffuse, specular;
	float4x4 rotateZ = { cos(input.rotation), -sin(input.rotation), 0.0, 0.0,
						 sin(input.rotation), cos(input.rotation), 0.0, 0.0,
						 0.0, 0.0, 1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0   };
						 
	// Build translation / rotation matrix for texture coords - JC	
	float2 TexCoord = input.texCoords;	
	
	// Translate
	TexCoord -= 0.5f;
	
	// Rotate
	TexCoord = mul(float4(TexCoord,0.0f,0.0f),rotateZ).xy;			
	
	// Translate	
	TexCoord += 0.5f; //mul(transMat,float4(TexCoord,0.0f,1.0f)).xy;		
	
	normal = float4(0.0f,0.0f,0.0f,0.0f);
	
	diffuse = tex2D(diffuseSampler,TexCoord);
	
	//diffuse = tex2D(diffuseSampler, input.texCoords);
	
	// This is for clipping out alpha values.
	// Also clips .1 around the edges of particles
	// It's more efficient than a conditional and discard. -JC
	clip(diffuse.a < 0.25f ? -1:1);
	clip(TexCoord.x < 0.1f ? -1:1);
	clip(TexCoord.x > 0.9f ? -1:1);
	clip(TexCoord.y < 0.1f ? -1:1);
	clip(TexCoord.y > 0.9f ? -1:1);
	
	diffuse = saturate(diffuse * input.colorMod);
	
	specular = tex2D(specularSampler, input.texCoords);//float4(0.0f,0.0f,0.0f,0.0f);	
	
	output.normal = normal;
	output.diffuse = diffuse;
	output.specular = specular;
	return output;
} 

technique DiffuseParticles
{
	pass P0
	{
		vertexShader = compile vs_3_0 DiffuseParticleVertex();
		pixelShader = compile ps_3_0 DiffuseParticlePixel();
		
		cullMode = CCW;
		ZWriteEnable = false;
		POINTSPRITEENABLE = true;
		POINTSCALEENABLE  = true;
		POINTSIZE_MIN	  = 1.0f;
		POINTSIZE_MAX	  = 256.0f;
		
		AlphaBlendEnable = true;
		BlendOp = ADD;
		SrcBlend = sBlend;
		DestBlend = dBlend;
	}
}

#endif