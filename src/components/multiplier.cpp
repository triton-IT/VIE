#include "multiplier.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	multiplier::multiplier(nlohmann::json multiplier_definition) : processor_component(),
		id_(multiplier_definition["id"]),
		name_(multiplier_definition["name"]),
		nb_inputs_multipliers_(0),
		multipliers_(nullptr),
		nb_inputs_multiplicands_(0),
		multiplicands_(nullptr),
		multipliers_filled_(false),
		multiplicands_filled_(false),
		nb_products_(0)
	{
	}

	uint16_t multiplier::get_id()
	{
		return id_;
	}

	std::string multiplier::get_name()
	{
		return name_;
	}

	processor_component_type multiplier::get_type()
	{
		return processor_component_type::middle;
	}

	void multiplier::set_sample_rate(const double sample_rate)
	{
	}

	bool multiplier::can_process()
	{
		return multipliers_filled_ && multiplicands_filled_;
	}

	void multiplier::process(output_process_data& output_process_data)
	{
		multipliers_filled_ = false;
		multiplicands_filled_ = false;

		nb_products_ = 0;

		for (uint_fast16_t input_id = 0; (input_id < nb_inputs_multipliers_) && (input_id < nb_inputs_multiplicands_); input_id++)
		{
			float_array_component_output& output = products_[input_id];

			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > output.nb_samples)
			{
				if (output.nb_samples > 0)
				{
					delete output.values;
				}
				output.values = new float[output_process_data.num_samples];
				output.nb_samples = output_process_data.num_samples;
			}

			for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
			{
				output.output_id = id_;

				float operand1 = 0;
				note_mode operand1_node_mode = note_mode::normal;
				if (nb_inputs_multipliers_ > input_id) {
					operand1 = multipliers_[input_id].values[frame];
					operand1_node_mode = multipliers_[input_id].note_mode;
				}

				float operand2 = 0;
				note_mode operand2_node_mode = note_mode::normal;
				if (nb_inputs_multiplicands_ > input_id) {
					operand2 = multiplicands_[input_id].values[frame];
					operand2_node_mode = multipliers_[input_id].note_mode;
				}

				const float result = operand1 * operand2;

				output.values[frame] = result;
				output.note_mode = (operand1_node_mode == note_mode::zombie && operand2_node_mode == note_mode::zombie)?note_mode::zombie:note_mode::normal;
			}

			nb_products_++;
		}

		nb_inputs_multipliers_ = 0;
		nb_inputs_multiplicands_ = 0;
	}

	uint_fast32_t multiplier::get_output_values(const uint_fast16_t slot_id, void* output_values[])
	{
		*output_values = products_;

		return nb_products_;
	}

	bool multiplier::has_finished()
	{
		return true;
	}

	void multiplier::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	void multiplier::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	uint_fast16_t multiplier::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == multipliers_input_name)
		{
			return multipliers_input_id;
		}
		if (slot_name == multiplicands_input_name)
		{
			return multiplicands_input_id;
		}
		if (slot_name == products_output_name)
		{
			return products_output_id;
		}

		return -1;
	}

	void multiplier::set_input_values(const uint_fast16_t slot_id, void* values, const uint_fast32_t nb_values)
	{
		switch (slot_id)
		{
		case multipliers_input_id:
			assert(nb_values <= 32);
			nb_inputs_multipliers_ = nb_values;
			multipliers_ = static_cast<float_array_component_output*>(values);
			multipliers_filled_ = true;
			break;

		case multiplicands_input_id:	
			assert(nb_values <= 32);
			nb_inputs_multiplicands_ = nb_values;
			multiplicands_ = static_cast<float_array_component_output*>(values);
			multiplicands_filled_ = true;
			break;
		default:
			break;
		}
	}

	uint_fast32_t multiplier::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		switch (slot_id)
		{
		case multipliers_input_id:
		case multiplicands_input_id:
			return 32;
		default:
			break;
		}

		return -1;
	}

	void multiplier::set_parameter(parameter parameter)
	{

	}
} // namespace
