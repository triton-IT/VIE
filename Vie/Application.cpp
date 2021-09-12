#include <Windows.h>

#include "Application.h"
#include "Logger.h"

extern "C" {
	bool InitDll()
	{
		return true;
	}

	bool ExitDll()
	{
		return true;
	}

	void* moduleHandle;
	std::string contentPath;

#ifdef DEBUG
	live::tritone::vie::utils::Logger debugLogger;
#endif
} // extern "C"

void GetContentPath(CHAR (&_contentPath)[1024])
{
	HMODULE hPlugin;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPWSTR)&GetContentPath,
		&hPlugin);

	//Get full path including dll file.
	CHAR dllPath[1024];
	GetModuleFileNameA(hPlugin, dllPath, sizeof(dllPath) / sizeof(wchar_t));

	//Remove the dll file name.
	CHAR* dllFileName = strrchr(dllPath, '\\');
	strncpy_s(_contentPath, dllPath, strlen(dllPath) - strlen(dllFileName));
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD /*dwReason*/, LPVOID /*lpvReserved*/)
{
	moduleHandle = hInst;
	CHAR localContentPath[1024];
	GetContentPath(localContentPath);
	localContentPath[1023] = '\0';
	contentPath = std::string(localContentPath) + "\\";

#ifdef DEBUG
	debugLogger.open("debug.log", true);
#endif // DEBUG
	
	return TRUE;
}