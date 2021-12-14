#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/ivstevents.h>
#include <base/source/fstreamer.h>

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
		frequencyMultiplicator_(0.f),
		phases_(),
		bypass_(false),
		notes_(),
		envelope_(nullptr)
	{
		setControllerClass(live::tritone::vie::ControllerUID);
	}

	tresult PLUGIN_API VieProcessor::initialize(FUnknown* context)
	{
		tresult result = AudioEffect::initialize(context);
		if (result == kResultTrue)
		{
			addEventInput(STR16("MIDI input"), 1);
			addAudioOutput(STR16("Stereo output"), SpeakerArr::kStereo);
		}

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

		return result;
	}

	tresult PLUGIN_API VieProcessor::setupProcessing(ProcessSetup& setup) {
		AudioEffect::setupProcessing(setup);

		envelope_ = new envelope(processSetup.sampleRate);

		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
	{
		// We only support one stereo output stereo bus.
		if (numIns == 0 && numOuts == 1 && outputs[0] == SpeakerArr::kStereo)
		{
			return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
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

	tresult PLUGIN_API VieProcessor::setState(IBStream* /*state*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::getState(IBStream* /*state*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieProcessor::process(ProcessData& data)
	{
		tresult result = kResultOk;

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

		return result;
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
				phase_.set(qFrequency, processSetup.sampleRate);

				for (int frame = 0; frame < numSamples; ++frame) {
					auto env_ = (*envelope_)();

					float value = q::sin(phase_);

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
				case kFrequencyId:
					if (paramValueQueue->getPoint(pointsCount - 1, sampleOffset, paramValue) == kResultTrue) {
						frequencyMultiplicator_ = paramValue;
					}
					break;

				case kBypassId:
					if (paramValueQueue->getPoint(pointsCount - 1, sampleOffset, paramValue) == kResultTrue)
					{
						bypass_ = (paramValue > 0.5f);
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
} // namespace
