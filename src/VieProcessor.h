#pragma once

#include <pluginterfaces/vst/ivstcomponent.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivstprocesscontext.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/base/iupdatehandler.h>
#include <pluginterfaces/vst/ivstevents.h>

#include <q/synth/sin.hpp>
#include <q/synth/envelope.hpp>
#include <q/fx/lowpass.hpp>
#include <q/fx/waveshaper.hpp>

#include <map>
#include <vector>

#include "Bus.h"

namespace live::tritone::vie {
	class VieProcessor : 
		//public Steinberg::Vst::AudioEffect,
		public Steinberg::Vst::IComponent, 
		public Steinberg::Vst::IAudioProcessor, 
		public Steinberg::Vst::IProcessContextRequirements,
		public Steinberg::Vst::IConnectionPoint,
		public Steinberg::IDependent
	{
	public:
		static Steinberg::FUnknown* createInstance(void* context);

		VieProcessor();

		//Inheritance from FUnknown
		Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API addRef() SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API release() SMTG_OVERRIDE;

		//Inheritance from IPluginBase
		Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

		//Inheritance from IComponent
		Steinberg::tresult PLUGIN_API getControllerClassId(Steinberg::TUID classId) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setIoMode(Steinberg::Vst::IoMode mode) SMTG_OVERRIDE;
		Steinberg::int32 PLUGIN_API getBusCount(Steinberg::Vst::MediaType type, Steinberg::Vst::BusDirection dir) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getBusInfo(Steinberg::Vst::MediaType type, Steinberg::Vst::BusDirection dir, Steinberg::int32 index, Steinberg::Vst::BusInfo& bus /*out*/) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getRoutingInfo(Steinberg::Vst::RoutingInfo& inInfo, Steinberg::Vst::RoutingInfo& outInfo /*out*/) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API activateBus(Steinberg::Vst::MediaType type, Steinberg::Vst::BusDirection dir, Steinberg::int32 index, Steinberg::TBool state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;

		//Inheritance from IAudioProcessor
		Steinberg::tresult PLUGIN_API setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs, Steinberg::int32 numIns, Steinberg::Vst::SpeakerArrangement* outputs, Steinberg::int32 numOuts) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getBusArrangement(Steinberg::Vst::BusDirection dir, Steinberg::int32 index, Steinberg::Vst::SpeakerArrangement& arr) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API getLatencySamples() SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup& setup) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setProcessing(Steinberg::TBool state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		Steinberg::uint32 PLUGIN_API getTailSamples() SMTG_OVERRIDE;

		//Inheritance from IProcessContextRequirements
		Steinberg::uint32 PLUGIN_API getProcessContextRequirements() SMTG_OVERRIDE;

		//Inheritance from IConnectionPoint
		Steinberg::tresult PLUGIN_API connect(Steinberg::Vst::IConnectionPoint* other) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API disconnect(Steinberg::Vst::IConnectionPoint* other) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) SMTG_OVERRIDE;

		//Inheritance from IDependent
		void PLUGIN_API update(Steinberg::FUnknown* changedUnknown, Steinberg::int32 message);

	private:
		bool active_;

		Steinberg::IPtr<Steinberg::FUnknown> hostContext;
		Steinberg::uint32 nbRef_;

		template<typename SampleType> Steinberg::tresult processSamples(Steinberg::Vst::AudioBusBuffers& output, Steinberg::int32 numSamples);

		template<typename SampleType> SampleType** getBuffer(Steinberg::Vst::AudioBusBuffers& buffer);
		template<> Steinberg::Vst::Sample32** getBuffer(Steinberg::Vst::AudioBusBuffers& buffer);
		template<> Steinberg::Vst::Sample64** getBuffer(Steinberg::Vst::AudioBusBuffers& buffer);

		void handleParameterChanges(Steinberg::Vst::IParameterChanges* inputParameterChanges);
		void processEvents(Steinberg::Vst::IEventList* events);

		std::vector<Bus*>* getBuses(Steinberg::Vst::MediaType type, Steinberg::Vst::BusDirection dir);
		Bus* getBus(Steinberg::Vst::MediaType type, Steinberg::Vst::BusDirection dir, int index);

		float frequencyMultiplicator_;
		bool bypass_;

		Steinberg::Vst::ProcessSetup processSetup_;

		//std::vector<Steinberg::Vst::SpeakerArrangement> inputSpeakerArrangements;
		//std::vector<Steinberg::Vst::SpeakerArrangement> outputSpeakerArrangements;

		std::vector<Bus*> audioInputBuses;
		std::vector<Bus*> audioOutputBuses;
		std::vector<Bus*> eventInputBuses;
		std::vector<Bus*> eventOutputBuses;

		std::map<Steinberg::int32, Steinberg::Vst::NoteOnEvent> notes_;
		std::map<Steinberg::int32, cycfi::q::phase_iterator> phases_;
		cycfi::q::phase_iterator phase_;
		cycfi::q::envelope* envelope_;
		cycfi::q::soft_clip clip_;
	};
} // namespace
