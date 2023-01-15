#pragma once

#include <pluginterfaces/vst/ivstcomponent.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/base/iupdatehandler.h>
#include <pluginterfaces/vst/ivstevents.h>

#include "processor_definition.hpp"
#include "processor_component.hpp"
#include "vie_processor.hpp"

namespace live::tritone::vie {
	class vst_processor final :
		public Steinberg::Vst::IComponent,
		public Steinberg::Vst::IAudioProcessor,
		public Steinberg::Vst::IProcessContextRequirements,
		public Steinberg::Vst::IConnectionPoint,
		public Steinberg::IDependent {
	public:
		static Steinberg::FUnknown* createInstance(void* context);

		vst_processor();
		vst_processor(const vst_processor&) = default;
		vst_processor(vst_processor&&) = default;

		virtual ~vst_processor() = default;

		vst_processor& operator=(const vst_processor&) = default;
		vst_processor& operator=(vst_processor&&) = default;

		//Inheritance from FUnknown
		Steinberg::tresult __stdcall queryInterface(const Steinberg::TUID iid, void** obj) override;
		Steinberg::uint32 __stdcall addRef() override;
		Steinberg::uint32 __stdcall release() override;

		//Inheritance from IPluginBase
		Steinberg::tresult __stdcall initialize(Steinberg::FUnknown* context) override;
		Steinberg::tresult __stdcall terminate() override;

		//Inheritance from IComponent
		Steinberg::tresult __stdcall getControllerClassId(Steinberg::TUID class_id) override;
		Steinberg::tresult __stdcall setIoMode(Steinberg::Vst::IoMode mode) override;
		Steinberg::int32 __stdcall getBusCount(Steinberg::Vst::MediaType type,Steinberg::Vst::BusDirection dir) override;
		Steinberg::tresult __stdcall getBusInfo(Steinberg::Vst::MediaType type,Steinberg::Vst::BusDirection dir, Steinberg::int32 index,Steinberg::Vst::BusInfo& bus_info /*out*/) override;
		Steinberg::tresult __stdcall  getRoutingInfo(Steinberg::Vst::RoutingInfo& in_info,Steinberg::Vst::RoutingInfo& out_info /*out*/) override;
		Steinberg::tresult __stdcall activateBus(Steinberg::Vst::MediaType type,Steinberg::Vst::BusDirection dir, Steinberg::int32 index,Steinberg::TBool state) override;
		Steinberg::tresult __stdcall setActive(Steinberg::TBool active) override;
		Steinberg::tresult __stdcall setState(Steinberg::IBStream* state) override;
		Steinberg::tresult __stdcall getState(Steinberg::IBStream* state) override;

		//Inheritance from IAudioProcessor
		Steinberg::tresult __stdcall setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs,Steinberg::int32 num_ins, Steinberg::Vst::SpeakerArrangement* outputs,Steinberg::int32 num_outs) override;
		Steinberg::tresult __stdcall getBusArrangement(Steinberg::Vst::BusDirection dir,Steinberg::int32 index, Steinberg::Vst::SpeakerArrangement& arr) override;
		Steinberg::tresult __stdcall canProcessSampleSize(Steinberg::int32 symbolic_sample_size) override;
		Steinberg::uint32 __stdcall getLatencySamples() override;
		Steinberg::tresult __stdcall setupProcessing(Steinberg::Vst::ProcessSetup& setup) override;
		Steinberg::tresult __stdcall setProcessing(Steinberg::TBool state) override;
		Steinberg::tresult __stdcall process(Steinberg::Vst::ProcessData& data) override;
		Steinberg::uint32 __stdcall getTailSamples() override;

		//Inheritance from IProcessContextRequirements
		Steinberg::uint32 __stdcall getProcessContextRequirements() override;

		//Inheritance from IConnectionPoint
		Steinberg::tresult __stdcall connect(Steinberg::Vst::IConnectionPoint* other) override;
		Steinberg::tresult __stdcall disconnect(Steinberg::Vst::IConnectionPoint* other) override;
		Steinberg::tresult __stdcall notify(Steinberg::Vst::IMessage* message) override;

		//Inheritance from IDependent
		void __stdcall update(Steinberg::FUnknown* changed_unknown,Steinberg::int32 message) override;

	private:
		event to_event(Steinberg::Vst::Event& vst_event) const;

		note_event to_note_event(Steinberg::Vst::NoteOnEvent& vst_note_on_event) const;

		note_event to_note_event(Steinberg::Vst::NoteOffEvent& vst_note_off_event) const;

		data_event to_data_event(Steinberg::Vst::DataEvent& vst_data_event) const;

		void to_audio_bus_buffer(audio_bus_buffers* source, Steinberg::Vst::AudioBusBuffers& vst_audio_bus_buffers, int sample_size) const;

		void handle_input_parameter_changes(Steinberg::Vst::IParameterChanges* input_parameter_changes);

		/**
		 * Send events before processing to processor components capable of handling them.
		*/
		void process_input_events(Steinberg::Vst::IEventList* events) const;

		/**
		 * Send data before processing to processor components capable of handling them.
		*/
		void process_input_data(audio_bus_buffers* source_buffers, Steinberg::Vst::AudioBusBuffers* buffers, int32_t nb_buffers, int32_t sample_size) const;

		vie_processor vie_processor_;

		Steinberg::IPtr <Steinberg::FUnknown> host_context_;
		Steinberg::uint32 nb_ref_;
	};
} // namespace
