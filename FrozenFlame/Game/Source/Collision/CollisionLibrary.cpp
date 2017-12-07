#include "CollisionLibrary.h"
#include "ED2CollisionLibrary.h"
#include "../Objects/PlayerObject.h"
#include "../Core/CWorldManager.h"
#include <algorithm>
using namespace::std;

bool CheckCollision(CCollidableObject* cObject1,CCollidableObject* cObject2,int nCollisionType)
{
	switch(nCollisionType)
	{
	case AABB_TO_AABB:
		{
			AABB tObject1;
			AABB tObject2;

			cObject1->GetCollisionAABB(tObject1.min,tObject1.max);
			cObject2->GetCollisionAABB(tObject2.min,tObject2.max);

			return AABBtoAABB(tObject1,tObject2);

		}

	case SPHERE_TO_SPHERE:
		{
			Sphere tObject1;
			Sphere tObject2;

			cObject1->GetCollisionSphere(tObject1.m_Center,tObject1.m_Radius);
			cObject2->GetCollisionSphere(tObject2.m_Center,tObject2.m_Radius);

			return SphereToSphere(tObject1,tObject2);

		}
	case SPHERE_TO_AABB:
		{
			Sphere tObject1;
			AABB tObject2;

			cObject1->GetCollisionSphere(tObject1.m_Center,tObject1.m_Radius);
			cObject2->GetCollisionAABB(tObject2.min,tObject2.max);

			return SphereToAABB(tObject1,tObject2);
		}
	case AABB_TO_SPHERE:
		{
			AABB tObject1;
			Sphere tObject2;

			cObject1->GetCollisionAABB(tObject1.min,tObject1.max);
			cObject2->GetCollisionSphere(tObject2.m_Center,tObject2.m_Radius);

			return SphereToAABB(tObject2,tObject1);
		}
	}

	return false;

}

bool PlayerAttackCheck(CPlayerObject * pcPlayer,CEnemy* pcEnemy)
{
	pcPlayer->SetSphereRad(pcPlayer->GetSphereRad()*1.5f);

	if(!CheckCollision((CCollidableObject*)pcPlayer,(CCollidableObject*)pcEnemy,SPHERE_TO_AABB))
	{
		pcPlayer->SetSphereRad(pcPlayer->GetSphereRad()/1.5f);
		return false;
	}

	pcPlayer->SetSphereRad(pcPlayer->GetSphereRad()/1.5f);

	if(pcEnemy->GetType() == RANGED_OBJECT)
		return false;

	Sphere testSphere;
	AABB testAABB;

	pcPlayer->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);

	testSphere.m_Radius *= 10;

	//pcEnemy->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);
	pcEnemy->GetCollisionAABB(testAABB.min,testAABB.max);

	if(!SphereToAABB(testSphere,testAABB))
	{
		return false;
	}
	else 
	{
		return PlayerHalfSpaceTest(pcPlayer,pcEnemy);
	}
	//vec3f pointA = pcPlayer->GetMatrix()->axis_pos;
	//vec3f pointB = pointA;
	//pointB.y += 10.0f;
	//vec3f pointC = pcPlayer->GetMatrix()->axis_pos;

	//switch((int)pcPlayer->GetCurrRotation())
	//{
	//case 0:
	//	{
	//		pointC.x += 1;
	//		break;
	//	}
	//case 360:
	//	{
	//		pointC.x += 1;
	//		break;
	//	}

	//case 45:
	//	{
	//		pointC.x += 1;
	//		pointC.z -= 1;
	//		break;
	//	}
	//case 90:
	//	{
	//		pointC.z -= 1;
	//		break;
	//	}
	//case 135:
	//	{
	//		pointC.z -= 1;
	//		pointC.x -= 1;
	//		break;
	//	}
	//case 180:
	//	{
	//		pointC.x -= 1;
	//		break;
	//	}
	//case 225:
	//	{
	//		pointC.x -= 1;
	//		pointC.z += 1;
	//		break;
	//	}
	//case 270:
	//	{
	//		pointC.z += 1;
	//		break;
	//	}
	//case -45:
	//	{
	//		pointC.x += 1;
	//		pointC.z += 1;
	//		break;
	//	}
	//case 315:
	//	{
	//		pointC.x += 1;
	//		pointC.z += 1;
	//		break;
	//	}
	//}

	//Plane testPlane;

	//ComputePlane(testPlane,pointA,pointB,pointC);

	//Sphere testSphere;
	//AABB testAABB;

	//pcPlayer->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);

	//testSphere.m_Radius *= 10;

	////pcEnemy->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);
	//pcEnemy->GetCollisionAABB(testAABB.min,testAABB.max);
	//
	//if(!SphereToAABB(testSphere,testAABB))
	//{
	//	return false;
	//}
	//else if( (ClassifyAabbToPlane(testPlane,testAABB) != 2))
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	//return (ClassifySphereToPlane(testPlane,testSphere) == 1);
}

void NormalCollisionReaction(CCollidableObject * pcObject1,CCollidableObject * pcObject2)
{
	vec3f vObject1Min;
	vec3f vObject1Max;
	vec3f vObject2Min;
	vec3f vObject2Max;

	pcObject1->GetCollisionAABB(vObject1Min,vObject1Max);
	pcObject2->GetCollisionAABB(vObject2Min,vObject2Max);

	vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;

	vec3f vExtent1 = vObject1Max - vCenter1;
	vec3f vExtent2 = vObject2Max - vCenter2;

	vec3f vD = vCenter1-vCenter2;

	float fMinPenetration = FLT_MAX;
	unsigned int nPenAxis = 0;

	for(unsigned int i = 0; i < 3;i++)
	{
		float fPenetration = (vExtent1.array()[i] + vExtent2.array()[i]) - abs(vD.array()[i]);

		if(fPenetration <= 0.0f)
		{
			return;
		}

		if(fPenetration < fMinPenetration)
		{
			fMinPenetration = fPenetration;
			nPenAxis = i;
		}

	}

	if(vD.array()[nPenAxis] < 0)
	{
		fMinPenetration = - fMinPenetration;
	}
	matrix4f worldMatrix = *(pcObject1->GetWorldMatrix());
	worldMatrix.axis_pos.array()[nPenAxis] += fMinPenetration * 0.5f;
	pcObject1->SetWorldMatrix(worldMatrix);

	worldMatrix = *(pcObject2->GetWorldMatrix());
	worldMatrix.axis_pos.array()[nPenAxis] -= fMinPenetration * 0.5f;
	pcObject2->SetWorldMatrix(worldMatrix);
//	pcObject1->GetMatrix()->axis_pos.array()[nPenAxis] += fMinPenetration * 0.5f;
//	pcObject2->GetMatrix()->axis_pos.array()[nPenAxis] -= fMinPenetration * 0.5f;

	//vec3f vOverlap1;
	//vec3f vOverlap2;

	//vOverlap1 = vObject1Max - vObject2Min;
	//vOverlap2 = vObject2Max - vObject1Min;
	//vOverlap1.y = 0.0f;
	//vOverlap2.y = 0.0f;

	//

	//if(vOverlap1.magnitude() < vOverlap2.magnitude())
	//{
	//	vOverlap1 = vOverlap1 * 0.5f;
	//	if(vOverlap1.x < vOverlap1.z)
	//	{
	//		pcObject1->GetMatrix()->axis_pos.x -= vOverlap1.x;
	//		pcObject2->GetMatrix()->axis_pos.x += vOverlap1.x;
	//	}
	//	else
	//	{
	//		pcObject1->GetMatrix()->axis_pos.z -= vOverlap1.z;
	//		pcObject2->GetMatrix()->axis_pos.z += vOverlap1.z;
	//	}
	//}
	//else//if(vOverlap1.magnitude() < vOverlap2.magnitude())
	//{
	//	vOverlap2 = vOverlap2 * 0.5f;
	//	if(vOverlap2.x < vOverlap2.z)
	//	{
	//		pcObject1->GetMatrix()->axis_pos.x -= vOverlap2.x;
	//		pcObject2->GetMatrix()->axis_pos.x += vOverlap2.x;
	//	}
	//	else
	//	{
	//		pcObject1->GetMatrix()->axis_pos.z -= vOverlap2.z;
	//		pcObject2->GetMatrix()->axis_pos.z += vOverlap2.z;
	//	}
	//}

	


}

void AverageCollisionReaction(CPlayerObject* pObjOne, CCollidableObject* pObjTwo, vec3f& vCorrection)
{
	if (nullptr == pObjOne || nullptr == pObjTwo)
		return;

	vec3f vObject1Min;
	vec3f vObject1Max;
	vec3f vObject2Min;
	vec3f vObject2Max;

	if(pObjOne->IsDashing() && !(pObjTwo->GetType() == ENVIRONMENT_OBJECT && ((CEnvironmentObject*)pObjTwo)->GetSubType() == CEnvironmentObject::ENVOBJ_ICECUBE))
	{
		matrix4f worldMatrix = *(pObjOne->GetWorldMatrix());
		worldMatrix.axis_pos = pObjOne->GetLastPosition();
		worldMatrix.axis_pos.x += 0.5f;
		worldMatrix.axis_pos.z += 0.5f;
		pObjOne->SetWorldMatrix(worldMatrix);
//		pObjOne->GetMatrix()->axis_pos = pObjOne->GetLastPosition();
//		pObjOne->GetMatrix()->axis_pos.x += 0.5f;
//		pObjOne->GetMatrix()->axis_pos.z += 0.5f;

		const CRenderInstance* player = CView::GetRenderInstance(pObjOne->GetRenderID());
		player->RecalculateBounds();

		pObjOne->SetDashing(false);
		pObjOne->SetVelocity(vec3f(0.0f,0.0f,0.0f));
	}

	pObjOne->GetCollisionAABB(vObject1Min,vObject1Max);
	pObjTwo->GetCollisionAABB(vObject2Min,vObject2Max);

	if(pObjOne->IsDashing())
	{
		vObject1Min -= pObjOne->GetVelocity();
		vObject1Max -= pObjOne->GetVelocity();
	}

	vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;

	vec3f vExtent1 = vObject1Max - vCenter1;
	vec3f vExtent2 = vObject2Max - vCenter2;	

	vec3f vD = vCenter1-vCenter2;

	float fMinPenetration = FLT_MAX;
	unsigned int nPenAxis = 0;

	for(unsigned int i = 0; i < 3;i++)
	{
		float fPenetration = (vExtent1.array()[i] + vExtent2.array()[i]) - abs(vD.array()[i]);

		if(fPenetration <= 0.0f)
		{
			return;
		}

		if(fPenetration < fMinPenetration)
		{
			fMinPenetration = fPenetration;
			nPenAxis = i;
		}

	}

	if(vD.array()[nPenAxis] < 0)
	{
		fMinPenetration = - fMinPenetration;
	}
		
	//if(vCorrection.v[nPenAxis] > fMinPenetration)
		vCorrection.v[nPenAxis] = fMinPenetration;
}

void PlayerDashingCollisionReaction(CPlayerObject * pcPlayer,CEnemy * pcObject)
{
	if(pcObject->GetIsDying())
		return;

	vec3f vObject1Min;
	vec3f vObject1Max;
	vec3f vObject2Min;
	vec3f vObject2Max;

	pcPlayer->GetCollisionAABB(vObject1Min,vObject1Max);
	pcObject->GetCollisionAABB(vObject2Min,vObject2Max);

	vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;

	vec3f vExtent1 = vObject1Max - vCenter1;
	vec3f vExtent2 = vObject2Max - vCenter2;

	vec3f vD = vCenter1-vCenter2;

	float fMinPenetration = FLT_MAX;
	unsigned int nPenAxis = 0;

	for(unsigned int i = 0; i < 3;i++)
	{
		float fPenetration = (vExtent1.array()[i] + vExtent2.array()[i]) - abs(vD.array()[i]);

		if(fPenetration <= 0.0f)
		{
			return;
		}

		if(fPenetration < fMinPenetration)
		{
			fMinPenetration = fPenetration;
			nPenAxis = i;
		}

	}

	if(vD.array()[nPenAxis] < 0)
	{
		fMinPenetration = - fMinPenetration;
	}

	if(fMinPenetration < 0.0f)
	{
		fMinPenetration = -1.0f;
	}
	else
	{
		fMinPenetration = 1.0f;
	}

	vec3f vNewVelocity;
	vNewVelocity.make_zero();

	if(!pcObject->GetIsDying()) {
		vNewVelocity.array()[nPenAxis] = fMinPenetration * -100.0f; }
	else {
		vNewVelocity.array()[nPenAxis] = fMinPenetration * -10.0f; }
	//pcObject1->GetMatrix()->axis_pos.array()[nPenAxis] += fMinPenetration * 0.5f;
	pcObject->SetKnockbackVelocity(vNewVelocity);
}

void EnviromentCollisionReaction(CEnvironmentObject * pcEnvironment,CCollidableObject * pcObject)
{
	vec3f vObject1Min;
	vec3f vObject1Max;
	vec3f vObject2Min;
	vec3f vObject2Max;

	pcObject->GetCollisionAABB(vObject1Min,vObject1Max);
	pcEnvironment->GetCollisionAABB(vObject2Min,vObject2Max);

	vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;

	vec3f vExtent1 = vObject1Max - vCenter1;
	vec3f vExtent2 = vObject2Max - vCenter2;

	vec3f vD = vCenter1-vCenter2;

	float fMinPenetration = FLT_MAX;
	unsigned int nPenAxis = 0;

	for(unsigned int i = 0; i < 3;i++)
	{
		float fPenetration = (vExtent1.array()[i] + vExtent2.array()[i]) - abs(vD.array()[i]);

		if(fPenetration <= 0.0f)
		{
			return;
		}

		if(fPenetration < fMinPenetration)
		{
			fMinPenetration = fPenetration;
			nPenAxis = i;
		}

	}

	if(vD.array()[nPenAxis] < 0)
	{
		fMinPenetration = - fMinPenetration;
	}

	matrix4f worldMatrix = *(pcObject->GetWorldMatrix());
	worldMatrix.axis_pos.array()[nPenAxis] += fMinPenetration;
	pcObject->SetWorldMatrix(worldMatrix);
//	pcObject->GetMatrix()->axis_pos.array()[nPenAxis] += fMinPenetration;
	//pcObject2->GetMatrix()->axis_pos.array()[nPenAxis] -= fMinPenetration * 0.5f;
}

void FireRollingCollisionReaction(CFireEnemy * pcEnemy, CCollidableObject * pcPlayer)
{
	//vec3f vObject1Min;
	//vec3f vObject1Max;
	//vec3f vObject2Min;
	//vec3f vObject2Max;

	//pcPlayer->GetCollisionAABB(vObject1Min,vObject1Max);
	//pcEnemy->GetCollisionAABB(vObject2Min,vObject2Max);

	//vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	//vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;

	//vec3f vExtent1 = vObject1Max - vCenter1;
	//vec3f vExtent2 = vObject2Max - vCenter2;

	//vec3f vD = vCenter1-vCenter2;

	//float fMinPenetration = FLT_MAX;
	//unsigned int nPenAxis = 0;

	//for(unsigned int i = 0; i < 3;i++)
	//{
	//	float fPenetration = (vExtent1.array()[i] + vExtent2.array()[i]) - abs(vD.array()[i]);

	//	if(fPenetration <= 0.0f)
	//	{
	//		return;
	//	}

	//	if(fPenetration < fMinPenetration)
	//	{
	//		fMinPenetration = fPenetration;
	//		nPenAxis = i;
	//	}

	//}

	//if(vD.array()[nPenAxis] < 0)
	//{
	//	fMinPenetration = - fMinPenetration;
	//}

	//if(fMinPenetration < 0.0f)
	//{
	//	fMinPenetration = -1.0f;
	//}
	//else
	//{
	//	fMinPenetration = 1.0f;
	//}

	//vec3f vNewVelocity;
	//vNewVelocity.make_zero();

	//// Player dash fix when dashing into a rolling fire beast. - JC
	//if(pcPlayer->GetType() == PLAYER_OBJECT && static_cast<CPlayerObject*>(pcPlayer)->IsDashing()){
	//	vNewVelocity.array()[nPenAxis] = fMinPenetration; }
	//else {
	//	vNewVelocity.array()[nPenAxis] = fMinPenetration * 100.0f; }

	//pcObject1->GetMatrix()->axis_pos.array()[nPenAxis] += fMinPenetration * 0.5f;
	//pcPlayer->SetVelocity(vNewVelocity);
	if(pcPlayer->GetType() != ENVIRONMENT_OBJECT)
	{
		EnviromentCollisionReaction((CEnvironmentObject*)pcEnemy,pcPlayer);
		//pcEnemy->GetFSM()->TransitionTo(ALIGN_BEHAVIOR);
	}
	else//if(pcPlayer->GetType() == ENVIRONMENT_OBJECT)
		pcEnemy->CollisionReaction(pcPlayer);
}

void GroundClamp(CCollidableObject * pcObject)
{
	if(!pcObject || pcObject->GetType() > WIZARD_OBJECT)
		return;

	vec3f vAABBMin;
	vec3f vAABBMax;
	float newYPos = 0.0f;

	pcObject->GetCollisionAABB(vAABBMin,vAABBMax);

	newYPos = vAABBMax.y - vAABBMin.y;

	matrix4f worldMatrix = *(pcObject->GetWorldMatrix());
	worldMatrix.axis_pos.y =CWorldManager::GetInstance()->GetGroundHeight()+0.01f;// newYPos 
	pcObject->SetWorldMatrix(worldMatrix);
//	pcObject->GetMatrix()->axis_pos.y =CWorldManager::GetInstance()->GetGroundHeight();// newYPos 

}

bool IceTrapActivationCheck(CIceTrap * pcIceTrap, CCollidableObject * pcObject)
{
	Sphere pcIceSphere;
	Sphere pcCharSphere;

	pcIceTrap->GetCollisionSphere(pcIceSphere.m_Center,pcIceSphere.m_Radius);
	pcObject->GetCollisionSphere(pcCharSphere.m_Center,pcCharSphere.m_Radius);

	pcIceSphere.m_Radius *= 0.75;
	pcCharSphere.m_Radius *= 0.75;

	return SphereToSphere(pcIceSphere,pcCharSphere);

}

bool IceBreathCheck(CCollidableObject *_pcBeast, CCollidableObject* _pcPlayer)
{
	if(!CheckCollision((CCollidableObject*)_pcBeast,(CCollidableObject*)_pcPlayer, SPHERE_TO_SPHERE))
		return false;
	
	vec3f pointA, pointB, pointC;

	matrix4f rightPoint = *_pcBeast->GetWorldMatrix();
	matrix4f leftPoint = rightPoint;

	rightPoint.rotate_y_post(D3DXToRadian(90));

	vec3f tempVec = rightPoint.axis_z;
	tempVec = tempVec.normalize();

	tempVec *= 1.2f;

	rightPoint.axis_pos += tempVec * -1.0f;

	leftPoint.rotate_y_post(D3DXToRadian(-90));

	leftPoint.axis_pos += tempVec;

	pointC = rightPoint.axis_pos;
	pointB = _pcBeast->GetWorldMatrix()->axis_pos;
	pointB.y += 10.0f;
	pointA = leftPoint.axis_pos;

	
	Plane testPlane;

	ComputePlane(testPlane,pointA,pointB,pointC);

	Sphere testSphere;
	AABB testAABB;

	_pcBeast->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);

	testSphere.m_Radius *= 20;

	//pcEnemy->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);
	_pcPlayer->GetCollisionAABB(testAABB.min,testAABB.max);
	
	if(!SphereToAABB(testSphere,testAABB))
		return false;

	//return (ClassifySphereToPlane(testPlane,testSphere) == 1);
	return (ClassifyAabbToPlane(testPlane,testAABB) != 2);
	//return true;
}

bool PlayerDeflectCheck(CPlayerObject * pcPlayer, CProjectileObject * pcProjectile)
{
	
	int rotation = (int)pcPlayer->GetCurrRotation() + 45;

	if(rotation >= 360)
	{
		rotation -= 360;
	}
	else if(rotation < 0)
	{
		rotation += 360;
	}

	vec3f playerPos = pcPlayer->GetPos();
	vec3f forwardVec = playerPos;
	forwardVec.x += cos((float)rotation);
	forwardVec.z -= sin((float)rotation);


	//playerPos.normalize();
	//forwardVec.normalize();
	forwardVec = forwardVec - playerPos;
	vec3f vecToEnemy = pcProjectile->GetOwner()->GetPos() - playerPos;

	forwardVec.y = 0;
	vecToEnemy.y = 0;
	forwardVec.normalize();
	vecToEnemy.normalize();

	//std::cout << "Dot Product Output From Deflect Check ";

	float f = (dot_product(vecToEnemy,forwardVec));

	//std::cout << f << std::endl;

	if(f > 0.5f)
		return true;

	return false;

	//return (PlayerHalfSpaceTest(pcPlayer,pcProjectile->GetOwner(),45) && PlayerHalfSpaceTest(pcPlayer,pcProjectile->GetOwner(),-45));

	//pointA = pcPlayer->GetMatrix()->axis_pos;
	//pointB = pointA;
	//pointB.y += 10.0f;
	//pointC = pcPlayer->GetMatrix()->axis_pos;

	//rotation = (int)pcPlayer->GetCurrRotation() - 45;

	//if(rotation > 360)
	//{
	//	rotation -= 360;
	//}
	//else if(rotation < 0)
	//{
	//	rotation += 360;
	//}

	//switch(rotation)
	//{
	//case 0:
	//	{
	//		pointC.x += 1;
	//		break;
	//	}
	//case 360:
	//	{
	//		pointC.x += 1;
	//		break;
	//	}

	//case 45:
	//	{
	//		pointC.x += 1;
	//		pointC.z -= 1;
	//		break;
	//	}
	//case 90:
	//	{
	//		pointC.z -= 1;
	//		break;
	//	}
	//case 135:
	//	{
	//		pointC.z -= 1;
	//		pointC.x -= 1;
	//		break;
	//	}
	//case 180:
	//	{
	//		pointC.x -= 1;
	//		break;
	//	}
	//case 225:
	//	{
	//		pointC.x -= 1;
	//		pointC.z += 1;
	//		break;
	//	}
	//case 270:
	//	{
	//		pointC.z += 1;
	//		break;
	//	}
	//case -45:
	//	{
	//		pointC.x += 1;
	//		pointC.z += 1;
	//		break;
	//	}
	//case 315:
	//	{
	//		pointC.x += 1;
	//		pointC.z += 1;
	//		break;
	//	}
	//}

	//ComputePlane(testPlane,pointA,pointB,pointC);

	//pcPlayer->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);

	//testSphere.m_Radius *= 10;

	////pcEnemy->GetCollisionSphere(testSphere.m_Center,testSphere.m_Radius);
	//pcProjectile->GetCollisionAABB(testAABB.min,testAABB.max);
	//
	//if(!SphereToAABB(testSphere,testAABB))
	//{
	//	return false;
	//}
	//else if( (ClassifyAabbToPlane(testPlane,testAABB) == 2))
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;
	//}

}

bool PlayerHalfSpaceTest(CPlayerObject * pcPlayer, CCollidableObject * pcObject, int nRotationOffset,bool bNoStraddlingPlane)
{
	vec3f pointA = pcPlayer->GetWorldMatrix()->axis_pos;
	vec3f pointB = pointA;
	pointB.y += 10.0f;
	vec3f pointC = pcPlayer->GetWorldMatrix()->axis_pos;

	int rotation = (int)pcPlayer->GetCurrRotation() + nRotationOffset;

	if(rotation >= 360)
	{
		rotation -= 360;
	}
	else if(rotation < 0)
	{
		rotation += 360;
	}

	pointC.x += cos((float)rotation);
	pointC.z -= sin((float)rotation);

	

	/*switch(rotation)
	{
	case 0:
		{
			pointC.x += 1;
			break;
		}
	case 45:
		{
			pointC.x += 1;
			pointC.z -= 1;
			break;
		}
	case 90:
		{
			pointC.z -= 1;
			break;
		}
	case 135:
		{
			pointC.z -= 1;
			pointC.x -= 1;
			break;
		}
	case 180:
		{
			pointC.x -= 1;
			break;
		}
	case 225:
		{
			pointC.x -= 1;
			pointC.z += 1;
			break;
		}
	case 270:
		{
			pointC.z += 1;
			break;
		}
	case 315:
		{
			pointC.x += 1;
			pointC.z += 1;
			break;
		}
	}
*/
	Plane testPlane;
	ComputePlane(testPlane,pointA,pointB,pointC);

	AABB testAABB;
	pcObject->GetCollisionAABB(testAABB.min,testAABB.max);
	
	int nAABBtoPlane = ClassifyAabbToPlane(testPlane,testAABB);

	if( nAABBtoPlane == 2 || (bNoStraddlingPlane && nAABBtoPlane == 3) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool PremptiveDashCollision(CPlayerObject* pObjOne, CCollidableObject* pObjTwo)
{	
	vec3f vObject1Min;
	vec3f vObject1Max;
	vec3f vObject2Min;
	vec3f vObject2Max;

	/*AABB endBox;
	endBox.max = vObject1Max;
	endBox.min = vObject1Min;
	
	AABB beginBox;*/

	pObjOne->GetCollisionAABB(vObject1Min,vObject1Max);
	pObjTwo->GetCollisionAABB(vObject2Min,vObject2Max);

	vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;
	vec3f vOldCenter1 = pObjOne->GetLastPosition();

	vec3f vExtent1 = vObject1Max - vCenter1;
	vec3f vExtent2 = vObject2Max - vCenter2;

	// Move last position to center
	vOldCenter1.y += vExtent1.y;

	vec3f vD = vCenter1-vCenter2;

	vec3f sides[4];
	vec3f normals[4];
	float offsets[4];

	// 4 side's normals
	normals[0] = vec3f(-1.0f,0.0f,0.0f);
	normals[3] = vec3f(1.0f,0.0f,0.0f);
	normals[1] = vec3f(0.0f,0.0f,1.0f);
	normals[2] = vec3f(0.0f,0.0f,-1.0f);

	// For 4 sides, don't care about the Ys
	sides[0] = (vCenter2);
	sides[3] = (vCenter2); 
	sides[1] = (vCenter2);
	sides[2] = (vCenter2);
	
	// Find the center points of each of the side planes
	sides[0].x -= vExtent2.x;
	sides[3].x += vExtent2.x;
	sides[1].z += vExtent2.z;
	sides[2].z -= vExtent2.z;

	// Find plane's offsets
	offsets[0] 	= dot_product(sides[0],normals[0]);
	offsets[3]	= dot_product(sides[3],normals[3]);
	offsets[1]	= dot_product(sides[1],normals[1]);
	offsets[2]	= dot_product(sides[2],normals[2]);
	
	float distance = 9001.0f;
	int	  smallest = -1;

	for(unsigned int i = 0; i < 4; ++i)
	{
		// Half space tests
		float pointOffsetOne = dot_product(normals[i],vCenter1);
		float pointOffsetTwo = dot_product(normals[i],vOldCenter1);
		float resultOne =  pointOffsetOne - offsets[i];
		float resultTwo =  pointOffsetTwo - offsets[i];

		if( (resultOne < 0 && resultTwo >= 0) || (resultOne >= 0 && resultTwo < 0))
		{
			float testDist = (sides[i] - vCenter1).magnitude();		

			if(testDist < distance)
			{
				distance = testDist;
				smallest = i;
			}
		}
	}

	if(smallest == -1)
		return false;

	// Now check results and correct player to it.
	vec3f Correction(sides[smallest]);
	Correction.y = pObjOne->GetWorldMatrix()->axis_pos.y;	

	matrix4f worldMatrix = *(pObjOne->GetWorldMatrix());
	worldMatrix.axis_pos = Correction;
	pObjOne->SetWorldMatrix(worldMatrix);
//	pObjOne->GetMatrix()->axis_pos = Correction;

	return true;
}

bool LineAABBIntersect(CPlayerObject* pObjOne, CCollidableObject* pObjTwo)
{
	// Don't test against floors or walls. - JC
	if(pObjTwo->GetType() == ENVIRONMENT_OBJECT)
		if(static_cast<CEnvironmentObject*>(pObjTwo)->GetSubType() == 0 ||
			static_cast<CEnvironmentObject*>(pObjTwo)->GetSubType() == 5)
			return false;

	// Don't test against things not in our level
	if(CWorldManager::GetInstance()->GetLevelForObject(pObjTwo) != 
		CWorldManager::GetInstance()->GetCurrentLevel())
		return false;

	// Jacked from with some modifications:
	// http://www.gamedev.net/topic/338987-aabb---line-segment-intersection-test/ - JC

	vec3f vObject1Min;
	vec3f vObject1Max;
	vec3f vObject2Min;
	vec3f vObject2Max;

	/*AABB endBox;
	endBox.max = vObject1Max;
	endBox.min = vObject1Min;
	
	AABB beginBox;*/

	pObjOne->GetCollisionAABB(vObject1Min,vObject1Max);
	pObjTwo->GetCollisionAABB(vObject2Min,vObject2Max);

	vec3f vCenter1 = (vObject1Min + vObject1Max)*0.5f;
	vec3f vCenter2 = (vObject2Min + vObject2Max)*0.5f;
	vec3f vOldCenter1 = (vCenter1 - pObjOne->GetLastPosition());

	// Move last position to center
	vOldCenter1.y = vCenter1.y;
	// Extend the range of the ray by a small amount
	vOldCenter1 *= 3.0f;

	vec3f vExtent1 = vObject1Max - vCenter1;
	vec3f vExtent2 = vObject2Max - vCenter2;

	

	// initialise to the segment's boundaries. 
	float tenter = 0.0f, texit = 1.0f; 

	// test X slab
	if (!LineSideIntersect(vObject2Min.x, vObject2Max.x, vCenter1.x, vOldCenter1.x,  tenter, texit)) 
	{
		return false;
	}

	// test Y slab

	if (!LineSideIntersect(vObject2Min.y, vObject2Max.y, vCenter1.y, vOldCenter1.y, tenter, texit)) 
	{
		return false;
	}

	// test Z slab
	if (!!LineSideIntersect(vObject2Min.z, vObject2Max.z, vCenter1.z, vOldCenter1.z, tenter, texit)) 
	{
		return false;
	}

	// If all tests return as true, then tEnter will now == the time of intersection 
	// so you can find out exactly on that line where it intersects, if you need to. - JC

	// Finding where on line collision took place and correct player to there. - JC
	// P = startOfLine + DirectionofLine * timeofIntersection
	
	vec3f lineDir = vOldCenter1 - vCenter1;
	//lineDir.normalize();

	

	vec3f vIntersection = vCenter1 - lineDir * tenter;

	// Adjust intersection to take into account player's extents
	vIntersection.x -= vExtent1.x;
	vIntersection.z -= vExtent1.z;
	vIntersection.y = 0.5f;	

	// Correct player
	matrix4f worldMatrix = *(pObjOne->GetWorldMatrix());
	worldMatrix.axis_pos = vIntersection;	
	pObjOne->SetWorldMatrix(worldMatrix);
//	pObjOne->GetMatrix()->axis_pos = vIntersection;	
	
	return  true;
}

bool LineSideIntersect(float sidemin, float sidemax, float linestart, float lineend, float& tbenter, float& tbexit)
{
	// Jacked from: http://www.gamedev.net/topic/338987-aabb---line-segment-intersection-test/ - JC

	float linedir = lineend - linestart;

	// line parallel to the side
	if (fabs(linedir) < 1.0E-9f)
	{
		// line parallel to the side, but line not inside the side planes
		if(linestart < sidemin || linestart > sidemax)
		{
			return false;
		}
		// line parallel to the side, but line inside the side planes
		else
		{
			return true;
		}
	}

	// side's enter and exit parameters
	float tsenter = (sidemin - linestart) / linedir;
	float tsexit = (sidemax - linestart) / linedir;

	// order the enter / exit values.
	if(tsenter > tsexit)
	{
		swap(tsenter, tsexit);
	}

	// make sure the side interval and the current box intersection interval overlap
	if (tbenter > tsexit || tsenter > tbexit)
	{
		// nope. line missed the box.
		return false;
	}
	// yep, the side and current intersection interval overlap
	else
	{
		// update the intersection interval
		tbenter = max(tbenter, tsenter);
		tbexit = min(tbexit, tsexit);
		return true;
	}
}