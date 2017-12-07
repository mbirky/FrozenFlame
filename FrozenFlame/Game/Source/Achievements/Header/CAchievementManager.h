#ifndef CACHIEVEMENTMANAGER_H_
#define CACHIEVEMENTMANAGER_H_

#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <iostream>
#include "../../Globals.h"

using namespace std;

class CAchievementManager
{
	struct TAchievement
	{
		string m_szAchievementName;
		string m_szDescription;
		bool m_bIsAchieved;

		TAchievement() : m_szAchievementName("Default Achievement"), m_szDescription("Default Description"), m_bIsAchieved(false) {}
		~TAchievement() {m_szAchievementName = ""; m_szDescription = ""; m_bIsAchieved = false;}
	};

	vector<TAchievement*> m_vAchievements;
	queue<TAchievement*> m_qAchievementsToBeActivated;
	float m_fAchievementDisplayTimer;
	float m_fAchievementDisplayThreshold;
	bool m_bPopFront;
	int m_nFontID;
	TImage m_tButtonNormal;
	bool m_bIsFireBeastDefeated;
	bool m_bIsIceBeastDefeated;
	int m_nPickupsCollected;
	unsigned int m_nAchievementCount;
	bool m_bIsSpawnerDestroyed;
	int m_nDeflectionCount;
	bool m_bHasPlayerRaged;


	// Creating the private functions necessary to make a singleton.
	// Default Constructor
	CAchievementManager() {}
	// Copy Constructor
	CAchievementManager(const CAchievementManager&);
	// Assignment Operator
	CAchievementManager& operator=(const CAchievementManager&);
	// Destructor
	~CAchievementManager() {}

public:

	static CAchievementManager* GetInstance(void)
	{
		static CAchievementManager pInstance;
		return &pInstance;
	}

	/*****************************************************************
	*	Init():			Reads in the achievement data from a text file, or
	*					creates one if none exists.
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
	* AddAchievement():		Adds an achievement to the vector.
	*
	* Ins:					string szAchievementName, string szDescription, bool bHasBeenCompleted
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void AddAchievement(string szAchievementName, string szDescription, bool bHasBeenCompleted = false);

	/*****************************************************************
	* ActivateAchievement():	Searches for and activates an achievement, also putting
	*							it in the queue to be displayed.
	*
	* Ins:					string szAchievementName
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void ActivateAchievement(string szAchievementName);

	/*****************************************************************
	* UpdateAchievements():	Goes through the queue of completed achievements to 
	*						display them to the user.
	*
	* Ins:					float fElapsedTime
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void UpdateAchievements(float fElapsedTime);

	/*****************************************************************
	* DeleteAchievements():	Sets all Achievement's completed bools to false.
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
	void DeleteAchievements();

	/*****************************************************************
	* DisplayAchievement():	Shows the player that they've earned an achievement.
	*
	* Ins:					TAchievement* ptAchievement
	*
	* Outs:					void
	*
	* Returns:				void
	*
	* Mod. Date:		    1/21/2013
	* Mod. Initials:	    SW
	*****************************************************************/
	void DisplayAchievement(TAchievement* ptAchievement);

	/*****************************************************************
	* SaveAchievements():	Saves all earned achievements.
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
	void SaveAchievements(); 

	void Render();

	/*****************************************************************
	* Shutdown():			Deletes all achievements locally, called after saving them.
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

	void ResetAchievements();

	// This function will check to see if you should get the "No Life" achievement.
	int CheckForPlat();

	// ACCESSORS
	bool GetIsFireBeastDefeated()	{return m_bIsFireBeastDefeated;}
	bool GetIsIceBeastDefeated()	{return m_bIsIceBeastDefeated;}
	int  GetPickupsCollected()		{return m_nPickupsCollected;}
	unsigned int GetAchievementSize()	{return m_vAchievements.size();}
	vector<TAchievement*> GetAchievements()	{return m_vAchievements;}
	bool GetIsSpawnerDestroyed()	{return m_bIsSpawnerDestroyed;}
	int GetDeflectionCount()		{return m_nDeflectionCount;}
	bool GetHasPlayerRaged()		{return m_bHasPlayerRaged;}

	// MUTATORS
	void SetIsFireBeastDefeated(bool bIsFireBeastDefeated)	{m_bIsFireBeastDefeated = bIsFireBeastDefeated;}
	void SetIsIceBeastDefeated(bool bIsIceBeastDefeated)	{m_bIsIceBeastDefeated = bIsIceBeastDefeated;}
	void SetPickupsCollected(int nPickupsCollected)			{m_nPickupsCollected = nPickupsCollected;}
	void SetIsSpawnerDestroyed(bool bIsSpawnerDestroyed)	{m_bIsSpawnerDestroyed = bIsSpawnerDestroyed;}
	void SetDeflectionCount(int nDeflectionCount)			{m_nDeflectionCount = nDeflectionCount;}
	void SetHasPlayerRaged(bool bHasPlayerRaged)			{m_bHasPlayerRaged = bHasPlayerRaged;}
};

#endif /*CAHCIEVEMENTMANAGER_H_*/