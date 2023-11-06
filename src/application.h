#pragma once

#include <string>

#include "logger.h"
#include "parameter.h"

#include <pluginterfaces/base/ftypes.h>

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
	/*static processor_modules& get_processor_modules() {
		return processor_modules::get_instance();
	}*/
	static live::tritone::vie::core::IParameter& add_parameter(uint_fast8_t id, live::tritone::vie::core::IParameter* parameter);

	static live::tritone::vie::core::IParameter& add_parameter(uint_fast8_t id, Steinberg::char16 name[128], Steinberg::char16 short_name[128], Steinberg::char16 type[128], Steinberg::char16 unit[128], float value);

	static live::tritone::vie::core::IParameter& get_parameter(uint_fast8_t id);

	static uint_fast8_t get_parameters_count();

private:
	static live::tritone::vie::core::IParameter* parameters_[255];
	static uint_fast8_t nb_parameters_;
};