#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "VieController.h"
#include "Application.h"
#include "Constants.h"

#include "FrequencyParameter.h"
#include "WaveFormParameter.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace live::tritone::vie {
	VieController::VieController() :
		nbRef_(0),
		view_(nullptr),
		componentHandler_(nullptr)
	{
		//Notify this controller when parameter change. By design, we want the controller to communicate with VST host. Not the view or any parameter.
		//frequencyParameter_->setListener(this);
	}

	VieController::~VieController() {
		if (view_ != nullptr) {
			delete view_;
		}
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
		//FIXME: Try to delete this if 0.
		return --nbRef_;
	}

	tresult PLUGIN_API VieController::initialize(FUnknown* context)
	{
		hostContext = context;

		//FIXME: Do not call new here or delete variables on terminate.
		Parameter* bypass = new Parameter(kBypassId, L"Bypass", L"bp", L"", 1, .0f, kRootUnitId, ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass);
		parametersList_.push_back(bypass);
		parametersMap_[kBypassId] = parametersList_.size() - 1;

		FrequencyParameter* frequency = new FrequencyParameter();
		parametersList_.push_back(frequency);
		parametersMap_[frequency->getId()] = parametersList_.size() - 1;

		WaveFormParameter* waveForm = new WaveFormParameter();
		waveForm->setListener(this);
		parametersList_.push_back(waveForm);
		parametersMap_[waveForm->getId()] = parametersList_.size() - 1;

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
		return parametersList_.size();
	}

	tresult PLUGIN_API VieController::getParameterInfo(int32 paramIndex, ParameterInfo& info /*out*/)
	{
		Parameter* parameter = parametersList_[paramIndex];

		ParameterInfo paramInfo;
		paramInfo.id = parameter->getId();
		parameter->getTitle(paramInfo.title);
		parameter->getShortTitle(paramInfo.shortTitle);
		parameter->getUnits(paramInfo.units);
		paramInfo.stepCount = parameter->getStepCount();
		paramInfo.defaultNormalizedValue = parameter->getDefaultNormalizedValue();
		paramInfo.unitId = parameter->getUnitId();
		paramInfo.flags = parameter->getFlags();

		info = paramInfo;
		return kResultOk;
	}

	tresult PLUGIN_API VieController::getParamStringByValue(ParamID id, ParamValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/)
	{
		Parameter *parameter = parametersList_[parametersMap_[id]];

		if (parameter->getStepCount() == 1) {
			if (valueNormalized > 0.5) {
				UString(string, str16BufferSize(String128)).assign(USTRING("On"));
			} else {
				UString(string, str16BufferSize(String128)).assign(USTRING("Off"));
			}
		} else {
			UString(string, str16BufferSize(String128)).printFloat(valueNormalized, 2);
		}

		return kResultOk;
	}

	tresult PLUGIN_API VieController::getParamValueByString(ParamID id, TChar* string /*in*/, Steinberg::Vst::ParamValue& valueNormalized /*out*/)
	{
		Parameter* parameter = parametersList_[parametersMap_[id]];

		if (parameter->getStepCount() == 1) {
			if (wcscmp(string, L"On") == 0) {
				valueNormalized = 0.0f;
			} else {
				valueNormalized = 1.0f;
			}
		}
		else {
			UString wrapper((TChar*)string, 128);
			wrapper.scanFloat(valueNormalized);
		}
		return kResultOk;
	}

	ParamValue PLUGIN_API VieController::normalizedParamToPlain(ParamID id, ParamValue valueNormalized)
	{
		Parameter* parameter = parametersList_[parametersMap_[id]];
		return parameter->toPlainValue(valueNormalized);
	}

	ParamValue PLUGIN_API VieController::plainParamToNormalized(ParamID id, ParamValue plainValue)
	{
		Parameter* parameter = parametersList_[parametersMap_[id]];
		return parameter->toNormalizedValue(plainValue);
	}

	ParamValue PLUGIN_API VieController::getParamNormalized(ParamID id)
	{
		Parameter* parameter = parametersList_[parametersMap_[id]];
		return parameter->getNormalizedValue();
	}

	tresult PLUGIN_API VieController::setParamNormalized(ParamID id, ParamValue value)
	{
		Parameter* parameter = parametersList_[parametersMap_[id]];
		if (parameter->setNormalizedValue(value)) {
			//TODO: Update GUI with new value.
			return kResultOk;
		}

		return kResultFalse;
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
		view_ = new VieView(parametersList_);

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

	void VieController::parameterValueChanged(Steinberg::Vst::ParamID parameterId, Steinberg::Vst::ParamValue /*normalizedValue*/) {
		Parameter* parameter = parametersList_[parameterId];

		view_->debug(parameter);

		if (componentHandler_ != nullptr) {
			componentHandler_->beginEdit(parameterId);
			componentHandler_->performEdit(parameterId, parameter->getNormalizedValue());
			componentHandler_->endEdit(parameterId);
		}
	}
} // namespace