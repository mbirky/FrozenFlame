#include "ED2CollisionLibrary.h"
#include <math.h>

// ComputePlane
//
// Calculate the plane normal and plane offset from the input points
void ComputePlane(Plane &plane, const vec3f& pointA, const vec3f& pointB, const vec3f &pointC)
{
	vec3f edge0 = pointA-pointB;
	vec3f edge1 = pointC-pointB;
	cross_product(plane.normal,edge1,edge0);
	plane.normal.normalize();
	plane.offset = dot_product(plane.normal,pointA);
}

// ClassifyPointToPlane
//
// Perform a half-space test. Returns 1 if the point is on or in front of the plane.
// Returns 2 if the point is behind the plane.
int ClassifyPointToPlane(const Plane& plane, const vec3f& point)
{
	if(plane.offset - dot_product(plane.normal,point) >0)
		return 1;
	else
		return 2;
}

// ClassifySphereToPlane
//
// Perform a sphere-to-plane test. 
// Returns 1 if the sphere is in front of the plane.
// Returns 2 if the sphere is behind the plane.
// Returns 3 if the sphere straddles the plane.
int ClassifySphereToPlane(const Plane& plane, const Sphere& sphere)
{
	float d = dot_product(sphere.m_Center,plane.normal)-plane.offset;

	if(d > sphere.m_Radius)
		return 1;
	else if(d < -sphere.m_Radius)
		return 2;
	else
		return 3;
	
}

// ClassifyAabbToPlane
//
// Performs a AABB-to-plane test.
// Returns 1 if the aabb is in front of the plane.
// Returns 2 if the aabb is behind the plane.
// Returns 3 if the aabb straddles the plane.
int ClassifyAabbToPlane(const Plane& plane, const AABB& aabb)
{
	vec3f cp = (aabb.min + aabb.max)*0.5f;
	vec3f e = aabb.max - cp;

	float r = e.x * abs(plane.normal.x) + e.y * abs(plane.normal.y) + e.z * abs(plane.normal.z);

	float d = dot_product(cp,plane.normal)-plane.offset;

	if(d > r)
		return 1;
	else if(d < -r)
		return 2;
	else
		return 3;

}

// ClassifyCapsuleToPlane
//
// Performs a Capsule-to-plane test.
// Returns 1 if the aabb is in front of the plane.
// Returns 2 if the aabb is behind the plane.
// Returns 3 if the aabb straddles the plane.
int ClassifyCapsuleToPlane(const Plane& plane, const Capsule& capsule)
{
	Sphere s1;
	Sphere s2;
	s1.m_Center = capsule.m_Segment.m_Start;
	s2.m_Center = capsule.m_Segment.m_End;
	s2.m_Radius = s1.m_Radius = capsule.m_Radius;

	int s1Res = ClassifySphereToPlane(plane,s1);
	int s2Res = ClassifySphereToPlane(plane,s2);

	if(s1Res == s2Res)
	{
		if(s1Res == 1)
			return 1;
		else if(s1Res == 2)
			return 2;
		else
			return 3;
	}
	else
		return 3;

	return 0;
}

// BuildFrustum
//
// Calculates the corner points and planes of the frustum based upon input values.
// Should call ComputePlane.
void BuildFrustum( Frustum& frustum, float fov, float nearDist, float farDist, float ratio, const matrix4f& camXform )
{
	vec3f nearCenter = camXform.axis_pos - camXform.axis_z * nearDist;
	vec3f farCenter = camXform.axis_pos - camXform.axis_z * farDist;

	float hNear = 2 * tan(fov/2.0f)*nearDist;
	float hFar = 2 * tan(fov/2.0f)*farDist;
	float wNear = hNear * ratio;
	float wFar = hFar * ratio;

	frustum.corners[FTL] = farCenter + camXform.axis_y * (hFar*0.5f)-camXform.axis_x*(wFar*0.5f);
	frustum.corners[FTR] = farCenter + camXform.axis_y * (hFar*0.5f)+camXform.axis_x*(wFar*0.5f);
	frustum.corners[FBL] = farCenter - camXform.axis_y * (hFar*0.5f)-camXform.axis_x*(wFar*0.5f);
	frustum.corners[FBR] = farCenter - camXform.axis_y * (hFar*0.5f)+camXform.axis_x*(wFar*0.5f);

	frustum.corners[NTL] = nearCenter + camXform.axis_y * (hNear*0.5f)-camXform.axis_x*(wNear*0.5f);
	frustum.corners[NTR] = nearCenter + camXform.axis_y * (hNear*0.5f)+camXform.axis_x*(wNear*0.5f);
	frustum.corners[NBL] = nearCenter - camXform.axis_y * (hNear*0.5f)-camXform.axis_x*(wNear*0.5f);
	frustum.corners[NBR] = nearCenter - camXform.axis_y * (hNear*0.5f)+camXform.axis_x*(wNear*0.5f);

	ComputePlane(frustum.planes[LEFT_PLANE]		 ,frustum.corners[NBL],frustum.corners[FBL],frustum.corners[FTL]);
	ComputePlane(frustum.planes[RIGHT_PLANE]	 ,frustum.corners[FBR],frustum.corners[NBR],frustum.corners[NTR]);
	ComputePlane(frustum.planes[TOP_PLANE]		 ,frustum.corners[NTR],frustum.corners[FTL],frustum.corners[FTR]);
	ComputePlane(frustum.planes[BOTTOM_PLANE]	 ,frustum.corners[NBR],frustum.corners[FBR],frustum.corners[FBL]);
	ComputePlane(frustum.planes[NEAR_PLANE]		 ,frustum.corners[NBR],frustum.corners[NBL],frustum.corners[NTL]);
	ComputePlane(frustum.planes[FAR_PLANE]		 ,frustum.corners[FBL],frustum.corners[FBR],frustum.corners[FTR]);
	
	return;
}



// FrustumToSphere
//
// Perform a Sphere-to-Frustum check. Returns true if the sphere is inside. False if not.
bool FrustumToSphere(const Frustum& frustum, const Sphere& sphere)
{
	int numFound = 0;
	if(ClassifySphereToPlane(frustum.planes[0],sphere) != 2)
	{
		numFound++;
	}
	if(ClassifySphereToPlane(frustum.planes[1],sphere) != 2)
	{
		numFound++;
	}
	if(ClassifySphereToPlane(frustum.planes[2],sphere) != 2)
	{
		numFound++;
	}
	if(ClassifySphereToPlane(frustum.planes[3],sphere) != 2)
	{
		numFound++;
	}
	if(ClassifySphereToPlane(frustum.planes[4],sphere) != 2)
	{
		numFound++;
	}
	if(ClassifySphereToPlane(frustum.planes[5],sphere) != 2)
	{
		numFound++;
	}
	if(numFound == 6)
		return true;
	return false;
}

// FrustumToAABB
//
// Perform a Aabb-to-Frustum check. Returns true if the aabb is inside. False if not.
bool FrustumToAABB(const Frustum& frustum, const AABB& aabb)
{
	int numFound = 0;
	if(ClassifyAabbToPlane(frustum.planes[0],aabb) != 2)
	{
		numFound++;
	}
	if(ClassifyAabbToPlane(frustum.planes[1],aabb) != 2)
	{
		numFound++;
	}
	if(ClassifyAabbToPlane(frustum.planes[2],aabb) != 2)
	{
		numFound++;
	}
	if(ClassifyAabbToPlane(frustum.planes[3],aabb) != 2)
	{
		numFound++;
	}
	if(ClassifyAabbToPlane(frustum.planes[4],aabb) != 2)
	{
		numFound++;
	}
	if(ClassifyAabbToPlane(frustum.planes[5],aabb) != 2)
	{
		numFound++;
	}
	if(numFound == 6)
		return true;
	return false;
}

// FrustumToCapsule
//
// Perform a Capsule-to-Frustum check. Returns true if the Capsule is inside. False if not.
bool FrustumToCapsule(const Frustum& frustum, const Capsule& capsule)
{
	int numFound = 0;
	if(ClassifyCapsuleToPlane(frustum.planes[0],capsule) != 2)
	{
		numFound++;
	}
	if(ClassifyCapsuleToPlane(frustum.planes[1],capsule) != 2)
	{
		numFound++;
	}
	if(ClassifyCapsuleToPlane(frustum.planes[2],capsule) != 2)
	{
		numFound++;
	}
	if(ClassifyCapsuleToPlane(frustum.planes[3],capsule) != 2)
	{
		numFound++;
	}
	if(ClassifyCapsuleToPlane(frustum.planes[4],capsule) != 2)
	{
		numFound++;
	}
	if(ClassifyCapsuleToPlane(frustum.planes[5],capsule) != 2)
	{
		numFound++;
	}
	if(numFound == 6)
		return true;
	return false;
}

// AABBtoAABB
//
// Returns true if the AABBs collide. False if not.
bool AABBtoAABB(const AABB& lhs, const AABB& rhs)
{
	if( rhs.max.x < lhs.min.x || rhs.min.x > lhs.max.x ) return false;		
	if( rhs.max.y < lhs.min.y || rhs.min.y > lhs.max.y ) return false; 		
	if( rhs.max.z < lhs.min.z || rhs.min.z > lhs.max.z ) return false;

	return true;
}

// SphereToSphere
//
// Returns true if the Spheres collide. False if not.
bool SphereToSphere(const Sphere& lhs, const Sphere& rhs)
{
	float d = (lhs.m_Center.x-rhs.m_Center.x)*(lhs.m_Center.x-rhs.m_Center.x) + (lhs.m_Center.y-rhs.m_Center.y)*(lhs.m_Center.y-rhs.m_Center.y) + (lhs.m_Center.z-rhs.m_Center.z)*(lhs.m_Center.z-rhs.m_Center.z);
	float r2 = (lhs.m_Radius + rhs.m_Radius) * (lhs.m_Radius + rhs.m_Radius);

	if(d > r2)
		return false;
	return true;

}

// SphereToAABB
//
// Returns true if the sphere collides with the AABB. False if not.
bool SphereToAABB(const Sphere& lhs, const AABB& rhs)
{
	Sphere s;
	s.m_Radius = 0.0f;
	
	if(lhs.m_Center.x < rhs.min.x)
		s.m_Center.x = rhs.min.x;
	else if(lhs.m_Center.x > rhs.max.x)
		s.m_Center.x = rhs.max.x;
	else
		s.m_Center.x = lhs.m_Center.x;

	if(lhs.m_Center.y < rhs.min.y)
		s.m_Center.y = rhs.min.y;
	else if(lhs.m_Center.y > rhs.max.y)
		s.m_Center.y = rhs.max.y;
	else
		s.m_Center.y = lhs.m_Center.y;

	if(lhs.m_Center.z < rhs.min.z)
		s.m_Center.z = rhs.min.z;
	else if(lhs.m_Center.z > rhs.max.z)
		s.m_Center.z = rhs.max.z;
	else
		s.m_Center.z = lhs.m_Center.z;


	return SphereToSphere(s,lhs);
}

// CapsuleToSphere
//
// Returns true if the capsule collides with the sphere. False if not.
bool CapsuleToSphere(const Capsule& capsule, const Sphere& sphere)
{
	vec3f l = capsule.m_Segment.m_End-capsule.m_Segment.m_Start;
	//vec3f lNormalized = l;
	//lNormalized.normalize();
	vec3f n = l / l.magnitude();
	vec3f v = sphere.m_Center - capsule.m_Segment.m_Start;
	float d = dot_product(n,v);

	Sphere s;
	s.m_Radius = capsule.m_Radius;

	if(d < 0)
		s.m_Center = capsule.m_Segment.m_Start;
	else if(d > l.magnitude())
		s.m_Center = capsule.m_Segment.m_End;
	else
	{
		vec3f nPrime = n*d;
		s.m_Center = capsule.m_Segment.m_Start + nPrime;
	}

	return SphereToSphere(s,sphere);
}

