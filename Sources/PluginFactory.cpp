#include <public.sdk/source/main/pluginfactory.h>

#include "TriToneProcessor.h"
#include "TriToneController.h"
#include "Constants.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

using namespace tech::tritonit::tritone;

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
		TriToneProcessor::createInstance)

	DEF_CLASS2(INLINE_UID_FROM_FUID(ControllerUID),
		PClassInfo::kManyInstances,
		kVstComponentControllerClass,
		PLUGIN_NAME,
		0,
		"",
		VERSION,
		kVstVersionString,
		TriToneController::createInstance)
END_FACTORY