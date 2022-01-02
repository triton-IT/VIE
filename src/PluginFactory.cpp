#include <public.sdk/source/main/pluginfactory.h>

#include "VieProcessor.h"
#include "VieController.h"
#include "Constants.h"


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION // Need to define NK_IMPLEMENTATION once and only once before including nuklear.h.
#include <nuklear.h>

using namespace Steinberg;
using namespace Steinberg::Vst;

using namespace live::tritone::vie;

BEGIN_FACTORY_DEF(COMPANY_NAME,
	COMPANY_URL,
	COMPANY_EMAIL)

	DEF_CLASS2(INLINE_UID_FROM_FUID(ProcessorUID),
		PClassInfo::kManyInstances,
		kVstAudioEffectClass,
		PLUGIN_NAME,
		Vst::kDistributable,
		Vst::PlugType::kFxInstrument,
		VERSION,
		kVstVersionString,
		VieProcessor::createInstance)

	DEF_CLASS2(INLINE_UID_FROM_FUID(ControllerUID),
		PClassInfo::kManyInstances,
		kVstComponentControllerClass,
		PLUGIN_NAME,
		0,
		"",
		VERSION,
		kVstVersionString,
		VieController::createInstance)
END_FACTORY
