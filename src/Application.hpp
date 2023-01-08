#pragma once

#include <string>
#include "logger.hpp"

extern void* module_handle;
extern "C" std::wstring content_path;

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