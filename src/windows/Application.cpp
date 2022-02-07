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

	std::string content_path;

#ifdef VIE_DEBUG
	live::tritone::vie::utils::Logger debugLogger;
#endif //VIE_DEBUG
} // extern "C"

void get_content_path(CHAR(&local_content_path)[1024]) {
	HMODULE h_plugin;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCSTR>(&get_content_path),
		&h_plugin);

	//Get full path including dll file.
	char dll_path[1024];
	GetModuleFileNameA(h_plugin, dll_path, sizeof(dll_path) / sizeof(wchar_t));

	//Remove the dll file name.
	const char* dll_file_name = strrchr(dll_path, '\\');
	strncpy_s(local_content_path, dll_path, strlen(dll_path) - strlen(dll_file_name));
}

BOOL WINAPI DllMain(HINSTANCE /*h_instance*/, DWORD /*dw_reason*/, LPVOID /*lpv_reserved*/) {
	CHAR local_content_path[1024];
	get_content_path(local_content_path);
	local_content_path[1023] = '\0';
	content_path = std::string(local_content_path) + "\\";

#ifdef VIE_DEBUG
	debugLogger.open("debug.log", true);
#endif // VIE_DEBUG

	return TRUE;
}
