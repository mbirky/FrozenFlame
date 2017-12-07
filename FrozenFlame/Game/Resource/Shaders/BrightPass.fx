#ifndef _BRIGHT_PASS_FX_
#define _BRIGHT_PASS_FX_

#include "Globals.fx"

float gLuminance = 0.08f;
static const float sMiddleGray = 0.18f;	// default: 0.18
static const float sWhiteCutoff = 0.83f; // default: 0.8

TFinalOutput BrightPassVS(TGeneralInput input)
{
	TFinalOutput output;
	
	output.position = float4(input.locPos, 1.0f);
	output.screenPos = float4(input.locPos, 1.0f);
	output.texCoords = input.texCoords;
	
	return output;
}

float4 BrightPassPS(TFinalOutput input) : COLOR0
{
	float2 texCoord = input.texCoords + gHalfPixel;
	float4 color = tex2D(sourceTexSampler, texCoord);
	
    color *= sMiddleGray / ( gLuminance + 0.001f );
    color *= ( 1.0f + ( color / ( sWhiteCutoff * sWhiteCutoff ) ) );
    color -= 5.0f;

    color = max( color, 0.0f );

    color /= ( 10.0f + color );

	return color;
}

technique BrightPass
{
    pass P0
    {
        vertexShader = compile vs_2_0 BrightPassVS();
        pixelShader  = compile ps_2_0 BrightPassPS();
        ZEnable = false;
    }
}


#endif