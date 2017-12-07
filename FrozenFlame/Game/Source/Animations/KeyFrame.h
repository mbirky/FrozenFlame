// KeyFrame Class
// by Justin Maza & Daniel Lima
// - This contains the transform and time information for a joint's keyframe
// updated 11/24/2012

#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
using std::vector;

#include <string>
using std::string;

class CKeyFrame
{
	// transforms
	D3DXMATRIX			m_d3dLocalTransform;
	mutable D3DXMATRIX	m_d3dWorldTransform;

	// family
	CKeyFrame*			m_pParent;
	vector<CKeyFrame*>	m_vpChildren;

	// this frame's time
	FLOAT				m_fKeyTime;
	FLOAT				m_fTweenTime;
	// is this frame dirty?
	mutable bool		m_bDirty;
	// index of the joint this frame belongs to
	UINT32				m_uiJointIndex;

	// name of the joint
	string				m_szJointName;

public:
	CKeyFrame(void);
	// adds a child to the children vector
	void AddChild(CKeyFrame* pFrame);
	// removes a child from the children vector
	void RemoveChild(CKeyFrame* pFrame);
	// goes through the hierarchy setting all the frames to be dirty
	void Update(void);
	// if the frame is dirty, goes recursively through the hierarchy
	// multiplying the local matrices to get this frame's world
	const D3DXMATRIX& GetWorldTransform(void) const;
	// clean up
	~CKeyFrame(void);

	// accessors
	inline float GetTime(void)	const							{ return m_fKeyTime; }
	inline float GetTweenTime(void)	const						{ return m_fTweenTime ? m_fTweenTime : 1.0f; }
	inline const string& GetJointName(void) const				{ return m_szJointName; }
	// mutators
	inline void SetParent(CKeyFrame* pFrame)					{ m_pParent = pFrame; }
	inline void SetLocalTransform(const D3DXMATRIX& d3dLocal)	{ m_d3dLocalTransform = d3dLocal; }
	inline void SetWorldTransform(const D3DXMATRIX& d3dWorld)	{ m_d3dWorldTransform = d3dWorld; }
	inline void SetTime(float fTime)							{ m_fKeyTime = fTime; }
	inline void SetTweenTime(float fTween)						{ m_fTweenTime = fTween; }
	inline void SetJointName(const char* szJointName)			{ m_szJointName = szJointName; }
};

#endif