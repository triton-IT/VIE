#include "gain.hpp"

#include "../application.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace live::tritone::vie::processor::component
{
	gain::gain(nlohmann::json gain_definition) : processor_component(),
		id_(gain_definition["id"]),
		name_(gain_definition["name"]),
		nb_inputs_(0),
		inputs_(nullptr),
		gain_(1.1),
		sample_rate_(44100),
		inputs_set_(false)
	{
		amplified_output_ = new float_array_component_output();
	}

	gain::~gain() {
		delete amplified_output_;
	}

	uint16_t gain::get_id()
	{
		return id_;
	}

	std::string gain::get_name()
	{
		return name_;
	}

	processor_component_type gain::get_type()
	{
		return processor_component_type::middle;
	}

	void gain::set_sample_rate(double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	void gain::preprocess() {
	}

	bool gain::can_process()
	{
		return inputs_set_;
	}

	void gain::process(output_process_data& output_process_data)
	{
		inputs_set_ = false;

		amplified_output_->note_id = id_;

		if (nb_inputs_ > 0) {
			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > amplified_output_->values.nb_values)
			{
				if (amplified_output_->values.nb_values > 0)
				{
					delete amplified_output_->values.values;
				}
				amplified_output_->values.values = new float[output_process_data.num_samples];
				amplified_output_->values.nb_values = output_process_data.num_samples;
			}

			for (uint_fast32_t amplitude_id = 0; amplitude_id < nb_inputs_; amplitude_id++)
			{
				float_array& input_array = inputs_[amplitude_id]->to_float_array();
				
				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					if (input_array.nb_values > frame)
					{
						// Mise à jour de l'entrée et de la sortie courantes
						float input = input_array.values[frame];
						
						float output = input * gain_;

						amplified_output_->values.values[frame] = output;
					}
				}
			}
		}		
	}

	component_output** gain::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) &amplified_output_;
	}

	uint_fast32_t gain::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		output_values = (component_output**) amplified_output_;

		return (nb_inputs_ > 0) ? 1 : 0;
	}

	bool gain::has_finished()
	{
		return true;
	}

	uint_fast16_t gain::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == generics_input_name)
		{
			return generics_input_id;
		}
		else if (slot_name == gain_input_name)
		{
			return gain_input_id;
		}

		return -1;
	}

	void gain::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
		nb_inputs_ = nb_values;

		switch (slot_id)
		{
		case onoff_input_id:
			break;
		case generics_input_id:
			inputs_ = values;
			inputs_set_ = true;
			break;
		case gain_input_id:
			gain_ = ((component_output*)values[0])->to_float();
			break;
		default:
			break;
		}
	}

	uint_fast32_t gain::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == generics_input_id)
		{
			return 32;
		}

		return -1;
	}

	void gain::set_parameter(parameter parameter)
	{
	}
} // namespace
