#include "StdAfx.h"
#include "System/CrashReport.h"
//#include "Core.h"
//#include "GameOGL.h"

/*
* Start of Application
*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	char szGameName[] = { "Name of the Game" };

	// Prevent multiple program starts
	HANDLE hMutex;
	//CoInitialize(NULL);
	hMutex = CreateMutex(NULL, FALSE, szGameName);

	if(hMutex == NULL || (GetLastError() == ERROR_ALREADY_EXISTS) )
	{
		CloseHandle(hMutex);
		MessageBox(NULL, "Application has already been\nstarted in a different window.",
			"Application already running",
			MB_ICONWARNING | MB_OK);

		return 1;
	}

	// Game Code

	int iReturn = 0;

	// Error handler
	CrashReport errorLog("Log.txt");

	// Set the window parameters to use
	WindowParameters WndParam(szGameName, hInst);
	WndParam.iWidth = 800;
	WndParam.iHeight = 600;
	WndParam.iRefreshRate = 60;
	WndParam.b32bit = true;
	WndParam.bWindowed = true;

	// Main game window
	GameOGL myGame(&errorHandler);

	// Unique game start
	if(myGame.Create(&WndParam))
	{
		iReturn = myGame.StartMessageLoop();
	}
	else
	{
		errorLog.ShowErrorMessage();
		iReturn = -1;
	}

	// Close this unique program to allow restart again
	CloseHandle(hMutex);
	//CoUninitialize();

	return iReturn;
}