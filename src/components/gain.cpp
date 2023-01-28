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
		gain_(1.1),
		sample_rate_(44100),
		inputs_set_(false)
	{
		for (int i = 0; i < 32; i++) {
			amplified_output_[i] = new float_array_component_output();
		}
	}

	gain::~gain() {
		for (int i = 0; i < 32; i++) {
			delete amplified_output_[i];
		}
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

		const auto amplified_output = amplified_output_[0];
		amplified_output->note_id = id_;

		if (nb_inputs_ > 0) {
			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > amplified_output->values.nb_values)
			{
				if (amplified_output->values.nb_values > 0)
				{
					delete amplified_output->values.values;
				}
				amplified_output->values.values = new float[output_process_data.num_samples];
				amplified_output->values.nb_values = output_process_data.num_samples;
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
						
						float output = input * gain_;

						amplified_output->values.values[frame] = output;
					}
				}
			}
		}		
	}

	uint_fast8_t gain::get_output_values(const uint_fast16_t slot_id, std::array<component_output*, 32>& values)
	{
		switch (slot_id)
		{
			values = reinterpret_cast<std::array<component_output*, 32>&>(amplified_output_);
			return nb_inputs_;
		}
		
		throw std::invalid_argument("Invalid slot id");
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

		throw std::invalid_argument("Invalid slot name");
	}

	void gain::set_input_values(const uint_fast16_t slot_id, std::array<component_output*, 32>& values, uint_fast8_t nb_values)
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
		case gain_input_id:
			gain_ = values[0]->to_float();
			return;
		default:
			break;
		}

		throw std::invalid_argument("Invalid slot id");
	}

	uint_fast32_t gain::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		switch (slot_id)
		{
		case generics_input_id:
			return 32;
		}

		throw std::invalid_argument("Invalid slot id");
	}

	void gain::set_parameter(parameter parameter)
	{
	}
} // namespace
