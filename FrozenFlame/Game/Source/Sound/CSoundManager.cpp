/***********************************************
 * Filename:  		CSoundManager.cpp
 * Date:      		10/29/2012
 * Mod. Date: 		11/02/2012
 * Mod. Initials:	RJR
 * Author:    		Roberto J. Rubiano
 * Purpose:   		This is the wrapper for the Wwise API definitions
 ************************************************/

#include "CSoundManager.h"
#include "../Core/CWorldManager.h"

#if AK_ENABLE_PROFILING
#pragma comment(lib,"CommunicationCentral.lib") 
#include <AK/Comm/AkCommunication.h>	// Communication between Wwise designer and the game
#else
#define AK_OPTIMIZED
#endif //AK_USE_PROFILER

// load the libraries required, this alternatively could be done in the project properties 
// but having it defined in code makes it more portable and reusable.
#pragma comment(lib,"dxguid.lib")							//needed for 3d sounds
#pragma comment(lib,"Ws2_32.lib")							//wwise uses this for communication
#include <AK/SoundEngine/Common/AkSoundEngine.h>			// Sound Engine
#pragma comment(lib,"AkSoundEngine.lib")
#include <AK/MusicEngine/Common/AkMusicEngine.h>			// Music Engine
#pragma comment(lib,"AkMusicEngine.lib")
#include <AK/SoundEngine/Common/IAkStreamMgr.h>				// Streaming Manager
#pragma comment(lib,"AkStreamMgr.lib")
#include <AK/SoundEngine/Common/AkMemoryMgr.h>				// Memory Manager
#pragma comment(lib,"AkMemoryMgr.lib")
#include <AK/Plugin/AkVorbisFactory.h>						// Needed for the Ogg Vorbis codec plugin
#pragma comment(lib,"AkVorbisDecoder.lib")

#include <AK/Tools/Common/AkPlatformFuncs.h>				// Thread defines
#include "AKIncludes/AkFilePackageLowLevelIOBlocking.h"	    // Sample low-level I/O implementation
#include <AK/SoundEngine/Common/AkModule.h>					// Default memory and stream managers
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkDynamicSequence.h>
#include <AK/IBytes.h>

#define SOUNDS_FILE_PATH "Resource/Sound/"

// We're using the default Low-Level I/O implementation that's part
// of the SDK's sample code, with the file package extension
CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

namespace AK
{
#ifdef WIN32

	// This is the default implementation of the Memory Manager.
	void* AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}

	void FreeHook(void* in_ptr)
	{
		free(in_ptr);
	}

	// Note: VirtualAllocHook(() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void* VirtualAllocHook(void*  in_pMemAddress,size_t in_size,DWORD in_dwAllocationType,DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}

	void VirtualFreeHook(void* in_pMemAddress,size_t in_size,DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}

#endif
};

/******************************
* Function Definitions
*******************************/

/*******************************
* Private Functions
********************************/
int CSoundManager::sm_nNewestSoundID = 3;
CSoundManager::CSoundManager(void) : m_bIsInit(false), m_fMusicVolume(50), m_fSFXVolume(50), m_fMasterVolume(50)
{
	count = 10000;
}

CSoundManager::CSoundManager(const CSoundManager&)
{
}

CSoundManager::~CSoundManager(void)
{
	if(m_bIsInit == true)
	{
		Shutdown();
	}
}

void CSoundManager::ProcessAudio(void)
{
	// Process bank requests, events, positions, RTPC, etc.
	AKRESULT eResult = AK::SoundEngine::RenderAudio();
	assert(eResult == AK_Success);
}

void CSoundManager::UpdatePlayerListener(void)
{
	AK::SoundEngine::SetListenerPosition(listener,0);
	AK::SoundEngine::SetActiveListeners((AkGameObjectID)GAME_OBJECT_ID_PLAYER,0x00000001);
}

void CSoundManager::UpdatePlayerPos(void)
{
	m_playerPos.Orientation = listener.OrientationFront;
	m_playerPos.Position = listener.Position;
	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYER,m_playerPos);

	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYERDASH,m_playerPos);
	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYER_HURT1,m_playerPos);
	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYER_HURT2,m_playerPos);
}

void CSoundManager::UpdateGameObjects(void)
{
	unsigned int numObjs = m_vObjects.size();
	unsigned int numPos = m_vObjectPositions.size();

	if(numObjs > numPos || numObjs < numPos)
		return;

	for(unsigned int i = 0; i < numObjs; i++)
	{
		AK::SoundEngine::Query::GetPosition((m_vObjects[i]),(m_vObjectPositions[i]));
	}
}

/******************************
* Public Functions
*******************************/

bool CSoundManager::Init(void)
{
	if(m_bIsInit == false)
	{
		// Creating and initializing an instance of the default memory manager.
		// It is highly recommended you don't use the default settings for your game.
		AkMemSettings memSettings;
		memSettings.uMaxNumPools = 20;
		if(AK::MemoryMgr::Init(&memSettings) != AK_Success)
		{
			assert(!"Could not create the memory manager.");
			return false;
		}

		// Create and initialize an instance of the default streaming manager. Note
		// that you can override the dafault streaming manager with your own. Refer
		// to the SDK documentation for more information.
		// It is highly recommended you don't use the default settings for your game.
		AkStreamMgrSettings stmSettings;
		AK::StreamMgr::GetDefaultSettings(stmSettings);
		if(!AK::StreamMgr::Create(stmSettings))
		{
			assert(!"Could not create the Streaming Manager");
			return false;
		}

		// Create a streaming device with blocking low-level I/O handshaking.
		// Note that you can override the default low-level I/O module with your own. Refer
		// to the SDK documentation for more information.
		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
		// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
		// in the Stream Manager, and registers itself as the File Location Resolver.
		if(g_lowLevelIO.Init(deviceSettings) != AK_Success)
		{
			assert(!"Could not create the streaming device and Low-Level I/O system");
			return false;
		}

		// Create the Sound Engine
		// Using default initialization parameters
		AkInitSettings initSettings;
		AkPlatformInitSettings platformInitSettings;
		AK::SoundEngine::GetDefaultInitSettings(initSettings);
		AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

		if(AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
		{
			assert(!"Could not initialize the Sound Engine.");
			return false;
		}

		// Initialize the music engine
		// Using default initialization parameters
		// *****This must be done after the sound engine******
		AkMusicSettings musicInit;
		AK::MusicEngine::GetDefaultInitSettings(musicInit);
		musicInit.fStreamingLookAheadRatio = 100;
		if(AK::MusicEngine::Init(&musicInit) != AK_Success)
		{
			assert(!"Could not initialize the Music Engine.");
			return false;
		}

#ifndef AK_OPTIMIZED
		// Initialize communications
		// This is necessary to perform in-game mixing, profiling and troubleshooting.
		AkCommSettings commSettings;
		AK::Comm::GetDefaultInitSettings(commSettings);
		if(AK::Comm::Init(commSettings) != AK_Success)
		{
			assert(!"Could not initialize communication.");
			return false;
		}
#endif // AK_OPTIMIZED
		// After all initialization is completed we will register
		// any plugins that are needed for the game. In this example,
		// we will use the Ogg Vorbis codec plugin.
		AK::SoundEngine::RegisterCodec(AKCOMPANYID_AUDIOKINETIC,AKCODECID_VORBIS,CreateVorbisFilePlugin,
										CreateVorbisBankPlugin);

		// Setup banks path
		g_lowLevelIO.SetBasePath(DEFAULT_SOUNDBANK_PATH);

		AkBankID bankID;
		AKRESULT eResult;
		eResult = AK::SoundEngine::LoadBank("Init.bnk",AK_DEFAULT_POOL_ID,bankID);
		assert(eResult == AK_Success);

		m_bIsInit = true;

		LOG("Sound Manager Initialized\n");

		return true;
	}

	return false;
}

void CSoundManager::Update(void)
{
	UpdatePlayerPos();
	UpdateGameObjects();
	UpdatePlayerListener();
	ProcessAudio();

	static bool buffer = true;
	static bool isOn = true;
}

int CSoundManager::Loadbank(const char* pbankfile, const char* soundsfile)
{
	tBank item;

	int nSize;

	// set up default path for the sound file
	string soundpath = SOUNDS_FILE_PATH;
	soundpath += soundsfile;

	//read from file with each file name
	fstream in;
	in.open(soundpath,std::ios_base::in);

	if(in.is_open())
	{
		in >> nSize;
		in.ignore(INT_MAX,'\n');

		if(nSize == 0)
		{
			in.close();
			return -1;
		}

		for(int i = 0; i < nSize; i++)
		{
			string temp;
			in >> temp;
			in.ignore(INT_MAX,'\n');
			item.m_vSoundNames.push_back(temp);
		}
	}
	else
	{
		in.close();
		LOG("Unable to Find Sounds File");
		return -1;
	}

	in.close();
	
	item.m_sBankName = pbankfile;
	item.m_nNumObjs = nSize;
	
	AKRESULT eResult;
	eResult = AK::SoundEngine::LoadBank(item.m_sBankName.c_str(),AK_DEFAULT_POOL_ID,item.m_nID);
	assert(eResult == AK_Success);

	//loop for each sound
	for(int i = 0; i < item.m_nNumObjs; i++)
	{
		tSound Sound;
		Sound.m_AKID = count;
		Sound.m_sSoundName = item.m_vSoundNames[i];
		count += 100;
		eResult = AK::SoundEngine::RegisterGameObj(Sound.m_AKID, Sound.m_sSoundName.c_str());
		assert(eResult == AK_Success);
		Sound.m_sEventNames[PLAY]	= "Play_"	+ Sound.m_sSoundName;
		Sound.m_sEventNames[PAUSE]	= "Pause_"	+ Sound.m_sSoundName;
		Sound.m_sEventNames[STOP]	= "Stop_"	+ Sound.m_sSoundName;
		Sound.m_sEventNames[RESUME] = "Resume_" + Sound.m_sSoundName;
		item.m_mSoundz[Sound.m_sSoundName] = Sound;
	}

	m_mBanks[item.m_nID] = item;

	return item.m_nID;
}

void CSoundManager::Playsound(int bankid, const char* songname)
{
	AK::SoundEngine::PostEvent(m_mBanks[bankid].m_mSoundz[songname].m_sEventNames[PLAY].c_str(),
		m_mBanks[bankid].m_mSoundz[songname].m_AKID);
}

void CSoundManager::Playsound(AkUniqueID id, AkGameObjectID obj)
{
	AK::SoundEngine::PostEvent(id,obj);
}

void CSoundManager::StopSound(int bankid, const char* songname)
{
	AK::SoundEngine::PostEvent(m_mBanks[bankid].m_mSoundz[songname].m_sEventNames[STOP].c_str(),
		m_mBanks[bankid].m_mSoundz[songname].m_AKID);
}

void CSoundManager::StopSound(AkUniqueID id, AkGameObjectID obj)
{
	AK::SoundEngine::PostEvent(id,obj);
}

void CSoundManager::PauseSound(int bankid, const char* songname)
{
	AK::SoundEngine::PostEvent(m_mBanks[bankid].m_mSoundz[songname].m_sEventNames[PAUSE].c_str(),
		m_mBanks[bankid].m_mSoundz[songname].m_AKID);
}

void CSoundManager::ResumeSound(int bankid, const char* songname)
{
	AK::SoundEngine::PostEvent(m_mBanks[bankid].m_mSoundz[songname].m_sEventNames[RESUME].c_str(),
		m_mBanks[bankid].m_mSoundz[songname].m_AKID);
}

void CSoundManager::AdjustMusic(float nAmount)
{
	m_fMusicVolume = nAmount;

	AKRESULT eResult = AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::MX_VOLUME,m_fMusicVolume);
	
	ProcessAudio();
}

void CSoundManager::AdjustSFX(float nAmount)
{
	m_fSFXVolume = nAmount;

	AKRESULT eResult = AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::FX_VOLUME,m_fSFXVolume);
	eResult = AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::DX_VOLUME,m_fSFXVolume);

	ProcessAudio();
}

void CSoundManager::AdjustMaster(float nAmount)
{
	m_fMasterVolume = nAmount;

	AKRESULT eResult = AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::MASTER_VOLUME,m_fMasterVolume);

	ProcessAudio();
}

void CSoundManager::Unloadbank(int bankID)
{
	int nSize = m_mBanks[bankID].m_nNumObjs;

	//stop any sounds playing
	//unregister all sounds
	for(int i = 0; i < nSize; i++)
	{
		string debug = m_mBanks[bankID].m_vSoundNames[i];
		StopSound(bankID,debug.c_str());
		AK::SoundEngine::UnregisterGameObj(m_mBanks[bankID].m_mSoundz[debug].m_AKID);
		if(!m_mBanks[bankID].m_mSoundz.empty())
		{
			m_mBanks[bankID].m_mSoundz.erase(debug);
		}
	}

	// Unload Bank
	AK::SoundEngine::UnloadBank(m_mBanks[bankID].m_sBankName.c_str());
	m_mBanks.erase(bankID);

}

void CSoundManager::Shutdown(void)
{
	AK::SoundEngine::UnregisterAllGameObj();

	if(!m_mBanks.empty())
	{
		m_mBanks.clear();
	}

	if(!m_vObjects.empty())
	{
		m_vObjects.clear();
	}

	if(!m_vObjectPositions.empty())
	{
		m_vObjectPositions.clear();
	}

#ifndef AK_OPTIMIZED
	// Terminate Communication Services
	AK::Comm::Term();
#endif // AK_OPTIMIZED
	// Terminate the music engine
	AK::MusicEngine::Term();
	// Terminate the sound engine
	AK::SoundEngine::Term();
	// Terminate the streaming device and streaming manager
	// CAkFilePackageLowLevelIOBlocking::Term() destroys the associated streaming device
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();
	if(AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();
	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

	m_bIsInit = false;

	LOG("Sound Manager Shutdown\n");
}

void CSoundManager::StopAll(void)
{
	AK::SoundEngine::StopAll();
}

void CSoundManager::SetGameObjPos(AkGameObjectID gameObjectID, AkSoundPosition soundPosition)
{
	AK::SoundEngine::SetPosition(gameObjectID,soundPosition/*,AK_INVALID_LISTENER_INDEX*/);
}

void CSoundManager::SetListenerPosition(vec3f position)
{
	listener.Position.X = position.x;
	listener.Position.Y = position.y;
	listener.Position.Z = position.z;
}

void CSoundManager::SetListenerPosition(D3DVECTOR position)
{
	listener.Position.X = position.x;
	listener.Position.Y = position.y;
	listener.Position.Z = position.z;
}

void CSoundManager::SetListenerPosition(float x, float y, float z)
{
	listener.Position.X = x;
	listener.Position.Y = y;
	listener.Position.Z = z;
}

void CSoundManager::SetUpSoundObjects(void)
{
	memset(&listener, 0, sizeof(AkListenerPosition));
	memset(&m_playerPos, 0, sizeof(AkSoundPosition));
	memset(&m_globalPos, 0, sizeof(AkSoundPosition));

	//listener position/orientation setup
	listener.OrientationTop.Y = 1.0;
	listener.OrientationTop.X = listener.OrientationTop.Z = 0.0;
	listener.OrientationFront.Z = -1.0;
	listener.OrientationFront.X = listener.OrientationFront.Y = 0.0;

	UpdatePlayerListener();

	RegisterObject((AkGameObjectID)GAME_OBJECT_ID_PLAYER);
	RegisterObject((AkGameObjectID)GAME_OBJECT_ID_PLAYERDASH);
	RegisterObject((AkGameObjectID)GAME_OBJECT_ID_PLAYER_HURT1);
	RegisterObject((AkGameObjectID)GAME_OBJECT_ID_PLAYER_HURT2);

	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYERDASH,m_playerPos);
	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYER_HURT1,m_playerPos);
	SetGameObjPos((AkGameObjectID)GAME_OBJECT_ID_PLAYER_HURT2,m_playerPos);
}

void CSoundManager::SetObjectWithSoundPos(AkGameObjectID soundID, matrix4f matrix)
{
	vector<AkGameObjectID>::iterator iter = m_vObjects.begin();

	int count = 0;

	for(iter; iter != m_vObjects.end(); iter++)
	{
		if((*iter) == soundID)
		{
			m_vObjectPositions[count].Position.X = matrix.axis_pos.x;
			m_vObjectPositions[count].Position.Y = matrix.axis_pos.y;
			m_vObjectPositions[count].Position.Z = matrix.axis_pos.z;
			m_vObjectPositions[count].Orientation.X = matrix.axis_z.x;
			m_vObjectPositions[count].Orientation.Y = matrix.axis_z.y;
			m_vObjectPositions[count].Orientation.Z = matrix.axis_z.z;

			AK::SoundEngine::SetPosition(soundID,m_vObjectPositions[count]);
			break;
		}

		count++;
	}
}

void CSoundManager::SetListenerOrientation(vec3f position)
{
	listener.OrientationTop.Y = position.y;
	listener.OrientationTop.X = listener.OrientationTop.Z = 0.0f;

	listener.OrientationFront.Y = listener.OrientationFront.X = 0.0f;
	listener.OrientationFront.Z = position.z;
}

void CSoundManager::SetListenerOrientation(D3DVECTOR position)
{
	listener.OrientationTop.Y = position.y;
	listener.OrientationTop.X = listener.OrientationTop.Z = 0.0f;

	listener.OrientationFront.Y = listener.OrientationFront.X = 0.0f;
	listener.OrientationFront.Z = position.z;
}

void CSoundManager::SetListenerOrientation(float x, float y, float z)
{
	listener.OrientationTop.Y = y;
	listener.OrientationTop.X = listener.OrientationTop.Z = 0.0f;

	listener.OrientationFront.Y = listener.OrientationFront.X = 0.0f;
	listener.OrientationFront.Z = z;
}

void CSoundManager::RegisterObject(AkGameObjectID id)
{
	if(m_bIsInit == true)
	{
		AKRESULT eResult = AK::SoundEngine::RegisterGameObj(id);
		assert(eResult == AK_Success);
	}
}

void CSoundManager::AddObject(AkGameObjectID id)
{
	m_vObjects.push_back(id);
}

void CSoundManager::AddObjPos(AkSoundPosition pos)
{
	m_vObjectPositions.push_back(pos);
}

void CSoundManager::RemoveObject(AkGameObjectID id)
{
	if(id && m_vObjects.size() > 0)
	{
		vector<AkGameObjectID>::iterator iter = m_vObjects.begin();
		int count = 0;

		for(iter; iter != m_vObjects.end(); iter++)
		{
			if((*iter) == id)
			{
				break;
			}

			count++;
		}

		if((unsigned int)count < m_vObjects.size())
			m_vObjects.erase(m_vObjects.begin() + count);
	}
}

void CSoundManager::RemoveObjPos(AkSoundPosition pos)
{
	if(m_vObjectPositions.size() > 0)
	{
		vector<AkSoundPosition>::iterator iter = m_vObjectPositions.begin();
		int count = 0;

		for(iter; iter != m_vObjectPositions.end(); iter++)
		{
			if((*iter).Position.X == pos.Position.X && (*iter).Position.Y == pos.Position.Y &&
				(*iter).Position.Z == pos.Position.Z)
			{
				break;
			}

			count++;
		}

		if((unsigned int)count < m_vObjectPositions.size())
			m_vObjectPositions.erase(m_vObjectPositions.begin() + count);
	}
}

void CSoundManager::RemoveAllPositionalObjects()
{
	size_t vectorSize = m_vObjectPositions.size();
	for (int index = (int)vectorSize - 1; index > 0; index--)
	{
		m_vObjectPositions.erase(m_vObjectPositions.begin() + index);
	}
	m_vObjectPositions.clear();
}