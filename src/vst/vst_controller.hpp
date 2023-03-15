#pragma once

#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstnoteexpression.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/vst/vsttypes.h>
#include <pluginterfaces/base/iupdatehandler.h>

#include <map>
#include <unordered_map>
#include <vector>

#include "../vie_view.hpp"
#include "../parameter.hpp"
#include "../windows/vst_host_callback.hpp"

namespace live::tritone::vie::vst {
	class vst_controller final :
		public Steinberg::Vst::IEditController,
		public Steinberg::Vst::IConnectionPoint,
		public Steinberg::IDependent {
	public:
		static FUnknown* createInstance(void* /*context*/) {
			return static_cast<IEditController*>(new vst_controller);
		}

		vst_controller();

		~vst_controller();

		//Inheritance from IEditController
		Steinberg::tresult __stdcall queryInterface(const Steinberg::TUID iid, void** obj) override;
		Steinberg::uint32 __stdcall addRef() override;
		Steinberg::uint32 __stdcall release() override;
		Steinberg::tresult __stdcall initialize(Steinberg::FUnknown* context) override;
		Steinberg::tresult __stdcall terminate() override;
		Steinberg::tresult __stdcall setComponentState(Steinberg::IBStream* state) override;
		Steinberg::tresult __stdcall setState(Steinberg::IBStream* state) override;
		Steinberg::tresult __stdcall getState(Steinberg::IBStream* state) override;
		Steinberg::int32 __stdcall getParameterCount() override;
		Steinberg::tresult __stdcall getParameterInfo(Steinberg::int32 paramIndex, Steinberg::Vst::ParameterInfo& info) override;
		Steinberg::tresult __stdcall getParamStringByValue(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized, Steinberg::Vst::String128 string) override;
		Steinberg::tresult __stdcall getParamValueByString(Steinberg::Vst::ParamID id, Steinberg::Vst::TChar* string, Steinberg::Vst::ParamValue& valueNormalized) override;
		Steinberg::Vst::ParamValue __stdcall normalizedParamToPlain(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized) override;
		Steinberg::Vst::ParamValue __stdcall plainParamToNormalized(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue plainValue) override;
		Steinberg::Vst::ParamValue __stdcall getParamNormalized(Steinberg::Vst::ParamID id) override;
		Steinberg::tresult __stdcall setParamNormalized(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue value) override;
		Steinberg::tresult __stdcall setComponentHandler(Steinberg::Vst::IComponentHandler* handler) override;
		Steinberg::IPlugView* __stdcall createView(Steinberg::FIDString name) override;

		//Inheritance from IConnectionPoint
		Steinberg::tresult __stdcall connect(Steinberg::Vst::IConnectionPoint* other) override;
		Steinberg::tresult __stdcall disconnect(Steinberg::Vst::IConnectionPoint* other) override;
		Steinberg::tresult __stdcall notify(Steinberg::Vst::IMessage* message) override;

		//Inheritance from IDependent
		void __stdcall update(Steinberg::FUnknown* changed_unknown, Steinberg::int32 message) override;

	private:
		Steinberg::IPtr <Steinberg::FUnknown> host_context_;
		Steinberg::uint32 nb_ref_;
		vie_view* view_;
		vst_host_callback* vst_host_callback_;
	};
} // namespace
