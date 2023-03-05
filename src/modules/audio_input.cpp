#include "audio_input.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::module
{	
	audio_input::audio_input(nlohmann::json audio_input_definition) :
		id_(audio_input_definition["id"]),
		name_(audio_input_definition["name"]),
		buffer_(nullptr),
		sample_rate_(44100)
	{
		for (int i = 0; i < 32; i++) {
			outputs_[i] = new float_array_module_output();
		}
	}

	audio_input::~audio_input()
	{
		for (int i = 0; i < 32; i++) {
			delete outputs_[i];
		}
	}

	uint16_t audio_input::get_id()
	{
		return id_;
	}

	std::string audio_input::get_name()
	{
		return name_;
	}

	processor_module_type audio_input::get_type()
	{
		return processor_module_type::audio_input;
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
		assert(("Buffer must be set-up before processing", buffer_ != nullptr));

		if (buffer_->num_channels > 0)
		{
			assert(("Output module must be set-up before processing", outputs_[0] != nullptr));
			
			//If nb of audio_inputs is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > outputs_[0]->values.nb_values)
			{
				if (outputs_[0]->values.nb_values > 0)
				{
					delete outputs_[0]->values.values;
				}
				outputs_[0]->values.values = new float[output_process_data.num_samples];
				outputs_[0]->values.nb_values = output_process_data.num_samples;
			}

			//FIXME: We should handle the noteId correctly.
			//Have each input module configured return the maximum number of note id it can handle.
			//and then assign a starting noteId offset to each input module.
			//Each input module could then set the noteId it is processing according to the starting id specified.
			outputs_[0]->note_id = 256;

			//FIXME: Handle multiple channels !
			//output_->values.values = (float*)buffer_->channels_buffer[0];
			memcpy(outputs_[0]->values.values, buffer_->channels_buffer[0], output_process_data.num_samples * sizeof(float));

			//for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
			//{
			//	output_->values.values[frame] = ((float*) buffer_->channels_buffer[0])[frame];
			//}
		}
	}

	uint_fast8_t audio_input::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		switch (slot_id) {
		case audio_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(outputs_);
			return 1;
		}

		throw std::invalid_argument("Invalid slot id");
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

		throw std::invalid_argument("Invalid slot name");
	}

	void audio_input::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		throw std::invalid_argument("Invalid slot id");
	}

	uint_fast32_t audio_input::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		throw std::invalid_argument("Invalid slot id");
	}

    void audio_input::set_parameter(parameter parameter) {
    }

	void audio_input::set_buffer(audio_bus_buffers* buffer)
	{
		buffer_ = buffer;
	}

	nlohmann::json audio_input::serialize()
	{
		nlohmann::json root;
		root["id"] = id_;
		root["name"] = name_;

		return root;
	}
} // namespace
