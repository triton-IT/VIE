#include <Windows.h>

#include <codecvt>

#include "../application.hpp"
#include "../logger.hpp"

#include "../boolean_parameter.hpp"
#include "../float_parameter.hpp"

#include "../modules/audio_input.hpp"
#include "../modules/audio_output.hpp"
#include "../modules/envelope.hpp"
#include "../modules/gain.hpp"
#include "../modules/high_pass.hpp"
#include "../modules/low_pass.hpp"
#include "../modules/midi_input.hpp"
#include "../modules/mixer.hpp"
#include "../modules/multiplier.hpp"
#include "../modules/noise.hpp"
#include "../modules/oscillator.hpp"
#include "../modules/recorder.hpp"
#include "../modules/sample.hpp"

using namespace live::tritone::vie;
using namespace live::tritone::vie::vst;
using namespace live::tritone::vie::processor::module;
using namespace live::tritone::vie::view::module;

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
		//FIXME: use wstring in parameters and find a way to parse wstring with json.
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

project_info& application::new_project()
{
	//reinit all properties
	clear_current_project();
	
	vie_processor_.initialize();
	vst_view_->initialize();
	
	//Generate empty project.
	//FIXME: Generate a UUID.
	current_project_.id = nb_projects_;
	//FIXME: Generate a random name.
	current_project_.name = L"Project " + std::to_wstring(nb_projects_);
	
	//Update list of projects.
	projects_infos_[nb_projects_] = current_project_;
	nb_projects_++;
	
	//Return generated project infos.
	return current_project_;
}

std::array<project_info, 32> application::get_projects(int* nb_projects)
{	
	*nb_projects = nb_projects_;
	return projects_infos_;
}

void application::save_project()
{
	std::wstringstream path_stream;
	path_stream << L"c:/tmp/" << current_project_.id << L".json";
	export_project(path_stream.str());
}

project_info application::load_project(uint_fast16_t id)
{	
	std::wstringstream path_stream;
	path_stream << L"c:/tmp/" << id << L".json";
	
	return import_project(path_stream.str());
}

void application::export_project(std::wstring project_path)
{
	nlohmann::json project_json = nlohmann::json();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	project_json["id"] = current_project_.id;
	project_json["name"] = converter.to_bytes(current_project_.name);
	project_json["description"] = converter.to_bytes(current_project_.description);

	nlohmann::json processor_json = vie_processor_.serialize();
	nlohmann::json view_json = vst_view_->serialize();

	nlohmann::json root = nlohmann::json();
	root["project"] = project_json;
	root["instrument"] = processor_json;
	root["view"] = view_json;

	std::ofstream projectStream(project_path);
	projectStream << root;
}

project_info application::import_project(std::wstring project_path)
{
	//reinit all properties
	clear_current_project();
	
	std::ifstream projectStream(project_path);
	nlohmann::json project_json;
	projectStream >> project_json;

	//Load instrument.
	auto instrument_json = project_json["instrument"];
	for (auto& [index, module] : instrument_json["modules"].items()) {
		import_module(module);
	}
	for (auto& [index, link] : instrument_json["links"].items())
	{
		bool enabled = link["enabled"];
		uint_fast8_t source_module_id = link["source_module_id"];
		uint16_t source_slot_id = link["source_slot_id"];
		uint_fast8_t target_module_id = link["target_module_id"];
		uint_fast16_t target_slot_id = link["target_slot_id"];
		
		link_modules(source_module_id, source_slot_id, target_module_id, target_slot_id);

		if (!enabled)
		{
			disable_modules_link(source_module_id, source_slot_id, target_module_id, target_slot_id);
		}
	}

	//Load view.
	auto view_json = project_json["view"];
	vst_view_->deserialize(view_json);
	
	//Load project infos.
	nlohmann::json project_info_json = project_json["project"];

	uint_fast16_t id = project_info_json["id"];
	std::string name = project_info_json["name"];
	std::string description = project_info_json["description"];

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	std::wstring name_w = converter.from_bytes(name);
	std::wstring description_w = converter.from_bytes(description);
	
	return project_info({ id, name_w, description_w });
}

std::array<module_view_descriptor, 64> application::get_available_modules(uint_fast8_t* nb_modules)
{
	*nb_modules = 13;
	return std::array<module_view_descriptor, 64> {
			audio_input_descriptor(),
			audio_output_descriptor(),
			envelope_descriptor(),
			gain_descriptor(),
			high_pass_descriptor(),
			low_pass_descriptor(),
			midi_input_descriptor(),
			mixer_descriptor(),
			multiplier_descriptor(),
			noise_descriptor(),
			oscillator_descriptor(),
			recorder_descriptor(),
			sample_descriptor()
		};
}

uint16_t application::add_module(nlohmann::json module)
{
	uint16_t module_id = vie_processor_.get_nb_processors();

	module["id"] = module_id;
	
	vie_processor_.add_processor(module);
	vst_view_->add_module(module);

	return module_id;
}

void application::delete_module(uint16_t module_id)
{
	vst_view_->delete_module(module_id);
	vie_processor_.delete_processor(module_id);
}

uint_fast8_t application::get_nb_modules()
{
	return vie_processor_.get_nb_processors();
}

uint16_t application::link_modules(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
{
	return vie_processor_.link_modules(source_module_id, source_slot_id, target_module_id, target_slot_id);
}

void application::unlink_modules(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
{
	vie_processor_.unlink_modules(source_module_id, source_slot_id, target_module_id, target_slot_id);
}

void application::enable_modules_link(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
{
	vie_processor_.enable_modules_link(source_module_id, source_slot_id, target_module_id, target_slot_id);
}

void application::disable_modules_link(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id)
{
	vie_processor_.disable_modules_link(source_module_id, source_slot_id, target_module_id, target_slot_id);
}

void application::move_module(uint_fast8_t module_id, std::array<uint_fast8_t, 3> position)
{
	//FIXME: Never use vst_view. Prefer vie_view.
	return vst_view_->move_module(module_id, position);
}

std::shared_ptr<processor_module> application::get_processor_by_id(uint_fast8_t id)
{
	return vie_processor_.get_processor(id);
}

vie_processor& application::get_vie_processor()
{
	return vie_processor_;
}

vst_view* application::create_view(host_callback* callback)
{
	vst_view_ = new vst_view(callback);
	return vst_view_;
}

vst_view* application::deleteView()
{
	delete vst_view_;
	vst_view_ = nullptr;
	return nullptr;
}

uint16_t application::import_module(nlohmann::json module)
{
	uint16_t module_id = vie_processor_.get_nb_processors();

	module["id"] = module_id;

	vie_processor_.add_processor(module);

	return module_id;
}

void application::clear_current_project()
{
	nb_parameters_ = 0;
	vie_processor_.clear();
	vst_view_->clear();
}

application application_;