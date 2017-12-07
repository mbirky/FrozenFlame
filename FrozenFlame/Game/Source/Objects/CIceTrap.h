/***********************************************
 * Filename:  		CIceTrap.h
 * Date:      		11/05/2012
 * Mod. Date: 		11/06/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		The ice trap object class declerations
 ************************************************/
#ifndef CICETRAP_H_
#define CICETRAP_H_

#include "../Globals.h"
#include "EnvironmentObject.h"
#include "../Sound/CSoundManager.h"

class CIceTrap : public CEnvironmentObject
{
private:
	
	bool m_bIsReady;
	char m_cState;
	int m_nLightID;
	int m_nAttachedLightID;
	float m_fDelayTimer;
	float m_fAttackCooldown;
	float m_fTotalElapsedTime;
	float m_fAliveTimer;

	// This is the animated model
	CEnvironmentObject* m_pAnimatedModel;

public:

	/**************************************************************
	* CIceTrap()	:	The default Constructor
	* Ins			:	None.
	* Outs			:	None.
	* Returns		:	None.
	* Mod. Date		:	11/05/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	CIceTrap();

	/**************************************************************
	* ~CIceTrap()	:	The Destructor
	* Ins			:	None.
	* Outs			:	None.
	* Returns		:	None.
	* Mod. Date		:	11/05/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	~CIceTrap();

	/**************************************************************
	* void Initialize(void)	:	Initializes the ice trap
	* Ins							:	void
	* Outs							:	None
	* Returns						:	void
	* Mod. Date						:	11/10/2012
	* Mod. Initals					:	RJR
	***************************************************************/
	void Initialize(void);

	/**************************************************************
	* void Reinitialize(void)	:	Reinitializes the ice trap
	* Ins						:	None
	* Outs						:	None
	* Returns					:	void
	* Mod. Date					:	11/10/2012
	* Mod. Initals				:	RJR
	***************************************************************/
	void Reinitialize(void);

	/**************************************************************
	* void Update(float fElapsedTime)	:	Updates the ice trap
	* Ins								:	fElapsedTime
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	11/10/12
	* Mod. Initals						:	RJR
	***************************************************************/
	void Update(float fElapsedTime);


	/**************************************************************
	* void SummonTrap()					:	Summons an ice trap
	* Ins								:	float, vec3f, int
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	12/6/12
	* Mod. Initals						:	JM
	***************************************************************/
	void SummonTrap(float fDelay, vec3f vPosition, int nLightID = -1);



	void Uninitialize(void);

	void SetModelPosition(const vec3f& vPos);

	/***************
	*  Accessors
	***************/
	float GetTotalAttackCooldown(void)	{return m_fAttackCooldown;}
	float GetTotalElapsedTime(void)		{return m_fTotalElapsedTime;}
	char GetState(void)					{return m_cState;}
	float GetDelay(void)				{return m_fDelayTimer;}
	bool GetIsReady(void)				{return m_bIsReady;}

	/***************
	* Mutators
	***************/
	void SetAttackCooldown(float _fAttackCooldown)		{m_fAttackCooldown = _fAttackCooldown;}
	void SetTotalElapsedTime(float _fTotalElapsedTime)	{m_fTotalElapsedTime = _fTotalElapsedTime;}
	void SetState(char cState)							{m_cState = cState;}
};

#endif