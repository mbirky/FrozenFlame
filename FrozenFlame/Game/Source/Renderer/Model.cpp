// Our header file
#include "Model.h"

// For file I/O
#include <fstream>
using std::ifstream;
using std::ios_base;

// We'll be loading textures in here
#include "AssetManager.h"
#include "View.h"

static const char* g_szModelsLoc = "Resource/Models/";

CModel::CModel(string szModelName, bool bCollisionAround)
	: m_bCollisionCalculated(false), m_uiNumJoints(0), m_vCenter(0, 0, 0), m_vExtent(0, 0, 0)
{
	// If the model name string is empty, do nothing
	if(szModelName.empty())
		return;

	// Let's save the name of this model
	m_szModelName = szModelName;

	// Let's appens the proper path and extension to the model name
	string szModelPath = g_szModelsLoc;
	szModelPath.append(szModelName);
	szModelPath.append(".model");

	// Create an ifstream to read in binary and try to open the file
	ifstream in;
	in.open(szModelPath.c_str(), ios_base::in | ios_base::binary);
	if(!in.is_open())
		return;

	// Let's load all the meshes until we're not at the end of file
	while(!in.eof())
	{
		TMesh curMesh;

		// Load mesh name
		UINT32 meshNameSize;
		in.read((char*)&meshNameSize, sizeof(UINT32));

		// If something went wrong break;
		if(!in.good())
		{
			break;
		}

		curMesh.m_szMeshName = new char[meshNameSize];
		in.read((char*)curMesh.m_szMeshName, meshNameSize);

		// Load the number of vertices and triangles
		in.read((char*)&curMesh.m_uiNumVerts, sizeof(UINT32));
		in.read((char*)&curMesh.m_uiNumTris, sizeof(UINT32));

		// Load all the textures
		UINT32 tempTexCount = 0;
		in.read((char*)&tempTexCount, sizeof(UINT32));

		curMesh.m_uiTexCount = tempTexCount * 2;
		if(curMesh.m_uiTexCount)
		{
			curMesh.m_rgTextures = new int[curMesh.m_uiTexCount];

			for(UINT32 tex = 0; tex < tempTexCount; ++tex)
			{

				// Load the texture name
				UINT32 texNameSize;
				in.read((char*)&texNameSize, sizeof(UINT32));
				char texName[128];
				in.read((char*)texName, texNameSize);

				// The texture should be located on a folder with the same name as the model
				string diffuseLoc = m_szModelName;
				diffuseLoc += '/';
				diffuseLoc += texName;

				// Try to load the texture
				int texID = CAssetManager::LoadTexture(diffuseLoc.c_str());
				
				// If it failed load a default one
				curMesh.m_rgTextures[tex * 2] = (texID != -1 ? texID : CAssetManager::LoadTexture("DefaultTexture.png"));

				// Now let's look for a specular
				string specLoc;
				// If we find a "diffuse" in the string look for the same file replaced by "specular"
				size_t index = diffuseLoc.find("Diffuse");
				if(index != string::npos)
				{
					specLoc = diffuseLoc.substr(0, index);
					specLoc += "Specular";
					specLoc += diffuseLoc.substr(index + sizeof("Diffuse") - 1);
				}
				// Else let's just look for a Specular.png
				else
				{
					specLoc = m_szModelName;
					specLoc += "/";
					specLoc += "Specular.png";
				}

				// Try to load the specular
				int specID = CAssetManager::LoadTexture(specLoc.c_str(), false, nullptr, nullptr, TTexture::TEX_SPECULAR);

				// If it failed load a default one
				curMesh.m_rgTextures[tex * 2 + 1] = (specID != -1 ? specID : CAssetManager::LoadTexture("DefaultSpecular.png", false, nullptr, nullptr, TTexture::TEX_SPECULAR));
			}
		}
		else
		{
			curMesh.m_uiTexCount = 2;
			curMesh.m_rgTextures = new int[2];
			curMesh.m_rgTextures[0] = CAssetManager::LoadTexture("DefaultTexture.png");
			curMesh.m_rgTextures[1] = CAssetManager::LoadTexture("DefaultSpecular.png", false, nullptr, nullptr, TTexture::TEX_SPECULAR);
		}


		// Load all the vertices
		TRawVert* rawVerts = new TRawVert[curMesh.m_uiNumVerts];
		curMesh.m_rgUniqueVerts = new TVertex[curMesh.m_uiNumVerts];
		for(UINT32 v = 0; v < curMesh.m_uiNumVerts; ++v)
		{
			// Vert we will be loading
			TVertex vert;
			ZeroMemory(&vert, sizeof(TVertex));

			// Load the raw data
			in.read((char*)&vert.m_tVertData, sizeof(TRawVert));

			// Right hand to left hand conversion
			vert.m_tVertData.m_d3dPosition.z *= -1.0f;
			vert.m_tVertData.m_d3dNormal.z *= -1.0f;
			vert.m_tVertData.m_d3dTexCoords.y = 1.0f - vert.m_tVertData.m_d3dTexCoords.y;

			// Copy the data to the raw verts array
			rawVerts[v] = vert.m_tVertData;

			// Load all the influences for that vert
			UINT32 numInfs;
			in.read((char*)&numInfs, sizeof(UINT32));

			for(UINT32 i = 0; i < numInfs && i < 3; ++i)
			{
				UINT32 index;
				in.read((char*)&index, sizeof(UINT32));
				vert.m_rgIndices[i] = (float)index;


				in.read((char*)&vert.m_rgWeights[i], sizeof(float));
				/* TInfluence inf;
				in.read((char*)&inf.m_uiJointIndex, sizeof(UINT32));
				in.read((char*)&inf.m_fWeight, sizeof(float));
				vert.m_rgWeights[i] = inf.m_fWeight;
				vert.m_rgIndices[i] = inf.m_uiJointIndex; */
			}

			curMesh.m_rgUniqueVerts[v] = vert;
		}

		// Load all the triangles
		TPrimitive* triangles = new TPrimitive[curMesh.m_uiNumTris];
		for(UINT32 i = 0; i < curMesh.m_uiNumTris; ++i)
		{
			TPrimitive tri;
			
			//in.read((char*)&tri, sizeof(TPrimitive));
			in.read((char*)&tri.m_uiIndices[2], sizeof(UINT32));
			in.read((char*)&tri.m_uiIndices[1], sizeof(UINT32));
			in.read((char*)&tri.m_uiIndices[0], sizeof(UINT32));
			
			triangles[i] = tri;
		}

		// Number of bones
		in.read((char*)&m_uiNumJoints, sizeof(UINT32));

		// Load all the bones
		m_vBindPose.resize(m_uiNumJoints);

		for(UINT32 bone = 0; bone < m_uiNumJoints; ++bone)
		{
			CKeyFrame keyFrame;

			// Load bone name
			UINT32 boneNameSize;
			in.read((char*)&boneNameSize, sizeof(UINT32));
			char boneNameBuffer[128];
			in.read((char*)boneNameBuffer, boneNameSize);

			keyFrame.SetJointName(boneNameBuffer);

			// Parent index
			INT32 nParentID;
			in.read((char*)&nParentID, sizeof(INT32));

			if(nParentID != -1)
			{
				keyFrame.SetParent(&m_vBindPose[nParentID]);
			}

			// Local matrix
			matrix4f localMatrix;
			for(int x = 0; x < 16; ++x)
			{
				double d;
				in.read((char*)&d, sizeof(double));
				localMatrix.ma[x] = (float)d;
			}
			localMatrix.axis_z *= -1.0f;
			localMatrix.xz *= -1.0f;
			localMatrix.yz *= -1.0f;
			localMatrix.zz *= -1.0f;
			localMatrix.axis_pos.z *= -1.0f;

			keyFrame.SetLocalTransform(D3DXMATRIX(localMatrix.ma));

			// Number of children
			UINT32 numChildren;
			in.read((char*)&numChildren, sizeof(UINT32));
			for(UINT32 child = 0; child < numChildren; ++child)
			{
				UINT32 uiJointIndex;
				in.read((char*)&uiJointIndex, sizeof(UINT32));

				UINT32 childNameSize;
				in.read((char*)&childNameSize, sizeof(UINT32));
				char childNameBuffer[128];
				in.read((char*)childNameBuffer, childNameSize);

				keyFrame.AddChild(&m_vBindPose[uiJointIndex]);
			}

			m_vBindPose[bone] = keyFrame;
		}
		
		// Set the vert size
		m_uiVertSize = m_uiNumJoints ? sizeof(TVertex) : sizeof(TRawVert);

		// If this is the collision mesh, we can calculate the bounds for it and stop here
		if(strcmp(curMesh.m_szMeshName, "collision") == 0)
		{
			CalculateBoundsAround(rawVerts, curMesh.m_uiNumVerts);

			delete[] curMesh.m_rgUniqueVerts;
			delete[] curMesh.m_rgTextures;
			delete[] curMesh.m_szMeshName;
		}
		else
		{
			if(m_uiNumJoints)
			{
				CView::sm_cRenderer.CreateGeometryBuffers(&curMesh.m_pVertexBuff, &curMesh.m_pIndexBuff,
					curMesh.m_rgUniqueVerts, curMesh.m_uiNumVerts * m_uiVertSize, triangles, curMesh.m_uiNumTris * sizeof(TPrimitive));
			}
			else
			{
				CView::sm_cRenderer.CreateGeometryBuffers(&curMesh.m_pVertexBuff, &curMesh.m_pIndexBuff,
					rawVerts, curMesh.m_uiNumVerts * m_uiVertSize, triangles, curMesh.m_uiNumTris * sizeof(TPrimitive));
			}

			m_vtMeshes.push_back(curMesh);

			for(size_t key = 0; key < m_vBindPose.size(); ++key)
			{
				m_vBindPose[key].Update();
			}
		}

		delete[] rawVerts;
		delete[] triangles;
	}

	in.close();

	// Now that we're done loading the model let's calculate its bounds
	if(!m_bCollisionCalculated)
	{
		bCollisionAround ? CalculateBoundsAround() : CalculateBoundsWithin();
	}
}
void CModel::ApplyTextures(int nShaderID, int nMeshID) const
{
	for(UINT i = 0; i < m_vtMeshes[nMeshID].m_uiTexCount; ++i)
	{
		CAssetManager::ApplyTexture(nShaderID, m_vtMeshes[nMeshID].m_rgTextures[i]);
	}
}
CModel::~CModel(void)
{
	m_vBindPose.clear();

	for(size_t i = 0; i < m_vtMeshes.size(); ++i)
	{
		TMesh& curMesh = m_vtMeshes[i];

		if(curMesh.m_szMeshName)
		{
			delete[] curMesh.m_szMeshName;
			curMesh.m_szMeshName = nullptr;
		}
		if(curMesh.m_pVertexBuff)
		{
			curMesh.m_pVertexBuff->Release();
			curMesh.m_pVertexBuff = nullptr;
		}
		if(curMesh.m_pIndexBuff)
		{
			curMesh.m_pIndexBuff->Release();
			curMesh.m_pIndexBuff = nullptr;
		}
		if(curMesh.m_rgTextures)
		{
			delete[] curMesh.m_rgTextures;
			curMesh.m_rgTextures = nullptr;
		}
		if(curMesh.m_rgUniqueVerts)
		{
			delete[] curMesh.m_rgUniqueVerts;
			curMesh.m_rgUniqueVerts = nullptr;
		}
	}
}

void CModel::CalculateBoundsAround(void)
{
	vec3f vMin, vMax;
	// Initialize first
	vMin.x = vMax.x = m_vtMeshes[0].m_rgUniqueVerts[0].m_tVertData.m_d3dPosition.x;
	vMin.y = vMax.y = m_vtMeshes[0].m_rgUniqueVerts[0].m_tVertData.m_d3dPosition.y;
	vMin.z = vMax.z = m_vtMeshes[0].m_rgUniqueVerts[0].m_tVertData.m_d3dPosition.z;

	for(size_t i = 0; i < m_vtMeshes.size(); ++i)
	{
		TMesh& curMesh = m_vtMeshes[i];

		for(UINT v = 0; v < curMesh.m_uiNumVerts; ++v)
		{
			TVertex& curVert = curMesh.m_rgUniqueVerts[v];

			for(int k = 0; k < 3; ++k)
			{
				if(curVert.m_tVertData.m_d3dPosition[k] > vMax.v[k])
					vMax.v[k] = curVert.m_tVertData.m_d3dPosition[k];
				if(curVert.m_tVertData.m_d3dPosition[k] < vMin.v[k])
					vMin.v[k] = curVert.m_tVertData.m_d3dPosition[k];
			}
		}
	}
	

	m_vCenter = (vMin + vMax) * 0.5f;
	m_vExtent = vMax - m_vCenter;

	m_bCollisionCalculated = true;
}

void CModel::CalculateBoundsAround(const TRawVert* verts, UINT numVerts)
{
	vec3f vMin, vMax;
	// Initialize first
	vMin.x = vMax.x = verts[0].m_d3dPosition.x;
	vMin.y = vMax.y = verts[0].m_d3dPosition.y;
	vMin.z = vMax.z = verts[0].m_d3dPosition.z;

	for(UINT v = 0; v < numVerts; ++v)
	{
		const TRawVert& curVert = verts[v];

		for(int k = 0; k < 3; ++k)
		{
			if(curVert.m_d3dPosition[k] > vMax.v[k])
				vMax.v[k] = curVert.m_d3dPosition[k];
			if(curVert.m_d3dPosition[k] < vMin.v[k])
				vMin.v[k] = curVert.m_d3dPosition[k];
		}
	}

	m_vCenter = (vMin + vMax) * 0.5f;
	m_vExtent = vMax - m_vCenter;

	m_bCollisionCalculated = true;
}

void CModel::CalculateBoundsWithin(void)
{
	CalculateBoundsAround();

	vec3f vSumVerts(0, 0, 0);
	unsigned nVertCount = 0;

	vector<const D3DXVECTOR3*> vModelVerts;
	for(size_t meshID = 0; meshID < m_vtMeshes.size(); ++meshID)
	{
		TMesh& curMesh = m_vtMeshes[meshID];

		for(UINT vertID = 0; vertID < curMesh.m_uiNumVerts; ++vertID)
		{
			TVertex& curVert = curMesh.m_rgUniqueVerts[vertID];
			vSumVerts.x += curVert.m_tVertData.m_d3dPosition.x;
			vSumVerts.y += curVert.m_tVertData.m_d3dPosition.y;
			vSumVerts.z += curVert.m_tVertData.m_d3dPosition.z;
			vModelVerts.push_back(&curVert.m_tVertData.m_d3dPosition);
			nVertCount++;
		}
	}

	vec3f vCenterVert = vSumVerts / nVertCount;

	vec3f vSumMax(0, 0, 0);
	vec3f vSumMin(0, 0, 0);
	unsigned nMaxCount = 0;
	unsigned nMinCount = 0;

	for(vector<const D3DXVECTOR3*>::iterator iter = vModelVerts.begin();
		iter != vModelVerts.end(); ++iter)
	{
		if((*iter)->x > vCenterVert.x && (*iter)->y > vCenterVert.y && (*iter)->z > vCenterVert.z)
		{
			vSumMax.x += (*iter)->x;
			vSumMax.y += (*iter)->y;
			vSumMax.z += (*iter)->z;
			nMaxCount++;
		}
		else if((*iter)->x < vCenterVert.x && (*iter)->y < vCenterVert.y && (*iter)->z < vCenterVert.z)
		{
			vSumMin.x += (*iter)->x;
			vSumMin.y += (*iter)->y;
			vSumMin.z += (*iter)->z;
			nMinCount++;
		}
	}

	vec3f vAvgMax = vSumMax / nMaxCount;
	vec3f vAvgMin = vSumMin / nMinCount;

	vec3f vNewCenter = (vAvgMax + vAvgMin) * 0.5f;
	vec3f vNewExtent = vAvgMax - m_vCenter;

	m_vCenter = (m_vCenter + vNewCenter) * 0.5f;
	m_vExtent = (m_vExtent + vNewExtent) * 0.5f;

	m_bCollisionCalculated = true;
}


int CModel::GetJointID(string szJointName) const
{
	// Loop through all the joints comparing the names until the passed in name is found
	size_t numJoints = m_vBindPose.size();
	for(size_t idJoint = 0; idJoint < numJoints; ++idJoint)
	{
		if(m_vBindPose[idJoint].GetJointName() == szJointName)
		{
			// If found, return the id of that joint
			return (int)idJoint;
		}
	}

	// If none was found, return -1
	return -1;
}