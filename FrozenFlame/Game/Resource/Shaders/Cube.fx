#ifndef _CUBE_FX_
#define _CUBE_FX

// Uniforms
float4x4 gWorld : WORLD;
float4x4 gViewProj : VIEWPROJECTION;
float4 gColor;


float4 Debug_VertexShader(float3 vPos:POSITION) : POSITION0
{
	// TODO: Transform the position into world space.
	float4 worldPos = mul(float4(vPos, 1.0f), gWorld);

	// TODO: Transform the position into homogenous clip space.
	return mul(worldPos, gViewProj);
}

float4 Debug_PixelShader(float3 vClipPos : POSITION0) : COLOR
{
	return gColor;
}

// Techniques are read in by the effect framework.
// They allow one to add variation to how a particular shader might be executed. 
technique myTechnique
{
	// Each pass in the technique corresponds to a single iteration over all 
	// verticies sent during a draw call and the subsequently created pixels. 
	// If we had a special effect that required us to draw our geometry and pixels more than once,
	// we could acheive this by adding more passes across our vertex and pixel data.
    pass FirstPass
    {
		// request what vertex and pixel shader are to be used, this can change for each pass.
        vertexShader = compile vs_2_0 Debug_VertexShader();
        pixelShader  = compile ps_2_0 Debug_PixelShader();
		// Setting a few of the many D3D renderstates via the effect framework
		ShadeMode = FLAT; // smooth color interpolation across triangles
        FillMode = SOLID;// no wireframes, no point drawing.
        //CullMode = CW; // cull any clockwise polygons.
		ZWriteEnable = true;
    }
}

#endif