#include "../Header/BaseBehavior.h"
#include "../Header/CBehaviorManager.h"
#include "../../Collision/CollisionLibrary.h"
#include "../../Core/CWorldManager.h"

void CBaseBehavior::Move(vec3f _tVec, float _fElapsedTime)
{
	GetOwner()->GetAgent()->SetIsMoving(true);
	// Create a temporary vector equal to the agent's z vector and normalize it.
	vec3f tempVec = GetOwner()->GetAgent()->GetWorldMatrix()->axis_z;
	tempVec = tempVec.normalize();
	_tVec = _tVec.normalize();
	// Grab the length of the passed in vector, the amount to move.
	float length = _tVec.magnitude();

	// Scale the temporary vector by the length to get the amount of distance to travel on the z axis.
	tempVec *= length;

	 //Translate the object in that direction.
	GetOwner()->GetAgent()->SetVelocity(_tVec * GetOwner()->GetAgent()->GetMoveSpeed());
}

void CBaseBehavior::TurnTo(vec3f _tVec, float _fElapsedTime)
{
	vec3f vectorToPoint = _tVec - GetOwner()->GetAgent()->GetWorldMatrix()->axis_pos;

	vectorToPoint = vectorToPoint.normalize();

	float result = dot_product(GetOwner()->GetAgent()->GetWorldMatrix()->axis_x, vectorToPoint);

	if (result > FLT_EPSILON || result < -FLT_EPSILON)
	{
		matrix4f worldMatrix = *(GetOwner()->GetAgent()->GetWorldMatrix());
		worldMatrix.rotate_y_post(result * _fElapsedTime * GetOwner()->GetAgent()->GetMoveSpeed());
		GetOwner()->GetAgent()->SetWorldMatrix(worldMatrix);
	//	GetOwner()->GetAgent()->GetMatrix()->rotate_y_post(result * _fElapsedTime * GetOwner()->GetAgent()->GetMoveSpeed());
	}
}

void CBaseBehavior::LookAt(vec3f _tVec, float _fElapsedTime)
{

	matrix4f currMatrix = *GetOwner()->GetAgent()->GetWorldMatrix();
	matrix4f targetMatrix;
	targetMatrix.make_identity();
	targetMatrix.axis_pos = _tVec;

	//vec3f scale = currMatrix.scale();

	// geting the scale values separate from the rest of the matrix

	vec3f scale;

	scale.x = sqrt(dot_product(currMatrix.axis_x,currMatrix.axis_x));
	scale.y = sqrt(dot_product(currMatrix.axis_y,currMatrix.axis_y));
	scale.z = sqrt(dot_product(currMatrix.axis_z,currMatrix.axis_z));

	vec3f newX;
	vec3f newY;
	vec3f newZ;
	vec3f worldUp;

	worldUp = vec3f(0.0f,1.0f,0.0f);

	newZ = vec3f(targetMatrix.wx - currMatrix.wx,targetMatrix.wy - currMatrix.wy,targetMatrix.wz - currMatrix.wz);

	float normal = sqrt(dot_product(newZ,newZ));

	newZ.x /= normal;
	newZ.y /= normal;
	newZ.z /= normal;

	cross_product(newX,worldUp,newZ);
	newX.normalize();

	cross_product(newY,newZ,newX);
	newY.normalize();

	currMatrix.axis_x = newX;
	currMatrix.axis_y = newY;
	currMatrix.axis_z = newZ;


	currMatrix.scale_pre(scale);

	//currMatrix.xx = scale.x;
	//currMatrix.yy = scale.y;
	//currMatrix.zz = scale.z;

	GetOwner()->GetAgent()->SetWorldMatrix(currMatrix);
//	(*GetOwner()->GetAgent()->GetMatrix()) = currMatrix;

	// Agent Position
	/*vec3f agentPos = GetOwner()->GetAgent()->GetMatrix()->axis_pos;
	
	vec3f targetPos = _tVec;

	vec3f scaleValues = vec3f(GetOwner()->GetAgent()->GetMatrix()->xx, GetOwner()->GetAgent()->GetMatrix()->yy,
							  GetOwner()->GetAgent()->GetMatrix()->zz);

	vec3f zAxis = agentPos - targetPos;
	zAxis.normalize();

	vec3f xAxis;
	cross_product(xAxis, vec3f(0.0f, 1.0f, 0.0f), zAxis);

	xAxis.normalize();

	vec3f yAxis;
	cross_product(yAxis, zAxis, xAxis);
	yAxis.normalize();

	matrix4f tempMat;
	tempMat.make_identity();
	tempMat.axis_x = xAxis;
	tempMat.axis_y = yAxis;
	tempMat.axis_z = zAxis;
	tempMat.axis_w = GetOwner()->GetAgent()->GetMatrix()->axis_w;
	tempMat.xx = scaleValues.x;
	tempMat.yy = scaleValues.y;
	tempMat.zz = scaleValues.z;

	*GetOwner()->GetAgent()->GetMatrix() = tempMat;

	*/


	/*vec3f vectorToPoint = _tVec - GetOwner()->GetAgent()->GetMatrix()->axis_pos;

	vectorToPoint = vectorToPoint.normalize();

	float result = dot_product(GetOwner()->GetAgent()->GetMatrix()->axis_x, vectorToPoint);

	if (result > FLT_EPSILON || result < -FLT_EPSILON)
	{
		GetOwner()->GetAgent()->GetMatrix()->rotate_y_post(D3DXToRadian(result * 180.0f));
	}*/
}

bool CBaseBehavior::CheckTrapDanger(vector<CCollidableObject*> &cCloseTraps)
{
	vector<CCollidableObject*> cTraps = GetOwner()->GetObjectManager()->GetTraps();
	
	for(unsigned int i = 0; i < cTraps.size();i++)
	{
		if(CheckCollision(GetOwner()->GetAgent(),cTraps[i],SPHERE_TO_AABB))// || abs(vec3f(cTraps[i]->GetPos() - GetOwner()->GetAgent()->GetPos()).magnitude()) < 12.0f)
		{
			cCloseTraps.push_back(cTraps[i]);
		}
	}

	vector<AABB> & vAABBs =  CWorldManager::GetInstance()->GetWallAABBs();

	GetOwner()->GetAgent()->SetSphereRad(GetOwner()->GetAgent()->GetSphereRad() * 2);

	//for(unsigned int i = 0; i < vAABBs.size();i ++)
	//{
	//	CCollidableObject * cTemp = new CCollidableObject();
	//	cTemp->SetAABBMax(vAABBs[i].max);
	//	cTemp->SetAABBMin(vAABBs[i].min);
	//	cTemp->SetPos((vAABBs[i].max+vAABBs[i].min)/2);

	//	if(CheckCollision(GetOwner()->GetAgent(),cTemp,SPHERE_TO_AABB))// || abs(vec3f(cTraps[i]->GetPos() - GetOwner()->GetAgent()->GetPos()).magnitude()) < 12.0f)
	//	{
	//		cCloseTraps.push_back(cTemp);
	//	}
	//	else
	//	{
	//		delete cTemp;
	//	}
	//}

	GetOwner()->GetAgent()->SetSphereRad(GetOwner()->GetAgent()->GetSphereRad() / 2);


	if(cCloseTraps.size() > 0)
		return true;
	else
		return false;
}

void CBaseBehavior::AvoidTraps(float fElapsedTime)
{
	vector<CCollidableObject*> cTraps;

	if(CheckTrapDanger(cTraps))
	{
		TrapReaction(cTraps,fElapsedTime);
	}
	

}

void CBaseBehavior::TrapReaction(vector<CCollidableObject*> &cCloseTraps,float fElapsedTime)
{
	if(cCloseTraps.size() < 1)
		return;

	vec3f totalVectors;
	totalVectors.make_zero();

	for(unsigned int i =0; i < cCloseTraps.size();i++)
	{
		totalVectors +=  this->GetOwner()->GetAgent()->GetPos() - cCloseTraps[i]->GetPos();
		if(cCloseTraps[i]->GetType() == COLLIDABLE_OBJECT)
		{
			vec3f temp =  this->GetOwner()->GetAgent()->GetPos() - cCloseTraps[i]->GetPos();
			temp.z *= 100.0f;
			totalVectors += temp;
			//totalVectors.z -= 1000.0f;
			delete cCloseTraps[i];
		}
		else
		{
			totalVectors +=  this->GetOwner()->GetAgent()->GetPos() - cCloseTraps[i]->GetPos();
		}
	}

	totalVectors.y = 0.0f;

	totalVectors /= cCloseTraps.size();

	//totalVectors *= -1;

	//totalVectors.normalize();

	LookAt(totalVectors,fElapsedTime);
	Move(totalVectors,fElapsedTime);

}

void CBaseBehavior::SoftAttach(vec3f vPos, float fElapsedTime)
{
	vec3f vDir = vPos - GetOwner()->GetAgent()->GetPos();

	float fWeight = vDir.magnitude();
	//fWeight = fWeight*fWeight;
	//fWeight *= 0.05f;
	if(abs(fWeight) < 0.001f)
		return;
	//fWeight *= 0.2f;
	vDir = vDir.normalize();

	GetOwner()->GetAgent()->SetPos(GetOwner()->GetAgent()->GetPos() + vDir * fWeight * fElapsedTime);


}

