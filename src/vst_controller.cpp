#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "vst_controller.hpp"
#include "application.hpp"

#include "frequency_parameter.hpp"
#include "wave_form_parameter.hpp"

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie {
	vst_controller::vst_controller() : i_parameter_listener(),
	                                   nb_ref_(0),
	                                   view_(nullptr),
	                                   component_handler_(nullptr)
	{
		//Notify this controller when parameter change. By design, we want the controller to communicate with VST host. Not the view or any parameter.
		//frequencyParameter_->setListener(this);
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

		//FIXME: Do not call new here or delete variables on terminate.
		auto* bypass = new parameter(bypass_id, L"Bypass", L"bp", L"", 1, .0, 
			kRootUnitId, ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass);
		parameters_list_. push_back(bypass);
		parameters_map_[bypass_id] = static_cast<uint_fast16_t>(parameters_list_.size()) - 1;

		const auto frequency = new frequency_parameter();
		parameters_list_.push_back(frequency);
		parameters_map_[frequency->get_id()] =static_cast<uint_fast16_t>(parameters_list_.size()) - 1;

		auto* wave_form = new wave_form_parameter();
		wave_form->set_listener(this);
		parameters_list_.push_back(wave_form);
		parameters_map_[wave_form->get_id()] = static_cast<uint_fast16_t>(parameters_list_.size())- 1;

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
		return parameters_list_.size();
	}

	tresult __stdcall vst_controller::getParameterInfo(int32 paramIndex, ParameterInfo& info /*out*/)
	{
		const parameter* parameter = parameters_list_[paramIndex];

		ParameterInfo param_info{};
		param_info.id = parameter->get_id();
		
		#if defined(_WIN32) || defined(_WIN64) 
			parameter->get_title(param_info.title);
			parameter->get_short_title(param_info.shortTitle);
			parameter->get_units(param_info.units);
		#else
			parameter->get_title((wchar_t*)param_info.title);
			parameter->get_short_title((wchar_t*)param_info.shortTitle);
			parameter->get_units((wchar_t*)param_info.units);
		#endif
		param_info.stepCount = parameter->get_step_count();
		param_info.defaultNormalizedValue = parameter->get_default_normalized_value();
		param_info.unitId = parameter->get_unit_id();
		param_info.flags = parameter->get_flags();

		info = param_info;
		return
			kResultOk;
	}

	tresult __stdcall vst_controller::getParamStringByValue(ParamID id, ParamValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/)
	{
		if (const parameter* parameter = parameters_list_[parameters_map_[id]]; parameter->get_step_count()== 1) {
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

		return
			kResultOk;
	}

	tresult __stdcall vst_controller::getParamValueByString(ParamID id, TChar* string /*in*/, Steinberg::Vst::ParamValue & valueNormalized /*out*/)
	{
		if (const parameter* parameter = parameters_list_[parameters_map_[id]]; parameter->get_step_count() == 1) {
		
		#if defined(_WIN32) || defined(_WIN64) 
			if (wcscmp(string, L"On") == 0) {
		#else
			if (wcscmp((wchar_t*)string, L"On") == 0) {
		#endif
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
		return
			kResultOk;
	}

	ParamValue __stdcall vst_controller::normalizedParamToPlain(ParamID id, ParamValue valueNormalized )
	{
		parameter* parameter = parameters_list_[parameters_map_[id]];
		return parameter->to_plain_value(valueNormalized);
	}

	ParamValue __stdcall vst_controller::plainParamToNormalized(ParamID id, ParamValue plainValue)
	{
		parameter* parameter = parameters_list_[parameters_map_[id]];
		return parameter-> to_normalized_value(plainValue);
	}

	ParamValue __stdcall vst_controller::getParamNormalized(ParamID id)
	{
		const parameter* parameter = parameters_list_[parameters_map_[id]];
		return parameter->get_normalized_value();

	}

	tresult __stdcall vst_controller::setParamNormalized(ParamID id, ParamValue value)
	{
		if (parameter* parameter = parameters_list_[parameters_map_[id]]; parameter->
			set_normalized_value(value)
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

		return kResultTrue;
	}

	IPlugView* __stdcall vst_controller::createView(FIDString /*name*/)
	{
		view_ = new vie_view(parameters_list_);

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

	void vst_controller::parameter_value_changed(Steinberg::Vst::ParamID parameterId, Steinberg::Vst::ParamValue /*normalizedValue*/) {
		parameter* parameter = parameters_list_[parameterId];

		if (component_handler_ != nullptr) {
			component_handler_->beginEdit(parameterId);
			component_handler_->performEdit(parameterId, parameter->get_normalized_value());
			component_handler_->endEdit(parameterId);
		}
	}

} // namespace
