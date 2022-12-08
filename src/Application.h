#pragma once

#include <string>
#include "Logger.h"

extern void* moduleHandle;
extern std::string contentPath;

#ifdef VIE_DEBUG
extern live::tritone::vie::utils::Logger debugLogger;
#define DLOG(log) debugLogger.write(log)
#else
#define DLOG(log)
#endif // VIE_DEBUG

enum
{
	kBypassId,
	kFrequencyId,
	kWaveFormId
};