/***********************************************
 * Filename:  		CSoundManager.h
 * Date:      		10/29/2012
 * Mod. Date: 		11/02/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		This is the wrapper for the Wwise API
 ************************************************/

#ifndef CSOUNDMANAGER_H_
#define CSOUNDMANAGER_H_

// will setup Wwise with the information needed to link with the profiler, 
// if false will use the optimized version of wwise
#define AK_ENABLE_PROFILING 0 
#define DEFAULT_SOUNDBANK_PATH L"Resource/Sound/Soundbanks/"

#include <AK/SoundEngine/Common/AkTypes.h>

#include "../Globals.h"
#include "Wwise_IDs.h"

enum ePlayerObjectID {GAME_OBJECT_ID_PLAYER = 5000, GAME_OBJECT_ID_PLAYERDASH, GAME_OBJECT_ID_PLAYER_HURT1,
						GAME_OBJECT_ID_PLAYER_HURT2};	

class CSoundManager
{
private:

	struct tSound
	{
		AkGameObjectID m_AKID;
		string m_sSoundName;
		string m_sEventNames[4];
	};

	struct tBank
	{
		int m_nNumObjs;
		AkBankID m_nID;
		string m_sBankName;
		vector<string> m_vSoundNames;
		map<string,tSound> m_mSoundz;
	};

	bool m_bIsInit;
	int count;
	AkListenerPosition	listener;
	AkSoundPosition m_playerPos;
	AkSoundPosition m_globalPos;
	AkRtpcValue m_fMasterVolume;
	AkRtpcValue m_fMusicVolume;
	AkRtpcValue m_fSFXVolume;

	vector<AkGameObjectID> m_vObjects;
	vector<AkSoundPosition> m_vObjectPositions;

	map<AkBankID,tBank> m_mBanks;
	
	static int sm_nNewestSoundID;

	/**************************************************************
	* CSoundManager(void)	:	The default constructor
	* Ins					:	None.
	* Outs					:	None.
	* Returns				:	None.
	* Mod. Date				:	10/29/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	CSoundManager(void);
	
	/**************************************************************
	* CSoundManager(const CSoundManager&)	:	The Copy Constructor
	* Ins									:	None.
	* Outs									:	None.
	* Returns								:	None.
	* Mod. Date								:	10/29/2012
	* Mod. Initals							:	RJR
	***************************************************************/
	CSoundManager(const CSoundManager&);

	/**************************************************************
	* CSoundManager& operator=(const CSoundManager&)	: The Assingment Operator
	* Ins												: const CSoundmanager&
	* Outs												: None.
	* Returns											: CSoundManager&
	* Mod. Date											: 10/29/2012
	* Mod. Initals										: RJR
	***************************************************************/
	CSoundManager& operator=(const CSoundManager&);

	/**************************************************************
	* ~CSoundManager(void)	:	The Destructor of the class.
	* Ins					:	None.
	* Outs					:	None.
	* Returns				:	None.
	* Mod. Date				:	10/29/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	~CSoundManager(void);

	/**************************************************************
	* void ProcessAudio(void)	:	Processes bank requests, events, positions, RTPC, etc.
	* Ins						:	None.
	* Outs						:	None.
	* Returns					:	void
	* Mod. Date					:	10/31/2012
	* Mod. Initals				:	RJR
	***************************************************************/
	void ProcessAudio(void);

	/**************************************************************
	* void UpdatePlayerListener(void)	:	Updates the player's listener
	* Ins								:	None.
	* Outs								:	None.
	* Returns							:	void
	* Mod. Date							:	1/21/2013
	* Mod. Initals						:	RJR
	***************************************************************/
	void UpdatePlayerListener(void);

	/**************************************************************
	* void ProcessAudio(void)	:	Updates the player's postion based off the listener
	* Ins						:	None.
	* Outs						:	None.
	* Returns					:	void
	* Mod. Date					:	1/21/2013
	* Mod. Initals				:	RJR
	***************************************************************/
	void UpdatePlayerPos(void);

	void UpdateGameObjects(void);

public:

	/**************************************************************
	* static CSoundManager* GetInstance(void)	:	Makes an instance of the class.
	* Ins										:	None.
	* Outs										:	None.
	* Returns									:	CSoundManager*
	* Mod. Date									:	10/29/2012
	* Mod. Initals								:	RJR
	***************************************************************/
	static CSoundManager* GetInstance(void)
	{
		static CSoundManager instance;
		return &instance;
	}

	/**************************************************************
	* bool Init(void)	:	Initializes the Wwise API.
	* Ins				:	None.
	* Outs				:   None.
	* Returns			:	bool.
	* Mod. Date			:	10/29/2012
	* Mod. Initials		:	RJR
	***************************************************************/
	bool Init(void);

	/**************************************************************
	* void Update(void)	:	calls the ProcessAudio function
	* Ins				:	None.
	* Outs				:	None.
	* Returns			:	void
	* Mod. Date			:	10/31/2012
	* Mod. Initals		:	RJR
	***************************************************************/
	void Update(void);

	/**************************************************************
	* int LoadBank(const char* pfilename, int size)	:	Loads the specified bank and all sounds.
	* Ins											:	pbankfile, soundsfile
	* Outs											:   None.
	* Returns										:	int
	* Mod. Date										:	10/29/2012
	* Mod. Initials									:	RJR
	***************************************************************/
	int Loadbank(const char* pbankfile, const char* soundsfile);

	/**************************************************************
	* void PlaySound(int nID)	:	Plays the specified sound.
	* Ins						:	bankid, songname
	* Outs						:   None.
	* Returns					:	void
	* Mod. Date					:	10/29/2012
	* Mod. Initials				:	RJR
	***************************************************************/
	void Playsound(int bankid, const char* songname);
	void Playsound(AkUniqueID id, AkGameObjectID obj);

	/**************************************************************
	* void StopSound(int nID)	:	Stops the specified sound.
	* Ins						:	bankid, songname 
	* Outs						:	None.
	* Returns					:	void
	* Mod. Date					:	10/29/2012
	* Mod. Initals				:	RJR
	***************************************************************/
	void StopSound(int bankid, const char* songname);
	void StopSound(AkUniqueID id, AkGameObjectID obj);

	/**************************************************************
	* void PauseSound(int nID)	:	Pauses the specified sound.	
	* Ins						:	bankid, songname
	* Outs						:	None.
	* Returns					:	void
	* Mod. Date					:	10/29/2012
	* Mod. Initals				:	RJR
	***************************************************************/
	void PauseSound(int bankid, const char* songname);

	/**************************************************************
	* void ResumeSound(int bankid, const char* songname)	:	Resumes the specified sound
	* Ins													:	bankid, songname
	* Outs													:	None.
	* Returns												:	void
	* Mod. Date												:	10/31/2012
	* Mod. Initals											:	RJR
	***************************************************************/
	void ResumeSound(int bankid, const char* songname);

	/**************************************************************
	* void AdjustMusic(int nAmount)	:	Adjusts the volume of the Music.
	* Ins							:	nAmount
	* Outs							:	None.
	* Returns						:	void
	* Mod. Date						:	10/29/12
	* Mod. Initals					:	RJR
	***************************************************************/
	void AdjustMusic(float nAmount);

	/**************************************************************
	* void AdjustSFX(int nAmount)	:	Adjusts the volume of the Sound Effects.
	* Ins							:	nAmount
	* Outs							:	None
	* Returns						:	void
	* Mod. Date						:	10/29/2012
	* Mod. Initals					:	RJR
	***************************************************************/
	void AdjustSFX(float nAmount);
	
	/**************************************************************
	* void AdjustSFX(int nAmount)	:	Adjusts the master volume
	* Ins							:	nAmount
	* Outs							:	None
	* Returns						:	void
	* Mod. Date						:	10/29/2012
	* Mod. Initals					:	RJR
	***************************************************************/
	void AdjustMaster(float nAmount);

	/**************************************************************
	* void UnloadBank(int bankID)	: Unloads the specifed bank and all sounds.
	* Ins							: bankID
	* Outs							: None.
	* Returns						: void	
	* Mod. Date						: 10/29/2012
	* Mod. Initals					: RJR
	***************************************************************/
	void Unloadbank(int bankID);

	/**************************************************************
	* void Shutdown(void)	:	Shutdowns the Wwise API.
	* Ins					:	None.
	* Outs					:	None.
	* Returns				:	void
	* Mod. Date				:	10/29/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	void Shutdown(void);

	/**************************************************************
	* void StopAll(void)	:	Stops all sounds playing currently
	* Ins					:	None.
	* Outs					:	None.
	* Returns				:	void
	* Mod. Date				:	10/29/2012
	* Mod. Initals			:	RJR
	***************************************************************/
	void StopAll(void);

	void RegisterObject(AkGameObjectID id);

	void AddObject(AkGameObjectID id);
	void AddObjPos(AkSoundPosition pos);

	void RemoveObject(AkGameObjectID id);
	void RemoveObjPos(AkSoundPosition pos);

	/*********************************************
	*
	*					Mutators
	*
	**********************************************/

	void SetGameObjPos(AkGameObjectID gameObjectID, AkSoundPosition soundPosition);
	void SetListenerPosition(vec3f position);
	void SetListenerPosition(D3DVECTOR position);
	void SetListenerPosition(float x, float y, float z);

	void SetListenerOrientation(vec3f position);
	void SetListenerOrientation(D3DVECTOR position);
	void SetListenerOrientation(float x, float y, float z);

	void SetUpSoundObjects(void);

	void SetObjectWithSoundPos(AkGameObjectID soundID, matrix4f matrix);

	// gets rid of all the objects in the sound manager's m_vObjectPositions vector
	void RemoveAllPositionalObjects();
	/*********************************************
	*
	*					Accessors
	*
	**********************************************/
	//vector<AkGameObjectID> GetObjects(void) {return m_vObjects;}
	//vector<AkSoundPosition> GetObjPositions(void) {return m_vObjectPositions;}
	bool GetInit(void)						{return m_bIsInit;}
	int  GetNewestSoundID() const			{ return sm_nNewestSoundID++; }
	/**************************************************************
	* tBank GetBank(AkBankID id)	:	returns a specific bank in the map	
	* Ins							:	id
	* Outs							:	None.
	* Returns						:	tBank
	* Mod. Date						:	10/31/2012
	* Mod. Initals					:	RJR
	***************************************************************/
	tBank GetBank(AkBankID id)
	{
		return m_mBanks[id];
	}
};

#endif