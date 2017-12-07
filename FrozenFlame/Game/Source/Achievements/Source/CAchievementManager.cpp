#include "../Header/CAchievementManager.h"
#include "../../Renderer/View.h"
#include "../../CGame.h"
#include "../../Globals.h"
#include <Windows.h>
struct TAchievement;
#define MAX_ACHIEVEMENTS 14

void CAchievementManager::Init()
{
	m_fAchievementDisplayTimer = 0.0f;
	m_fAchievementDisplayThreshold = 3.0f;
	m_bPopFront = false;
	m_nAchievementCount = 0;
	m_bIsFireBeastDefeated = false;
	m_bIsIceBeastDefeated = false;
	m_nFontID = CView::LoadFont("font example.txt");
	m_tButtonNormal.m_nImageID = CView::LoadSprite("menu_button_normal.png", &m_tButtonNormal.m_nImageWidth, &m_tButtonNormal.m_nImageHeight);

	ifstream fin;
	fin.open("Resource/Achievements/Achievements.bin", ios_base::in | ios_base::binary);

	// If we cannot find the file...
	if (!fin.is_open())
	{
		ResetAchievements();

		// Time to write out this data!
		fstream fout;//("Resource/Achievements/Achievements.bin", ios_base::out | ios_base::binary);
		fout.clear();

		fout.open("Resource/Achievements/Achievements.bin", ios_base::out | ios_base::binary);

		if (fout.is_open())
		{
			// First, let's set the put pointer to the beginning :)
			fout.seekp(0, ios::beg);

			// Write out the number of achievements!
			unsigned int achievementSize = m_vAchievements.size();
			fout.write((char*)&achievementSize, sizeof(unsigned int));

			int achievementCount = CheckForPlat();
			fout.write((char*)&achievementCount, sizeof(int));

			// Iterate through the everything
			for (unsigned int i = 0; i < MAX_ACHIEVEMENTS; ++i)
			{
				// Let's grab the length of the achievement name!
				unsigned int achievementNameSize = m_vAchievements[i]->m_szAchievementName.size();
				fout.write((char*)&achievementNameSize, sizeof(unsigned int));

				// Now writing the name itself!
				char* tempAchievement = (char*)m_vAchievements[i]->m_szAchievementName.c_str();
				fout.write(tempAchievement, achievementNameSize);

				// Let's grab the length of the description!
				unsigned int descriptionNameSize = m_vAchievements[i]->m_szDescription.size();
				fout.write((char*)&descriptionNameSize, sizeof(unsigned int));

				// And now to write the description out
				char* tempDescription = (char*)m_vAchievements[i]->m_szDescription.c_str();
				fout.write(tempDescription, descriptionNameSize);

				// And finally, write out if the achievement is unlocked or not!
				fout.write((char*)&m_vAchievements[i]->m_bIsAchieved, sizeof(bool));

			}
			// Let's close out the file.
			fout.close();
		}
		fout.clear();

		// Also printing out in text file for human readability
		fout.open("Resource/Achievements/Achievements.txt", ios_base::out);

		if (fout.is_open())
		{
			fout << m_vAchievements.size() << endl;
			for (unsigned int i = 0; i < MAX_ACHIEVEMENTS; ++i)
			{
				// Printing the size and name of the achievement.
				unsigned int achievementNameSize = m_vAchievements[i]->m_szAchievementName.size();
				fout << achievementNameSize << endl;
				fout << m_vAchievements[i]->m_szAchievementName << endl;

				// Printing the description.
				unsigned int descriptionSize = m_vAchievements[i]->m_szDescription.size();
				fout << descriptionSize << endl;
				fout << m_vAchievements[i]->m_szDescription << endl;

				// And writing the boolean.
				fout << m_vAchievements[i]->m_bIsAchieved << endl;
			}
			fout.close();
		}
	}
	else
	{
		// Setting the get pointer to the beginning of the file :D
		fin.seekg(0, ios::beg);

		// Firstly, read in the number of achievements.
		unsigned int achievementAmount = 0;
		fin.read((char*)&achievementAmount, sizeof(unsigned int));

		if (achievementAmount != MAX_ACHIEVEMENTS)
		{
			fin.close();
			ResetAchievements();
			return;
		}

		fin.read((char*)&m_nAchievementCount, sizeof(unsigned int));

		for (unsigned int i = 0; i < achievementAmount; ++i)
		{
			unsigned int achievementNameSize = 0;
			string achievementName;
			// Reading in the size of the achievement name.
			fin.read((char*)&achievementNameSize, sizeof(unsigned int));
			// Reading in the name itself.
			for (unsigned int i = 0; i < achievementNameSize; ++i)
			{
				char tempChar;
				fin.get(tempChar);
				achievementName.push_back(tempChar);
			}
			//fin.read((char*)&achievementName, achievementNameSize);

			unsigned int descriptionSize = 0;
			string descriptionName;
			// Reading in the size of the description.
			fin.read((char*)&descriptionSize, sizeof(unsigned int));
			// Reading in the description.
			for (unsigned int i = 0; i < descriptionSize; ++i)
			{
				char tempChar;
				fin.get(tempChar);
				descriptionName.push_back(tempChar);
			}
			//fin.read((char*)&descriptionName, descriptionSize);

			bool bHasBeenAchieved = false;
			fin.read((char*)&bHasBeenAchieved, 1);

			AddAchievement(achievementName, descriptionName, bHasBeenAchieved);

		}
		fin.close();
		fin.clear();

		if (achievementAmount != MAX_ACHIEVEMENTS)
			ResetAchievements();
	}
}

void CAchievementManager::AddAchievement(string szAchievementName, string szDescription, bool bHasBeenCompleted)
{
	TAchievement* tempAchievement = new TAchievement();
	tempAchievement->m_szAchievementName = szAchievementName;
	//tempAchievement->m_szAchievementName += "\0";
	tempAchievement->m_szDescription = szDescription;
	//tempAchievement->m_szDescription += "\0";
	tempAchievement->m_bIsAchieved = bHasBeenCompleted;

	m_vAchievements.push_back(tempAchievement);
}

void CAchievementManager::ActivateAchievement(string szAchievementName)
{
	if (szAchievementName != "")
	{
		unsigned int arrSize = m_vAchievements.size();
		for (unsigned int i = 0; i < arrSize; ++i)
		{
			if (szAchievementName == m_vAchievements[i]->m_szAchievementName)
			{
				if (!m_vAchievements[i]->m_bIsAchieved)
				{
					m_vAchievements[i]->m_bIsAchieved = true;
					m_qAchievementsToBeActivated.push(m_vAchievements[i]);
					m_nAchievementCount += 1;
					if (m_nAchievementCount == m_vAchievements.size()-1)
						ActivateAchievement("No Life");
				}
			}
		}
	}
}

void CAchievementManager::UpdateAchievements(float fElapsedTime)
{
	if (GetIsFireBeastDefeated() && GetIsIceBeastDefeated())
		ActivateAchievement("Beast Tamer");

	if (m_qAchievementsToBeActivated.size() <= 0)
		return;

	if (!m_bPopFront)
	{
		m_fAchievementDisplayTimer += fElapsedTime;

		if (m_fAchievementDisplayTimer >= m_fAchievementDisplayThreshold)
		{
			m_bPopFront = true;
			m_fAchievementDisplayTimer = 0.0f;
		}
	}
	else
	{
		m_qAchievementsToBeActivated.pop();
		m_bPopFront = false;
	}
}

void CAchievementManager::Render()
{
	if (!m_bPopFront && m_qAchievementsToBeActivated.size() > 0)
		DisplayAchievement(m_qAchievementsToBeActivated.front());
}

void CAchievementManager::DeleteAchievements()
{
	unsigned int arrSize = m_vAchievements.size();
	for (unsigned int i = 0; i < arrSize; ++i)
	{
		m_vAchievements[i]->m_bIsAchieved = false;
	}
}

void CAchievementManager::DisplayAchievement(TAchievement* ptAchievement)
{
	RECT tTextRect;
	tTextRect.left = CGame::GetInstance()->GetScreenWidth()/2 - m_tButtonNormal.m_nImageWidth/2;
	tTextRect.top = 0;
	tTextRect.right = CGame::GetInstance()->GetScreenWidth()/2 + m_tButtonNormal.m_nImageWidth/2;
	tTextRect.bottom = 20;

	int nHalfScreenWidth = CGame::GetInstance()->GetScreenWidth() >> 1;
	int nHalfScreenHeight = CGame::GetInstance()->GetScreenHeight() >> 1;

	//CView::Draw(m_tButtonNormal.m_nImageID, tTextRect.left, tTextRect.top, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth))*1.0f, (CGame::GetInstance()->GetScreenHeight()/float(g_nDevelopmentResolutionHeight))*1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 3.0f, 3.0f, "", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, -1.5f);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.4f, 0.4f, "You have earned an achievement!", false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, -9.5f);
	CView::DrawButton(nHalfScreenWidth, nHalfScreenHeight, 0.4f, 0.4f, ptAchievement->m_szAchievementName, false, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, -9.2f);

	tTextRect.top += (m_tButtonNormal.m_nImageHeight >> 4);
	tTextRect.bottom += (m_tButtonNormal.m_nImageHeight >> 4);

	string concatString = "You have earned an achievement!";
	//CView::Write(m_nFontID, concatString, &tTextRect, true, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth)) * 0.24f, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth)) * 0.24f);

	tTextRect.top += (m_tButtonNormal.m_nImageHeight >> 1);
	tTextRect.bottom += (m_tButtonNormal.m_nImageHeight >> 1);

	/*tTextRect.left = CGame::GetInstance()->GetScreenWidth()/2 - 20;
	tTextRect.top = 20;
	tTextRect.right = CGame::GetInstance()->GetScreenWidth()/2 + 20;
	tTextRect.bottom = 40;*/
	concatString = ptAchievement->m_szAchievementName;
	//CView::Write(m_nFontID, concatString, &tTextRect, true, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth)) * 0.24f, (CGame::GetInstance()->GetScreenWidth()/float(g_nDevelopmentResolutionWidth)) * 0.24f);
}

void CAchievementManager::SaveAchievements()
{
	fstream fout;
	fout.open("Resource/Achievements/Achievements.bin", ios_base::out | ios_base::binary);

		if (fout.is_open())
		{
			// First, let's set the put pointer to the beginning :)
			fout.seekp(0, ios::beg);

			int achievementCount = CheckForPlat();

			// Write out the number of achievements!
			unsigned int achievementSize = m_vAchievements.size();
			fout.write((char*)&achievementSize, sizeof(unsigned int));

			// Write out the number of achievements obtained.
			fout.write((char*)&achievementCount, sizeof(int));

			// Iterate through the everything
			for (unsigned int i = 0; i < MAX_ACHIEVEMENTS; ++i)
			{
				// Let's grab the length of the achievement name!
				unsigned int achievementNameSize = m_vAchievements[i]->m_szAchievementName.size();
				fout.write((char*)&achievementNameSize, sizeof(unsigned int));

				// Now writing the name itself!
				char* tempAchievement = (char*)m_vAchievements[i]->m_szAchievementName.c_str();
				//strcpy_s(tempAchievement, achievementNameSize, m_vAchievements[i]->m_szAchievementName.c_str());
				fout.write(tempAchievement, achievementNameSize);

				// Let's grab the length of the description!
				unsigned int descriptionNameSize = m_vAchievements[i]->m_szDescription.size();
				fout.write((char*)&descriptionNameSize, sizeof(unsigned int));

				// And now to write the description out
				char* tempDescription = (char*)m_vAchievements[i]->m_szDescription.c_str();
				//strcpy_s(tempDescription, descriptionNameSize, m_vAchievements[i]->m_szDescription.c_str());
				fout.write(tempDescription, descriptionNameSize);

				// And finally, write out if the achievement is unlocked or not!
				fout.write((char*)&m_vAchievements[i]->m_bIsAchieved, sizeof(bool));

			}
			// Let's close out the file.
			fout.close();
		}
		fout.clear();

		// Also printing out in text file for human readability
		fout.open("Resource/Achievements/Achievements.txt", ios_base::out);

		if (fout.is_open())
		{
			fout << m_vAchievements.size() << endl;
			for (unsigned int i = 0; i < MAX_ACHIEVEMENTS; ++i)
			{
				// Printing the size and name of the achievement.
				unsigned int achievementNameSize = m_vAchievements[i]->m_szAchievementName.size();
				fout << achievementNameSize << endl;
				fout << m_vAchievements[i]->m_szAchievementName << endl;

				// Printing the description.
				unsigned int descriptionSize = m_vAchievements[i]->m_szDescription.size();
				fout << descriptionSize << endl;
				fout << m_vAchievements[i]->m_szDescription << endl;

				// And writing the boolean.
				fout << m_vAchievements[i]->m_bIsAchieved << endl;
			}
			fout.close();
		}
}

void CAchievementManager::Shutdown()
{
	unsigned int arrSize = m_vAchievements.size();
	for (unsigned int i = 0; i < arrSize; ++i)
	{
		delete m_vAchievements[i];
		m_vAchievements[i] = nullptr;
	}
	m_vAchievements.clear();
}

void CAchievementManager::ResetAchievements()
{
	Shutdown();

	// Re-adding the achievements
	// No Life - Collect every Achievement.
	AddAchievement("No Life", "Collected every achievement. Congrats!");

	// Avoid and Evade - Dodge an Explosive Minion's explosion.
	AddAchievement("Avoid and Evade", "Dodge an Explosive Minion's explosion.");

	// Beast Tamer - Defeat 1 Fire Beast and 1 Ice Beast.
	AddAchievement("Beast Tamer", "Defeat 1 Fire Beast and 1 Ice Beast.");

	// Champion - Defeat the wizard atop the mountain.
	AddAchievement("Champion", "Defeat the wizard atop the mountain.");

	// Cool-headed - Complete a section without using Rage.
	AddAchievement("Cool-headed", "Complete a section without using Rage.");

	// These Are My Angry Eyes: Finish a section with full Rage
	AddAchievement("These Are My Angry Eyes", "Finish a section with full Rage.");

	// My secret is... -  Use Rage 5 times in a single playthrough.
	AddAchievement("My Secret Is...", "Use Rage 5 times in a single playthrough.");

	// Don't Stand in the Fire! - Die from a fire trap.
	AddAchievement("Don't Stand in the Fire!", "Die from a fire trap.");

	// Cold as Ice - Die from an ice trap.
	AddAchievement("Cold as Ice", "Die from an ice trap.");

	// Speedrunner - Run through a section without destroying a single Spawner.
	AddAchievement("Speedrunner", "Complete a section without destroying a Spawner.");

	// Ow, my face! - Use a fire or Ice Trap to kill a Beast.
	AddAchievement("Ow, My Face!", "Use a fire or Ice Trap to kill a Beast.");

	// Well, that escalated quickly - Have a Fire or Ice Beast reach maximum size.
	AddAchievement("Well, that Escalated Quickly", "Have a Fire or Ice Beast reach maximum size.");

	// Look at all this change... - Collect 750 pickups.
	AddAchievement("Look at All This Change...", "Collect 750 pickups.");

	// Rebounder - Deflect 10 ranged orbs.
	AddAchievement("Rebounder", "Deflect 10 ranged orbs successfully.");
}

int CAchievementManager::CheckForPlat()
{
	unsigned int arrSize = m_vAchievements.size();
	int acvCounter = 0;

	for (unsigned int i = 0; i < arrSize; ++i)
	{
		if (m_vAchievements[i]->m_bIsAchieved)
		{
			acvCounter += 1;
		}
	}

	if (acvCounter == arrSize-1)
		ActivateAchievement("No Life");

	return acvCounter;
}