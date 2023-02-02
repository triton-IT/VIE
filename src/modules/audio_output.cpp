#include "audio_output.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::module
{
	audio_output::audio_output(nlohmann::json audio_output_definition) : processor_module(),
		id_(audio_output_definition["id"]),
		name_(audio_output_definition["name"]),
		nb_outputs_(0),
		can_process_(false),
		output_bus_id_(0)
	{
	}

	audio_output::~audio_output() = default;

	uint16_t audio_output::get_id()
	{
		return id_;
	}

	std::string audio_output::get_name()
	{
		return name_;
	}

	processor_module_type audio_output::get_type()
	{
		return processor_module_type::audio_output;
	}

	void audio_output::set_sample_rate(double sample_rate)
	{
	}

	void audio_output::preprocess() {
	}

	bool audio_output::can_process()
	{
		return can_process_;
	}

	void audio_output::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		const audio_bus_buffers* bus_buffer = &output_process_data.outputs[output_bus_id_];
		for (uint_fast8_t i = 0; i < nb_outputs_ && i < bus_buffer->num_channels; i++)
		{
			const auto channel_buffer = static_cast<float*>(bus_buffer->channels_buffer[i]);

			const float* amplitudes = amplitudes_[i].to_float_array().values;

			if (amplitudes_[i].to_float_array().nb_values == output_process_data.num_samples)
			{
				memcpy(channel_buffer, amplitudes, output_process_data.num_samples * sizeof(float));
			}
			else
			{
				memset(channel_buffer, 0, output_process_data.num_samples * sizeof(float));
			}
		}
		for (uint_fast8_t i = nb_outputs_; i < bus_buffer->num_channels; i++)
		{
			const auto channel_buffer = static_cast<float*>(bus_buffer->channels_buffer[i]);

			memset(channel_buffer, 0, output_process_data.num_samples * sizeof(float));
		}
	}

	uint_fast8_t audio_output::get_output_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		throw std::invalid_argument("Invalid slot id");
	}

	bool audio_output::has_finished()
	{
		return true;
	}

	uint_fast16_t audio_output::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == amplitudes_input_name)
		{
			return amplitudes_input_id;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	/**
	* An audio_output accepts only one channel.
	* The first channel is picked, the other ones are ignored.
	*/
	void audio_output::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		switch (slot_id)
		{
		case amplitudes_input_id:
			assert(nb_values <= 32);
			nb_outputs_ = nb_values;
			
			for (uint_fast8_t i = 0; i < nb_outputs_; i++)
			{
				amplitudes_[i].note_id = values[0]->note_id;
				amplitudes_[i].values = values[0]->to_float_array();
			}

			can_process_ = true;
				
			return;
		}
		
		throw std::invalid_argument("Invalid slot name");
	}

	uint_fast32_t audio_output::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		switch (slot_id)
		{ 
		case amplitudes_input_id: 
			return 1;
		}

		throw std::invalid_argument("Invalid slot id");
	}

	void audio_output::set_parameter(parameter parameter)
	{
	}

	void audio_output::set_output_bus_id(const uint_fast16_t bus_id)
	{
		output_bus_id_ = bus_id;
	}
} // namespace
