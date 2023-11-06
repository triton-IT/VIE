#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "vst_controller.h"
#include "application.h"

using namespace Steinberg;
using namespace Vst;

#define ON_PARAMETER_ID std::wstring on_parameter_id = L"On";

namespace live::tritone::vie::vst {
	vst_controller::vst_controller() : nb_ref_(0),
	                                   view_(nullptr),
	                                   component_handler_(nullptr)
	{
	}

	vst_controller::~vst_controller() {
		delete view_;
	}

	tresult __stdcall vst_controller::queryInterface(const TUID iid, void** obj) {
		if (FUnknownPrivate::iidEqual(iid, IEditController::iid)) {
			addRef();
			*obj = static_cast<IEditController*>(this);
			return kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IPluginBase::iid)) {
			addRef();
			*obj = static_cast<IPluginBase*>(this);
			return Steinberg::kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IConnectionPoint::iid)) {
			addRef();
			*obj = static_cast<IConnectionPoint*>(this);
			return Steinberg::kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IDependent::iid)) {
			addRef();
			*obj = static_cast<IDependent*>(this);
			return Steinberg::kResultOk;
		}

		*obj = nullptr;
		return Steinberg::kNoInterface;
	}

	uint32 __stdcall vst_controller::addRef() {
		return ++nb_ref_;
	}

	uint32 __stdcall vst_controller::release() {
		//FIXME: Try to delete this if 0.
		return --nb_ref_;
	}

	tresult __stdcall vst_controller::initialize(FUnknown * context) {
		host_context_ = context;

		return
			kResultOk;
	}

	tresult __stdcall vst_controller::terminate() {
		return kResultOk;
	}

	tresult __stdcall vst_controller::setComponentState(IBStream* /*state*/)
	{
		return
			kNotImplemented;
	}

	tresult __stdcall vst_controller::setState(IBStream* state)
	{
		return view_->set_state(state);
	}

	tresult __stdcall vst_controller::getState(IBStream* state)
	{
		return view_->get_state(state);
	}

	int32 __stdcall vst_controller::getParameterCount()
	{
		return application::get_parameters_count();
	}

	tresult __stdcall vst_controller::getParameterInfo(int32 param_index, ParameterInfo& info /*out*/)
	{
		live::tritone::vie::core::IParameter& parameter = application::get_parameter(param_index);

		ParameterInfo param_info{};
		param_info.id = parameter.getId();
		parameter.getTitle((char16_t*) param_info.title);
		parameter.getShortTitle((char16_t*) param_info.shortTitle);
		parameter.getUnits((char16_t*) param_info.units);
		param_info.stepCount = parameter.getStepCount();
		param_info.defaultNormalizedValue = parameter.getDefaultNormalizedValue();
		param_info.unitId = parameter.getUnitId();
		param_info.flags = parameter.getFlags();

		info = param_info;

		return kResultOk;
	}

	tresult __stdcall vst_controller::getParamStringByValue(ParamID id, ParamValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/)
	{
		//Print "on" or "off" if value is a boolean or print float value otherwise.
		if (live::tritone::vie::core::IParameter& parameter = application::get_parameter(id); parameter.getStepCount() == 1) {
			if (valueNormalized > 0.5) {
				UString(string, str16BufferSize(String128)).assign(USTRING("On"));
			}
			else {
				UString(string, str16BufferSize(String128)).assign(USTRING("Off"));
			}
		}
		else {
			UString(string, str16BufferSize(String128)).printFloat(valueNormalized, 2);
		}

		return kResultOk;
	}

	tresult __stdcall vst_controller::getParamValueByString(ParamID id, TChar* string /*in*/, Steinberg::Vst::ParamValue & valueNormalized /*out*/)
	{
		// Set 0 or 1 if value is "on" or "off" or set value as float otherwise.
		if (live::tritone::vie::core::IParameter& parameter = application::get_parameter(id); parameter.getStepCount() == 1) {
            ON_PARAMETER_ID
			if (on_parameter_id.compare(string) == 0) {
				valueNormalized = 0.0;
			}
			else {
				valueNormalized = 1.0;
			}
		}
		else {
			const UString wrapper((TChar*)string, 128);
			wrapper.scanFloat(valueNormalized);
		}
		return kResultOk;
	}

	ParamValue __stdcall vst_controller::normalizedParamToPlain(ParamID id, ParamValue valueNormalized)
	{
		live::tritone::vie::core::IParameter& parameter  = application::get_parameter(id);
		return parameter.toPlainValue(valueNormalized);
	}

	ParamValue __stdcall vst_controller::plainParamToNormalized(ParamID id, ParamValue plainValue)
	{
		live::tritone::vie::core::IParameter& parameter = application::get_parameter(id);
		return parameter.toNormalizedValue(plainValue);
	}

	ParamValue __stdcall vst_controller::getParamNormalized(ParamID id)
	{
		live::tritone::vie::core::IParameter& parameter = application::get_parameter(id);
		return parameter.getNormalizedValue();

	}

	tresult __stdcall vst_controller::setParamNormalized(ParamID id, ParamValue value)
	{
		if (live::tritone::vie::core::IParameter& parameter = application::get_parameter(id); parameter.setNormalizedValue(value)
			) {
			//TODO: Update GUI with new value.
			return kResultOk;
		}

		return kResultFalse;
	}

	tresult __stdcall vst_controller::setComponentHandler(IComponentHandler* handler)
	{
		//If already set, do nothing
		if (component_handler_ == handler)
		{
			return kResultOk;
		}

		//If already have one, (but another) release it before adding new one.
		if (component_handler_ != nullptr)
		{
			component_handler_->release();
		}

		component_handler_ = handler;

		//If new one is not null, add a ref.
		if (component_handler_ != nullptr)
		{
			component_handler_->addRef();
		}

		if (view_ != nullptr)
		{
			view_->set_module_handler(component_handler_);
		}

		return kResultTrue;
	}

	IPlugView* __stdcall vst_controller::createView(FIDString /*name*/)
	{
		view_ = new vie_view(component_handler_);

		return view_;
	}

	tresult __stdcall vst_controller::connect(IConnectionPoint* other)
	{
		if (!other)
			return kInvalidArgument;

		return
			kResultOk;
	}

	tresult __stdcall vst_controller::disconnect(IConnectionPoint* other)
	{
		if (!other)
			return kInvalidArgument;

		return
			kResultOk;
	}

	tresult __stdcall vst_controller::notify(IMessage* message)
	{
		if (!message)
			return kInvalidArgument;

		return
			kResultFalse;
	}

	void __stdcall vst_controller::update(Steinberg::FUnknown* changed_unknown, int32 message) {
	}

} // namespace
