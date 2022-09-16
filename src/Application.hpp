#pragma once

#include <string>
#include "logger.hpp"
#include "parameters.hpp"
#include "processor_components.hpp"

using namespace live::tritone::vie;

extern void* module_handle;
extern std::wstring content_path;

#ifdef VIE_DEBUG
extern live::tritone::vie::utils::Logger debugLogger;
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
	static parameters& get_parameters() {
		return parameters_;
	}
	static processor_components& get_processor_components() {
		return processor_components::get_instance();
	}

private:
	//FIXME: Make a singleton or transform processor_components to static too.
	static parameters parameters_;
};