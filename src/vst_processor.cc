#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/ivstevents.h>

#include <iostream>

#include "vst_plugin.h"
#include "vst_processor.h"
#include "application.h"

#include "pluginterfaces/base/ipluginbase.h"
#include <pluginterfaces/base/ustring.h>

using namespace std;
using json = nlohmann::basic_json<std::map, std::vector, std::u16string>;

namespace live::tritone::vie::vst
{
	Steinberg::FUnknown* vst_processor::createInstance(void* /*context*/)
	{
		return static_cast<IAudioProcessor*>(new vst_processor);
	}

	vst_processor::vst_processor() :
		nb_ref_(0)
	{
	}

	Steinberg::tresult __stdcall vst_processor::queryInterface(const Steinberg::TUID iid, void** obj)
	{
		if (Steinberg::FUnknownPrivate::iidEqual(iid, IComponent::iid))
		{
			addRef();
			*obj = static_cast<IComponent*>(this);
			return Steinberg::kResultOk;
		}

		if (Steinberg::FUnknownPrivate::iidEqual(iid, Steinberg::IPluginBase::iid))
		{
			addRef();
			*obj = reinterpret_cast<Steinberg::IPluginBase*>(this);
			return Steinberg::kResultOk;
		}

		if (Steinberg::FUnknownPrivate::iidEqual(iid, IAudioProcessor::iid))
		{
			addRef();
			*obj = static_cast<IAudioProcessor*>(this);
			return Steinberg::kResultOk;
		}

		if (Steinberg::FUnknownPrivate::iidEqual(iid, IProcessContextRequirements::iid))
		{
			addRef();
			*obj = static_cast<IProcessContextRequirements*>(this);
			return Steinberg::kResultOk;
		}

		if (Steinberg::FUnknownPrivate::iidEqual(iid, IConnectionPoint::iid))
		{
			addRef();
			*obj = static_cast<IConnectionPoint*>(this);
			return Steinberg::kResultOk;
		}

		if (Steinberg::FUnknownPrivate::iidEqual(iid, IDependent::iid))
		{
			addRef();
			*obj = static_cast<IDependent*>(this);
			return Steinberg::kResultOk;
		}

		*obj = nullptr;
		return Steinberg::kNoInterface;
	}

	Steinberg::uint32 __stdcall vst_processor::addRef()
	{
		return ++nb_ref_;
	}

	Steinberg::uint32 __stdcall vst_processor::release()
	{
		//FIXME: Try to delete this if 0.
		return --nb_ref_;
	}

	Steinberg::tresult __stdcall vst_processor::initialize(FUnknown* context)
	{
		host_context_ = context;

		try {
			vie_processor_->load("file path");
		}
		catch (const std::runtime_error exception) {
			return Steinberg::kResultFalse;
		}

		return Steinberg::kResultTrue;
	}

	Steinberg::tresult __stdcall vst_processor::terminate()
	{
		host_context_ = nullptr;

		vie_processor_->terminate();

		return Steinberg::kResultOk;
	}

	Steinberg::tresult __stdcall vst_processor::getControllerClassId(Steinberg::TUID class_id)
	{
		vst_plugin::controller_uid.toTUID(class_id);

		return Steinberg::kResultTrue;
	}

	Steinberg::tresult __stdcall vst_processor::setIoMode(Steinberg::Vst::IoMode mode)
	{
		return Steinberg::Vst::IoModes::kAdvanced;
	}

	Steinberg::int32 __stdcall vst_processor::getBusCount(Steinberg::Vst::MediaType
		type,
		Steinberg::Vst::BusDirection dir
	)
	{
		const core::MediaType media_type = (type == Steinberg::Vst::kAudio) ? core::MediaType::kAudio : core::MediaType::kEvent;
		const core::BusDirection direction = (dir == Steinberg::Vst::kInput) ? core::BusDirection::kInput : core::BusDirection::kOutput;

		return vie_processor_->getBusCount(media_type, direction);
	}

	Steinberg::tresult __stdcall vst_processor::getBusInfo(const Steinberg::Vst::MediaType type,
		const Steinberg::Vst::BusDirection dir,
		const Steinberg::int32 index,
		Steinberg::Vst::BusInfo& bus_info /*out*/)
	{
		const core::MediaType media_type = (type == Steinberg::Vst::kAudio) ? core::MediaType::kAudio : core::MediaType::kEvent;
		const core::BusDirection direction = (dir == Steinberg::Vst::kInput) ? core::BusDirection::kInput : core::BusDirection::kOutput;

		core::BusInfo core_bus_info{};

		if (vie_processor_->getBusInfo(media_type, direction, index, core_bus_info))
		{
			bus_info.busType = (core_bus_info.busType == core::BusType::kMain) ? Steinberg::Vst::kMain : Steinberg::Vst::kAux;
			bus_info.channelCount = core_bus_info.channelCount;
			bus_info.direction = (core_bus_info.direction == core::BusDirection::kInput)
				? Steinberg::Vst::kInput
				: Steinberg::Vst::kOutput;
			bus_info.flags = (core_bus_info.isDefaultActive ? Steinberg::Vst::BusInfo::kDefaultActive : 0)
				|| (core_bus_info.isControlVoltage ? Steinberg::Vst::BusInfo::kIsControlVoltage : 0);
			bus_info.mediaType = (core_bus_info.mediaType == core::MediaType::kAudio)
				? Steinberg::Vst::kAudio
				: Steinberg::Vst::kEvent;
			Steinberg::UString128 core_bus_info_name = Steinberg::UString128((wchar_t*) core_bus_info.name, 128);
			core_bus_info_name.copyTo(bus_info.name, 128);

			return Steinberg::kResultTrue;
		}

		return Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::getRoutingInfo(Steinberg::Vst::RoutingInfo& in_info,
		Steinberg::Vst::RoutingInfo& out_info /*out*/)
	{
		//TODO: describe how input buses are related to output buses.
		return Steinberg::kNotImplemented;
	}

	Steinberg::tresult __stdcall vst_processor::activateBus(const Steinberg::Vst::MediaType type,
		const Steinberg::Vst::BusDirection dir,
		const Steinberg::int32 index,
		const Steinberg::TBool state
	)
	{
		const core::MediaType media_type = (type == Steinberg::Vst::kAudio) ? core::MediaType::kAudio : core::MediaType::kEvent;
		const core::BusDirection direction = (dir == Steinberg::Vst::kInput) ? core::BusDirection::kInput : core::BusDirection::kOutput;

		const bool result = vie_processor_->activateBus(media_type, direction, index, state);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::setActive(const Steinberg::TBool active)
	{
		const bool result = vie_processor_->setActive(active);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::setState(Steinberg::IBStream* /*state*/)
	{
		return Steinberg::kResultOk;
	}

	Steinberg::tresult __stdcall vst_processor::getState(Steinberg::IBStream* /*state*/)
	{
		return Steinberg::kResultOk;
	}

	Steinberg::tresult __stdcall vst_processor::setBusArrangements(
		Steinberg::Vst::SpeakerArrangement* inputs,
		const Steinberg::int32 num_ins, Steinberg::Vst::SpeakerArrangement* outputs,
		const Steinberg::int32 num_outs)
	{
		const bool result = vie_processor_->setBusArrangement(inputs, num_ins, outputs, num_outs);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::getBusArrangement(const Steinberg::Vst::BusDirection dir,
		const Steinberg::int32 index,
		Steinberg::Vst::SpeakerArrangement& arr)
	{
		const core::BusDirection direction = (dir == Steinberg::Vst::kInput) ? core::BusDirection::kInput : core::BusDirection::kOutput;

		const bool result = vie_processor_->getBusArrangement(direction, index, arr);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::canProcessSampleSize(const Steinberg::int32 symbolic_sample_size)
	{
		if (symbolic_sample_size == Steinberg::Vst::kSample32 || symbolic_sample_size == Steinberg::Vst::kSample64)
		{
			return Steinberg::kResultTrue;
		}

		return Steinberg::kResultFalse;
	}

	Steinberg::uint32 __stdcall vst_processor::getLatencySamples()
	{
		//TODO: Get latency based on modules needs.
		return 64;
	}

	Steinberg::tresult __stdcall vst_processor::setupProcessing(Steinberg::Vst::ProcessSetup& setup)
	{
		core::ProcessingSetup core_processing_setup;  // NOLINT(cppcoreguidelines-pro-type-member-init) Reassigned in all path.
		core_processing_setup.maxSamplesPerBlock = setup.maxSamplesPerBlock;

		switch (setup.processMode)
		{
		case Steinberg::Vst::ProcessModes::kOffline:
			core_processing_setup.processingMode = core::ProcessingMode::kOffline;
			break;
		case Steinberg::Vst::ProcessModes::kPrefetch:
			core_processing_setup.processingMode = core::ProcessingMode::kPrefetch;
			break;
		default:
			core_processing_setup.processingMode = core::ProcessingMode::kRealtime;
		}

		core_processing_setup.sampleRate = setup.sampleRate;
		core_processing_setup.sampleSize = (setup.symbolicSampleSize == Steinberg::Vst::SymbolicSampleSizes::kSample32)
			? core::SampleSize::kSampleSize32
			: core::SampleSize::kSampleSize64;

		const bool result = vie_processor_->setupProcessing(core_processing_setup);

		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::setProcessing(Steinberg::TBool state)
	{
		const bool result = vie_processor_->setProcessing(state);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::process(Steinberg::Vst::ProcessData& data)
	{
		handle_input_parameter_changes(data.inputParameterChanges);
		process_input_events(data.inputEvents);
		core::AudioBusBuffers* input_buffers = new core::AudioBusBuffers[data.numInputs];
		process_input_data(input_buffers, data.inputs, data.numInputs, data.symbolicSampleSize);

		//TODO: handle data.inputs, data.outputParameterChanges and data.outputEvents;

		if (data.numOutputs > 0 && data.numSamples > 0)
		{
			core::OutputProcessData output_process_data;

			output_process_data.numOutputs = data.numOutputs;
			output_process_data.numSamples = data.numSamples;
			for (int i = 0; i < data.numOutputs; i++)
			{
				to_audio_bus_buffer(&output_process_data.outputs[i], data.outputs[i], data.symbolicSampleSize);
			}
			vie_processor_->processOutputData(output_process_data);
		}

		delete[] input_buffers;

		return Steinberg::kResultTrue;
	}

	Steinberg::uint32 __stdcall vst_processor::getTailSamples()
	{
		//TODO: Compute based on middle modules needs.
		return Steinberg::Vst::kNoTail;
	}

	Steinberg::uint32 __stdcall vst_processor::getProcessContextRequirements()
	{
		Steinberg::uint32 flags = 0;

		//FIXME: Do not request all but compute based on middle modules.
		flags |= kNeedSystemTime;
		flags |= kNeedContinousTimeSamples;
		flags |= kNeedProjectTimeMusic;
		flags |= kNeedBarPositionMusic;
		flags |= kNeedCycleMusic;
		flags |= kNeedSamplesToNextClock;
		flags |= kNeedTempo;
		flags |= kNeedTimeSignature;
		flags |= kNeedChord;
		flags |= kNeedFrameRate;
		flags |= kNeedTransportState;

		return flags;
	}

	Steinberg::tresult __stdcall vst_processor::connect(IConnectionPoint* other)
	{
		if (!other)
			return Steinberg::kInvalidArgument;

		return Steinberg::kResultOk;
	}

	Steinberg::tresult __stdcall vst_processor::disconnect(IConnectionPoint* other)
	{
		if (!other)
			return Steinberg::kInvalidArgument;

		return Steinberg::kResultOk;
	}

	Steinberg::tresult __stdcall vst_processor::notify(Steinberg::Vst::IMessage* message)
	{
		if (!message)
			return Steinberg::kInvalidArgument;

		return Steinberg::kResultFalse;
	}

	void __stdcall vst_processor::update(FUnknown* changed_unknown,
		Steinberg::int32 message
	)
	{
	}

	void vst_processor::handle_input_parameter_changes(Steinberg::Vst::IParameterChanges* input_parameter_changes)
	{
		if (!input_parameter_changes)
		{
			return;
		}

		const long parameters_count = input_parameter_changes->getParameterCount();
		for (auto index = 0; index < parameters_count; index++)
		{
			if (Steinberg::Vst::IParamValueQueue* param_value_queue = input_parameter_changes->getParameterData(index))
			{
				double parameter_value;
				Steinberg::int32 sample_offset;
				const long points_count = param_value_queue->getPointCount();
				const unsigned long parameter_id = param_value_queue->getParameterId();
				for (int i = 0; i < points_count; i++)
				{
					if (param_value_queue->getPoint(i, sample_offset, parameter_value) == Steinberg::kResultTrue)
					{
						vie_processor_->inputParameterChanged(parameter_id, sample_offset, parameter_value);
					}
				}
			}
		}
	}

	void vst_processor::process_input_events(Steinberg::Vst::IEventList* events) const
	{
		if (events)
		{
			const Steinberg::int32 event_count = events->getEventCount();
			for (Steinberg::int32 i = 0; i < event_count; i++)
			{
				Steinberg::Vst::Event vst_event{};
				events->getEvent(i, vst_event);
				core::Event event = to_event(vst_event);
				vie_processor_->processInputEvent(event);
			}
		}
	}

	void vst_processor::process_input_data(core::AudioBusBuffers* source_buffers, Steinberg::Vst::AudioBusBuffers* buffers, int32_t nb_buffers, int32_t sample_size) const
	{
		for (int32_t i = 0; i < nb_buffers; i++)
		{
			to_audio_bus_buffer(&source_buffers[i], buffers[i], sample_size);
			vie_processor_->processInputAudio(&source_buffers[i], i);
		}
	}

	core::Event vst_processor::to_event(Steinberg::Vst::Event& vst_event) const
	{
		core::Event event{};
		event.busIndex = vst_event.busIndex;
		event.sampleOffset = vst_event.sampleOffset;
		event.position = vst_event.ppqPosition;
		event.isLive = (vst_event.flags & Steinberg::Vst::Event::EventFlags::kIsLive);
		switch (vst_event.type)
		{
		case Steinberg::Vst::Event::EventTypes::kNoteOnEvent:
			event.type = core::Event::Type::kNoteOn;
			event.coreEvent.kNoteOn = to_note_event(vst_event.noteOn);
			break;
		case Steinberg::Vst::Event::EventTypes::kNoteOffEvent:
			event.type = core::Event::Type::kNoteOff;
			event.coreEvent.kNoteOff = to_note_event(vst_event.noteOff);
			break;
		case Steinberg::Vst::Event::EventTypes::kDataEvent:
			event.type = core::Event::Type::kDataEvent;
			event.coreEvent.kData = to_data_event(vst_event.data);
			break;
		default:
			break;
		}

		//FIXME: handle all event types.
		return event;
	}

	core::NoteEvent vst_processor::to_note_event(Steinberg::Vst::NoteOnEvent& vst_note_on_event) const
	{
		return {
			0,
			vst_note_on_event.channel,
			vst_note_on_event.pitch,
			vst_note_on_event.tuning,
			vst_note_on_event.velocity
		};
	}

	core::NoteEvent vst_processor::to_note_event(Steinberg::Vst::NoteOffEvent& vst_note_off_event) const
	{
		return {
			0,
			vst_note_off_event.channel, 
			vst_note_off_event.pitch,
			vst_note_off_event.tuning,
			vst_note_off_event.velocity
		};
	}

	core::DataEvent vst_processor::to_data_event(Steinberg::Vst::DataEvent& vst_data_event) const
	{
		return {
			vst_data_event.size,
			vst_data_event.type,
			vst_data_event.bytes
		};
	}

	void vst_processor::to_audio_bus_buffer(core::AudioBusBuffers* source, Steinberg::Vst::AudioBusBuffers& vst_audio_bus_buffers,
	                                                     int sample_size) const
	{
		if (sample_size == Steinberg::Vst::kSample32)
		{
			source->channelsBuffer = reinterpret_cast<void**>(vst_audio_bus_buffers.channelBuffers32);
			source->sampleSize = core::SampleSize::kSampleSize32;
		}
		else
		{
			source->channelsBuffer = reinterpret_cast<void**>(vst_audio_bus_buffers.channelBuffers64);
			source->sampleSize = core::SampleSize::kSampleSize64;
		}
		source->numChannels = vst_audio_bus_buffers.numChannels;
		source->silenceFlags = vst_audio_bus_buffers.silenceFlags;
	}
} // namespace
