#include <Windows.h>

#include <codecvt>

#include "../application.hpp"
#include "../logger.hpp"

#include "../boolean_parameter.hpp"
#include "../float_parameter.hpp"

using namespace live::tritone::vie;

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

parameter application::add_parameter(uint_fast8_t id, parameter parameter)
{
	auto parameter_ptr = new live::tritone::vie::parameter(parameter);

	parameters_[nb_parameters_] = parameter_ptr;
	nb_parameters_++;

	return *parameter_ptr;
}

parameter application::add_parameter(uint_fast8_t id, std::string name, std::string short_name, std::string type, std::string unit, float value) {
	if (type == "boolean") {
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
	}
}

parameter application::get_parameter(uint_fast8_t id)
{
	return *parameters_[id];
}

uint_fast8_t application::get_parameters_count()
{
	return nb_parameters_;
}

project& application::new_project()
{
	//FIXME: Generate a UUID.
	projects_[nb_projects_] = project{ std::to_wstring(nb_projects_), L"Project " + std::to_wstring(nb_projects_) };
	nb_projects_++;
	return projects_[nb_projects_ - 1];
}

std::array<project, 32> application::get_projects(int* nb_projects)
{
	*nb_projects = nb_projects_;
	return projects_;
}

project application::load_project(std::wstring project_path)
{
	std::ifstream projectStream(project_path);
	nlohmann::json project_json;
	projectStream >> project_json;

	vie_processor_.load(project_json);
	vie_view_->load(project_json);

	nlohmann::json project_info = project_json["project"];
	
	std::string id = project_info["id"];
	std::string name = project_info["name"];
	std::string description = project_info["description"];

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	std::wstring id_w = converter.from_bytes(id);
	std::wstring name_w = converter.from_bytes(name);
	std::wstring description_w = converter.from_bytes(description);
	
	return project({ id_w, name_w, description_w });
}

vie_processor& application::get_vie_processor()
{
	return vie_processor_;
}

vie_view* application::create_view(host_callback* callback)
{
	vie_view_ = new vie_view(callback);
	return vie_view_;
}

vie_view* application::deleteView()
{
	delete vie_view_;
	vie_view_ = nullptr;
	return nullptr;
}

application application_;