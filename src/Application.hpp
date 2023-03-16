#pragma once

#include <string>
#include "logger.hpp"
#include "project.hpp"
#include "vie_processor.hpp"
#include "vst/vst_view.hpp"
#include "host_callback.hpp"

extern void* module_handle;
extern "C" std::wstring content_path;

#ifdef VIE_DEBUG
extern live::tritone::vie::debug::logger debugLogger;
#define DLOG(log) debugLogger.write(log)
#else
// ReSharper disable once IdentifierTypo
#define DLOG(log)
#endif // VIE_DEBUG

namespace live::tritone::vie {
	enum {
		bypass_id,
		frequency_id,
		wave_form_id
	};
	
	class application {
	public:
		project_info& new_project();
		std::array<project_info, 32> get_projects(int* nb_projects);
		void save_project();
		project_info load_project(uint_fast16_t id);
		void export_project(std::wstring project_path);
		project_info import_project(std::wstring project_path);
		void delete_project(std::string id);

		std::array<view::module::module_view_descriptor, 64> get_available_modules(uint_fast8_t* nb_modules);
		uint16_t add_module(nlohmann::json module);
		void delete_module(uint16_t module_id);
		uint_fast8_t get_nb_modules();
		uint16_t link_modules(nlohmann::json link);
		void move_module(uint_fast8_t module_id, std::array<uint_fast8_t, 3> position);

		std::shared_ptr<processor_module> get_processor_by_id(uint_fast8_t id);

		vie_processor& get_vie_processor();

		parameter add_parameter(uint_fast8_t id, parameter parameter);
		parameter add_parameter(uint_fast8_t id, std::string name, std::string short_name, std::string type, std::string unit, float value);

		parameter get_parameter(uint_fast8_t id);

		uint_fast8_t get_parameters_count();

		vst::vst_view* create_view(host_callback* callback);
		vst::vst_view* deleteView();

#ifdef UNIT_TESTING
	public:
#else
	private:
#endif
		/**
		* Unload current project.
		*/
		void clear_current_project();
		
		module_link& create_modules_link(nlohmann::json link_definition);
		uint16_t import_module(nlohmann::json module);
		
		std::array<parameter*, 255> parameters_;
		uint_fast8_t nb_parameters_;
		
		project_info current_project_;
		std::array<project_info, 32> projects_infos_;
		uint_fast16_t nb_projects_;

		vie_processor vie_processor_;
		vst::vst_view* vst_view_;
	};
}

extern live::tritone::vie::application application_;