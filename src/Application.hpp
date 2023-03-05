#pragma once

#include <string>
#include "logger.hpp"
#include "project.hpp"
#include "vie_processor.hpp"
#include "vie_view.hpp"
#include "host_callback.hpp"
#include "modules/module_descriptor.hpp"

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
#ifdef UNIT_TESTING
		void clear();
#endif
		project_info& new_project();
		std::array<project_info, 32> get_projects(int* nb_projects);
		void save_project();
		project_info load_project(std::wstring id);
		void export_project(std::wstring project_path);
		project_info import_project(std::wstring project_path);
		void delete_project(std::string id);

		std::array<processor::module::module_descriptor, 64> get_modules();
		uint16_t add_module(nlohmann::json module);
		uint16_t add_relation(nlohmann::json relation);

		processor_module& get_processor_by_id(uint_fast8_t id);
		module_relation& get_relation_by_id(uint_fast8_t id);

		vie_processor& get_vie_processor();

		parameter add_parameter(uint_fast8_t id, parameter parameter);
		parameter add_parameter(uint_fast8_t id, std::string name, std::string short_name, std::string type, std::string unit, float value);

		parameter get_parameter(uint_fast8_t id);

		uint_fast8_t get_parameters_count();

		vie_view* create_view(host_callback* callback);
		vie_view* deleteView();

	private:
		/**
		* Create a processor module from json file.
		*/
		processor_module* create_processor(nlohmann::json processor_definition);
		module_relation* create_relation(nlohmann::json relation_definition);
		
		parameter* parameters_[255];
		uint_fast8_t nb_parameters_;
		
		project_info current_project_;
		std::array<project_info, 32> projects_infos_;
		uint_fast16_t nb_projects_;

		processor_module* modules_[255];
		uint_fast8_t nb_modules_ = 0;

		module_relation* relations_[255];
		uint_fast8_t nb_relations_ = 0;

		vie_processor vie_processor_;
		vie_view* vie_view_;
	};
}

extern live::tritone::vie::application application_;