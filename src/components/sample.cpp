#include "sample.hpp"

#include "../application.hpp"

#include <sndfile.h>
#include <sndfile.hh>

namespace live::tritone::vie::processor::component
{
	sample::sample(nlohmann::json sample_definition) :
		id_(sample_definition["id"]),
		name_(sample_definition["name"]),
		can_process_(false)
	{
		std::string file_path = sample_definition["file_path"];

		loadFile(file_path);
		
		for (int i = 0; i < 32; i++)
		{
			outputs_[i] = new float_array_component_output();
		}
	}

	sample::~sample()
	{

		for (int i = 0; i < 32; i++)
		{
			delete outputs_[i];
		}
	}

    void sample::loadFile(std::string filename)
    {
		SndfileHandle sndfileHandle = SndfileHandle(filename);

        // Create the sample descriptor
        sample_descriptor sample_descriptor;
        sample_descriptor.nb_channels = sndfileHandle.channels();
        sample_descriptor.nb_frames = sndfileHandle.frames();
		sample_descriptor.rate = sndfileHandle.samplerate();
		sample_descriptor.format = sndfileHandle.format();

		for (int i = 0; i < sndfileHandle.channels() && i < 8; i++) {
			sample_descriptor.buffers[i] = new float[sample_descriptor.nb_frames];
		}

		//TODO: Optimize: Do not read a first the full buffer to copy it after in each channel
		uint_fast32_t full_buffer_size = sample_descriptor.nb_frames * sample_descriptor.nb_channels;
		float* full_buffer = new float[full_buffer_size];
		sndfileHandle.read(full_buffer, full_buffer_size);

		for (int i = 0; i < full_buffer_size; i++) {
			int channel = i % sample_descriptor.nb_channels;
			int frame = i / sample_descriptor.nb_channels;
			sample_descriptor.buffers[channel][frame] = full_buffer[i];
		}

		delete[] full_buffer;
		
		samples_descriptors_[0] = sample_descriptor;
    }

	uint16_t sample::get_id()
	{
		return id_;
	}

	std::string sample::get_name()
	{
		return name_;
	}

	processor_component_type sample::get_type()
	{
		return processor_component_type::middle;
	}

	void sample::set_sample_rate(const double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	void sample::preprocess() {
		already_processed_ = false;
	}

	//FIXME: Can't know if all inputs are set-up. So, we should check note by note if it can be processed. 
	bool sample::can_process()
	{
		return can_process_;
	}

	void sample::process(output_process_data& output_process_data)
	{
		//can_process_ = false;
		if (!already_processed_) {
			nb_outputs_ = 0;
			int_fast16_t note_id = 0;
			for (sample_state& sample_state : samples_states_)
			{
				if (sample_state.activated) {
					float_array_component_output* output = outputs_[nb_outputs_];

					//If nb of samples is greater than the ones currently allocated, reallocate.
					if (output_process_data.num_samples > output->values.nb_values)
					{
						if (output->values.nb_values > 0)
						{
							delete output->values.values;
						}
						output->values.values = new float[output_process_data.num_samples];
						output->values.nb_values = output_process_data.num_samples;
					}

					output->note_id = note_id;

					for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
					{
						//TODO: Copy whole buffer at once instead of iterating.
						output->values.values[frame] = samples_descriptors_[0].buffers[0][sample_state.position];
						sample_state.position++;
					}

					nb_outputs_++;
				}
				note_id++;
			}

			already_processed_ = true;
		}
	}

	component_output** sample::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) outputs_;
	}

	uint_fast32_t sample::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		switch (slot_id) {
		case amplitude_output_id:
			output_values = (component_output**)outputs_;
			return nb_outputs_;
		}

		return 0;
	}

	bool sample::has_finished()
	{
		return false;
	}

	uint_fast16_t sample::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == play_input_name)
		{
			return play_input_id;
		}
		else if (slot_name == stop_input_name)
		{
			return stop_input_id;
		}
		else if (slot_name == name_input_name)
		{
			return name_input_id;
		}
		else if (slot_name == play_at_input_name)
		{
			return play_at_input_id;
		}
		else if (slot_name == amplitude_output_name)
		{
			return amplitude_output_id;
		}

		return -1;
	}

	void sample::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
		//Process if we have an output
		if (nb_values > 0) {
			//32 is the maximum number of inputs authorized by design.
			assert(nb_values <= 32);
			//Check which input slot is requested
			switch (slot_id) {
			case play_input_id:
				//Iterate over each value to set to input.
				for (uint_fast16_t i = 0; i < nb_values; i++)
				{
					//Get current value which is output value of previous component.
					//We don't care about its value, we just need to know if it is set or not.
					const auto& component_output = values[i];

					uint32_t input_id = component_output->note_id;

					//And unmute the ones which are requested.
					sample_state& sample_state = samples_states_[input_id];
					if (!sample_state.activated)
					{
						//If sample is a new one, activate it.
						sample_state.activated = true;
					}
					sample_state.position = 0;
				}
				break;
			case stop_input_id:
				//Iterate over each value to set to input.
				for (uint_fast16_t i = 0; i < nb_values; i++)
				{
					//Get current value which is output value of previous component.
					//We don't care about its value, we just need to know if it is set or not.
					//FIXME: Casting can never work. We need to be sure of input type required.
					const auto& component_output = values[i];

					uint32_t input_id = component_output->note_id;

					//And mute the ones which are requested.
					sample_state& sample_state = samples_states_[input_id];
					if (sample_state.activated)
					{
						//If sample is a new one, activate it.
						sample_state.activated = false;
						sample_state.position = 0;
					}
				}
				break;
			case play_at_input_id:
				//Iterate over each value to set to input.
				for (uint_fast16_t i = 0; i < nb_values; i++)
				{
					const auto& component_output = values[i];
					uint32_t input_id = component_output->note_id;
					sample_state& sample_state = samples_states_[input_id];
					sample_state.position = component_output->to_float();
					
				}
				break;
			}
		}

		can_process_ = true;
	}

	uint_fast32_t sample::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == play_input_id)
		{
			return 32;
		}
		if (slot_id == stop_input_id)
		{
			return 32;
		}
		if (slot_id == name_input_id)
		{
			return 1;
		}

		return -1;
	}

    void sample::set_parameter(parameter parameter) {
        /*if (parameter.get_title() == "sample_name") {
            name_ = parameter.get_title();
        }*/
    }

	/*void sample::set_sample_type(sample_type sample_type) {
		sample_type_ = sample_type;
	}*/
} // namespace
