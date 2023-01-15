#include "audio_input.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	audio_input::audio_input() :
		id_(0),
		name_("test"),
		buffer_(nullptr),
		sample_rate_(44100)
	{
		output_ = new float_array_component_output();
	}
	
	audio_input::audio_input(nlohmann::json audio_input_definition) :
		id_(audio_input_definition["id"]),
		name_(audio_input_definition["name"]),
		buffer_(nullptr),
		sample_rate_(44100)
	{
		output_ = new float_array_component_output();
	}

	audio_input::~audio_input()
	{
		delete output_;
	}

	uint16_t audio_input::get_id()
	{
		return id_;
	}

	std::string audio_input::get_name()
	{
		return name_;
	}

	processor_component_type audio_input::get_type()
	{
		return processor_component_type::audio_input;
	}

	void audio_input::set_sample_rate(const double audio_input_rate)
	{
		sample_rate_ = audio_input_rate;
	}

	void audio_input::preprocess() {
	}

	//FIXME: Can't know if all inputs are set-up. So, we should check note by note if it can be processed. 
	bool audio_input::can_process()
	{
		return true;
	}

	void audio_input::process(output_process_data& output_process_data)
	{
		if (buffer_->num_channels > 0)
		{
			//If nb of audio_inputs is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > output_->values.nb_values)
			{
				if (output_->values.nb_values > 0)
				{
					delete output_->values.values;
				}
				output_->values.values = new float[output_process_data.num_samples];
				output_->values.nb_values = output_process_data.num_samples;
			}

			//FIXME: We should handle the noteId correctly.
			//Have each input component configured return the maximum number of note id it can handle.
			//and then assign a starting noteId offset to each input component.
			//Each input component could then set the noteId it is processing according to the starting id specified.
			output_->note_id = 256;

			//FIXME: Handle multiple channels !
			//output_->values.values = (float*)buffer_->channels_buffer[0];
			memcpy(output_->values.values, buffer_->channels_buffer[0], output_process_data.num_samples * sizeof(float));

			//for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
			//{
			//	output_->values.values[frame] = ((float*) buffer_->channels_buffer[0])[frame];
			//}
		}
	}

	component_output** audio_input::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) &output_;
	}

	uint_fast32_t audio_input::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		switch (slot_id) {
		case audio_output_id:
			output_values = (component_output**) &output_;
			return 1;
		}

		return 0;
	}

	bool audio_input::has_finished()
	{
		return false;
	}

	uint_fast16_t audio_input::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == audio_output_name)
		{
			return audio_output_id;
		}

		return -1;
	}

	void audio_input::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
	}

	uint_fast32_t audio_input::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		return -1;
	}

    void audio_input::set_parameter(parameter parameter) {
    }

	void audio_input::set_buffer(audio_bus_buffers* buffer)
	{
		buffer_ = buffer;
	}
} // namespace
