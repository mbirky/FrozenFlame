#ifndef _UTIL_H_
#define _UTIL_H_

#include "Matrix4x4.h"
#include "vec3.h"
#include "vec4.h"

#include <d3d9.h>
#include <d3dx9.h>

typedef matrix4<double> mat4d;
typedef matrix4<float> mat4f;

class CBaseObject;

void OrthoNormalInverse( mat4d &MatrixO, const mat4d &MatrixA );
mat4d OrthoInverse(const mat4d & myMatrix);
void VectorByMatrixMult(vec3d & Out, vec3d Vector, mat4d & Matrix);
void VectorByMatrixMult(vec3f & Out, vec3f Vector, mat4f & Matrix);
vec4d vec4Multiply(vec4d & a, mat4d & b);

D3DXMATRIX InterpolateD3DXMatrix(const D3DXMATRIX& MatrixA, const D3DXMATRIX& MatrixB, float lamda);

// returns true if this is a valid float, false if the value is invalid (not a number)
bool FloatIsGood(float testValue);

// Makes the object oscillate left and right
// fStartPos = oscillates about this point on the x, in world coordinates
// fFrequency = how quickly to oscillate
// fAmplitude = how far to move left and how far to move right
void OscillateOnX(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude);

// Makes the object oscillate up and down
// fStartPos = oscillates about this point on the y, in world coordinates
// fFrequency = how quickly to oscillate
// fAmplitude = how far to move up and how far to move down
void OccillateOnY(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude);

// Makes the object oscillate up and down
// fStartPos = oscillates about this point on the z, in world coordinates
// fFrequency = how quickly to oscillate
// fAmplitude = how far to move in and how far to move out
void OccillateOnZ(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude);

//void OscillateRotationOnX(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude);

#endif