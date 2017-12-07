
#include "Util.h"
#include "../Objects/BaseObject.h"
#include "../CGame.h"

void OrthoNormalInverse( mat4d &MatrixO, const mat4d &MatrixA )
{
	MatrixO = MatrixA;
	MatrixO.xy = MatrixA.yx;
	MatrixO.xz = MatrixA.zx;
	MatrixO.yx = MatrixA.xy;
	MatrixO.yz = MatrixA.zy;
	MatrixO.zx = MatrixA.xz;
	MatrixO.zy = MatrixA.yz;

	MatrixO.wx = -dot_product(MatrixA.axis_pos, MatrixA.axis_x);
	MatrixO.wy = -dot_product(MatrixA.axis_pos, MatrixA.axis_y);
	MatrixO.wz = -dot_product(MatrixA.axis_pos, MatrixA.axis_z);
}

mat4d OrthoInverse(const mat4d & myMatrix)
{
	mat4d temp;
	temp.wx = -dot_product(myMatrix.axis_x, myMatrix.axis_w);
	temp.wy = -dot_product(myMatrix.axis_y, myMatrix.axis_w);
	temp.wz = -dot_product(myMatrix.axis_z, myMatrix.axis_w);
	temp.ww = 1.0f;

	temp.xx = myMatrix.xx;
	temp.xy = myMatrix.yx;
	temp.xz = myMatrix.zx;
	temp.xw = 0;

	temp.yx = myMatrix.xy;
	temp.yy = myMatrix.yy;
	temp.yz = myMatrix.zy;
	temp.yw = 0;

	temp.zx = myMatrix.xz;
	temp.zy = myMatrix.yz;
	temp.zz = myMatrix.zz;
	temp.zw = 0;

	return temp;
}

vec4d vec4Multiply(vec4d & a, mat4d & b)
{
	vec4d temp;
	temp.x = a.x * b.xx + a.y * b.yx + a.z * b.zx + a.w * b.wx;
	temp.y = a.x * b.xy + a.y * b.yy + a.z * b.zy + a.w * b.wy;
	temp.z = a.x * b.xz + a.y * b.yz + a.z * b.zz + a.w * b.wz;
	temp.w = a.x * b.xw + a.y * b.yw + a.z * b.zw + a.w * b.ww;
	return temp;
}

void VectorByMatrixMult(vec3d & Out, vec3d Vector, mat4d & Matrix)
{
	//vec3d Out;
	Out.x = Vector.x * Matrix.xx + Vector.y * Matrix.yx + Vector.z * Matrix.zx + Matrix.wx;
	Out.y = Vector.x * Matrix.xy + Vector.y * Matrix.yy + Vector.z * Matrix.zy + Matrix.wy;
	Out.z = Vector.x * Matrix.xz + Vector.y * Matrix.yz + Vector.z * Matrix.zz + Matrix.wz;

	//return Out;
}

void VectorByMatrixMult(vec3f & Out, vec3f Vector, mat4f & Matrix)
{
	//vec3d Out;

	Out.x = Vector.x * Matrix.xx + Vector.y * Matrix.yx + Vector.z * Matrix.zx + Matrix.wx;
	Out.y = Vector.x * Matrix.xy + Vector.y * Matrix.yy + Vector.z * Matrix.zy + Matrix.wy;
	Out.z = Vector.x * Matrix.xz + Vector.y * Matrix.yz + Vector.z * Matrix.zz + Matrix.wz;

	//return Out;
}

D3DXMATRIX InterpolateD3DXMatrix(const D3DXMATRIX& MatrixA, const D3DXMATRIX& MatrixB, float lamda)
{
	D3DXMATRIX iMat;
	D3DXMATRIX result(MatrixB);

	// Inverse of MatrixA
	D3DXMatrixInverse(&iMat, nullptr, &MatrixA);

	// Remove MatrixA's transformation from MatrixB
	result *= iMat;

	// iMat is now the intermediary transformation from MatrixA to MatrixB
	// ie: iMat * MatrixA = MatrixB
	iMat = result;

	// The trace of our matrix
	float trace = 1.0f + iMat[0] + iMat[5] + iMat[10];

	float quatResult[4];

	// Calculate the quaternion of iMat
	// If trace is greater than 0, but consider small values that
	// might result in 0 when operated upon due to floating point error
	if( trace > 0.00000001f )
	{
		float S = sqrt(trace)*2.0f;
		quatResult[0] = (iMat[9] - iMat[6]) / S;
		quatResult[1] = (iMat[2] - iMat[8]) / S;
		quatResult[2] = (iMat[4] - iMat[1]) / S;
		quatResult[3] = 0.25f * S;
	}
	else
	{
		if( iMat[0] > iMat[5] && iMat[0] > iMat[10] )
		{
			float S = sqrt( 1.0f + iMat[0] - iMat[5] - iMat[10] ) * 2.0f;
			quatResult[0] = 0.25f * S;
			quatResult[1] = (iMat[4] + iMat[1]) / S;
			quatResult[2] = (iMat[2] + iMat[8]) / S;
			quatResult[3] = (iMat[9] - iMat[6]) / S;
		}
		else if( iMat[5] > iMat[10] )
		{
			float S = sqrt( 1.0f + iMat[5] - iMat[0] - iMat[10] ) * 2.0f;
			quatResult[0] = (iMat[4] + iMat[1]) / S;
			quatResult[1] = 0.25f * S;
			quatResult[2] = (iMat[9] + iMat[6]) / S;
			quatResult[3] = (iMat[2] - iMat[8]) / S;
		}
		else
		{
			float S = sqrt( 1.0f + iMat[10] - iMat[0] - iMat[5] ) * 2.0f;
			quatResult[0] = (iMat[2] + iMat[8]) / S;
			quatResult[1] = (iMat[9] + iMat[6]) / S;
			quatResult[2] = 0.25f * S;
			quatResult[3] = (iMat[4] - iMat[1]) / S;
		}
	}

	// Get the magnitude of our quaternion
	float quatMagnitude = sqrt( quatResult[0]*quatResult[0] + quatResult[1]*quatResult[1] + quatResult[2]*quatResult[2] + quatResult[3]*quatResult[3] );

	// Normalize our quaternion
	float quatNormalized[4] = { quatResult[0]/quatMagnitude, quatResult[1]/quatMagnitude, quatResult[2]/quatMagnitude, quatResult[3]/quatMagnitude }; 

	// Calculate the angles relevant to our quaternion
	float cos_a = quatNormalized[3];
	float angle = acos( cos_a ) * 2;
	float sin_a = sqrt( 1.0f - cos_a * cos_a );

	// If there was no rotation between matrices, calculation
	// of the rotation matrix will end badly. So just do the linear
	// interpolation of the translation component and return
	if( angle == 0.0f )
	{
		result = MatrixA;

		result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0])*lamda);
		result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1])*lamda);
		result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2])*lamda);

		return result;
	}


	// Our axis of abitrary rotation
	vec3<float> axis;

	if( fabs( sin_a ) < 0.0005f )
		sin_a = 1;

	axis.x = quatNormalized[0] / sin_a;
	axis.y = quatNormalized[1] / sin_a;
	axis.z = quatNormalized[2] / sin_a;

	// Get the portion of the angle to rotate by
	angle *= lamda;

	axis.normalize();

	// Calculate the quaternion for our new (partial) angle of rotation
	sin_a = sin( angle / 2 );
	cos_a = cos( angle / 2 );
	quatNormalized[0] = axis.x * sin_a;
	quatNormalized[1] = axis.y * sin_a;
	quatNormalized[2] = axis.z * sin_a;
	quatNormalized[3] = cos_a;

	quatMagnitude = sqrt( quatNormalized[0]*quatNormalized[0] + quatNormalized[1]*quatNormalized[1] + quatNormalized[2]*quatNormalized[2] + quatNormalized[3]*quatNormalized[3] );		
	quatNormalized[0] /= quatMagnitude;
	quatNormalized[1] /= quatMagnitude;
	quatNormalized[2] /= quatMagnitude;
	quatNormalized[3] /= quatMagnitude;

	// Calculate our partial rotation matrix
	float xx      = quatNormalized[0] * quatNormalized[0];
	float xy      = quatNormalized[0] * quatNormalized[1];
	float xz      = quatNormalized[0] * quatNormalized[2];
	float xw      = quatNormalized[0] * quatNormalized[3];
	float yy      = quatNormalized[1] * quatNormalized[1];
	float yz      = quatNormalized[1] * quatNormalized[2];
	float yw      = quatNormalized[1] * quatNormalized[3];
	float zz      = quatNormalized[2] * quatNormalized[2];
	float zw      = quatNormalized[2] * quatNormalized[3];

	result[0]  = 1 - 2 * ( yy + zz );
	result[1]  =     2 * ( xy - zw );
	result[2]  =     2 * ( xz + yw );
	result[4]  =     2 * ( xy + zw );
	result[5]  = 1 - 2 * ( xx + zz );
	result[6]  =     2 * ( yz - xw );
	result[8]  =     2 * ( xz - yw );
	result[9]  =     2 * ( yz + xw );
	result[10] = 1 - 2 * ( xx + yy );
	result[3]  = result[7] = result[11] = result[12] = result[13] = result[14] = 0;
	result[15] = 1;

	// Combine our partial rotation with MatrixA
	result *= MatrixA;

	// Linear interpolation of the translation components of the matrices
	result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0])*lamda);
	result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1])*lamda);
	result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2])*lamda);

	return result;
}

// returns true if this is a valid float, false if the value is invalid (not a number)
bool FloatIsGood(float testValue)
{
	if (testValue == testValue)
	{
		// So far so good, but check to make sure the test value isn't infinity or negative infinity
		if ( (testValue + 1) > testValue)
			return true;	// good value
		else
			return false;	// valid number, but is infinity or negative infinity
	}
	//else
		return false;	// not a valid number (e.g. -1.#IND000)
}

void OscillateOnX(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude)
{
	//SetPosX(fStartPos + sin((CGame::GetInstance()->GetGameTime() ) * fFrequency) * fAmplitude);
	matrix4f worldMatrix = *(pObject->GetWorldMatrix());
	worldMatrix.axis_pos.x = (fStartPos + sin(CGame::GetInstance()->GetRunningGameTime() * fFrequency * fElapsedTime) * fAmplitude);
	pObject->SetWorldMatrix(worldMatrix);
}

void OccillateOnY(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude)
{
	//SetPosY(fStartPos + sin((CGame::GetInstance()->GetGameTime() ) * fFrequency) * fAmplitude);
	matrix4f worldMatrix = *(pObject->GetWorldMatrix());
	worldMatrix.axis_pos.y = (fStartPos + sin(CGame::GetInstance()->GetRunningGameTime() * fFrequency * fElapsedTime) * fAmplitude);
	pObject->SetWorldMatrix(worldMatrix);
}

void OccillateOnZ(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude)	
{
	matrix4f worldMatrix = *(pObject->GetWorldMatrix());
	worldMatrix.axis_pos.z = (fStartPos + sin(CGame::GetInstance()->GetRunningGameTime() * fFrequency * fElapsedTime) * fAmplitude);
	pObject->SetWorldMatrix(worldMatrix);
}

//void OscillateRotationOnX(CBaseObject* pObject, float fElapsedTime, float fStartPos, float fFrequency, float fAmplitude)
//{
//	matrix4f worldMatrix = *(pObject->GetWorldMatrix());
//	worldMatrix.rotate_x_pre((sin(CGame::GetInstance()->GetRunningGameTime() * fFrequency * fElapsedTime) * fAmplitude));
//	pObject->SetWorldMatrix(worldMatrix);
//}