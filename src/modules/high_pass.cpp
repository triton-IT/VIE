#include "high_pass.hpp"

#include "../application.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace live::tritone::vie::processor::module
{
	high_pass::high_pass(nlohmann::json high_pass_definition) : processor_module(),
		id_(high_pass_definition["id"]),
		name_(high_pass_definition["name"]),
		nb_inputs_(0),
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
		for (int i = 0; i < 32; i++) {
			filtered_[i] = new float_array_module_output();
		}
	}

	high_pass::~high_pass() {
		for (int i = 0; i < 32; i++) {
			delete filtered_[i];
		}
	}

	uint16_t high_pass::get_id()
	{
		return id_;
	}

	std::string high_pass::get_name()
	{
		return name_;
	}

	processor_module_type high_pass::get_type()
	{
		return processor_module_type::middle;
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

		filtered_[0]->note_id = id_;

		if (nb_inputs_ > 0) {
			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > filtered_[0]->values.nb_values)
			{
				if (filtered_[0]->values.nb_values > 0)
				{
					delete filtered_[0]->values.values;
				}
				filtered_[0]->values.values = new float[output_process_data.num_samples];
				filtered_[0]->values.nb_values = output_process_data.num_samples;
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
				float_array& input_array = inputs_[amplitude_id];

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

						filtered_[0]->values.values[frame] = output;
					}
				}
			}
		}
	}

	uint_fast8_t high_pass::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		switch (slot_id)
		{
		case average_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(filtered_);
			return nb_inputs_;
		}

		throw std::invalid_argument("Invalid slot id");
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

		throw std::invalid_argument("Invalid slot name");
	}

	void high_pass::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		switch (slot_id)
		{
		case onoff_input_id:
			return;
		case generics_input_id:
			for (nb_inputs_ = 0; nb_inputs_ < nb_values; nb_inputs_++)
			{
				inputs_[nb_inputs_] = values[nb_inputs_]->to_float_array();
			}
			inputs_set_ = true;
			return;
		case cutoff_input_id:
			{
				float new_cutoff = values[0]->to_float();
				if (new_cutoff != cutoff_) {
					input_modified_ = true;
				}
				cutoff_ = new_cutoff;
			}
			cutoff_set_ = true;
			return;
		case resonance_input_id:
		{
			float new_resonance = values[0]->to_float();
			if (new_resonance != resonance_) {
				input_modified_ = true;
			}
			resonance_ = new_resonance;
		}
			resonance_set_ = true;
			return;
		default:
			break;
		}
		
		throw std::invalid_argument("Invalid slot id");
	}

	uint_fast32_t high_pass::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		switch (slot_id)
		{
		case generics_input_id:
			return 32;
		}

		throw std::invalid_argument("Invalid slot id");
	}

	void high_pass::set_parameter(parameter parameter)
	{
	}
} // namespace
