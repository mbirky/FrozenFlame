/********************************************************************
* Filename:			CTimer.h
* Date:				10/24/2012
* Mod. Date:		11/16/2012
* Mod. Initials:	MB
* Author:			Matthew G. Birky
* Purpose:			This class is a timer that allows us to keep
					track fo time for our application
********************************************************************/

#ifndef CTIMER_H
#define CTIMER_H

#include <Windows.h>

class CTimer
{
private:
	__int64		m_nCurrentTime;
	float		m_fTotalTime;
	float		m_fSecPerCount;
	bool		m_bIsRunning;
public:
	/********************************************************************
	* CTimer():				Constructo for the CTimer class.  It sets the
	*						current time variable to the current tick count,
	*						initializes the total time to 0.0, sets the
	*						seconds per count, and sets the isrunning to true
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/24/2012
	* Mod. Initials:		MB
	********************************************************************/
	CTimer(void);

	/********************************************************************
	* GetElapsedTime():		Gets the elapsed time since GetElapsed time
	*						was last called.  If the timer is not running
	*						returns 0
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				double
	*
	* Mod. Date:			10/24/2012
	* Mod. Initials:		MB
	********************************************************************/
	float GetElapsedTime(void);

	/********************************************************************
	* GetTotalTime():		Gets the total time the the timer has been
	*						running.
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				double
	*
	* Mod. Date:			10/24/2012
	* Mod. Initials:		MB
	********************************************************************/
	float GetTotalTime(void);
	
	/********************************************************************
	* Start():				Starts the timer
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/24/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Start(void);
	
	/********************************************************************
	* Stop():				Stops the timer
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/24/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Stop(void);
	
	/********************************************************************
	* reset():				Resets the current time variable and the
	*						total time variable
	*
	* Ins:					void
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:			10/24/2012
	* Mod. Initials:		MB
	********************************************************************/
	void Reset(void);
};

#endif CTIMER_H