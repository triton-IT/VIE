#include "application.h"

#include <Windows.h>

#include "parameter.h"

extern "C" {
	bool InitDll() {
		return true;
	}

	bool ExitDll() {
		return true;
	}

	std::wstring content_path;

#ifdef VIE_DEBUG
	live::tritone::vie::debug::logger debugLogger;
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

live::tritone::vie::core::IParameter& application::add_parameter(uint_fast8_t id, live::tritone::vie::core::IParameter* parameter)
{
	live::tritone::vie::core::IParameter* parameter_ptr = nullptr;
	//auto parameter_ptr = new live::tritone::vie::core::IParameter(parameter);

	//parameters_[nb_parameters_] = parameter_ptr;
	//nb_parameters_++;

	return *parameter_ptr;
}

live::tritone::vie::core::IParameter& application::add_parameter(uint_fast8_t id, Steinberg::char16 name[128], Steinberg::char16 short_name[128], Steinberg::char16 type[128], Steinberg::char16 unit[128], float value) {
	/*if (type == "boolean") {
		//FIXME: use wstring in parameters and fund a way to parse wstring with json.
		boolean_parameter parameter(id,
			std::wstring(name.begin(), name.end()).c_str(),
			std::wstring(short_name.begin(), short_name.end()).c_str(),
			std::wstring(unit.begin(), unit.end()).c_str(),
			value);
		return add_parameter(id, parameter);
	}
	else {
		float_parameter parameter(id,
			std::wstring(name.begin(), name.end()).c_str(),
			std::wstring(short_name.begin(), short_name.end()).c_str(),
			std::wstring(unit.begin(), unit.end()).c_str(),
			value);
		return add_parameter(id, parameter);
	}*/
	live::tritone::vie::core::IParameter* parameter_ptr = nullptr;
	return *parameter_ptr;
}

live::tritone::vie::core::IParameter& application::get_parameter(uint_fast8_t id)
{
	return *parameters_[id];
}

uint_fast8_t application::get_parameters_count()
{
	return nb_parameters_;
}

live::tritone::vie::core::IParameter* application::parameters_[255] = { nullptr };
uint_fast8_t application::nb_parameters_ = 0;