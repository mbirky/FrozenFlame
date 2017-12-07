#ifndef CEMITTERMANAGER_H_
#define CEMITTERMANAGER_H_

#include "Emitter.h"

class CEmitter;

class CEmitterManager
{
	struct TEmit
	{
		CEmitter* m_pcEmitter;
		bool m_bIsKilled;

		TEmit() : m_pcEmitter(nullptr), m_bIsKilled(false) {}
	};

	// Vector of Emitters :D
	vector<TEmit> m_vEmitters;

public:

	CEmitterManager();

	/*****************************************************************
	*	Init():			Initializes any data members that need initializing.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void Init();

	/*****************************************************************
	*	Shutdown():			Kills all emitters and clears the vector.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void Shutdown();

	/*****************************************************************
	* AddEmitter():		Adds an emitter to the vector.
	*
	* Ins:					bool bIsLooping, bool bIsStartActive, vec3f vPosition, string szFilename
	*						pAttached is the object this emitter is attached to. use pos and nullptr if no object
	*						pSprayOn creates a flamethrower effect toward it's destination object
	*						pTracking on sprays like spray does but it tracks the object
	*
	* Outs:					void
	*
	* Returns:				CEmitter*
	*
	* Mod. Date:		    2/12/2013
	* Mod. Initials:	    JC
	*****************************************************************/
	CEmitter* AddEmitter(bool bIsLooping, bool bIsStartActive, vec3f vPosition, string szFilename,
		bool bKillOnEnd = false, CBaseObject* pAttached = nullptr, CBaseObject* pDestination = nullptr,
		bool bSprayOn = false, bool bTrackingOn = false);

	/*****************************************************************
	* AddEmitter():		Adds an emitter to the vector.
	*
	* Ins:					filename - filename of emitter file		
	*						flags	 - flags for emitter behavior (check globals.h)
	*						position - position of emitter if no destination attachment
	*						sprayoffset - position to spray to based on emitter position if no attachment
	*						attached - object emitter can be attached to
	*						destination - destination object attachment
	*
	* Outs:					void
	*
	* Returns:				CEmitter*
	*
	* Mod. Date:		    2/18/2013
	* Mod. Initials:	    JC
	*****************************************************************/
	CEmitter* AddEmitter(string szFilename, vec3f vPosition, vec3f vSprayOffset = vec3f(0.0f,0.0f,0.0f),
		char cFlags = EMIT_LOOP, CBaseObject* pAttached = nullptr, CBaseObject* pDestination = nullptr);

	/*****************************************************************
	* UpdateEmitters():		Updates all emitters in the vector.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void UpdateEmitters(float fElapsedTime);

	/*****************************************************************
	* KillEmitter():		Stops and removes an emitter from the vector.
	*
	* Ins:					bool bIsImmediate, int nRemainingCycles
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void KillEmitter(CEmitter* pcEmitter, bool bIsImmediate = true, int nRemainingCycles = 0);
};

#endif /*CEMITTERMANAGER_H_*/