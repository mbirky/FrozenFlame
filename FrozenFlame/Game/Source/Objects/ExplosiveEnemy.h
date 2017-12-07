/***********************************************
 * Filename:  		ExplosiveEnemy.h
 * Date:      		10/30/2012
 * Mod. Date: 		11/28/2012
 * Mod. Initials:	SW
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the explosive enemy object
 ************************************************/

#ifndef EXPLOSIVE_ENEMY_H
#define EXPLOSIVE_ENEMY_H

#include "enemy.h"
#include "../Renderer/Emitter.h"
#include "../Sound/CSoundManager.h"

class CExplosiveEnemy :
	public CEnemy
{
	bool m_bExploding;
	bool m_bDamageDealt;
	CEmitter* m_pcEmitter;
	CEmitter* m_pcSmoke;
	int m_nSubType;
	float m_fExplosionScale;
	bool m_bScaleChanged;
	int m_nBombID;
public:
	/*****************************************************************
	*	CExplosiveEnemy():		Default constructor for the explosive enemy object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CExplosiveEnemy(void);

	/*****************************************************************
	*	~CExplosiveEnemy():		Default destructor for the explosive enemy object
	*						
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	~CExplosiveEnemy(void);

	/*****************************************************************
	*	Initialize():		The default funciton that sets all starting values for
	*						this instance of the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Initialize(void);

	/*****************************************************************
	*	Reinitialize():		The default funciton that resets all starting values for
	*						this instance of the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Reinitialize();

	/*****************************************************************
	*	Update():			Updates all data for the class based off of time
	*
	* Ins:					fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/30/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void Update(float fElapsedTime);

	bool GetExploding()	{return m_bExploding;}

	void SetExploding(bool _bExploding)	{m_bExploding = _bExploding;}

	bool GetIsDamageDealt()	{return m_bDamageDealt;}

	void SetIsDamageDealt(bool _bIsDamageDealt)	{m_bDamageDealt = _bIsDamageDealt;}

	int GetSubType()	{return m_nSubType;}

	float GetExplodeScale(void)const {return m_fExplosionScale;}
	
	bool GetIsScaleChanged(void)const {return m_bScaleChanged;}

	void SetSubType(int _nSubType);

	void Uninitialize();


	// This will not only set the attack element for the explosive minion but will
	// also load his canister based on the element.
	void SetAttackElement(int _nAttackElement);
	void SetExplodeScale(float _fScale) {m_fExplosionScale = _fScale;}
	void SetIsScaleChanged(bool _bScaleChanged) {m_bScaleChanged = _bScaleChanged;}

};

#endif