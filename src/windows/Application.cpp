#include <Windows.h>

#include "../application.hpp"
#include "../logger.hpp"

extern "C" {
	bool InitDll() {
		return true;
	}

	bool ExitDll() {
		return true;
	}

	std::wstring content_path;

#ifdef VIE_DEBUG
	live::tritone::vie::utils::logger debugLogger;
#endif //VIE_DEBUG
} // extern "C"

void get_content_path(wchar_t(&local_content_path)[1024]) {
	HMODULE h_plugin;
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&get_content_path),
		&h_plugin);

	//Get full path including dll file.
	wchar_t dll_path[1024];
	GetModuleFileNameW(h_plugin, dll_path, sizeof(dll_path) / sizeof(wchar_t));

	//Remove the dll file name.
	const wchar_t* dll_file_name = wcsrchr(dll_path, '\\');
	wcsncpy_s(local_content_path, dll_path, wcslen(dll_path) - wcslen(dll_file_name));
}

BOOL WINAPI DllMain(HINSTANCE /*h_instance*/, DWORD /*dw_reason*/, LPVOID /*lpv_reserved*/) {
	wchar_t local_content_path[1024];
	get_content_path(local_content_path);
	local_content_path[1023] = '\0';
	content_path = std::wstring(local_content_path) + L"\\";

#ifdef VIE_DEBUG
	debugLogger.open("debug.log", true);
#endif // VIE_DEBUG

	return TRUE;
}

parameters application::parameters_;