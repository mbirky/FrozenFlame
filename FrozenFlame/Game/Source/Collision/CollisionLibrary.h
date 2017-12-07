/***********************************************
* Filename:  		CollisionLibrary.h
* Date:      		11/02/2012
* Mod. Date: 		11/02/2012
* Mod. Initials:	CM
* Author:    		Charles Meade
* Purpose:   		public function declarations for the collision library
************************************************/

#ifndef COLLISION_LIBRARY_H
#define COLLISION_LIBRARY_H

#include "../Objects/CollidableObject.h"
#include "../Util/Matrix4x4.h"
#include "../Util/vec3.h"
#include "../Objects/Enemy.h"
#include "../Objects/EnvironmentObject.h"
#include "../Objects/FireEnemy.h"
#include "../Objects/CIceTrap.h"
#include "../Objects/ProjectileObject.h"

enum eCollisionTypes {AABB_TO_AABB,AABB_TO_SPHERE,SPHERE_TO_AABB,SPHERE_TO_SPHERE};

/*****************************************************************
* CheckCollision():		Checks the collision of the 2 objects
*
* Ins:					void
*
* Outs:					void
*
* Returns:				n/a
*
* Mod. Date:		    11/02/2012
* Mod. Initials:	    CM
*****************************************************************/
bool CheckCollision(CCollidableObject* cObject1,CCollidableObject* cObject2,int nCollisionType = AABB_TO_AABB);

bool PlayerAttackCheck(CPlayerObject * pcPlayer,CEnemy* pcEnemy);

bool IceBreathCheck(CCollidableObject *_pcBeast, CCollidableObject* _pcPlayer);

void NormalCollisionReaction(CCollidableObject * pcObject1,CCollidableObject * pcObject2);

void AverageCollisionReaction(CPlayerObject* pObjOne,CCollidableObject* pObjTwo, vec3f& vCorrection);

void PlayerDashingCollisionReaction(CPlayerObject * pcPlayer,CEnemy * pcObject);

void EnviromentCollisionReaction(CEnvironmentObject * pcEnvironment,CCollidableObject * pcObject);

//bool FireRollingCollisionCheck(CFireEnemy * pcEnemy, CPlayerObject * pcPlayer);

void FireRollingCollisionReaction(CFireEnemy * pcEnemy, CCollidableObject * pcPlayer);

void GroundClamp(CCollidableObject * pcObject);

bool IceTrapActivationCheck(CIceTrap * pcIceTrap, CCollidableObject * pcObject);

bool PlayerDeflectCheck(CPlayerObject * pcPlayer, CProjectileObject * pcProjectile);

bool PlayerHalfSpaceTest(CPlayerObject * pcPlayer, CCollidableObject * pcObject, int nRotationOffset = 0, bool bNoStraddlingPlane = false);

bool PremptiveDashCollision(CPlayerObject* pObjOne, CCollidableObject* pObjTwo);

bool LineAABBIntersect(CPlayerObject* pObjOne, CCollidableObject* pObjTwo);

bool LineSideIntersect(float sidemin, float sidemax, float linestart, float lineend, float& tbenter, float& tbexit);

#endif