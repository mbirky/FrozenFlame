
#if 0
/***********************************************
 * Filename:  		AniNode.h
 * Date:      		10/29/2012
 * Mod. Date: 		10/29/2012
 * Mod. Initials:	JM
 * Author:    		Justin Maza
 * Purpose:   		This class is used to create a 
 *					hierarchy of Keyframes so
 *					we can flatten it for when the
 *					joint's position changes
 ************************************************/
#ifndef _ANINODE_H_
#define _ANINODE_H_

//#include "Animation.h"
#include <vector>
using std::vector;

class CAniNode
{
	

public:
	//friend class CAnimation;
	// Pointer to parent within the node tree
	CAniNode* m_pParentNode;

	// Vector to hold all the children in the tree
	vector<CAniNode*> m_vpChildVec;

	unsigned int m_uiJointIndex;

	/*****************************************************************
	* CAniNode():			Default constructor for the AniNodes that
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
	CAniNode(void) : m_pParentNode(0){}


	/*****************************************************************
	* ~CAniNode():			Default destructor for the AniNodes that
	*						may or may not be overridden
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
	virtual ~CAniNode(void) {}

	//********************Accessors******************************

	// Return the parent of this node (if it has one)

	/*****************************************************************
	* GetParentNode():		Returns the node's parent if it has one.
	*						Otherwise it just returns NULL
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				CAniNode*
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	CAniNode *GetParentNode(void) {return m_pParentNode;}

	// Return the children of this node (if it has any)

	/*****************************************************************
	* GetChildNodes():		Returns a vector of the node's children.
	*						Otherwise it just returns an empty vector.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				vector<CAniNode*>
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    JM
	*****************************************************************/
	vector<CAniNode*> &GetChildNodes(void) {return m_vpChildVec;}

	//***********************************************************

	/*****************************************************************
	* AddChildToTree():		Adds the passed in AniNode to this Node's vector
	*						of child AniNodes if a valid value is passed in.
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
	void AddChildToTree(CAniNode *pNode)
	{
		if(pNode != NULL)
		{
			pNode->m_pParentNode = this;
			m_vpChildVec.push_back(pNode);
		}
	}

	/*****************************************************************
	* RemoveChildFromTree(): Searches through this AniNode's vector of
	*						 child nodes and if the passed in value is
	*						 a valid node, will remove it from the vector
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
	void RemoveChildFromTree(CAniNode *pNode)
	{
		if(pNode != NULL)
		{
			if(pNode->m_pParentNode != this)
				return;
			for(unsigned int n = 0; n < m_vpChildVec.size(); ++n)
			{
				if(pNode == m_vpChildVec[n])
				{
					m_vpChildVec.erase(m_vpChildVec.begin() + n);
					pNode->m_pParentNode = NULL;
				}
			}
		}
	}


};
#endif //_ANINODE_H_

#endif