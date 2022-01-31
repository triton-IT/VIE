#pragma once

#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstnoteexpression.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/vst/vsttypes.h>
#include <pluginterfaces/base/iupdatehandler.h>

#include <map>
#include <vector>

#include "VieView.h"
#include "IParameterListener.h"
#include "Parameter.h"

namespace live::tritone::vie {
	class VieController : 
		public Steinberg::Vst::IEditController,
		public Steinberg::Vst::IConnectionPoint,
		public Steinberg::IDependent,
		public IParameterListener
	{
	public:
		static FUnknown* createInstance(void* /*context*/)
		{
			return (IEditController*) new VieController;
		}

		VieController();
		~VieController();

		//Inheritance from IEditController
		Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API addRef() SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API release() SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::int32 PLUGIN_API getParameterCount() SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getParameterInfo(Steinberg::int32 paramIndex, Steinberg::Vst::ParameterInfo& info) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getParamStringByValue(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized, Steinberg::Vst::String128 string) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getParamValueByString(Steinberg::Vst::ParamID id, Steinberg::Vst::TChar* string, Steinberg::Vst::ParamValue& valueNormalized) SMTG_OVERRIDE;
		Steinberg::Vst::ParamValue PLUGIN_API normalizedParamToPlain(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized) SMTG_OVERRIDE;
		Steinberg::Vst::ParamValue PLUGIN_API plainParamToNormalized(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue plainValue) SMTG_OVERRIDE;
		Steinberg::Vst::ParamValue PLUGIN_API getParamNormalized(Steinberg::Vst::ParamID id) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setParamNormalized(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setComponentHandler(Steinberg::Vst::IComponentHandler* handler) SMTG_OVERRIDE;
		Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) SMTG_OVERRIDE;

		//Inheritance from IConnectionPoint
		Steinberg::tresult PLUGIN_API connect(Steinberg::Vst::IConnectionPoint* other) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API disconnect(Steinberg::Vst::IConnectionPoint* other) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) SMTG_OVERRIDE;

		//Inheritance from IDependent
		void PLUGIN_API update(Steinberg::FUnknown* changedUnknown, Steinberg::int32 message) SMTG_OVERRIDE;

		void parameterValueChanged(Steinberg::Vst::ParamID parameterId, Steinberg::Vst::ParamValue normalizedValue) SMTG_OVERRIDE;

	private:
		Steinberg::IPtr<Steinberg::FUnknown> hostContext;
		Steinberg::uint32 nbRef_;
		VieView* view_;
		Steinberg::Vst::IComponentHandler* componentHandler_;

		std::vector<Parameter*> parametersList_;
		//VST needs to access parameters by index but also by identifier. So map the parameter id with the index of the parameter in the vector.
		std::map<unsigned long, int> parametersMap_;
	};
} // namespace
