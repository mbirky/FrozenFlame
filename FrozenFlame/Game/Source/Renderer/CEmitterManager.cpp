#include "CEmitterManager.h"

CEmitterManager::CEmitterManager()
{

}

void CEmitterManager::Init()
{

}

void CEmitterManager::Shutdown()
{
	unsigned int arrSize = m_vEmitters.size();
	for (unsigned int i = 0; i < arrSize; ++i)
	{
		if(m_vEmitters[i].m_pcEmitter)
			KillEmitter(m_vEmitters[i].m_pcEmitter);
	}
	m_vEmitters.clear();
}

CEmitter* CEmitterManager::AddEmitter(bool bIsLooping, bool bIsStartActive, vec3f vFlexPosition, 
				string szFilename,bool bKillOnEnd, CBaseObject* pAttached, CBaseObject* pDestination,
				bool bSprayOn, bool bTrackingOn)
{
	TEmit tEmit;
	CEmitter* pcEmitter = new CEmitter();
	pcEmitter->SetLoop(bIsLooping);	
	pcEmitter->SetEmittPos(vFlexPosition);
	pcEmitter->LoadParticle(szFilename.c_str());
	pcEmitter->SetActive(bIsStartActive);
	// These two new pointer parameters are used for firing particles toward a target.
	// Source will now always override passed in position in emitter's updating now. 
	// That way we don't need to have moving objects call an emitter's SetEmittPos in their updates. - JC
	pcEmitter->SetAttachedSource(pAttached);
	pcEmitter->SetAttachedDest(pDestination);
	pcEmitter->ActivateDestination(bSprayOn);
	pcEmitter->ActivateDestinationTracking(bTrackingOn);
	tEmit.m_pcEmitter = pcEmitter;
	tEmit.m_bIsKilled = bKillOnEnd; // This kills the emitter after it fires once and all its particles are gone. 
									// Undefined w/ looping activated.  I guess if it has no active particles for a frame it'll die.
	m_vEmitters.push_back(tEmit);

	return pcEmitter;
}

CEmitter* CEmitterManager::AddEmitter(string szFilename, vec3f vPosition, vec3f vSprayOffset, char cFlags,
		CBaseObject* pAttached, CBaseObject* pDestination)
{
	TEmit tEmit;
	CEmitter* pcEmitter = new CEmitter();

	// Load / Set attachments
	pcEmitter->LoadParticle(szFilename.c_str());
	pcEmitter->SetEmittPos(vPosition);
	pcEmitter->SetDestinationPos(vSprayOffset);
	pcEmitter->SetAttachedSource(pAttached);
	pcEmitter->SetAttachedDest(pDestination);

	// Parse and activate flags
	pcEmitter->SetLoop(cFlags & EMIT_LOOP ? true : false);
	pcEmitter->SetActive(cFlags & EMIT_STARTACTIVE ? true : false);
	pcEmitter->ActivateDestination(cFlags & EMIT_SPRAY ? true : false);
	pcEmitter->ActivateDestinationTracking(cFlags & EMIT_TRACKING ? true : false);

	// Set Emitter structure values
	tEmit.m_pcEmitter = pcEmitter;
	tEmit.m_bIsKilled = (cFlags & EMIT_KILLONEND ? true : false);
	m_vEmitters.push_back(tEmit);

	return pcEmitter;	
}

void CEmitterManager::UpdateEmitters(float fElapsedTime)
{
	unsigned int arrSize = m_vEmitters.size();

	for (unsigned int i = 0; i < arrSize; ++i)
	{
		if (m_vEmitters[i].m_bIsKilled == true && m_vEmitters[i].m_pcEmitter)
		{
			if (!m_vEmitters[i].m_pcEmitter->GetActive())
				KillEmitter(m_vEmitters[i].m_pcEmitter);
		}

		if (m_vEmitters[i].m_pcEmitter && m_vEmitters[i].m_pcEmitter->GetActive())
			m_vEmitters[i].m_pcEmitter->Update(fElapsedTime);
	}
}

void CEmitterManager::KillEmitter(CEmitter* pcEmitter, bool bIsImmediate, int nRemainingCycles)
{
	if(!pcEmitter)
		return;

	unsigned int arrSize = m_vEmitters.size();

	if (bIsImmediate)
	{
		for (unsigned int i = 0; i <  arrSize; ++i)
		{
			if (pcEmitter == m_vEmitters[i].m_pcEmitter)
			{			
				delete m_vEmitters[i].m_pcEmitter;
				m_vEmitters[i].m_pcEmitter = nullptr;
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i <  arrSize; ++i)
		{
			if (pcEmitter == m_vEmitters[i].m_pcEmitter)
			{
				m_vEmitters[i].m_bIsKilled = true;
				m_vEmitters[i].m_pcEmitter->SetLoop(false);
			}
		}
		
	}
}