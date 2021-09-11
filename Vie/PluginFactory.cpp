#include <public.sdk/source/main/pluginfactory.h>

#include "VieProcessor.h"
#include "VieController.h"
#include "Constants.h"

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