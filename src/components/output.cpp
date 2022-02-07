#include "output.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	output::output(nlohmann::json output_definition) :
		id_(output_definition["id"]),
		name_(output_definition["name"]),
		amplitudes_(nullptr),
		can_process_(false),
		output_bus_id_(0)
	{
	}

	output::~output() = default;

	uint16_t output::get_id()
	{
		return id_;
	}

	processor_component_type output::get_type()
	{
		return processor_component_type::output;
	}

	void output::set_sample_rate(double sample_rate)
	{
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

		const float* amplitudes = amplitudes_->values;

		if (amplitudes_->nb_samples == output_process_data.num_samples)
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

	uint_fast32_t output::get_output_values(uint_fast16_t slot_id, void* output_values[])
	{
		return 0;
	}

	bool output::has_finished()
	{
		return true;
	}

	uint_fast8_t output::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
		return 0;
	}

	void output::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	uint_fast16_t output::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == amplitudes_input_name)
		{
			return amplitudes_input_id;
		}

		return -1;
	}

	void output::set_input_values(const uint_fast16_t slot_id, void* values, const uint_fast32_t nb_values)
	{
		can_process_ = true;

		if (nb_values > 0)
		{
			if (slot_id == amplitudes_input_id)
			{
				amplitudes_ = static_cast<float_array_component_output*>(values);
			}
		}
		else
		{
			amplitudes_ = &empty_array_component_;
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

	void output::set_output_bus_id(const uint_fast16_t bus_id)
	{
		output_bus_id_ = bus_id;
	}
} // namespace
