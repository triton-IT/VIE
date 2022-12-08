#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/ivstevents.h>

#include <q/support/literals.hpp>

#include <stdio.h>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

#include "VieProcessor.h"
#include "Application.h"
#include "Constants.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace cycfi;
using namespace cycfi::q;
using namespace cycfi::q::literals;

namespace live::tritone::vie {
	FUnknown* VieProcessor::createInstance(void* /*context*/)
	{
		return (IAudioProcessor*) new VieProcessor;
	}

	VieProcessor::VieProcessor() :
		active_(false),
		processing_(false),
		frequencyMultiplicator_(0.f),
		waveForm_(.0f),
		phases_(),
		bypass_(false),
		notes_(),
		envelope_(nullptr),
		nbRef_(0),
		processSetup_()
	{
		//FIXME: Just for testing. Should be computed from configuration file.
		Bus* audioInputBus = new Bus(std::wstring(L"Audio input"), BusType::main);
		audioInputBuses.push_back(audioInputBus);
		Bus* audioOutputBus = new Bus(std::wstring(L"Audio output"), BusType::main);
		audioOutputBuses.push_back(audioOutputBus);
		Bus* eventInputBus = new Bus(std::wstring(L"Event input"), BusType::main);
		eventInputBuses.push_back(eventInputBus);
		Bus* eventOutputBus = new Bus(std::wstring(L"Event output"), BusType::main);
		eventOutputBuses.push_back(eventOutputBus);
	}

	tresult PLUGIN_API VieProcessor::queryInterface(const TUID iid, void** obj) {
		if (FUnknownPrivate::iidEqual(iid, IComponent::iid)) {
			addRef();
			*obj = static_cast<IComponent*>(this);
			return kResultOk;
		}
		if (FUnknownPrivate::iidEqual(iid, IPluginBase::iid)) {
			addRef();
			*obj = static_cast<IPluginBase*>(this);
			return kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IAudioProcessor::iid)) {
			addRef();
			*obj = static_cast<IAudioProcessor*>(this);
			return Steinberg::kResultOk;
		}

		if (FUnknownPrivate::iidEqual(iid, IProcessContextRequirements::iid)) {
			addRef();
			*obj = static_cast<IProcessContextRequirements*>(this);
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

	uint32 PLUGIN_API VieProcessor::addRef() {
		return ++nbRef_;
	}

	uint32 PLUGIN_API VieProcessor::release() {
		//FIXME: Try to delete this if 0.
		return --nbRef_;
	}

	tresult PLUGIN_API VieProcessor::initialize(FUnknown* context)
	{
		hostContext = context;
		
		/*auto processorsDefinition = R"(
		{
			"processor": {
				"name": "oscillator",
				"id": "F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4",
				"type": "live.tritone.oscillator",
				"parameters": [
					"waveform": "saw"
				]
			},
			"processor": {
				"signal_inputs": [
					"input": {
						"name": "signal",
						"type": "signal"
					}
				]
			}
		)"_json;

		auto patchBayDefinition = R"(
		{
			"processor": {
				"id": "F9168C5E-CEB2-4faa-B6BF-329BF39FA1E4",
				"type": "live.tritone.oscillator",
				"parameters": [
					"waveform": "saw"
				]
			},
			"processor": {
				"name": "oscillator",
				"signal_inputs": [
					"input": {
						"name": "signal",
						"type": "signal"
					}
				]
			}
		)"_json;*/

		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::terminate() {
		hostContext = nullptr;

		return kResultOk;
	}

	Steinberg::tresult PLUGIN_API VieProcessor::getControllerClassId(TUID classId) {
		live::tritone::vie::ControllerUID.toTUID(classId);

		return kResultTrue;
	}

	Steinberg::tresult PLUGIN_API VieProcessor::setIoMode(IoMode mode) {
		DLOG("setIoMode requested but not implememented!");
		return kNotImplemented;
	}

	Steinberg::int32 PLUGIN_API VieProcessor::getBusCount(MediaType type, BusDirection dir) {
		std::vector<Bus*>* buses = getBuses(type, dir);
		return buses->size();
	}

	Steinberg::tresult PLUGIN_API VieProcessor::getBusInfo(MediaType type, BusDirection dir, int32 index, BusInfo& busInfo /*out*/) {
		Bus* bus = getBus(type, dir, index);

		if (bus) {
			busInfo.mediaType = type;
			busInfo.direction = dir;
			busInfo.channelCount = bus->getChannelCount();
			strcpy16(busInfo.name, bus->getName().c_str());
			switch (bus->getType()) {
			case BusType::main:
				busInfo.busType = BusTypes::kMain;
				break;
			case BusType::aux:
				busInfo.busType = BusTypes::kAux;
				break;
			}
			busInfo.flags = bus->getFlags();

			return kResultTrue;
		}

		return kResultFalse;
	}

	Steinberg::tresult PLUGIN_API VieProcessor::getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo /*out*/) {
		DLOG("getRoutingInfo requested but not implememented!");
		return kNotImplemented;
	}

	Steinberg::tresult PLUGIN_API VieProcessor::activateBus(MediaType type, BusDirection dir, int32 index, TBool state) {
		Bus* bus = getBus(type, dir, index);

		if (bus) {
			bus->setActive(true);

			return kResultTrue;
		}

		return kResultFalse;
	}

	Steinberg::tresult PLUGIN_API VieProcessor::setActive(TBool state) {
		active_ = state==1?true:false;
		return kResultTrue;
	}

	tresult PLUGIN_API VieProcessor::setState(IBStream* /*state*/)
	{
		DLOG("setState requested but not implememented!");
		return kNotImplemented;
	}

	tresult PLUGIN_API VieProcessor::getState(IBStream* /*state*/)
	{
		DLOG("getState requested but not implememented!");
		return kNotImplemented;
	}

	tresult PLUGIN_API VieProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
	{
		// We support what the host wants us to support;
		for (int32 i = 0; i < static_cast<int32> (numIns); ++i)
		{
			Bus* bus = audioInputBuses.at(i);
			bus->setArrangement(inputs[i]);
		}

		for (int32 i = 0; i < static_cast<int32> (numOuts); ++i)
		{
			Bus* bus = audioOutputBuses.at(i);
			bus->setArrangement(outputs[i]);
		}
		return kResultTrue;
	}

	Steinberg::tresult PLUGIN_API VieProcessor::getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr) {
		Bus* bus = getBus(kAudio, dir, index);

		if(bus) {
			arr = bus->getArrangement();
			return kResultTrue;
		}
		return kResultFalse;

	}

	tresult PLUGIN_API VieProcessor::canProcessSampleSize(int32 symbolicSampleSize)
	{
		if (symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64)
		{
			return kResultTrue;
		}
		return kResultFalse;
	}

	uint32 PLUGIN_API VieProcessor::getLatencySamples() {
		//TODO: Compute needed latency based on components used.
		return 0;
	}

	tresult PLUGIN_API VieProcessor::setupProcessing(ProcessSetup& setup) {
		processSetup_.sampleRate = setup.sampleRate;
		processSetup_.processMode = setup.processMode;
		processSetup_.maxSamplesPerBlock = setup.maxSamplesPerBlock;
		processSetup_.symbolicSampleSize = setup.symbolicSampleSize;

		envelope_ = new envelope(setup.sampleRate);

		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::setProcessing(TBool state) {
		processing_ = state;

		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::process(ProcessData& data)
	{
		tresult result = kResultOk;
		if (active_ && processing_) {
			handleParameterChanges(data.inputParameterChanges);
			processEvents(data.inputEvents);

			if (data.numOutputs > 0 && data.numSamples > 0) {
				if (data.symbolicSampleSize == kSample32) {
					result = processSamples<Sample32>(data.outputs[0], data.numSamples);
				}
				else {
					result = processSamples<Sample64>(data.outputs[0], data.numSamples);
				}
			}
		}

		return result;
	}

	uint32 PLUGIN_API VieProcessor::getTailSamples() {
		//TODO: Compute based on internal components.
		return kNoTail;
	}

	uint32 PLUGIN_API VieProcessor::getProcessContextRequirements() {
		uint32 flags = 0;

		//TODO: Do not request all but compute based on internal components.

		//Plugin is generic, so get as many context requirements as possible.
		flags |= Flags::kNeedSystemTime;
		flags |= Flags::kNeedContinousTimeSamples;
		flags |= Flags::kNeedProjectTimeMusic;
		flags |= Flags::kNeedBarPositionMusic;
		flags |= Flags::kNeedCycleMusic;
		flags |= Flags::kNeedSamplesToNextClock;
		flags |= Flags::kNeedTempo;
		flags |= Flags::kNeedTimeSignature;
		flags |= Flags::kNeedChord;
		flags |= Flags::kNeedFrameRate;
		flags |= Flags::kNeedTransportState;

		return flags;
	}

	tresult PLUGIN_API VieProcessor::connect(IConnectionPoint* other)
	{
		if (!other)
			return kInvalidArgument;

		DLOG("Connection to another connection point requested. should store and manage peer connection.");
		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::disconnect(IConnectionPoint* other)
	{
		if (!other)
			return kInvalidArgument;

		DLOG("Disconnection from another connection point requested.");
		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::notify(IMessage* message)
	{
		DLOG("Notification received from the other connection point");
		if (!message)
			return kInvalidArgument;

		DLOG(message->getMessageID());

		return kResultFalse;
	}

	void PLUGIN_API VieProcessor::update(Steinberg::FUnknown* changedUnknown, Steinberg::int32 message) {
		DLOG("Update request received.");
	}

	template<typename SampleType> tresult VieProcessor::processSamples(AudioBusBuffers& output, int32 numSamples)
	{
		if (!bypass_) {
			SampleType** outputSamples = getBuffer<SampleType>(output);

			SampleType* left = outputSamples[0];
			SampleType* right = outputSamples[1];

			memset(left, 0, numSamples * sizeof(SampleType));
			memset(right, 0, numSamples * sizeof(SampleType));
			
			for (auto const& [noteId, NoteOnEvent] : notes_) {
				float rawFrequency = MIDI_NOTES_FREQUENCIES[NoteOnEvent.pitch] * (1.0f + frequencyMultiplicator_);
				frequency qFrequency = q::frequency(rawFrequency);
				phase_.set(qFrequency, processSetup_.sampleRate);

				for (int frame = 0; frame < numSamples; ++frame) {
					auto env_ = (*envelope_)();

					float value = .0f;
					if (waveForm_ == .0f) {
						value = q::sin(phase_);
					} else {
						value = q::saw(phase_);
					}

					value = clip_(value * env_);

					right[frame] = left[frame] = value;
					phase_++;
				}
			}

		}

		return kResultOk;
	}

	template<> Sample32** VieProcessor::getBuffer(AudioBusBuffers& buffer)
	{
		return buffer.channelBuffers32;
	}

	template<> Sample64** VieProcessor::getBuffer(AudioBusBuffers& buffer)
	{
		return buffer.channelBuffers64;
	}

	void VieProcessor::handleParameterChanges(IParameterChanges* inputParameterChanges) {
		if (!inputParameterChanges)
		{
			return;
		}

		int32 parametersCount = inputParameterChanges->getParameterCount();
		for (auto index = 0; index < parametersCount; index++)
		{
			IParamValueQueue* paramValueQueue = inputParameterChanges->getParameterData(index);
			if (paramValueQueue)
			{
				ParamValue paramValue;
				int32 sampleOffset;
				int32 pointsCount = paramValueQueue->getPointCount();
				switch (paramValueQueue->getParameterId())
				{
				case kBypassId:
					if (paramValueQueue->getPoint(pointsCount - 1, sampleOffset, paramValue) == kResultTrue)
					{
						bypass_ = (paramValue > 0.5f);
					}
					break;

				case kFrequencyId:
					if (paramValueQueue->getPoint(pointsCount - 1, sampleOffset, paramValue) == kResultTrue) {
						frequencyMultiplicator_ = paramValue;
					}
					break;

				case kWaveFormId:
					if (paramValueQueue->getPoint(pointsCount - 1, sampleOffset, paramValue) == kResultTrue)
					{
						waveForm_ = (paramValue > 0.5f);
					}
					break;
				}
			}
		}
	}

	void VieProcessor::processEvents(IEventList* events)
	{
		if (events)
		{
			int32 eventCount = events->getEventCount();
			for (int32 i = 0; i < eventCount; i++)
			{
				Event event;
				events->getEvent(i, event);
				switch (event.type)
				{
				case Event::kNoteOnEvent:
				{
					notes_[event.noteOn.noteId] = event.noteOn;
					envelope_->trigger(event.noteOn.velocity);
					break;
				}
				case Event::kNoteOffEvent:
				{
					notes_.erase(event.noteOff.noteId);
					envelope_->release();
					break;
				}
				default:
					continue;
				}
			}
		}
	}

	std::vector<Bus*>* VieProcessor::getBuses(MediaType type, BusDirection dir) {
		std::vector<Bus*>* buses = nullptr;
		switch (type) {
		case kAudio:
			switch (dir)
			{
			case kInput:
				buses = &audioInputBuses;
				break;
			case kOutput:
				buses = &audioOutputBuses;
				break;
			}
		case kEvent:
			switch (dir)
			{
			case kInput:
				buses = &eventInputBuses;
				break;
			case kOutput:
				buses = &eventOutputBuses;
				break;
			}
			break;
		}

		return buses;
	}

	Bus* VieProcessor::getBus(Steinberg::Vst::MediaType type, Steinberg::Vst::BusDirection dir, int index) {
		std::vector<Bus*>* buses = getBuses(type, dir);
		if (buses->size() > index) {
			return buses->at(index);
		}

		return nullptr;
	}
} // namespace
