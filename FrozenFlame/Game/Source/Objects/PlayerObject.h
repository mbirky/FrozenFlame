/***********************************************
 * Filename:  		PlayerObject.h
 * Date:      		10/30/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	JM
 * Author:    		Charles Meade
 * Purpose:   		Function declarations for the player object
 ************************************************/

#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include "CollidableObject.h"
#include "../Renderer/Emitter.h"
#include "../Sound/CSoundManager.h"

#include <deque> // I have no idea why I can't just include Globals.h for this
using std::deque;

class CSceneryObject;

struct tRumble
{
	float fDuration;
	float fTimer;
	int   nInitialRumble;
	int	  nCurrentRumble;
	bool  bActive;
};

struct tDashGhost
{
	CSceneryObject* ghost;
	float fAge;				// how long its been alive
	//int   nRenderID;
};

class CPlayerObject :
	public CCollidableObject
{
private:
	//friend class CCamera;

	enum eRageFlags			{RAGE_LVL1 = 1, RAGE_LVL2, RAGE_LVL3 = 4};

	const float m_fRageDecAmt;
	const float m_fMaxAccumulatedRageValue;

	static const int m_nMaxPlayerRage			= 100;

	CEmitter* m_pcFireEmitter;
	CEmitter* m_pcIceEmitter;
	//CEmitter* m_pcStunEmitter;
	//CEmitter* m_pcRageEmitter;
	//CEmitter* m_pcRageEmitter2;
	CEmitter* m_pcFireAttackEmitter;
	CEmitter* m_pcIceAttackEmitter;
	CEmitter* m_pcAmbientFireEmitter;
	CEmitter* m_pcAmbientColdEmitter;

	CEmitter * testEmitter;

	vec3f m_vOldVel;
	float m_fSpeed;
	char m_chInputBits;

	float m_fRage;
	float m_fAttackCooldown;
	float m_fRageDecreaseTimer;
	float m_fCurrRot;
	float m_fRageCooldown;
	float m_fAccumulatedRage;
	float m_fDashCooldown;
	float m_fDamageSoundTimer;		// how long it's been since the player played a hurt sound
	float m_fDamageSoundBuffer;		// the player will play the hurt sound no more often than this
	float m_fAttackButtonTimer;		// how long it's been since pressing an attack button
	float m_fAttackButtonBuffer;	// how long the player has before they must press an attack button again to continue attacking
	float m_fIdleTimer;
	float m_fGrowthTimer;
	float m_fCurrentSize;
	float m_fDashTimer;
	float m_fRageSpeedFactor;		// modifies the animation speed and attack cooldown by this much
	float m_fStunDuration;			// the player will stay stunned this long if they hit projectiles with the opposite element
	float m_fStunTimer;				// keeps track of how long the player has been stunned
	float m_fKnockbackFactor;		// when the player gets stunned, he gets moved by this much times his forward vector
	float m_fFireTrapRumbleDuration;// the rumble will play for this long when the player gets hurt from a fire trap
	float m_fFireTrapRumbleTimer;	// keeps track of how long the rumble has been on for fire trap rumble
	float m_fIceTrapRumbleDuration; // the rumble will play for this long when the player is getting hurt from an ice trap
	float m_fIceTrapRumbleTimer;	// keeps track of how long the rumble has been on for ice trap rumble
	float m_fProjectileHitRumbleDuration;	// the rumble will play for this long when the player gets hurt from a projectile
	float m_fProjectileHitRumbleTimer;		// keeps track of how long the rumble has been on for ice trap rumble
	float m_fFireBeastRollRumbleDuration;	// the rumble will play for this long when the player gets hurt from a fire beast rolling into them
	float m_fFireBeastRollRumbleTimer;		// keeps track of how long the rumble has been on for fire beast roll rumble
	

	// controller rumble
	int m_nStunRumbleInitial;		// the initial rumble strength when the player gets stunned
	int m_nStunRumbleCurrent;		// the current rumble strength when the player gets stunned
	int m_nFireTrapRumbleInitial;	// the initial rumble strength when the player gets hurt from a fire trap
	int m_nFireTrapRumbleCurrent;	// the current rumble strength when the player gets hurt from a fire trap
	int m_nIceTrapRumbleInitial;	// the initial rumble strength when the player gets hurt from an ice trap
	int m_nIceTrapRumbleCurrent;	// the current rumble strength when the player gets hurt from an ice trap
	int m_nProjectileRumbleInitial;	// the initial rumble strength when the player gets hurt from a projectile
	int m_nFireBeastRollRumbleInitial;	// the initial rumble strength when the player gets hurt from a fire beast rolling into them
	int m_nFireBeastRollRumbleCurrent;	// the current rumble strength when the player gets hurt from a fire beast rolling into them
	int m_nAmountofTimesRaged;			// Part of Achievement Manger: Keeps track of the amount of times raged.
	int m_nMinDamage;					// The amount of damage caused by the first swing.
	int m_nMaxDamage;					// The amount of damage caused by the second swing.

	tRumble m_FireBeastMeleeRumble;
	tRumble m_MeleeMinionMeleeRumble;
	
	// Those will store the IDs for the sword attachments
	int m_nFireSwordAttID;
	int m_nIceSwordAttID;

	bool m_bSpeedRage;				// if true, the player attacks very quickly when enraged; if false, the player does double damage instead
	bool m_bAttackedLastFrame;
	bool m_bDashing;
	bool m_bCanGetRage;
	bool m_bRaging;
	bool m_bCanRage;
	bool m_bActivateRage;
	bool m_bHitLastFrame;
	bool m_bEverlastingRage;
	bool m_bStunned;
	bool m_bFireTrapRumble;			// the rumble is on from hitting a fire trap
	bool m_bIceTrapRumble;			// the rumble is on from hitting an ice trap
	bool m_bProjectileRumble;		// the rumble is on from getting hit by a projectile
	bool m_bFireBeastRollRumble;	// the rumble is on from a fire beast rolling into you
	bool m_bMeleeMinionLeftSideRumble;	// if a melee minion is hitting the player and to the player's left side (false for right side)
	bool m_bIsRageUsed;					// If the player uses rage during a level, this will activate.
	bool m_bPrevRage;
	bool m_bStoppedRage;

	vec3f m_tLastValidVelocity;
	vec3f m_vLastPosition;  // Used for calculating dashing collisions - JC

	// elemental dashing
	deque<tDashGhost*> m_qDashGhosts;		// elemental dashing while enraged will produce a stream of translucent players behind the player
	unsigned int m_unMaxDashGhosts;			// the queue will not hold more than this many dash ghosts
	float m_fDashGhostTimer;				// how long it's been since spawning a dash ghost
	float m_fDashGhostSpawnDelay;			// wait this long between making each new dash ghost
	float m_fInitialDashGhostAlpha;			// the starting transparency (0 to 1) of a ghost
	float m_fGhostLifetime;					// the length of time each ghost will last before disappearing (only used for alpha factoring)

	// transparency = (1 - age/lifetime) * initialAlpha

	char m_cRageFlag;
	char m_cPrevRage;
	float m_fStartRot;

	vec3f playerPos;

	int m_nRageLight;

public:

	enum eCharacterBitMask	{CH_UP = 1, CH_DOWN = 2, CH_LEFT = 4, CH_RIGHT = 8, CH_DASH = 16, CH_FIRE_ATK = 32, CH_ICE_ATK = 64, CH_RAGE = 128};		// This needs to be global

	/*****************************************************************
	*	CPlayerObject():		Default constructor for the player object
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
	CPlayerObject(void);

	/*****************************************************************
	*	~CPlayerObject():		Default destructor for the player object
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
	~CPlayerObject(void);

	/*****************************************************************
	*	Initialize():		The default funciton that sets all starting values for
	*						this instance of the class
	*
	* Ins:					nType
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/01/2012
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
	void ReinitializeEmitters();

	/*****************************************************************
	*	Uninitialize():		The default funciton that uninitilizes all starting values for
	*						this instance of the class
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    12/5/2012
	* Mod. Initials:	    CMB
	*****************************************************************/
	void Uninitialize();

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
	void UpdateAnimations(float fElapsedTime);

	/*****************************************************************
	*	Update():			Updates the player based off of input. must be called once before the update
	*
	* Ins:					chInputBits
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    11/01/2012
	* Mod. Initials:	    CM
	*****************************************************************/
	void UpdateInput(char chInputBits);

	// adds (or subtracts) elapsed time, does what it needs to at the end, and resets the timers
	void UpdateBufferTimers(float fElapsedTime);

	void ResetRotation(void);
	void ResetSize(void);
	void OnAttack(int nDamage,int nElementType);

	void StunThePlayer();
	void InitiateFireTrapRumble();
	void InitiateIceTrapRumble();
	void InitiateProjectileRumble();
	/*****************************************************************
	* InitiateFireBeastRollRumble(): begins the rumble from a fire beast rolling into the player
	*
	* Ins:					elementalState - the fire beast's current elemental state
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:		    01/21/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void InitiateFireBeastRollRumble(int elementalState = 0);

	/*****************************************************************
	* InitiateBeastMeleeRumble(): begins the rumble from a fire beast smacking the player
	*
	* Ins:					damageToPlayer - the amount of damage the player is taking from the melee attack
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:		    01/21/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void InitiateBeastMeleeRumble(int damageToPlayer);

	/*****************************************************************
	* InitiateMeleeMinionMeleeRumble(): begins the rumble from a melee minion smacking the player
	*
	* Ins:					damageToPlayer - the amount of damage the player is taking from the melee attack
	*						minionIsOnPlayersLeft	- true if the minion is on the left side of the player, false if on the right
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:		    01/21/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void InitiateMeleeMinionMeleeRumble(int damageToPlayer, bool minionIsOnPlayersLeft);

	/*****************************************************************
	* CreateElementalDashGhosts(): creates the elemental dash ghost objects and places them in the queue
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:		    02/08/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void CreateElementalDashGhosts();
	/*****************************************************************
	* UpdateElementalDashGhosts(): updates the queue of elemental dash ghosts
	*
	* Ins:					none
	*
	* Outs:					none
	*
	* Returns:				none
	*
	* Mod. Date:		    02/08/2013
	* Mod. Initials:	    SMS
	*****************************************************************/
	void UpdateElementalDashGhosts(float fElapsedTime);

	/***************
	*  Accessors
	***************/
	inline float GetMaxAccumulatedRage	(void)	const { return m_fMaxAccumulatedRageValue; }
	inline int   GetMaxPlayerRage		(void)	const { return m_nMaxPlayerRage;		}
	inline float GetRageDecAmount		(void)	const { return m_fRageDecAmt; }
	inline char	 GetInputBits			(void)	const { return m_chInputBits; }
	inline float GetRage				(void)	const { return m_fRage; }
	inline float GetAttackCooldown		(void)	const { return m_fAttackCooldown; }
	inline float GetRageDecreaseTimer	(void)	const { return m_fRageDecreaseTimer; }
	inline float GetCurrRotation		(void)	const { return m_fCurrRot; }
	inline float GetRageCooldown		(void)	const { return m_fRageCooldown; }
	inline float GetAccumulatedRage		(void)	const { return m_fAccumulatedRage; }
	inline bool	 GetAttackedLastFrame	(void)	const { return m_bAttackedLastFrame; }
	inline float GetDashCooldown		(void)	const { return m_fDashCooldown; }
	inline bool	 IsDashing				(void)	const { return m_bDashing; }
	inline bool	 CanGetRage				(void)	const { return m_bCanGetRage; }
	inline vec3f GetLastValidVelocity	(void)	const { return m_tLastValidVelocity; }
	inline bool	 IsRaging				(void)	const { return m_bRaging; }
	inline bool	 CanRage				(void)	const { return m_bCanRage; }
	inline bool	 GetHitLastFrame		(void)	const { return m_bHitLastFrame;	}
	inline float GetIdleTime			(void)  const { return m_fIdleTimer; }
	inline float GetStartRot			(void)	const { return m_fStartRot; }
	inline vec3f GetLastPosition        (void)  const { return m_vLastPosition; }
	inline bool  GetIsActivatingRage	(void)	const { return m_bActivateRage; }
	inline bool  GetIfStunned			(void)	const { return m_bStunned; }
	inline bool  GetIsRageUsed			(void)	const { return m_bIsRageUsed;}
	inline bool  GetIsEverlastingRage	(void)	const { return m_bEverlastingRage;}
	inline bool  GetIsSpeedRaging		(void)  const { return m_bSpeedRage; }
	inline int   GetMinDamage			(void)  const { return m_nMinDamage; }
	inline int   GetMaxDamage			(void)  const { return m_nMaxDamage; }


	/***************
	* Mutators
	***************/
	void SetRage						(float	fRage				);
	inline void SetInputBits			(char	chInputBits			) { m_chInputBits			= chInputBits			; }
	inline void SetAttackCooldown		(float	fAttackCooldown		) { m_fAttackCooldown		= fAttackCooldown		; }
	inline void SetRageDecreaseTimer	(float	fRageDecreaseTimer	) { m_fRageDecreaseTimer	= fRageDecreaseTimer	; }
	inline void SetCurrRotation			(float	fCurrRot			) { m_fCurrRot				= fCurrRot				; }
	inline void SetRageCooldown			(float	fRageCooldown		) { m_fRageCooldown			= fRageCooldown			; }
	inline void SetAccumulatedRage		(float	fAccumulatedRage	) { m_fAccumulatedRage		= fAccumulatedRage		; }
	inline void SetAttackedLastFrame	(bool	bAttackedLastFrame	) { m_bAttackedLastFrame	= bAttackedLastFrame	; }
	inline void SetDashCooldown			(float	fDashCooldown		) { m_fDashCooldown			= fDashCooldown			; }
	void SetDashing				(bool	bDashing			);
	inline void SetCanGetRage			(bool	bCanGetRage			) { m_bCanGetRage			= bCanGetRage			; }
	inline void SetLastValidVelocity	(vec3f	tLastValidVelocity	) { m_tLastValidVelocity	= tLastValidVelocity	; }
	inline void SetRaging				(bool	bRaging				) { m_bRaging				= bRaging				; }
	inline void SetCanRage				(bool	bCanRage			) { m_bCanRage				= bCanRage				; }
	inline void SetHitLastFrame			(bool	bHitLastFrame		) { m_bHitLastFrame			= bHitLastFrame			; }
	inline void SetIdleTime				(float  fIdleTimer			) { m_fIdleTimer			= fIdleTimer			; }
	inline void SetStartRot				(float  fStartRot			) { m_fStartRot				= fStartRot				; }
	inline void SetFireEmitterOn		(bool	bOn					) { m_pcFireEmitter->SetActive(bOn)					; }
	inline void SetIceEmitterOn			(bool	bOn					) { m_pcIceEmitter->SetActive(bOn)					; }
	inline void SetFireOffset			(vec3f& in					) { m_pcFireEmitter->SetOffset(in)					; }
	inline void SetIceOffset			(vec3f& in					) { m_pcIceEmitter->SetOffset(in)					; }
	inline void SetLastPosition			(vec3f& tPosition           ) { m_vLastPosition			= tPosition				; } 
	inline void SetIsEverlastingRage	(bool _IsEverlastingRage	) { m_bEverlastingRage = _IsEverlastingRage			; }

	void SetVelocity(vec3f vVelocity)		
	{
		if(!GetIsDying())
			CBaseObject::SetVelocity(vVelocity);
	}

	void SetPos(vec3f _vPos)
	{
		if(!GetIsDying())
			CBaseObject::SetPos(_vPos);
	}

	void SetPos(vec3f _vPos, bool flag)				
	{
		if(!GetIsDying())
		{
			playerPos = GetPos();
			CBaseObject::SetPos(_vPos);
		}
	}
};

#endif
