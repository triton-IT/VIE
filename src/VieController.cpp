#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>
#include <base/source/fstreamer.h>

#include "VieController.h"
#include "Application.h"
#include "Constants.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace live::tritone::vie {
	VieController::VieController() :
		nbRef_(0),
		view_(nullptr),
		componentHandler_(nullptr),
		frequencyParameter_(new FrequencyParameter())
	{
		//Notify this controller when parameter change. By design, we want the controller to communicate with VST host. Not the view or any parameter.
		frequencyParameter_->setListener(this);
	}

	VieController::~VieController() {
		if (view_ != nullptr) {
			delete view_;
		}

		delete frequencyParameter_;
	}

	tresult PLUGIN_API VieController::queryInterface(const TUID iid, void** obj) {
		if (FUnknownPrivate::iidEqual(iid, IEditController::iid)) {
			addRef();
			*obj = static_cast<IEditController*>(this);
			return kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IPluginBase::iid)) {
			addRef();
			* obj = static_cast<IPluginBase*>(this);
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

	uint32 PLUGIN_API VieController::addRef() {
		return ++nbRef_;
	}

	uint32 PLUGIN_API VieController::release() {
		return --nbRef_;
	}

	tresult PLUGIN_API VieController::initialize(FUnknown* context)
	{
		hostContext = context;

		parameters_.addParameter(USTRING("Bypass"), nullptr, 1, 0,
			ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass, kBypassId);

		parameters_.addParameter(frequencyParameter_);

		return kResultOk;
	}

	tresult PLUGIN_API VieController::terminate()
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieController::setComponentState(IBStream* /*state*/)
	{
		return kNotImplemented;
	}

	tresult PLUGIN_API VieController::setState(IBStream* state)
	{
		return view_->setState(state);
	}

	tresult PLUGIN_API VieController::getState(IBStream* state)
	{
		return view_->getState(state);
	}

	int32 PLUGIN_API VieController::getParameterCount()
	{
		return parameters_.getParameterCount();
	}

	tresult PLUGIN_API VieController::getParameterInfo(int32 paramIndex, ParameterInfo& info /*out*/)
	{
		info = parameters_.getParameterByIndex(paramIndex)->getInfo();
		return kResultOk;
	}

	tresult PLUGIN_API VieController::getParamStringByValue(ParamID id, ParamValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/)
	{
		Parameter* parameter = parameters_.getParameter(id);
		parameter->toString(valueNormalized, string);
		return kResultOk;
	}

	tresult PLUGIN_API VieController::getParamValueByString(ParamID id, TChar* string /*in*/, Steinberg::Vst::ParamValue& valueNormalized /*out*/)
	{
		Parameter* parameter = parameters_.getParameter(id);
		parameter->fromString(string, valueNormalized);
		return kResultOk;
	}

	ParamValue PLUGIN_API VieController::normalizedParamToPlain(ParamID id, ParamValue valueNormalized)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->toPlain(valueNormalized);
	}

	ParamValue PLUGIN_API VieController::plainParamToNormalized(ParamID id, ParamValue plainValue)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->toNormalized(plainValue);
	}

	ParamValue PLUGIN_API VieController::getParamNormalized(ParamID id)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->getNormalized();
	}

	tresult PLUGIN_API VieController::setParamNormalized(ParamID id, ParamValue value)
	{
		Parameter* parameter = parameters_.getParameter(id);
		return parameter->setNormalized(value);
	}

	tresult PLUGIN_API VieController::setComponentHandler(IComponentHandler* handler)
	{
		//If already set, do nothing
		if (componentHandler_ == handler)
		{
			return kResultOk;
		}

		//If already have one, (but another) release it before adding new one.
		if (componentHandler_ != nullptr)
		{
			componentHandler_->release();
		}

		componentHandler_ = handler;

		//If new one is not null, add a ref.
		if (componentHandler_ != nullptr)
		{
			componentHandler_->addRef();
		}

		return kResultTrue;
	}

	IPlugView* PLUGIN_API VieController::createView(FIDString /*name*/)
	{
		view_ = new VieView(frequencyParameter_);

		return view_;
	}

	tresult PLUGIN_API VieController::connect(IConnectionPoint* other)
	{
		if (!other)
			return kInvalidArgument;

		DLOG("Connection to another connection point requested. should store and manage peer connection.");
		return kResultOk;
	}

	tresult PLUGIN_API VieController::disconnect(IConnectionPoint* other)
	{
		if (!other)
			return kInvalidArgument;

		DLOG("Disconnection from another connection point requested.");
		return kResultOk;
	}

	tresult PLUGIN_API VieController::notify(IMessage* message)
	{
		DLOG("Notification received from the other connection point");
		if (!message)
			return kInvalidArgument;

		DLOG(message->getMessageID());

		return kResultFalse;
	}

	void PLUGIN_API VieController::update(Steinberg::FUnknown* changedUnknown, int32 message) {
		DLOG("Update request received.");
	}

	void VieController::parameterValueChanged(Steinberg::int32 parameterId, Steinberg::Vst::ParamValue /*normalizedValue*/) {
		if (parameterId == kFrequencyId && componentHandler_ != nullptr) {
			componentHandler_->beginEdit(kFrequencyId);
			componentHandler_->performEdit(kFrequencyId, frequencyParameter_->getNormalized());
			componentHandler_->endEdit(kFrequencyId);
		}
	}
} // namespace
