#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/ivstevents.h>

#include <q/support/literals.hpp>

#include <iostream>

#include "vst_processor.hpp"
#include "application.hpp"
#include "constants.hpp"

#include "pluginterfaces/base/ipluginbase.h"

using namespace std;
using json = nlohmann::basic_json<std::map, std::vector, std::wstring>;

namespace live::tritone::vie
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
			vie_processor_.initialize();
		}
		catch (const std::runtime_error exception) {
			return Steinberg::kResultFalse;
		}

		return Steinberg::kResultTrue;
	}

	Steinberg::tresult __stdcall vst_processor::terminate()
	{
		host_context_ = nullptr;

		vie_processor_.terminate();

		return Steinberg::kResultOk;
	}

	Steinberg::tresult __stdcall vst_processor::getControllerClassId(Steinberg::TUID class_id)
	{
		controller_uid.toTUID(class_id);

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
		const media_type media_type = (type == Steinberg::Vst::kAudio) ? media_type::audio : media_type::event;
		const bus_direction direction = (dir == Steinberg::Vst::kInput) ? bus_direction::input : bus_direction::output;

		return vie_processor_.get_bus_count(media_type, direction);
	}

	Steinberg::tresult __stdcall vst_processor::getBusInfo(const Steinberg::Vst::MediaType type,
		const Steinberg::Vst::BusDirection dir,
		const Steinberg::int32 index,
		Steinberg::Vst::BusInfo& bus_info /*out*/)
	{
		const media_type media_type = (type == Steinberg::Vst::kAudio) ? media_type::audio : media_type::event;
		const bus_direction direction = (dir == Steinberg::Vst::kInput) ? bus_direction::input : bus_direction::output;

		vie::bus_info core_bus_info{};

		if (vie_processor_.get_bus_info(media_type, direction, index, core_bus_info))
		{
			bus_info.busType = (core_bus_info.bus_type == bus_type::main) ? Steinberg::Vst::kMain : Steinberg::Vst::kAux;
			bus_info.channelCount = core_bus_info.channel_count;
			bus_info.direction = (core_bus_info.direction == bus_direction::input)
				? Steinberg::Vst::kInput
				: Steinberg::Vst::kOutput;
			bus_info.flags = (core_bus_info.is_default_active ? Steinberg::Vst::BusInfo::kDefaultActive : 0)
				|| (core_bus_info.is_control_voltage ? Steinberg::Vst::BusInfo::kIsControlVoltage : 0);
			bus_info.mediaType = (core_bus_info.media_type == media_type::audio)
				? Steinberg::Vst::kAudio
				: Steinberg::Vst::kEvent;
			wcscpy(bus_info.name, core_bus_info.name);

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
		const media_type media_type = (type == Steinberg::Vst::kAudio) ? media_type::audio : media_type::event;
		const bus_direction direction = (dir == Steinberg::Vst::kInput) ? bus_direction::input : bus_direction::output;

		const bool result = vie_processor_.activate_bus(media_type, direction, index, state);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::setActive(const Steinberg::TBool active)
	{
		const bool result = vie_processor_.set_active(active);
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
		const bool result = vie_processor_.set_bus_arrangement(inputs, num_ins, outputs, num_outs);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::getBusArrangement(const Steinberg::Vst::BusDirection dir,
		const Steinberg::int32 index,
		Steinberg::Vst::SpeakerArrangement& arr)
	{
		const bus_direction direction = (dir == Steinberg::Vst::kInput) ? bus_direction::input : bus_direction::output;

		const bool result = vie_processor_.get_bus_arrangement(direction, index, arr);
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
		//TODO: Get latency based on components needs.
		return 64;
	}

	Steinberg::tresult __stdcall vst_processor::setupProcessing(Steinberg::Vst::ProcessSetup& setup)
	{
		processing_setup core_processing_setup;  // NOLINT(cppcoreguidelines-pro-type-member-init) Reassigned in all path.
		core_processing_setup.max_samples_per_block = setup.maxSamplesPerBlock;

		switch (setup.processMode)
		{
		case Steinberg::Vst::ProcessModes::kOffline:
			core_processing_setup.processing_mode = processing_mode::offline;
			break;
		case Steinberg::Vst::ProcessModes::kPrefetch:
			core_processing_setup.processing_mode = processing_mode::prefetch;
			break;
		default:
			core_processing_setup.processing_mode = processing_mode::realtime;
		}

		core_processing_setup.sample_rate = setup.sampleRate;
		core_processing_setup.sample_size = (setup.symbolicSampleSize == Steinberg::Vst::SymbolicSampleSizes::kSample32)
			? sample_size::sample_size32
			: sample_size::sample_size64;

		const bool result = vie_processor_.setup_processing(core_processing_setup);

		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::setProcessing(Steinberg::TBool state)
	{
		const bool result = vie_processor_.set_processing(state);
		return result ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}

	Steinberg::tresult __stdcall vst_processor::process(Steinberg::Vst::ProcessData& data)
	{
		handle_input_parameter_changes(data.inputParameterChanges);
		process_input_events(data.inputEvents);
		audio_bus_buffers* input_buffers = new audio_bus_buffers[data.numInputs];
		process_input_data(input_buffers, data.inputs, data.numInputs, data.symbolicSampleSize);

		//TODO: handle data.inputs, data.outputParameterChanges and data.outputEvents;

		if (data.numOutputs > 0 && data.numSamples > 0)
		{
			output_process_data output_process_data;

			output_process_data.num_outputs = data.numOutputs;
			output_process_data.num_samples = data.numSamples;
			for (int i = 0; i < data.numOutputs; i++)
			{
				to_audio_bus_buffer(&output_process_data.outputs[i], data.outputs[i], data.symbolicSampleSize);
			}
			vie_processor_.process_output_data(output_process_data);
		}

		delete[] input_buffers;

		return Steinberg::kResultTrue;
	}

	Steinberg::uint32 __stdcall vst_processor::getTailSamples()
	{
		//TODO: Compute based on middle components needs.
		return Steinberg::Vst::kNoTail;
	}

	Steinberg::uint32 __stdcall vst_processor::getProcessContextRequirements()
	{
		Steinberg::uint32 flags = 0;

		//FIXME: Do not request all but compute based on middle components.
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
				long sample_offset;
				const long points_count = param_value_queue->getPointCount();
				const unsigned long parameter_id = param_value_queue->getParameterId();
				for (int i = 0; i < points_count; i++)
				{
					if (param_value_queue->getPoint(i, sample_offset, parameter_value) == Steinberg::kResultTrue)
					{
						vie_processor_.input_parameter_changed(parameter_id, sample_offset, parameter_value);
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
				event event = to_event(vst_event);
				vie_processor_.process_input_event(event);
			}
		}
	}

	void vst_processor::process_input_data(audio_bus_buffers* source_buffers, Steinberg::Vst::AudioBusBuffers* buffers, int32_t nb_buffers, int32_t sample_size) const
	{
		for (int32_t i = 0; i < nb_buffers; i++)
		{
			to_audio_bus_buffer(&source_buffers[i], buffers[i], sample_size);
			vie_processor_.process_input_audio(&source_buffers[i], i);
		}
	}

	event vst_processor::to_event(Steinberg::Vst::Event& vst_event) const
	{
		event event{};
		event.bus_index = vst_event.busIndex;
		event.sample_offset = vst_event.sampleOffset;
		event.position = vst_event.ppqPosition;
		event.is_live = (vst_event.flags & Steinberg::Vst::Event::EventFlags::kIsLive);
		switch (vst_event.type)
		{
		case Steinberg::Vst::Event::EventTypes::kNoteOnEvent:
			event.type = event::type::note_on;
			event.core_event.note_on = to_note_event(vst_event.noteOn);
			break;
		case Steinberg::Vst::Event::EventTypes::kNoteOffEvent:
			event.type = event::type::note_off;
			event.core_event.note_off = to_note_event(vst_event.noteOff);
			break;
		case Steinberg::Vst::Event::EventTypes::kDataEvent:
			event.type = event::type::data_event;
			event.core_event.data = to_data_event(vst_event.data);
			break;
		default:
			break;
		}

		//FIXME: handle all event types.
		return event;
	}

	note_event vst_processor::to_note_event(Steinberg::Vst::NoteOnEvent& vst_note_on_event) const
	{
		return {
			0,
			vst_note_on_event.channel,
			vst_note_on_event.pitch,
			vst_note_on_event.tuning,
			vst_note_on_event.velocity
		};
	}

	note_event vst_processor::to_note_event(Steinberg::Vst::NoteOffEvent& vst_note_off_event) const
	{
		return {
			0,
			vst_note_off_event.channel, 
			vst_note_off_event.pitch,
			vst_note_off_event.tuning,
			vst_note_off_event.velocity
		};
	}

	data_event vst_processor::to_data_event(Steinberg::Vst::DataEvent& vst_data_event) const
	{
		return {
			vst_data_event.size,
			vst_data_event.type,
			vst_data_event.bytes
		};
	}

	void vst_processor::to_audio_bus_buffer(audio_bus_buffers* source, Steinberg::Vst::AudioBusBuffers& vst_audio_bus_buffers,
	                                                     int sample_size) const
	{
		if (sample_size == Steinberg::Vst::kSample32)
		{
			source->channels_buffer = reinterpret_cast<void**>(vst_audio_bus_buffers.channelBuffers32);
			source->sample_size = sample_size::sample_size32;
		}
		else
		{
			source->channels_buffer = reinterpret_cast<void**>(vst_audio_bus_buffers.channelBuffers64);
			source->sample_size = sample_size::sample_size64;
		}
		source->num_channels = vst_audio_bus_buffers.numChannels;
		source->silence_flags = vst_audio_bus_buffers.silenceFlags;
	}
} // namespace
