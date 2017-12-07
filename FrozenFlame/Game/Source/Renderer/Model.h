/********************************************************************
* Filename:			Model.h
* Date:				10/15/2012
* Mod. Date:		11/11/2012
* Mod. Initials:	DL
* Author:			Daniel Lima
* Purpose:			This module contains all the geomtry-related information for
*					a given model to be rendered, such as the vertex and index
*					buffers, the vertex declaration, the number of vertices and
*					primitives, etc. It also contains IDs for textures in the
*					Texture Manager that the meshes are using. Most of this
*					information is inside its vector of TMesh, whose declaration
*					comes from RenderDefinitions.h.
********************************************************************/

#ifndef _MODEL_H_
#define _MODEL_H_

// Basic render definitions that this module uses
#include "RenderDefinitions.h"

// CModel class
class CModel
{	
	friend class CAssetManager;

	// Name of the model
	string m_szModelName;
	// Size (in bytes) of a vertex for this model
	UINT32 m_uiVertSize;
	// Vector of meshes
	vector<TMesh> m_vtMeshes;

	// Number of joints this model has
	UINT32 m_uiNumJoints;
	// Vector of keyframes for the bind pose
	vector<CKeyFrame> m_vBindPose;

	// Was the collision for this model already calculated?
	bool m_bCollisionCalculated;

	// Center of the model
	vec3f m_vCenter;
	// Extent of the model (Center -> Max)
	vec3f m_vExtent;

	// Calculates the center and the extent of this model
	void CalculateBoundsAround(void);
	void CalculateBoundsAround(const TRawVert* verts, UINT numVerts);
	void CalculateBoundsWithin(void);

public:
	/********************************************************************
	* CModel():				Loads a model from a file
	* Ins:					szModelName - relative name of the model file
	* Returns:				true if succeeded, false otherwise
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	CModel(string szModelName, bool bCollisionAround);

	/********************************************************************
	* ApplyTextures():		Applies all the textures for a mesh from
	*						this model to be rendered
	* Ins:					nShaderID - the shader we're using
	*						nMeshID - the mesh we're using
	* Returns:				void
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	void ApplyTextures(int nShaderID, int nMeshID) const;

	/********************************************************************
	* ~CModel():			Cleans up any allocated memory
	* Mod. Date:			11/12/2012
	* Mod. Initials:		DL
	********************************************************************/
	~CModel(void);

	// Returns the ID of the joint with the passed in name
	int GetJointID(string szJointName) const;

	/***************
	*	Accessors
	***************/
	inline const vector<TMesh>&		GetMeshArray(void) const		{ return m_vtMeshes; }
	inline const CKeyFrame&			GetBindPose(UINT jointID) const	{ return m_vBindPose[jointID]; } 
	inline const vector<CKeyFrame>& GetBindPose(void) const			{ return m_vBindPose; }
	inline const UINT&				GetVertSize(void) const			{ return m_uiVertSize; }
	inline UINT32					GetNumJoints(void) const		{ return m_uiNumJoints; }
	inline const vec3f&				GetCenter(void) const			{ return m_vCenter; }
	inline const vec3f&				GetExtent(void) const			{ return m_vExtent; }
};


#endif