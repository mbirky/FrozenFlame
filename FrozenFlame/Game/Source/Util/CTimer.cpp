/********************************************************************
* Filename:			CTimer.cpp
* Date:				10/24/2012
* Mod. Date:		11/12/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This class is a timer that allows us to keep
					track fo time for our application
********************************************************************/

#include "CTimer.h"

CTimer::CTimer(void)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
	
	m_fTotalTime = 0.0f;

	__int64 nCountsPerSecs;
	QueryPerformanceFrequency((LARGE_INTEGER*)&nCountsPerSecs);
	m_fSecPerCount = 1.0f / static_cast<float>(nCountsPerSecs);

	m_bIsRunning = false;
}

float CTimer::GetElapsedTime(void)
{
	if(m_bIsRunning)
	{
		__int64 nPrevTime = m_nCurrentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

		float fElapsedTime = (m_nCurrentTime - nPrevTime) * m_fSecPerCount;
		if(fElapsedTime > 0.125f)
			fElapsedTime = 0.125f;
		m_fTotalTime += fElapsedTime;
		return fElapsedTime;
	}

	return 0.0f;
}

float CTimer::GetTotalTime(void)
{
	if(m_bIsRunning)
	{
		__int64 nTmpTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nTmpTime);

		return m_fTotalTime + (nTmpTime - m_nCurrentTime) * m_fSecPerCount;
	}

	return m_fTotalTime;
}

void CTimer::Start(void)
{
	if(!m_bIsRunning)
	{
		m_bIsRunning = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
	}
}

void CTimer::Stop(void)
{
	if(m_bIsRunning)
	{
		__int64 nPrevTime = m_nCurrentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

		float fElapsedTime = (m_nCurrentTime - nPrevTime) * m_fSecPerCount;
		m_fTotalTime += fElapsedTime;
	}

	m_bIsRunning = false;
}

void CTimer::Reset(void)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);
	m_fTotalTime = 0.0f;
}