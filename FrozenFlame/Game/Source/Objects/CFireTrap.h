/***********************************************
 * Filename:  		CFireTrap.h
 * Date:      		11/10/2012
 * Mod. Date: 		11/10/2012
 * Mod. Initials:	MB
 * Author:    		Justin Maza
 * Purpose:   		This is the file for the Fire
 *					traps in the game
 ************************************************/


#ifndef CFIRETRAP_H_
#define CFIRETRAP_H_

#include "../Globals.h"
#include "EnvironmentObject.h"
#include "../Renderer/Emitter.h"
#include "../Sound/CSoundManager.h"

class CFireTrap : public CEnvironmentObject
{
private:
	const float m_fAttackLength;
	const float m_fAttackCooldownLength;

	char m_cState;
	float m_fTotalElapsedTime;
	bool m_bFirstUpdate;

	float m_fStartYPos;

	float m_fDelayTimer;
	float m_fAliveTimer;

	int m_nLightID;
	int m_nAttachedLightID;

	CEmitter* m_pEmitter;
	CEmitter* m_pFireEmit;

	CEnvironmentObject* m_pBase;

	// buffered sound flags
	bool  m_bCanPlayEruptionSFX;		// since this is a looping sound, only play it once, not every frame
	bool  m_bCanPlayWarningSFX;			// since this is a looping sound, only play it once, not every frame
	float m_fWarningTimer;				// how long the trap has been idle
	float m_fIdleDurationTime;			// how duration time the trap will be idle

public:
	/********************************************************************
	* CFireTrap():			Constructo for the Fire trap class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/11/2012
	* Mod. Initials:		MB
	********************************************************************/
	CFireTrap();

	/********************************************************************
	* `CFireTrap():			Deconstructo for the Fire trap class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/11/2012
	* Mod. Initials:		MB
	********************************************************************/
	~CFireTrap();

	/********************************************************************
	* Initialize():			Sets up the varibales in the class and
	*						gets the trap ready
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/11/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Initialize(void);

	/********************************************************************
	* CFireTrap():			Resets all the variable making the fire trap
	*						ready to be used again
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/11/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Reinitialize(void);

	/********************************************************************
	* Update():				Passes a time varable in so that the trap
	*						can keep track of when it needs to be
	*						attacking or is off at the time
	*
	* Ins:					float
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			11/11/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Update(float fElapsedTime);

	/**************************************************************
	* void SummonTrap()					:	Summons a fire trap
	* Ins								:	float, vec3f, int
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	12/6/12
	* Mod. Initals						:	JM
	***************************************************************/
	void SummonTrap(float fDelay, vec3f vPosition, int nLightID = -1);


	void RemoveEmitters(void);
	void SetEmittersPosition(const vec3f& vPos);
	void SetBaseObjPosition(const vec3f& vPos);

	void Uninitialize(void);

	/***************
	*  Accessors
	***************/
	float GetAttackCooldownLength(void) const	{ return m_fAttackCooldownLength;}
	int GetState(void) const					{ return this->m_cState;}
	float GetDelay(void) const					{ return m_fDelayTimer;}
	const CEmitter* GetEmitter(void) const		{ return m_pEmitter;}
	const CEmitter* GetFireEmitter(void) const	{ return m_pFireEmit;}
	const CEnvironmentObject* GetBaseObj(void)	{ return m_pBase; }

	/***************
	* Mutators
	***************/
	void SetElapsedTime(float _fElapsedTime)	{ m_fTotalElapsedTime = _fElapsedTime; }
	void SetEmitter(CEmitter* emitter)			{ m_pEmitter = emitter; }
	void SetFireEmitter(CEmitter* emitter)		{ m_pFireEmit = emitter; }
	void SetBaseObj(CEnvironmentObject* object)	{ m_pBase = object; }
};

#endif