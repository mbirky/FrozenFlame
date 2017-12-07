#ifndef ATTACKOBJECT_H
#define ATTACKOBJECT_H

#include "CollidableObject.h"
#include "BaseObject.h"
class CAttackObject :
	public CCollidableObject
{
private:
	CBaseObject *m_pcOwner;
	float m_fActiveTimer;

public:
	CAttackObject(void);
	~CAttackObject(void);

	void Update(float fElapsedTime);

	void Initialize(void);

	void Reinitialize(void);

	void Uninitialize(void);

	/***************
	*  Accessors
	***************/
	CBaseObject* GetOwner(void)	{return m_pcOwner;}
	float GetActiveTimer()		{return m_fActiveTimer;}
	
	/***************
	* Mutators
	***************/
	void SetOwner(CBaseObject * _pcObject)		{m_pcOwner = _pcObject;}
	void SetActiveTimer(float _fActiveTimer)	{m_fActiveTimer = _fActiveTimer;}
};
#endif		//ATTACKOBJECT_H

