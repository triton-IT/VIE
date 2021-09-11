#pragma once

#include <public.sdk/source/vst/vsteditcontroller.h>
#include <pluginterfaces/vst/ivstnoteexpression.h>
#include <pluginterfaces/vst/vsttypes.h>
#include <base/source/fobject.h>

#include "VieView.h"
#include "IParameterListener.h"
#include "FrequencyParameter.h"

namespace live::tritone::vie {
	class VieController : public Steinberg::Vst::ComponentBase, public Steinberg::Vst::IEditController, public IParameterListener
	{
	public:
		static FUnknown* createInstance(void*)
		{
			return (IEditController*) new VieController;
		}

		VieController();
		~VieController();

		Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API addRef() SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API release() SMTG_OVERRIDE;
		Steinberg::FClassID isA() const;
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

		void parameterValueChanged(Steinberg::int32 parameterId, Steinberg::Vst::ParamValue normalizedValue) SMTG_OVERRIDE;

	private:
		Steinberg::uint32 nbRef_;
		VieView* view_;
		FrequencyParameter* frequencyParameter_;
		Steinberg::Vst::IComponentHandler* componentHandler_;

		Steinberg::Vst::ParameterContainer parameters_;
	};
} // namespace
