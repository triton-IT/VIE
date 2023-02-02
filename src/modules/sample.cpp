#include "sample.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::module
{
	sample::sample(nlohmann::json sample_definition) :
		id_(sample_definition["id"]),
		name_(sample_definition["name"]),
		can_process_(false)
	{
		std::string file_path = sample_definition["file_path"];

		for (int i = 0; i < 32; i++) {
			outputs_[i] = new float_array_module_output();
		}

		loadFile(file_path);
	}

	sample::~sample()
	{		
		for (int i = 0; i < max_nb_descriptors_; i++)
		{
			if (samples_descriptors_[i].handle != nullptr)
			{
				if (samples_descriptors_[i].buffer != nullptr)
				{
					delete[] samples_descriptors_[i].buffer;
				}
				delete samples_descriptors_[i].handle;
			}
		}
		for (int i = 0; i < 32; i++) {
			delete[] outputs_[i]->values.values;
			delete outputs_[i];
		}
	}

    void sample::loadFile(std::string filename)
    {
		//TODO: Only one file for all notes for now. Implement one file for each.
		for (sample_descriptor& descriptor : samples_descriptors_)
		{
			SndfileHandle* sndFileHandle = new SndfileHandle(filename);
			descriptor.activated = false;
			descriptor.handle = sndFileHandle;
			descriptor.nb_frames = 0;
			descriptor.buffer = nullptr;
		}
    }

	uint16_t sample::get_id()
	{
		return id_;
	}

	std::string sample::get_name()
	{
		return name_;
	}

	processor_module_type sample::get_type()
	{
		return processor_module_type::middle;
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
		can_process_ = false;
		if (!already_processed_) {
			nb_outputs_ = 0;
			int_fast16_t note_id = 0;
			for (sample_descriptor& sample_descriptor : samples_descriptors_)
			{
				if (sample_descriptor.activated) {
					int nb_channels = sample_descriptor.handle->channels();
					uint_fast32_t buffer_size = output_process_data.num_samples * nb_channels;
					
					//Augment buffer if not large enough.
					if (output_process_data.num_samples > sample_descriptor.nb_frames) {
						if (sample_descriptor.buffer != nullptr) {
							delete[] sample_descriptor.buffer;
						}
						sample_descriptor.buffer = new float[buffer_size];
						sample_descriptor.nb_frames = output_process_data.num_samples;
					}					

					sample_descriptor.handle->read(sample_descriptor.buffer, buffer_size);

					for (int output_id = 0; output_id < nb_channels; output_id++)
					{
						assert(nb_outputs_ < max_nb_outputs_);
						
						float_array_module_output* output = outputs_[nb_outputs_];

						//If nb of samples is greater than the ones currently allocated, reallocate.
						if (output_process_data.num_samples > output->values.nb_values)
						{
							if (output->values.values != nullptr)
							{
								delete[] output->values.values;
							}

							output->values.nb_values = output_process_data.num_samples;
							output->values.values = new float[output_process_data.num_samples];
						}

						output->note_id = note_id;

						//TODO: Copy whole buffer at once instead of iterating.
						for (uint_fast32_t i = 0; i < output_process_data.num_samples; i++)
						{
							output->values.values[i] = sample_descriptor.buffer[(i + output_id) * nb_channels];
						}

						nb_outputs_++;
					}
				}
				
				//Note_id is represented by the position in array.
				//So increment it at each loop.
				note_id++;
			}

			already_processed_ = true;
		}
	}

	uint_fast8_t sample::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		switch (slot_id) {
		case amplitude_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(outputs_);
			return nb_outputs_;
		}
		throw std::invalid_argument("Invalid slot id");
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

		throw std::invalid_argument("Invalid slot name");
	}

	void sample::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, max_nb_outputs_>& values, uint_fast8_t nb_values)
	{
		assert(nb_values <= max_nb_outputs_);
		//Check which input slot is requested
		switch (slot_id) {
		case play_input_id:
			//Iterate over each value to set to input.
			for (uint_fast16_t i = 0; i < nb_values; i++)
			{
				//Get current value which is output value of previous module.
				//We don't care about its value, we just need to know if it is set or not.
				const auto module_output = values[i];

				uint32_t note_id = module_output->note_id;

				//And unmute the ones which are requested.
				sample_descriptor& descriptor = samples_descriptors_[note_id];
				if (!descriptor.activated)
				{
					//If sample is a new one, activate it.
					descriptor.activated = true;
					descriptor.handle->seek(0, SEEK_SET);
				}
			}
			can_process_ = true;
			return;
		case stop_input_id:
			//Iterate over each value to set to input.
			for (uint_fast16_t i = 0; i < nb_values; i++)
			{
				//Get current value which is output value of previous module.
				//We don't care about its value, we just need to know if it is set or not.
				const auto module_output = values[i];

				uint32_t note_id = module_output->note_id;

				//And mute the ones which are requested.
				sample_descriptor& descriptor = samples_descriptors_[note_id];
				descriptor.activated = false;
				descriptor.handle->seek(0, SEEK_SET);
			}
			can_process_ = true;
			return;
		case play_at_input_id:
			//Iterate over each value to set to input.
			for (uint_fast16_t i = 0; i < nb_values; i++)
			{
				const auto module_output = values[i];
					
				uint32_t note_id = module_output->note_id;
					
				sample_descriptor& descriptor = samples_descriptors_[note_id];
				descriptor.activated = true;
				float position = module_output->to_float();
				descriptor.handle->seek(position, SEEK_SET);
					
			}
			can_process_ = true;
			return;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	uint_fast32_t sample::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == play_input_id)
		{
			return max_nb_outputs_;
		}
		if (slot_id == stop_input_id)
		{
			return max_nb_outputs_;
		}
		if (slot_id == name_input_id)
		{
			return 1;
		}

		throw std::invalid_argument("Invalid slot name");
	}

    void sample::set_parameter(parameter parameter) {
    }
} // namespace
