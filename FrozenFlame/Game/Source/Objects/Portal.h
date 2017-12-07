/***********************************************
 * Filename:		Crystal.h		
 * Date:      		11/28/2012
 * Mod. Date: 		11/28/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		Crystal fuction declerations
 ************************************************/
#ifndef CRYSTAL_H_
#define CRYSTAL_H_

class CWorldManager;

#include "../Globals.h"
#include "EnvironmentObject.h"
#include "../Renderer/Emitter.h"

class CPortal : public CEnvironmentObject
{
private:
	CEmitter* m_cEmit;

	bool m_bIsActive;

	// sound buffers
	bool  m_bCanPlayAmbientSound;	// start loop playing only once, not a new one every frame


public:
	/**************************************************************
	* 
	* Ins
	* Outs
	* Returns
	* Mod. Date
	* Mod. Initals
	***************************************************************/
	CPortal();

	/**************************************************************
	* 
	* Ins
	* Outs
	* Returns
	* Mod. Date
	* Mod. Initals
	***************************************************************/
	~CPortal();

	/**************************************************************
	* 
	* Ins
	* Outs
	* Returns
	* Mod. Date
	* Mod. Initals
	***************************************************************/
	void Initialize(void);

	/**************************************************************
	* 
	* Ins
	* Outs
	* Returns
	* Mod. Date
	* Mod. Initals
	***************************************************************/
	void Reinitialize(void);

	/**************************************************************
	* 
	* Ins
	* Outs
	* Returns
	* Mod. Date
	* Mod. Initals
	***************************************************************/
	void Update(float fElapsedTime);

	void Uninitialize();

	/*********************************************
	*
	*					Mutators
	*
	**********************************************/
	void SetActive(bool destruct) {m_bIsActive = destruct;}

	/*********************************************
	*
	*					Accessors
	*
	**********************************************/
	bool GetActive(void) {return m_bIsActive;}

	void OnAttack(int nDamage,int nElementType);
};

#endif