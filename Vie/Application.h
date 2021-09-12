#pragma once

#include <string>
#include "Logger.h"

extern void* moduleHandle;
extern std::string contentPath;

#ifdef DEBUG
extern live::tritone::vie::utils::Logger debugLogger;
#endif

#ifdef DEBUG
#define DLOG(log) debugLogger.write(log)
#else
#define DLOG(log)
#endif // DEBUG

enum
{
	kFrequencyId = 0,
	kBypassId
};