#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Include the Windows API using Lean and Mean to exclude rarely
// used stuff from its headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <map>
using std::map;

#include <algorithm>

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ios_base;

// Used to print out stuff to the console window
#include <iostream>

#ifdef _DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

#include "Util\matrix3.h"
#include "Util\Matrix4x4.h"
#include "Util\vec3.h"
#include "Util\vec4.h"
#include "Util\Util.h"
#include "Util\CTimer.h"

// Enumerations
enum Sound {PLAY, PAUSE, STOP, RESUME};
enum Trap {T_IDLE = 1, T_RISE, T_DOWN = 4, T_SUMMONED = 8};
enum EMITTER_TYPE {TWO_D = 9000, THREE_D = 9001}; // DONT QUESTION MAH VALUES!! - JC
enum eAttackElemStates	{ICE = -1, NORMAL, FIRE};
enum eControllerCommands	{COMMAND_FIRE_ATTACK, COMMAND_ICE_ATTACK, COMMAND_RAGE, COMMAND_DASH};
enum eDifficultyLevel{DIFF_EASY, DIFF_MEDIUM, DIFF_HARD};
enum eFadeState{FADE_NOT, FADE_IN, FADE_COMPLETE, FADE_OUT};
enum eEmitterBehavior{EMIT_LOOP = 1, EMIT_STARTACTIVE = 2, EMIT_KILLONEND = 4, EMIT_SPRAY = 8, EMIT_TRACKING = 16};
enum eGameReturn { GAMERETURN_QUIT = 0, GAMERETURN_RUN = 1, GAMERETURN_RELOAD = 2 };

struct TImage
{
	int m_nImageID;
	int m_nImageWidth;
	int m_nImageHeight;
};

#define DAMAGE_COOLDOWN 0.3f
#define TIMER_COOLDOWN 2.5f

#define MAP_HEIGHT 2.0f

#define min(a,b)            (((a) < (b)) ? (a) : (b))	// return the minimum of these two values
#define max(a,b)            (((a) > (b)) ? (a) : (b))	// return the maxiumum of these two values
#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

// Gravity!
const vec3f EARTH_GRAVITY(0.0f, -9.80665f, 0.0f);

// These are the resolutions we developed on for making the menus look right
static const int g_nDevelopmentResolutionWidth	= 1680;
static const int g_nDevelopmentResolutionHeight	= 1050;

#endif