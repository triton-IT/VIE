#include "vst_plugin_factory.hpp"
#include "vst_plugin.hpp"

#include "vst_controller.hpp"
#include "vst_processor.hpp"
#include "constants.hpp"

using namespace Steinberg;
using namespace Steinberg::Vst;

//Define IID for steinberg sdk that are not defined in pluginterfaces.
namespace Steinberg {
	namespace Vst {
		const FUID IComponent::iid(IComponent_iid);
		const FUID IAudioProcessor::iid(IAudioProcessor_iid);
		const FUID IProcessContextRequirements::iid(IProcessContextRequirements_iid);
		const FUID IConnectionPoint::iid(IConnectionPoint_iid);
		const FUID IEditController::iid(IEditController_iid);
	}
}

namespace live::tritone::vie::vst {	
	vst_plugin_factory::vst_plugin_factory() : nb_ref_(0) {
		strcpy(factory_info_.vendor, company_name);
		strcpy(factory_info_.url, company_url);
		strcpy(factory_info_.email, company_email);
		factory_info_.flags = PFactoryInfo::FactoryFlags::kUnicode;

		TUID controller_tuid = INLINE_UID_FROM_FUID(vst_plugin::controller_uid);
		controller_class_info_ = PClassInfo2(
			controller_tuid,
			PClassInfo::kManyInstances,
			kVstComponentControllerClass,
			plugin_name,
			0,
			"",
			nullptr,
			version,
			kVstVersionString
		);

		controller_class_info_w_.fromAscii(controller_class_info_);

		TUID processor_tuid = INLINE_UID_FROM_FUID(vst_plugin::processor_uid);
		processor_class_info_ = PClassInfo2(
			processor_tuid,
			PClassInfo::kManyInstances,
			kVstAudioEffectClass,
			plugin_name,
			Vst::kDistributable,
			Vst::PlugType::kFxInstrument,
			nullptr,
			version,
			kVstVersionString
		);

		processor_class_info_w_.fromAscii(processor_class_info_);
	}

	tresult vst_plugin_factory::getClassInfoUnicode(const int32 index, PClassInfoW* info) {
		switch (index) {
		case 0:
			memcpy(info, &controller_class_info_w_, sizeof(PClassInfo));
			return kResultOk;
		case 1:
			memcpy(info, &processor_class_info_w_, sizeof(PClassInfo));
			return kResultOk;
		default:
			return kResultFalse;
		}
	}

	tresult vst_plugin_factory::setHostContext(FUnknown* context) {
		host_context_ = context;
		return kResultOk;
	}

	tresult vst_plugin_factory::getClassInfo2(int32 index, PClassInfo2* info) {
		switch (index) {
		case 0:
			memcpy(info, &controller_class_info_, sizeof(PClassInfo2));
			return kResultOk;
		case 1:
			memcpy(info, &processor_class_info_, sizeof(PClassInfo2));
			return kResultOk;
		default:
			return kResultFalse;
		}
	}

	tresult vst_plugin_factory::getFactoryInfo(PFactoryInfo* info) {
		memcpy(info, &factory_info_, sizeof(PFactoryInfo));
		return kResultOk;
	}

	int32 vst_plugin_factory::countClasses() {
		return 2;
	}

	tresult vst_plugin_factory::getClassInfo(int32 index, PClassInfo* info) {
		switch (index) {
		case 0:
			memcpy(info, &controller_class_info_, sizeof(PClassInfo));
			return kResultOk;
		case 1:
			memcpy(info, &processor_class_info_, sizeof(PClassInfo));
			return kResultOk;
		default:
			return kResultFalse;
		}
	}

	tresult vst_plugin_factory::createInstance(FIDString cid, FIDString _iid, void** obj) {
		if (memcmp(controller_class_info_.cid, cid, sizeof(TUID)) == 0) {
			*obj = new vst_controller;
			return kResultOk;
		}

		if (memcmp(processor_class_info_.cid, cid, sizeof(TUID)) == 0) {
			*obj = new vst_processor;
			return kResultOk;
		}

		*obj = nullptr;
		return kNoInterface;
	}

	tresult vst_plugin_factory::queryInterface(const Steinberg::TUID _iid, void** obj) {
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

	uint32 vst_plugin_factory::addRef() {
		return ++nb_ref_;
	}

	uint32 vst_plugin_factory::release() {
		//FIXME: Try to delete this if 0.
		return --nb_ref_;
	}
}

using namespace live::tritone::vie::vst;

static vst_plugin_factory* gPluginFactory = nullptr;

SMTG_EXPORT_SYMBOL IPluginFactory* PLUGIN_API GetPluginFactory() {
	if (!gPluginFactory) {
		gPluginFactory = new vst_plugin_factory;
	}
	else {
		gPluginFactory->addRef();
	}

	return gPluginFactory;
}
