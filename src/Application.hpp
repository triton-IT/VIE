#pragma once

#include <string>
#include "logger.hpp"
#include "parameters.hpp"
#include "processor_components.hpp"

using namespace live::tritone::vie;

extern void* module_handle;
extern "C" std::wstring content_path;

#ifdef VIE_DEBUG
extern live::tritone::vie::debug::logger debugLogger;
#define DLOG(log) debugLogger.write(log)
#else
// ReSharper disable once IdentifierTypo
#define DLOG(log)
#endif // VIE_DEBUG

enum {
	bypass_id,
	frequency_id,
	wave_form_id
};

class application {
public:
	static processor_components& get_processor_components() {
		return processor_components::get_instance();
	}

	static parameter add_parameter(uint_fast8_t id, parameter parameter);
	static parameter add_parameter(uint_fast8_t id, std::string name, std::string short_name, std::string type, std::string unit, float value);

	static parameter get_parameter(uint_fast8_t id);

	static uint_fast8_t get_parameters_count();

private:
	static parameter* parameters_[255];
	static uint_fast8_t nb_parameters_;
};