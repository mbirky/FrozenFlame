#if 0


/***********************************************
 * Filename:  		Frame.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/29/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		This class uses a local and
 *					a world matrix to store the
 *					local and world positions of each
 *					joint
 ************************************************/

#ifndef _FRAME_H_
#define _FRAME_H_

#include <d3d9.h>
#include <d3dx9.h>

#include "AniNode.h"

class CKeyFrame : public CAniNode
{
public:
	D3DXMATRIX	m_d3dLocalTransform;
	D3DXMATRIX	m_d3dWorldTransform;

	UINT32		m_uiFlags;
	FLOAT		m_fKeyTime;

	enum { e_DIRTY = 1, e_REVERSE = 2};

	/*****************************************************************
	* CKeyFrame():			Default constructor for the KeyFrames that
	*						initializes all variables to safe values
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CKeyFrame(void) : m_uiFlags(0), m_fKeyTime(0.0f) { }

	/*****************************************************************
	* ~CKeyFrame():			Default destructor for the KeyFrame
	*							
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	~CKeyFrame(void) {}

	// Update "Dirties a frame and its children.  The world matrix of those frames are updated
	// and "cleaned" when the World Matrix's accesor is called.

	/*****************************************************************
	* Update():			Updates each KeyFrame
	*							
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	void Update(void)
	{
		if( 0 == (m_uiFlags & e_DIRTY) )
		{
			m_uiFlags |= e_DIRTY;
	
			for(unsigned int c = 0; c < GetChildNodes().size(); ++c)
			{
				((CKeyFrame*)GetChildNodes()[c])->Update();
			}
		}
	}

	//****************************Accessors********************************

	// Returns the local space matrix of this frame.

	/*****************************************************************
	* GetLocMatrix():		Returns the local matrix of the KeyFrame
	*							
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				mat4d (4x4 matrix of doubles)
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	__forceinline const D3DXMATRIX& GetLocalMatrix(void) { return m_d3dWorldTransform; }

	// Returns the world space matrix of this frame.
	// If the frame is dirtied, we re-calculate the frame's world matrix.


	/*****************************************************************
	* GetWorldMatrix():		Returns the world matrix of the KeyFrame.
	*						If the local matrix is "dirty", then we set the
	*						KeyFrame's world matrix to the product of its 
	*						local matrix and its parent's world matrix
	*							
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				mat4d (4x4 matrix of doubles)
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	const D3DXMATRIX& GetWorldMatrix(void)
	{
		if(m_uiFlags & e_DIRTY)
		{
			if(GetParentNode())
			{
				m_d3dWorldTransform = m_d3dLocalTransform * ((CKeyFrame*)GetParentNode())->GetWorldMatrix();
			}
			else
			{
				m_d3dWorldTransform = m_d3dLocalTransform;
			}
	
			m_uiFlags &= ~e_DIRTY;
		}
		
		return m_d3dWorldTransform;
	}

};
#endif

#endif