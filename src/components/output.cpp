#include "output.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	output::output(nlohmann::json output_definition) : processor_component(),
		id_(output_definition["id"]),
		name_(output_definition["name"]),
		can_process_(false),
		output_bus_id_(0)
	{
	}

	output::~output() = default;

	uint16_t output::get_id()
	{
		return id_;
	}

	std::string output::get_name()
	{
		return name_;
	}

	processor_component_type output::get_type()
	{
		return processor_component_type::output;
	}

	void output::set_sample_rate(double sample_rate)
	{
	}

	void output::preprocess() {
	}

	bool output::can_process()
	{
		return can_process_;
	}

	void output::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		const audio_bus_buffers* bus_buffer = &output_process_data.outputs[output_bus_id_];
		const auto left = static_cast<float*>(bus_buffer->channels_buffer[0]);
		const auto right = static_cast<float*>(bus_buffer->channels_buffer[1]);

		const float* amplitudes = amplitudes_.to_float_array().values;

		if (amplitudes_.to_float_array().nb_values == output_process_data.num_samples)
		{
			memcpy(left, amplitudes, output_process_data.num_samples * sizeof(amplitudes));
			memcpy(right, amplitudes, output_process_data.num_samples * sizeof(amplitudes));
		}
		else
		{
			memset(left, 0, output_process_data.num_samples * sizeof(amplitudes));
			memset(right, 0, output_process_data.num_samples * sizeof(amplitudes));
		}
	}

	component_output** output::get_outputs_pool(uint_fast16_t slot_id) {
		return nullptr;
	}

	uint_fast32_t output::get_output_values(uint_fast16_t slot_id, component_output* output_values[32])
	{
		return 0;
	}

	bool output::has_finished()
	{
		return true;
	}

	uint_fast16_t output::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == amplitudes_input_name)
		{
			return amplitudes_input_id;
		}

		return -1;
	}

	/**
	* An output accepts only one channel.
	* The first channel is picked, the other ones are ignored.
	*/
	void output::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
		can_process_ = true;

		if (nb_values > 0)
		{
			if (slot_id == amplitudes_input_id)
			{
				amplitudes_.note_id = ((float_array_component_output*)values[0])->note_id;
				amplitudes_.values = ((float_array_component_output*)values[0])->to_float_array();
			}
		}
		else
		{
			amplitudes_.note_id = 0;
			amplitudes_.values.nb_values = 0;
		}
	}

	uint_fast32_t output::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == amplitudes_input_id)
		{
			return 1;
		}

		//FIXME: Do not return -1 on a uint
		return -1;
	}

	void output::set_parameter(parameter parameter)
	{

	}

	void output::set_output_bus_id(const uint_fast16_t bus_id)
	{
		output_bus_id_ = bus_id;
	}
} // namespace
