#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>
#include <base/source/fstreamer.h>

#include "TriToneController.h"
#include "Application.h"
#include "Constants.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace tech::tritonit::tritone {
	TriToneController::TriToneController() :
		nbRef_(0),
		view_(nullptr),
		componentHandler_(nullptr),
		frequencyParameter_(new FrequencyParameter())
	{
		//Notify this controller when parameter change. By design, we want the controller to communicate with VST host. Not the view or any parameter.
		frequencyParameter_->setListener(this);
	}

	TriToneController::~TriToneController() {
		if (view_ != nullptr) {
			delete view_;
		}

		delete frequencyParameter_;
	}

	tresult PLUGIN_API TriToneController::queryInterface(const TUID iid, void** obj) {
		if (FUnknownPrivate::iidEqual(iid, IEditController::iid)) {
			addRef();
			*obj = static_cast<IEditController*>(this);
			return kResultOk;
		}
		return ComponentBase::queryInterface(iid, obj);
	}

	uint32 PLUGIN_API TriToneController::addRef() {
		return ++nbRef_;
	}

	uint32 PLUGIN_API TriToneController::release() {
		return --nbRef_;
	}

	FClassID TriToneController::isA() const {
		return "Controller";
	}

	tresult PLUGIN_API TriToneController::initialize(FUnknown* context)
	{
		ComponentBase::initialize(context);

		parameters_.addParameter(USTRING("Bypass"), nullptr, 1, 0,
			ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass, kBypassId);

		parameters_.addParameter(frequencyParameter_);

		return kResultOk;
	}

	tresult PLUGIN_API TriToneController::terminate()
	{
		return kResultOk;
	}

	tresult PLUGIN_API TriToneController::setComponentState(IBStream* /*state*/)
	{
		return kNotImplemented;
	}

	tresult PLUGIN_API TriToneController::setState(IBStream* /*state*/)
	{
		return kNotImplemented;
	}

	tresult PLUGIN_API TriToneController::getState(IBStream* /*state*/)
	{
		return kNotImplemented;
	}

	int32 PLUGIN_API TriToneController::getParameterCount()
	{
		return parameters_.getParameterCount();
	}

	tresult PLUGIN_API TriToneController::getParameterInfo(int32 paramIndex, ParameterInfo& info /*out*/)
	{
		info = parameters_.getParameterByIndex(paramIndex)->getInfo();
		return kResultOk;
	}

	tresult PLUGIN_API TriToneController::getParamStringByValue(ParamID id, ParamValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/)
	{
		Parameter* parameter = parameters_.getParameter(id);
		parameter->toString(valueNormalized, string);
		return kResultOk;
	}

	tresult PLUGIN_API TriToneController::getParamValueByString(ParamID id, TChar* string /*in*/, Steinberg::Vst::ParamValue& valueNormalized /*out*/)
	{
		Parameter* parameter = parameters_.getParameter(id);
		parameter->fromString(string, valueNormalized);
		return kResultOk;
	}

	ParamValue PLUGIN_API TriToneController::normalizedParamToPlain(ParamID id, ParamValue valueNormalized)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->toPlain(valueNormalized);
	}

	ParamValue PLUGIN_API TriToneController::plainParamToNormalized(ParamID id, ParamValue plainValue)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->toNormalized(plainValue);
	}

	ParamValue PLUGIN_API TriToneController::getParamNormalized(ParamID id)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->getNormalized();
	}

	tresult PLUGIN_API TriToneController::setParamNormalized(ParamID id, ParamValue value)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->setNormalized(value);
	}

	tresult PLUGIN_API TriToneController::setComponentHandler(IComponentHandler* handler)
	{
		if (componentHandler_ == handler)
		{
			return kResultOk;
		}

		//If already have one, release it before adding new one.
		if (componentHandler_ != nullptr)
		{
			componentHandler_->release();
		}

		componentHandler_ = handler;
		if (componentHandler_ != nullptr)
		{
			componentHandler_->addRef();
		}

		return kResultTrue;
	}

	IPlugView* PLUGIN_API TriToneController::createView(FIDString /*name*/)
	{
		view_ = new TriToneView(frequencyParameter_);

		return view_;
	}

	void TriToneController::parameterValueChanged(Steinberg::int32 parameterId, Steinberg::Vst::ParamValue /*normalizedValue*/) {
		if (parameterId == kFrequencyId && componentHandler_ != nullptr) {
			componentHandler_->beginEdit(kFrequencyId);
			componentHandler_->performEdit(kFrequencyId, frequencyParameter_->getNormalized());
			componentHandler_->endEdit(kFrequencyId);
		}
	}
} // namespace
