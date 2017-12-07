////////////////////////////////////////////////////////////////////////////
//	WinMain.cpp
//		Entry point of the program - defines the window class, creates the
//		window and handles the Windows message loop
//
//	Author:					Daniel Lima - March 2012
//	Partially based on:		David Brown's WinMain from Full Sail University
//	Special thanks:			Doug Monroe - for introducing me to C++
//							Robert Muscarella - for introducing me to the
//							basics of programming with the Windows API
/////////////////////////////////////////////////////////////////////////////

// Regular includes
#include "Globals.h"

// Needed for dump files
#include <stdio.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp")

// Include our CGame class
#include "CGame.h"
#include <ctime>

// If the project is being built on Debug mode, this will set the program's
// entry point to Console "main" instead of Windows "WinMain", so there will
// be a console window to help debug stuff.
// TODO: There are simpler ways to create a console window - We should probably change this
#ifdef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:Console")
#include <vld_def.h>
#include <vld.h>
#else
#pragma comment(linker, "/SUBSYSTEM:Windows")
#endif

// Globals for window properties
LPSTR		g_szWINDOW_CLASS	= "WindowClass";
LPSTR		g_szWINDOW_TITLE	= "Frozen Flame";

// Functions to register and create the window
void ReadInitialVideoOptions(const char* szFile, unsigned& nOutWindowWidth, unsigned& nOutWindowHeight, bool& bOutFullScreen);
ATOM WINAPI RegisterWindow(HINSTANCE hInstance);
HWND MakeWindow(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// The function that gets called when there is an unhandled error
LONG WINAPI errorFunc(_EXCEPTION_POINTERS *pExceptionInfo)
{
	HANDLE hFile = ::CreateFile("dumpfile.mdmp", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL );

	if (hFile!=INVALID_HANDLE_VALUE)
	{
	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;
		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
		//MessageBox("Dump File Saved look x directory please email to developer at the following email adress crashdmp@gmail.com with the subject Gamename - Version ");
		::CloseHandle(hFile);
	}

	return 0;
} 

// Console "main" - only called in Debug Mode
//		Calls Windows' WinMain
int main(int argc, char* argv[])
{
	srand(time_t(0));
	std::cout << "Running " << GetCommandLine() << " in Debug Mode\n\n";
	std::cout << "Debug Log:\n------------\n\n";
	std::cout << "``_```_``````_`_````````__````````__`````````_`````_`\n"
				<< "`|`|`|`|`___|`|`|`___```\\`\\``````/`/__``_`__|`|`__|`|\n"
				<< "`|`|_|`|/`_`\\`|`|/`_`\\```\\`\\`/\\`/`/`_`\\|`'__|`|/`_``|\n"
				<< "`|``_``|``__/`|`|`(_)`|```\\`V``V`/`(_)`|`|``|`|`(_|`|\n"
				<< "`|_|`|_|\\___|_|_|\\___/`````\\_/\\_/`\\___/|_|``|_|\\__,_|\n"
				<< "`````````````````````````````````````````````````````\n";

	WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWDEFAULT);

	LOG("Main finished.\n");
	return 0;
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//set up the function to create the dump file
	SetUnhandledExceptionFilter(errorFunc);

	// Will be used to test if the functions succeeded
	HRESULT hResult = false;
	HWND hWnd = nullptr;

	// Register and create our window
	hResult = RegisterWindow(hInstance);
	if(!hResult) return 0;
	LOG("Window class registered.");

	hWnd = MakeWindow(hInstance);
	if(!hWnd) return 0;
	LOG("Window created.");

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Main message loop
	CGame* pGame = CGame::GetInstance();
	MSG Message = {};
	eGameReturn eGameState;
	while(hWnd)
	{
		if(PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if(Message.message == WM_QUIT) break;

			// Peek message and dispatch it to the message proc
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		// Run game
		eGameState = pGame->Main();
		switch(eGameState)
		{
		case GAMERETURN_QUIT:
			PostQuitMessage(0);
			break;
		case GAMERETURN_RELOAD:
			pGame->Shutdown();
			DestroyWindow(hWnd);
			hWnd = MakeWindow(hInstance);
			if(hWnd)
			{
				ShowWindow(hWnd, SW_SHOW);
				UpdateWindow(hWnd);
			}
			break;
		}
	}

	// Exit the game - we'll have to change this later
	pGame->Shutdown();
	LOG("Game finished.");

	// Unregister window class and return
	UnregisterClass(g_szWINDOW_CLASS, hInstance);
	return (int)Message.wParam;
}

ATOM WINAPI RegisterWindow(HINSTANCE hInstance)
{
	WNDCLASSEX WindowClass;

	WindowClass.cbSize			= sizeof(WindowClass);
	WindowClass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc		= WindowProc;
	WindowClass.cbClsExtra		= 0;
	WindowClass.cbWndExtra		= 0;
	WindowClass.hInstance		= hInstance;
	WindowClass.hIcon			= LoadIcon(hInstance, nullptr);
	WindowClass.hIconSm			= LoadIcon(hInstance, nullptr);
	WindowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	WindowClass.lpszMenuName	= nullptr; 
	WindowClass.lpszClassName	= g_szWINDOW_CLASS;

	return RegisterClassEx(&WindowClass);
}

#include "States\CVideoOptionsState.h"

HWND MakeWindow(HINSTANCE hInstance)
{
	unsigned nWindowWidth, nWindowHeight;
	bool bFullScreen;
	ReadInitialVideoOptions("Resource/Options/Video.txt", nWindowWidth, nWindowHeight, bFullScreen);

	DWORD dwWindowStyleFlags = WS_VISIBLE;
	dwWindowStyleFlags |= (bFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);

	ShowCursor(FALSE);

	RECT rClientArea  = { 0, 0, nWindowWidth, nWindowHeight };
	AdjustWindowRectEx(&rClientArea, dwWindowStyleFlags, false, WS_EX_APPWINDOW);

	int nClientWidth	= rClientArea.right - rClientArea.left;
	int nClientHeight	= rClientArea.bottom - rClientArea.top;

	HWND hWnd = CreateWindowEx(0, g_szWINDOW_CLASS, g_szWINDOW_TITLE, dwWindowStyleFlags,										
		(GetSystemMetrics(SM_CXSCREEN)/2) - (nClientWidth/2),
		(GetSystemMetrics(SM_CYSCREEN)/2) - (nClientHeight/2),
		nClientWidth, nClientHeight, HWND_DESKTOP, nullptr, hInstance, nullptr);

	if(hWnd)
	{
		// Initialize the game - we'll have to change this later
		CGame* pGame = CGame::GetInstance();
		pGame->Init(hWnd, hInstance, nWindowWidth, nWindowHeight, bFullScreen);
		LOG("Game initialized.");
	}

	return hWnd;
}


LRESULT CALLBACK WindowProc(HWND hWnd, unsigned int uiMessage, WPARAM wParam, LPARAM lParam)
{
	switch(uiMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);			
		break;

	case WM_ACTIVATE:
		//	gaining focus
		if (LOWORD(wParam) != WA_INACTIVE)
		{
			// unpause game code here

			CGame::GetInstance()->GainFocus();
		}
		else // losing focus
		{
			// pause game code here

			CGame::GetInstance()->LoseFocus();
		}
		break;

	case WM_CREATE: 
		//	Do initialization here
		break;

	case WM_DESTROY: 
		//	Kill the application
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}

	return 0;
}

// This function will take care of reading the video options that need to be set before the game is
// initialized. Such things as the window resolution and if it's full screen or not.
void ReadInitialVideoOptions(const char* szFile, unsigned& nOutWindowWidth, unsigned& nOutWindowHeight, bool& bOutFullScreen)
{
	fstream in;
	unsigned char ucFlags = 0;
	in.open(szFile, ios_base::in | ios_base::binary);
	if(in.is_open())
	{
		in.read((char*)&ucFlags, sizeof(unsigned char));
		in.close();
	}

	bOutFullScreen = bool(ucFlags & RF_FULLSCREEN);

	if(ucFlags & RF_RES_640x480)
	{
		nOutWindowWidth = 640;
		nOutWindowHeight = 480;
	}
	else if(ucFlags & RF_RES_1024x768)
	{
		nOutWindowWidth = 1024;
		nOutWindowHeight = 768;
	}
	else if(ucFlags & RF_RES_1280x720)
	{
		nOutWindowWidth = 1280;
		nOutWindowHeight = 720;
	}
	else if(ucFlags & RF_RES_1680x1050)
	{
		nOutWindowWidth = 1680;
		nOutWindowHeight = 1050;
	}
	else if(ucFlags & RF_RES_1920x1080)
	{
		nOutWindowWidth = 1920;
		nOutWindowHeight = 1080;
	}
	else
	{
#ifdef _DEBUG
		nOutWindowWidth = 1024;
		nOutWindowHeight = 768;
#else
		nOutWindowWidth = GetSystemMetrics(SM_CXSCREEN);
		nOutWindowHeight = GetSystemMetrics(SM_CYSCREEN);
#endif
	}
}