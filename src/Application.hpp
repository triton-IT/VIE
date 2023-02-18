#pragma once

#include <string>
#include "logger.hpp"
#include "parameters.hpp"
#include "processor_modules.hpp"
#include "project.hpp"
#include "vie_processor.hpp"
#include "vie_view.hpp"
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
		processor_modules& get_processor_components() {
			return processor_modules::get_instance();
		}

		project& new_project();
		std::array<project, 32> get_projects(int* nb_projects);
		project load_project(std::wstring project_path);

		vie_processor& get_vie_processor();

		parameter add_parameter(uint_fast8_t id, parameter parameter);
		parameter add_parameter(uint_fast8_t id, std::string name, std::string short_name, std::string type, std::string unit, float value);

		parameter get_parameter(uint_fast8_t id);

		uint_fast8_t get_parameters_count();

		vie_view* create_view(host_callback* callback);
		vie_view* deleteView();

	private:
		parameter* parameters_[255];
		uint_fast8_t nb_parameters_;
		std::array<project, 32> projects_;
		int nb_projects_;

		vie_processor vie_processor_;
		vie_view* vie_view_;
	};
}

extern live::tritone::vie::application application_;