#include <pluginterfaces/vst/ivstcomponent.h>

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

using namespace Steinberg;

#include <public.sdk/source/main/pluginfactory.h>

SMTG_EXPORT_SYMBOL IPluginFactory* PLUGIN_API GetPluginFactory() {
	if (!gPluginFactory) {
		static PFactoryInfo factoryInfo(COMPANY_NAME, COMPANY_URL, COMPANY_EMAIL, Vst::kDefaultFactoryFlags);
		gPluginFactory = new CPluginFactory(factoryInfo);

		TUID processorTUID = INLINE_UID_FROM_FUID(ProcessorUID);
		static PClassInfo2 processorClass(
			processorTUID,
			PClassInfo::kManyInstances, 
			kVstAudioEffectClass, 
			PLUGIN_NAME, 
			Vst::kDistributable, 
			Vst::PlugType::kFxInstrument, 
			0, 
			VERSION, 
			kVstVersionString
		);
		gPluginFactory->registerClass(&processorClass, VieProcessor::createInstance);

		TUID controllerTUID = INLINE_UID_FROM_FUID(ControllerUID);
		static PClassInfo2 controllerClass(
			controllerTUID,
			PClassInfo::kManyInstances, 
			kVstComponentControllerClass, 
			PLUGIN_NAME, 
			0, 
			"", 
			0, 
			VERSION, 
			kVstVersionString
		);
		gPluginFactory->registerClass(&controllerClass, VieController::createInstance);
	} else
		gPluginFactory->addRef();

	return gPluginFactory;
}
