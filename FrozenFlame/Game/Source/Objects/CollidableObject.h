/***********************************************
* Filename:  		CollidableObject.h
* Date:      		10/31/2012
* Mod. Date: 		11/12/2012
* Mod. Initials:	MB
* Author:    		Charles Meade
* Purpose:   		Function declarations for the collidable object
************************************************/

#ifndef COLLIDABLE_OBJECT_H
#define COLLIDABLE_OBJECT_H

#include "baseobject.h"
#include "../Renderer/View.h"
#include "../Animations/AnimPlayer.h"

class CCollidableObject :
	public CBaseObject
{
private:
	float m_fSphereRad;
	float m_fHealth;
	float m_fMaxHealth;
	float m_fElementalTimer;
	float m_fDamageTimer;
	float m_fAttackedTimer;
	float m_fAttackSpeed;

	int m_nElementalState;
	int m_nAttackElement;
	int m_nDamage;

	// For pick-ups
	int m_nAmountToDrop;
	int m_nHeatlhChance;

	bool m_bDebuffSet;
	bool m_bAttacking;
	bool m_bMoving;
	bool m_bDying;
	bool m_bDashedInto;

	vec3f m_cSpherePos;
	vec3f m_cAABBMin;
	vec3f m_cAABBMax;

	vec3f m_vKnockbackVelocity;

	CAnimPlayer* m_pAniPlayer;
	vec3f m_vScale;

	// sound buffers
	bool  m_bCanPlayDeathSFX;	// only play once instead of every frame

public:
	enum eElementalStates {FROZEN = -2, COLD, NEUTRAL, HOT, BURNING};

	/*****************************************************************
	*	CCollidableObject():		Default constructor for the collidable base object
	*						that all other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/31/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CCollidableObject(void);

	/*****************************************************************
	*	~CCollidableObject():	Default destructor for the collidable base object
	*							that all other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/31/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	virtual ~CCollidableObject(void);

	/*****************************************************************
	*	Initialize():		The default funciton that sets all starting values for
	*						this instance of the class that other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/31/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	virtual void Initialize(void) = 0;

	/*****************************************************************
	*	Reinitialize():		The default funciton that resets all starting values for
	*						this instance of the class that other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/31/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	virtual void Reinitialize() = 0;
		
	virtual void Uninitialize(void) = 0;

	/*****************************************************************
	*	Update():			Updates all data for the class based off of time and must be overloaded
	*
	* Ins:					fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/13/2012
	* Mod. Initials:	    MB
	*****************************************************************/
	virtual void Update(float fElapsedTime);

	/*****************************************************************
	*	GetCollisionAABB():		gives the passed in params the data for the aabb
	*
	* Ins:					void
	*
	* Outs:					vfMin vfMax
	*
	* Returns:				void
	*
	* Mod. Date:		    11/03/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void GetCollisionAABB(vec3f& vfMin,vec3f& vfMax);

	/*****************************************************************
	*	GetCollisionSphere():		gives the passed in params the data for the sphere
	*
	* Ins:					void
	*
	* Outs:					vfPos fRadius
	*
	* Returns:				void
	*
	* Mod. Date:		    11/03/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void GetCollisionSphere(vec3f& vfPos,float& fRadius);

	/*****************************************************************
	*	SetCollisionSphere(): Sets the passed in parameters of the collision volume.
	*
	* Ins:					void
	*
	* Outs:					vfPos fRadius
	*
	* Returns:				void
	*
	* Mod. Date:		    12/04/2012
	* Mod. Initials:	    SW
	*****************************************************************/
	void SetCollisionSphere(vec3f& vfPos, float& fRadius)	{m_cSpherePos = vfPos; m_fSphereRad = fRadius;}

	/*****************************************************************
	*	OnAttack():			runs any logic when this object gets attacked by something
	*
	* Ins:					void
	*
	* Outs:					nDamage nElementType
	*
	* Returns:				void
	*
	* Mod. Date:		    11/13/2012
	* Mod. Initials:	    MB
	*****************************************************************/
	virtual void OnAttack(int nDamage,int nElementType);

	void SetHealth(float _fHealth);
	
	virtual void UpdateAnimations(float fElapsedTime);

protected:
	/*****************************************************************
	* UpdateCollisionData():	Updates the collision sphere and aabb for this object
	*
	* Ins:					fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    10/31/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void UpdateCollisionData();

public:
	/***************
	*  Accessors
	***************/
	float GetSphereRad(void)				{return m_fSphereRad;}
	float GetHealth(void)					{return m_fHealth;}
	float GetMaxHealth(void)				{return m_fMaxHealth;}
	float GetElementalTimer(void)			{return m_fElementalTimer;}
	float GetDamageTimer(void)				{return m_fDamageTimer;}
	float GetAttackedTimer(void)			{return m_fAttackedTimer;}
	float GetAttackSpeed()					{return m_fAttackSpeed;}

	int GetElementalState(void)				{return m_nElementalState;}
	int GetAttackElement(void)				{return m_nAttackElement;}
	int GetDamage(void)						{return m_nDamage;}

	bool GetIsDebuffSet(void)					{return m_bDebuffSet;}
	bool GetIsAttacking(void)					{return m_bAttacking;}
	bool GetIsMoving(void)					{return m_bMoving;}
	bool GetIsDying(void)						{return m_bDying;}
	bool GetIsDashedInto()					{return this->m_bDashedInto;}

	vec3f GetSpherPos(void)					{return m_cSpherePos;}
	vec3f GetAABBMin(void)					{return m_cAABBMin;}
	vec3f GetAABBMax(void)					{return m_cAABBMax;}
	vec3f GetKnockbackVelocity()			{return this->m_vKnockbackVelocity;}
	vec3f GetScale(void)					{return m_vScale;}

	CAnimPlayer* GetAnimationPlayer(void)	{return m_pAniPlayer;}

	/***************
	* Mutators
	***************/
	void SetSphereRad(float _fSphereRad)			{m_fSphereRad = _fSphereRad;}
	void SetMaxHealth(float _fMaxHealth)			{m_fMaxHealth = _fMaxHealth;}
	void SetElementalTimer(float _fElementalTimer)	{m_fElementalTimer = _fElementalTimer;}
	void SetDamageTimer(float _fDamageTimer)		{m_fDamageTimer = _fDamageTimer;}
	void SetAttackedTimer(float _fAttackedTimer)	{m_fAttackedTimer = _fAttackedTimer;}
	void SetAttackSpeed(float _fAttackSpeed)		{m_fAttackSpeed = _fAttackSpeed;}

	void SetElementalState(int _nElementalState)	{m_nElementalState = _nElementalState;}
	void SetScale(vec3f _vScale)					{m_vScale = _vScale;}
	
	// This is now a virtual function because certain enemies will have to do more stuff when their
	// attack element is set
	virtual void SetAttackElement(int _nAttackElement) { m_nAttackElement = _nAttackElement; }
	
	
	void SetDamage(int _nDamage)					{m_nDamage = _nDamage;}
	
	void SetIsDebuffSet(bool _bDebuffSet)			{m_bDebuffSet = _bDebuffSet;}
	void SetIsAttacking(bool _bAttacking)			{m_bAttacking = _bAttacking;}
	void SetIsMoving(bool _bMoving)					{m_bMoving = _bMoving;}
	void SetIsDying(bool _bDying)					{m_bDying = _bDying;}
	void SetIsDashedInto(bool bDashedInto)			{m_bDashedInto = bDashedInto;}

	void SetSpherePos(vec3f _cSpherePos)			{m_cSpherePos = _cSpherePos;}
	void SetAABBMin(vec3f _cAABBMin)				{m_cAABBMin = _cAABBMin;}
	void SetAABBMax(vec3f _cAABBMax)				{m_cAABBMax = _cAABBMax;}
	void SetKnockbackVelocity(vec3f vKnockback)		{m_vKnockbackVelocity = vKnockback;}

	void SetAnimPlayer(CAnimPlayer* _pAniPlayer)	{m_pAniPlayer = _pAniPlayer;}

	void SpawnPickups(void);

	void SetAmountToDrop(int amount)				{m_nAmountToDrop = amount;}
	void SetHealthChance(int chance)				{m_nHeatlhChance = chance;}

	void SetWorldMatrix(const matrix4f& _matrix)
	{
		if(!GetIsDying())
			CBaseObject::SetWorldMatrix(_matrix);
	}

};

#endif