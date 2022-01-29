#include "VstPluginFactory.h"

#include "VieController.h"
#include "VieProcessor.h"
#include "Constants.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

//Define IID for steinberg sdk that are not defined in pluginterfaces.
namespace Steinberg {
	namespace Vst {
		const FUID IAudioProcessor::iid(IAudioProcessor_iid);
		const FUID IComponent::iid(IComponent_iid);
		const FUID IConnectionPoint::iid(IConnectionPoint_iid);
		const FUID IEditController::iid(IEditController_iid);
		const FUID IProcessContextRequirements::iid(IProcessContextRequirements_iid);
	}
}

namespace live::tritone::vie {
	static VstPluginFactory* gPluginFactory = nullptr;

	__declspec (dllexport) IPluginFactory* __stdcall GetPluginFactory() {
		if (!gPluginFactory) {
			gPluginFactory = new VstPluginFactory;
		}
		else {
			gPluginFactory->addRef();
		}

		return gPluginFactory;
	}

	VstPluginFactory::VstPluginFactory() {
		strcpy(factoryInfo_.vendor, COMPANY_NAME);
		strcpy(factoryInfo_.url, COMPANY_URL);
		strcpy(factoryInfo_.email, COMPANY_EMAIL);
		factoryInfo_.flags = PFactoryInfo::FactoryFlags::kUnicode;

		TUID controllerTUID = INLINE_UID_FROM_FUID(ControllerUID);
		controllerClassInfo_ = PClassInfo2(
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

		controllerClassInfoW_.fromAscii(controllerClassInfo_);

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

		processorClassInfoW_.fromAscii(controllerClassInfo_);
	}

	tresult VstPluginFactory::getClassInfoUnicode(int32 index, PClassInfoW* info) {
		switch(index) {
		case 0:
			memcpy(info, &controllerClassInfoW_, sizeof(PClassInfo));
			return kResultOk;
		case 1:
			memcpy(info, &processorClassInfoW_, sizeof(PClassInfo));
			return kResultOk;
		default:
			return kResultFalse;
		}
	}

	tresult VstPluginFactory::setHostContext(FUnknown* context) {
		hostContext_ = context;
		return kResultOk;
	}

	tresult VstPluginFactory::getClassInfo2(int32 index, PClassInfo2* info) {
		switch (index) {
		case 0:
			memcpy(info, &controllerClassInfo_, sizeof(PClassInfo2));
			return kResultOk;
		case 1:
			memcpy(info, &processorClassInfo_, sizeof(PClassInfo2));
			return kResultOk;
		default:
			return kResultFalse;
		}
	}

	tresult VstPluginFactory::getFactoryInfo(PFactoryInfo* info) {
		memcpy(info, &factoryInfo_, sizeof(PFactoryInfo));
		return kResultOk;
	}

	int32 VstPluginFactory::countClasses() {
		return 2;
	}

	tresult VstPluginFactory::getClassInfo(int32 index, PClassInfo* info) {
		switch (index) {
		case 0:
			memcpy(info, &controllerClassInfo_, sizeof(PClassInfo));
			return kResultOk;
		case 1:
			memcpy(info, &processorClassInfo_, sizeof(PClassInfo));
			return kResultOk;
		default:
			return kResultFalse;
		}
	}

	tresult VstPluginFactory::createInstance(FIDString cid, FIDString _iid, void** obj) {
		if (memcmp(controllerClassInfo_.cid, cid, sizeof(TUID)) == 0)
		{
			*obj = new VieController;
			return kResultOk;
		}

		if (memcmp(processorClassInfo_.cid, cid, sizeof(TUID)) == 0)
		{
			*obj = new VieProcessor;
			return kResultOk;
		}

		*obj = nullptr;
		return kNoInterface;
	}

	tresult VstPluginFactory::queryInterface(const Steinberg::TUID _iid, void** obj) {
		if (FUnknownPrivate::iidEqual(iid, IPluginFactory3::iid)) {
			addRef();
			*obj = static_cast<IPluginFactory3*>(this);
			return kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IPluginFactory2::iid)) {
			addRef();
			*obj = static_cast<IPluginFactory2*>(this);
			return kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IPluginFactory::iid)) {
			addRef();
			*obj = static_cast<IPluginFactory*>(this);
			return kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, FUnknown::iid)) {
			addRef();
			*obj = static_cast<FUnknown*>(this);
			return kResultOk;
		}

		*obj = nullptr;
		return Steinberg::kNoInterface;
	}

	uint32 VstPluginFactory::addRef() {
		return ++nbRef_;
	}

	uint32 VstPluginFactory::release() {
		//FIXME: Try to delete this if 0.
		return --nbRef_;
	}
}
