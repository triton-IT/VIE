#include "high_pass.hpp"

#include "../application.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace live::tritone::vie::processor::component
{
	high_pass::high_pass(nlohmann::json high_pass_definition) : processor_component(),
		id_(high_pass_definition["id"]),
		name_(high_pass_definition["name"]),
		nb_inputs_(0),
		inputs_(nullptr),
		cutoff_set_(false),
		resonance_set_(false),
		cutoff_(2000),
		resonance_(16),
		input_prev_{ 0, 0 },
		output_prev_{ 0, 0 },
		sample_rate_(44100),
		omega(0),
		sin_omega(0),
		alpha(0),
		cos_omega(0),
		input_modified_(true)
	{
		filtered_ = new float_array_component_output();
	}

	high_pass::~high_pass() {
		delete filtered_;
	}

	uint16_t high_pass::get_id()
	{
		return id_;
	}

	std::string high_pass::get_name()
	{
		return name_;
	}

	processor_component_type high_pass::get_type()
	{
		return processor_component_type::middle;
	}

	void high_pass::set_sample_rate(double sample_rate)
	{
		sample_rate_ = sample_rate;
		input_modified_ = true;
	}

	void high_pass::preprocess() {
	}

	bool high_pass::can_process()
	{
		//FIXME: Pass input parameters
		//return cutoff_set_ && resonance_set_ && inputs_set_;
		return inputs_set_;
	}

	void high_pass::process(output_process_data& output_process_data)
	{
		cutoff_set_ = false;
		resonance_set_ = false;

		filtered_->note_id = id_;

		if (nb_inputs_ > 0) {
			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > filtered_->values.nb_values)
			{
				if (filtered_->values.nb_values > 0)
				{
					delete filtered_->values.values;
				}
				filtered_->values.values = new float[output_process_data.num_samples];
				filtered_->values.nb_values = output_process_data.num_samples;
			}

			if (input_modified_)
			{
				// Calcul des coefficients du filtre en fonction de la fréquence de coupure et de la résonance
				omega = 2 * M_PI * cutoff_ / sample_rate_;
				sin_omega = sin(omega);
				alpha = sin_omega / (2 * resonance_);
				cos_omega = cos(omega);

				float a0 = 1 + alpha; //Impulse normalizer
				
				a1 = (-2 * cos_omega) / a0; //Normalized delay for previous sample.
				a2 = (1 - alpha) / a0; //Normalized delay for older sample.

				b0 = ((1 + cos_omega) / 2) / a0; //Normalized response for current sample.
				b1 = -(1 + cos_omega) / a0; //Normalized response to previous sample.
				b2 = b0; //Normalized response for older sample.

				input_modified_ = false;
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

						float output = b0 * input + b1 * input_prev_[0] + b2 * input_prev_[1] - a1 * output_prev_[0] - a2 * output_prev_[1];

						// Mise à jour des entrées et sorties précédentes
						input_prev_[1] = input_prev_[0];
						input_prev_[0] = input;
						output_prev_[1] = output_prev_[0];
						output_prev_[0] = output;

						filtered_->values.values[frame] = output;
					}
				}
			}
		}
	}

	component_output** high_pass::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) &filtered_;
	}

	uint_fast32_t high_pass::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		output_values = (component_output**) filtered_;

		return (nb_inputs_ > 0) ? 1 : 0;
	}

	bool high_pass::has_finished()
	{
		return true;
	}

	uint_fast16_t high_pass::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == generics_input_name)
		{
			return generics_input_id;
		}
		else if (slot_name == cutoff_input_name)
		{
			return cutoff_input_id;
		}
		else if (slot_name == resonance_input_name)
		{
			return resonance_input_id;
		}
		else if (slot_name == average_output_name)
		{
			return average_output_id;
		}

		return -1;
	}

	void high_pass::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
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
		case cutoff_input_id:
		{
			float new_cutoff = ((component_output*)values[0])->to_float();
			if (new_cutoff != cutoff_) {
				input_modified_ = true;
			}
			cutoff_ = new_cutoff;
		}
		cutoff_set_ = true;
		break;
		case resonance_input_id:
		{
			float new_resonance = ((component_output*)values[0])->to_float();
			if (new_resonance != resonance_) {
				input_modified_ = true;
			}
			resonance_ = new_resonance;
		}
			resonance_set_ = true;
			break;
		default:
			break;
		}
	}

	uint_fast32_t high_pass::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == generics_input_id)
		{
			return 32;
		}

		return -1;
	}

	void high_pass::set_parameter(parameter parameter)
	{

	}
} // namespace
