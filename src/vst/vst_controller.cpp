#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "vst_controller.hpp"
#include "../application.hpp"

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie::vst {
	vst_controller::vst_controller() : nb_ref_(0),
	                                   view_(nullptr),
									   vst_host_callback_(nullptr)
	{
	}

	vst_controller::~vst_controller() {
		if (view_ != nullptr) {
			delete view_;
			view_ = nullptr;
		}
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
		return application_.get_parameters_count();
	}

	tresult __stdcall vst_controller::getParameterInfo(int32 param_index, ParameterInfo& info /*out*/)
	{
		parameter parameter = application_.get_parameter(param_index);

		ParameterInfo param_info{};
		param_info.id = parameter.get_id();
		parameter.get_title(param_info.title);
		parameter.get_short_title(param_info.shortTitle);
		parameter.get_units(param_info.units);
		param_info.stepCount = parameter.get_step_count();
		param_info.defaultNormalizedValue = parameter.get_default_normalized_value();
		param_info.unitId = parameter.get_unit_id();
		param_info.flags = parameter.get_flags();

		info = param_info;

		return kResultOk;
	}

	tresult __stdcall vst_controller::getParamStringByValue(ParamID id, ParamValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/)
	{
		//Print "on" or "off" if value is a boolean or print float value otherwise.
		if (parameter parameter = application_.get_parameter(id); parameter.get_step_count() == 1) {
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
		if (parameter parameter = application_.get_parameter(id); parameter.get_step_count() == 1) {
			if (wcscmp(string, L"On") == 0) {
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
		parameter parameter  = application_.get_parameter(id);
		return parameter.to_plain_value(valueNormalized);
	}

	ParamValue __stdcall vst_controller::plainParamToNormalized(ParamID id, ParamValue plainValue)
	{
		parameter parameter = application_.get_parameter(id);
		return parameter.to_normalized_value(plainValue);
	}

	ParamValue __stdcall vst_controller::getParamNormalized(ParamID id)
	{
		parameter parameter = application_.get_parameter(id);
		return parameter.get_normalized_value();

	}

	tresult __stdcall vst_controller::setParamNormalized(ParamID id, ParamValue value)
	{
		if (parameter parameter = application_.get_parameter(id); parameter.set_normalized_value(value)
			) {
			//TODO: Update GUI with new value.
			return kResultOk;
		}

		return kResultFalse;
	}

	tresult __stdcall vst_controller::setComponentHandler(IComponentHandler* handler)
	{
		if (vst_host_callback_ == nullptr) {
			vst_host_callback_ = new vst_host_callback(handler);
		}
		
		Steinberg::Vst::IComponentHandler* current_handler = vst_host_callback_->get_vst_handler();
		
		//If already set, do nothing
		if (current_handler == handler)
		{
			return kResultOk;
		}

		//If already have one, (but another) release it before adding new one.
		if (current_handler != nullptr)
		{
			current_handler->release();
		}

		current_handler = handler;

		//If new one is not null, add a ref.
		if (current_handler != nullptr)
		{
			current_handler->addRef();
		}

		if (view_ != nullptr)
		{
			view_->set_host_callback(vst_host_callback_);
		}

		return kResultTrue;
	}

	IPlugView* __stdcall vst_controller::createView(FIDString /*name*/)
	{
		if (view_ != nullptr) {
			delete view_;
		}
		
		view_ = application_.create_view(vst_host_callback_);

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