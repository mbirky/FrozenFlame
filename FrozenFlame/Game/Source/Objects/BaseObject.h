/***********************************************
 * Filename:  		BaseObject.h
 * Date:      		10/29/2012
 * Mod. Date: 		11/03/2012
 * Mod. Initials:	CM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the base object
 ************************************************/
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

enum eObjectTypes {BASE_OBJECT, COLLIDABLE_OBJECT, PLAYER_OBJECT, ENVIRONMENT_OBJECT, PROJECTILE_OBJECT, 
				   ENEMY_OBJECT, MELEE_OBJECT, EXPLOSIVE_OBJECT, RANGED_OBJECT, ICE_OBJECT, FIRE_OBJECT,
				   WIZARD_OBJECT, WALL_OBJECT, SPAWNER_OBJECT, PORTAL_OBJECT, ATTACK_OBJECT, ICETRAP_OBJECT, FIRETRAP_OBJECT,
				   SNOWBALL_OBJECT, PARTICLE_OBJECT, PICKUP_OBJECT, SCENERY_OBJECT, SIGN_OBJECT, NUM_OBJECTS};

#include "../Util/Matrix4x4.h"
#include "../Renderer/View.h"
#include "../Sound/CSoundManager.h"

class CBaseObject
{
private:
	friend class CRenderInstance;

private:
	int m_nType;
	int m_nRenderID;
	bool m_bInitialized;

	vec3f m_tVelocity;
	vec3f m_tColor;
	vec3f m_tPrevColor;
	bool m_bInvertColors;
	
	// Is this a transparent object?
	bool m_bTransparent;
	float m_fTransparency;

	// variables for object shaking left and right
	bool m_bShakeLeft;
	bool m_bIsShaking;
	float m_fShakeTimer;
	float m_fShakeAmount;
	int m_nShakeFrameCounter;

	// sound data for the sound manager
	AkGameObjectID m_nSoundID;
	AkSoundPosition m_nSoundPos;

	matrix4f m_tPositionData;

	int m_nToBeDeleted;			// flag for deletion iteration; 0 for false, 1 if true

public:
	/*****************************************************************
	*	CBaseObject():		Default constructor for the base object
	*						that all other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    11/02/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	CBaseObject(void);
	
	/*****************************************************************
	*	~CBaseObject():		Default destructor for the base object
	*						that all other objects must overload
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				n/a
	*
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	virtual ~CBaseObject(void);

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
	* Mod. Date:		    10/29/2012
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
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	virtual void Reinitialize() = 0;

	/*****************************************************************
	* Uninitialize():		returns the object to the starting state
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/03/2012
	* Mod. Initials:	    CM
	*****************************************************************/
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
	* Mod. Date:		    10/29/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	virtual void Update(float fElapsedTime) = 0;

	/*****************************************************************
	* LookAt():		Manipulates the object's vector to look at the position passed in
	*
	* Ins:			 vec3f _tVec, float _fElapsedTime
	*				 
	* Outs:			 void
	*				 
	* Returns:		 void
	*				 
	* Mod. Date:	 11/17/2012
	* Mod. Initials: SW
	*****************************************************************/
	void LookAt(vec3f _tVec, float _fElapsedTime);
	void FullLookAt(const vec3f& vPos);

	/*****************************************************************
	* FlashColor():	Rapidly changes the objects color to the one passed in
	*
	* Ins:			 vec3f _tColor
	*				 
	* Outs:			 void
	*				 
	* Returns:		 void
	*				 
	* Mod. Date:	 11/30/2012
	* Mod. Initials: SW
	*****************************************************************/
	void FlashColor(vec3f _tColor);

	/*****************************************************************
	* ShakeObject():	Rapidly moves the object left and right for a time.
	*
	* Ins:			 void
	*				 
	* Outs:			 void
	*				 
	* Returns:		 void
	*				 
	* Mod. Date:	 11/30/2012
	* Mod. Initials: SW
	*****************************************************************/
	void ShakeObject(void);

	/*****************************************************************
	* RegisterWithSoundManager():	Callling this function handles everything nececessary
	*								for getting this object identified and registered in the
	*								sound manager (setting position still needs to be done in Update())
	*
	* Ins:			 void
	*				 
	* Outs:			 void
	*				 
	* Returns:		 void
	*				 
	* Mod. Date:	02/18/2013
	* Mod. Initials: SMS
	*****************************************************************/
	void RegisterWithSoundManager(void);

	/***************
	*  Accessors
	***************/
	virtual int GetType()							{ return m_nType;}
	int GetRenderID()								{ return m_nRenderID;}
	bool GetInitialized()							{ return m_bInitialized;}
	vec3f GetVelocity()								{ return m_tVelocity;}
	vec3f GetColor()								{ return m_tColor;}
	vec3f GetPos()									{ return m_tPositionData.axis_pos;}
	const matrix4f* GetWorldMatrix()				{ return &m_tPositionData;}
	const int GetIsToBeDeleted() const				{ return m_nToBeDeleted; }
	inline float GetTransparency(void) const		{ return m_fTransparency; }
	bool GetIsShaking()								{ return m_bIsShaking;}
	float GetShakeTimer()							{ return m_fShakeTimer;}
	AkGameObjectID GetSoundID(void)					{ return m_nSoundID;}
	AkSoundPosition GetSoundPos(void)				{ return m_nSoundPos;}
	float GetShakeAmount()							{ return m_fShakeAmount;}

	/***************
	* Mutators
	***************/
	void SetType(int _nType)						{ m_nType = _nType;}
	void SetRenderID(int _nRenderID)				{ m_nRenderID = _nRenderID;}
	void SetInitialized(bool _bInitialized)			{ m_bInitialized = _bInitialized;}
	virtual void SetWorldMatrix(const matrix4f& _matrix)	{ m_tPositionData = _matrix; }
	void SetIsToBeDeleted(int falseOrTrue)			{ m_nToBeDeleted = falseOrTrue; }
	virtual void SetVelocity(vec3f vVelocity)		{ m_tVelocity= vVelocity; }
	void SetColor(vec3f _tColor)					{ m_tColor = _tColor;}
	virtual void SetPos(vec3f _vPos)				{ m_tPositionData.axis_pos = _vPos;}
	void SetTransparency(float fTransparency)		{ m_fTransparency = fTransparency; }
	void SetIsShaking(bool _bIsShaking)				{ m_bIsShaking = _bIsShaking;}
	void SetShakeTimer(float _fShakeTimer)			{ m_fShakeTimer = _fShakeTimer;}
	void SetSoundID(AkGameObjectID id)				{ m_nSoundID = id;}
	void SetSoundPos(vec3f position);
	void SetSoundPos(D3DVECTOR position);
	void SetSoundPos(float x, float y, float z);
	void SetShakeAmoutn(float _fShakeAmount)		{ m_fShakeAmount = _fShakeAmount;}
};
#endif		//BASE_OBJECT_H