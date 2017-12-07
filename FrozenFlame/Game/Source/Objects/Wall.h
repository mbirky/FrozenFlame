/***********************************************
 * Filename:		Wall.h 		
 * Date:      		11/13/2012
 * Mod. Date: 		11/13/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		Wall object declerations
 ************************************************/
#ifndef WALL_H_
#define WALL_H_

#include "../Globals.h"
#include "CollidableObject.h"
#include "EnvironmentObject.h"

class CWall : public CEnvironmentObject
{
private:

public:

	/**************************************************************
	* CWall(void)	:	The default constructor
	* Ins			:	None	
	* Outs			:	None
	* Returns		:	None
	* Mod. Date		:	11/13/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	CWall(void);

	/**************************************************************
	* ~CWall(void)	:	The destructor
	* Ins			:	None
	* Outs			:	None
	* Returns		:	None
	* Mod. Date		:	11/13/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	~CWall(void);

	/**************************************************************
	* void Initialize(void)	:	Initializes class variables
	* Ins							:	void
	* Outs							:	None
	* Returns						:	void
	* Mod. Date						:	11/13/2012
	* Mod. Initals					:	RJR
	***************************************************************/
	void Initialize(void);

	/**************************************************************
	* void Reinitialize(void)	:	Reinitalizes the class variables
	* Ins						:	None
	* Outs						:	None	
	* Returns					:	void
	* Mod. Date					:	11/13/2012
	* Mod. Initals				:	RJR
	***************************************************************/
	void Reinitialize(void);

	/**************************************************************
	* void Update(float fElapsedTime)	:	Updates the class
	* Ins								:	fElapsedTime
	* Outs								:	None
	* Returns							:	void	
	* Mod. Date							:	11/13/2012
	* Mod. Initals						:	RJR
	***************************************************************/
	void Update(float fElapsedTime);
};

#endif