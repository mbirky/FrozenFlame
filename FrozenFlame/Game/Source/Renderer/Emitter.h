/***********************************************
 * Filename:  		Emitter.h
 * Date:      		11/16/2012
 * Mod. Date: 		02/06/2013
 * Mod. Initials:	JC
 * Author:    		Roberto J. Rubiano
 * Purpose:   		Emitter to render particles and declerations
 ************************************************/
#ifndef EMITTER_H_
#define EMITTER_H_

#include "View.h"
#include "RenderInstance.h"
#include "../Globals.h"
#include "../Objects/BaseObject.h"
#include "AssetManager.h"
#include "CEmitterManager.h"

class CEmitter : public CBaseObject
{
private:
	friend class CParticle;

	struct tModifiers
	{	
		bool  m_bToDestination;
		bool  m_bDestinationTrack;
		bool  m_bAreaRotation;
		CBaseObject* m_pDestObj;
		int	  m_nSourceBlend;
		int   m_nDestinationBlend;
		float m_fMinLife;
		float m_fMaxLife;
		float m_fRangeW;
		float m_fRangeH;
		D3DXVECTOR4 m_vEmitterRotation;
		D3DXCOLOR m_cStartColor;
		D3DXCOLOR m_cMidColor;
		D3DXCOLOR m_cEndColor;
		D3DXVECTOR3 m_vMinGravity;
		D3DXVECTOR3 m_vMaxGravity;
		D3DXVECTOR3 m_vVel1;
		D3DXVECTOR3 m_vVel2;
		D3DXVECTOR3 m_vVelocity;
		D3DXVECTOR3 m_vStartPosition;
		D3DXVECTOR3 m_vEndPosition;
		D3DXVECTOR3 m_vOffset; // Ask me about our fabulous deals on offsets! - JC
		D3DXVECTOR3 m_vRotation;
		D3DXVECTOR3 m_fStartScale;
		D3DXVECTOR3 m_fEndScale;
	};

public:

	class CParticle : public CBaseObject
	{
	private:

		friend class CEmitter;

		bool m_bIsEmitActive;
		bool m_bIsAlive;
		bool IsMid;

		float m_fLifeTime;
		float m_fCurrentlife;
		float m_fRotationX;
		float m_fRotationY;
		float m_fRotationZ;

		vec3f m_vOriginalVelocity;

		int m_nWidth;
		int m_nHeight;
		int m_nTextureID;

		D3DXCOLOR m_cColor;
		D3DVECTOR m_vGravity;
		D3DXVECTOR3 m_vScale;

		// Returns a random float in a range, duuuuuuuuh - JC
		float RandomFloatInARange(const float fHigh, const float fLow);

	public:

		/**************************************************************
		* CParticle(void)	:	default constructor for the particle class
		* Ins				:	None
		* Outs				:	None
		* Returns			:	None
		* Mod. Date			:	11/17/2012
		* Mod. Initals		:	RJR
		***************************************************************/
		CParticle(void);

		/**************************************************************
		* ~CParticle(void)	:	default destructor for the particle class
		* Ins				:	None
		* Outs				:	None
		* Returns			:	None
		* Mod. Date			:	11/17/2012
		* Mod. Initals		:	RJR
		***************************************************************/
		~CParticle(void);

		/**************************************************************
		* bool GetAlive(void)	:	returns if the particle is alive or not
		* Ins					:	None
		* Outs					:	None
		* Returns				:	bool
		* Mod. Date				:	11/17/2012
		* Mod. Initals			:	RJR
		***************************************************************/
		bool GetAlive(void) {return m_bIsAlive;}

		void Initialize(void);

		void Reinitialize(void);

		void Uninitialize(void);
			 
		void Update(float fElapsedTime) {}

		void Updates(float fElapsedTime,tModifiers& tMods, int nType, bool bIsLooping,float fGravityStrength);

		/**************************************************************
		* void ResetParticle(CParticle& particle)	:	Resets a particle back to it's original state
		* Ins										:	particle
		* Outs										:	particle
		* Returns									:	void
		* Mod. Date									:	11/19/2012
		* Mod. Initals								:	RJR
		***************************************************************/
		void ResetParticle(tModifiers& tMods, int nType);

		void PlayAnim(int AnimID,bool loop);

		bool GetActive(void) {return m_bIsEmitActive;}

		int  GetTextureID()	 {return m_nTextureID;}

		float GetRotation()	 {return m_fRotationX;}

		float GetScale()	 {return m_vScale.x;}

		D3DXCOLOR& GetParticleColor() { return m_cColor; }
	};

private:

	/**************************************************************
	* void InitModifiers	:	intializes a modifier
	* Ins					:	mod
	* Outs					:	mod
	* Returns				:	void
	* Mod. Date				:	11/17/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	void InitModifiers(tModifiers& mod);

	// Emitter attachment and destination objects
	CBaseObject* m_pSourceAttach;
	CBaseObject* m_pDestinationAttach;

	bool m_bIsLooping;
	bool m_bIsActive;
	bool m_bToDestination;
	bool m_bDestinationTrack;
	bool m_bAreaRotation;
	int m_nNumParticles;
	int m_nRenderType;
	int m_nAliveCount;
	int m_nAliveLastFrame; // Count of particles alive last frame. -JC
	int m_nSourceBlend;
	int m_nDestBlend;
	int m_nShader;		   // Shader used for this emitter's particles. -JC
	float m_fRangeW;
	float m_fRangeH;
	float m_fGravityStrength;	// The strength of the pull towards the dest object. - Sean
	int m_nMinToSpit;	// Min 
	int m_nMaxToSpit;	// Max to fire out each frame.
	int m_nTotalSpat;	// Total particles spat so far for this emitter.
	float m_fEmitterLifetime;	// Lifetime of emitter (not used if not looping)
	float m_fCurrentLifetime;   // Current lifetime
	D3DXVECTOR3 m_vEmittPosition;
	D3DXVECTOR3 m_vDestinationPosition; // Use this if you want to emit to a non object destination - JC
	tModifiers m_tMods;
	CParticle* m_vParticles;

	/**************************************************************
	* CEmitter(void)	:	defualt constructor for the emitter 
	* Ins				:	None
	* Outs				:	None
	* Returns			:	None
	* Mod. Date			:	11/16/2012
	* Mod. Initals		:	RJR
	***************************************************************/
	CEmitter(void);

	/**************************************************************
	* ~CEmitter(void)	:	default destructor for the emitter
	* Ins				:	None
	* Outs				:	None
	* Returns			:	None
	* Mod. Date			:	11/16/2012
	* Mod. Initals		:	RJR
	***************************************************************/
	~CEmitter(void);

	/**************************************************************
	* void Update(float fElapsedTime)	:	updates all the particles in the emitter
	* Ins								:	fElapsedTime
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	11/21/2012
	* Mod. Initals						:	RJR
	***************************************************************/
	void Update(float fElapsedTime);

	/**************************************************************
	* void LoadParticle(const char* cFilename)	:	loads all particle and modifier data from a file
	* Ins										:	cFilename
	* Outs										:	None
	* Returns									:	None
	* Mod. Date									:	11/21/2012
	* Mod. Initals								:	RJR
	***************************************************************/
	void LoadParticle(const char* cFilename);

	void PlayAnims(int AnimID, bool loop);

	void ResetParticles(void);

	void Uninitialize(void);
	
	/**************************************************************
	* void RotateParticles(bool bPrePost,int nAmount,int nAxis)
					:	rotates all the particles once.
	* Ins			:	bPrePost, nAmount, nAxis
	* Outs			:	none
	* Returns		:	void
	* Mod. Date		:	11/21/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	void RotateParticles(bool bPrePost,float nAmount,int nAxis);

	/**************************************************************
	* void ScaleParticles(bool bPrePost,int nAmount,int nAxis)
					:	Scales all the particles once.
	* Ins			:	bPrePost, nAmount, nAxis
	* Outs			:	none
	* Returns		:	void
	* Mod. Date		:	11/21/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	void ScaleParticles(float nAmount,int nAxis);

	/**************************************************************
	* void ScaleParticles(bool bPrePost,int nAmount,int nAxis)
					:	Scales all the particles once.
	* Ins			:	bPrePost, nAmount, nAxis
	* Outs			:	none
	* Returns		:	void
	* Mod. Date		:	11/21/2012
	* Mod. Initals	:	RJR
	***************************************************************/
	void ScaleParticles(float x, float y, float z);

	/**************************************************************
	* void SetLoop(bool bLoop)	:	Sets the state of looping for this emitter
	* Ins						:	bLoop	
	* Outs						:	None
	* Returns					:	void
	* Mod. Date					:	11/21/2012
	* Mod. Initals				:	RJR
	***************************************************************/
	void SetLoop(bool bLoop) {m_bIsLooping = bLoop;}

	/**************************************************************
	* void LogansRun			:	Checks if a particle is too old and kills it
	*							:   Returns true if a particle's life cycle is up.
	* Ins						:	in	
	* Outs						:	None
	* Returns					:	bool
	* Mod. Date					:	02/06/2013
	* Mod. Initals				:	JC
	***************************************************************/
	bool LogansRun(CParticle* in);

	/**************************************************************
	* void CreateParticle		:	Creates a particle and ups count
	* Ins						:	numtospit, in, numspitthisframe	
	* Outs						:	numspitthisframe
	* Returns					:	void
	* Mod. Date					:	02/06/2013
	* Mod. Initals				:	JC
	***************************************************************/
	void CreateParticle(const int numToSpit,CParticle* in,int& numSpitThisFrame);

	/**************************************************************
	* void UpdatePhase			:	1st phase of particle update, actually updates.
	* Ins						:	fElapsedTime	
	* Outs						:	None
	* Returns					:	None
	* Mod. Date					:	02/06/2013
	* Mod. Initals				:	JC
	***************************************************************/
	void UpdatePhase(float fElapsedTime);

	/**************************************************************
	* void KillCreatePhase		:	2nd phase of part. update, kills, makes, and counts particles
	* Ins						:	fElapsedTime	
	* Outs						:	None
	* Returns					:	None
	* Mod. Date					:	02/06/2013
	* Mod. Initals				:	JC
	***************************************************************/
	void KillCreatePhase(void);

	void MakeEmitterRotation();

public:

	friend class CParticle;
	friend class CEmitterManager;

	

	/*********************************************
	*
	*					Mutators
	*
	**********************************************/
	void SetActive(bool bActive);
	

	/**************************************************************
	* void SetEmittPos(D3DVECTOR vPos)	:	Sets the emitters position
	* Ins								:	vPos
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	11/17/2012
	* Mod. Initals						:	RJR
	***************************************************************/
	void SetEmittPos(const D3DVECTOR& vPos);

	/**************************************************************
	* void SetEmittPos(D3DVECTOR vPos)	:	Sets the emitters position
	* Ins								:	vPos
	* Outs								:	None
	* Returns							:	void
	* Mod. Date							:	11/17/2012
	* Mod. Initals						:	RJR
	***************************************************************/
	void SetEmittPos(const vec3f& vPos);

	/**************************************************************
	* void SetEmittPos(float x, float y, float z)	:	Sets the emitters position
	* Ins											:	x,y,z
	* Outs											:	None
	* Returns										:	void
	* Mod. Date										:	11/17/2012
	* Mod. Initals									:	RJR
	***************************************************************/
	void SetEmittPos(float x, float y, float z);

	void SetDestBlend(int nBlendMode) {m_nDestBlend = nBlendMode;}
	void SetSourceBlend(int nBlendMode) {m_nSourceBlend = nBlendMode;}
	void SetShader(string szFileName);
	void SetOffset(vec3f& in) { m_tMods.m_vOffset = D3DXVECTOR3(in.x,in.y,in.z); }
	void SetAttachedSource( CBaseObject* in )	{ m_pSourceAttach = in; }
	void SetAttachedDest( CBaseObject* in );
	void SetDestinationPos( vec3f in);
	void AreaRotation(bool in);
	void ActivateDestination(bool in);
	void ActivateDestinationTracking(bool in);

	void Initialize();
	void Reinitialize();

	/*********************************************
	*
	*					Accessors
	*
	**********************************************/
	bool GetActive(void)			{return m_bIsActive;}
	int GetType(void)				{return m_nRenderType;}
	int GetParticleCount()			{return m_nNumParticles;}
	int GetAliveCount()				{return m_nAliveCount;}
	int GetAliveLastFrameCount()	{return m_nAliveLastFrame;}
	int GetEmitterType()			{return m_nRenderType;}
	CParticle* GetParticles()		{return m_vParticles;}
	int GetShader()					{return m_nShader;}
	tModifiers&	GetModifiers()		{return m_tMods;}
	float GetRangeW()				{return m_fRangeW;}
	float GetRangeH()				{return m_fRangeH;}
	vec3f GetOffset()				{ return vec3f(m_tMods.m_vOffset.x, m_tMods.m_vOffset.y, m_tMods.m_vOffset.z); }
	int GetSourceBlend()			{return m_nSourceBlend; }
	int GetDestBlend()				{return m_nDestBlend;	}
	CBaseObject* GetAttachedSource() { return m_pSourceAttach; }
	CBaseObject* GetAttachedDest()	 { return m_pDestinationAttach; }
	bool GetToDestination()			{ return m_bToDestination; }

};

#endif