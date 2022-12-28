#include "mixer.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	mixer::mixer(nlohmann::json mixer_definition) : processor_component(),
		id_(mixer_definition["id"]),
		name_(mixer_definition["name"]),
		nb_inputs_(0),
		inputs_(nullptr),
		can_process_(true)
	{
		average_ = new float_array_component_output();
	}

	mixer::~mixer() {
		delete average_;
	}

	uint16_t mixer::get_id()
	{
		return id_;
	}

	std::string mixer::get_name()
	{
		return name_;
	}

	processor_component_type mixer::get_type()
	{
		return processor_component_type::middle;
	}

	void mixer::set_sample_rate(double sample_rate)
	{
	}

	void mixer::preprocess() {
	}

	bool mixer::can_process()
	{
		return can_process_;
	}

	void mixer::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		average_->note_id = id_;

		if (nb_inputs_ > 0) {

			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > average_->values.nb_values)
			{
				if (average_->values.nb_values > 0)
				{
					delete average_->values.values;
				}
				average_->values.values = new float[output_process_data.num_samples];
				average_->values.nb_values = output_process_data.num_samples;
			}

			for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
			{
				float amplitude = .0f;

				float nb_amplitude_for_frame = 0;
				for (uint_fast32_t amplitude_id = 0; amplitude_id < nb_inputs_; amplitude_id++)
				{
					float_array& generic = inputs_[amplitude_id]->to_float_array();
					if (generic.nb_values > frame)
					{
						nb_amplitude_for_frame++;
						amplitude += generic.values[frame];
					}
				}
				if (nb_amplitude_for_frame > 0)
				{
					average_->values.values[frame] = amplitude / nb_amplitude_for_frame;
				}
				else
				{
					average_->values.values[frame] = 0;
				}
			}
		}
		
	}

	component_output** mixer::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) &average_;
	}

	uint_fast32_t mixer::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		output_values = (component_output**) average_;

		return (nb_inputs_ > 0) ? 1 : 0;
	}

	bool mixer::has_finished()
	{
		return true;
	}

	uint_fast16_t mixer::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == generics_input_name)
		{
			return generics_input_id;
		}
		else if (slot_name == average_output_name)
		{
			return average_output_id;
		}

		return -1;
	}

	void mixer::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
		nb_inputs_ = nb_values;

		if (slot_id == generics_input_id)
		{
			inputs_ = values;

			can_process_ = true;
		}
	}

	uint_fast32_t mixer::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == generics_input_id)
		{
			return 32;
		}

		return -1;
	}

	void mixer::set_parameter(parameter parameter)
	{

	}
} // namespace
