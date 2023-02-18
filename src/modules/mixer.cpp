#include "mixer.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::module
{
	mixer::mixer(nlohmann::json mixer_definition) : processor_module(),
		id_(mixer_definition["id"]),
		name_(mixer_definition["name"]),
		nb_inputs_(0),
		can_process_(true)
	{
		for (uint_fast8_t i = 0; i < 32; i++) {
			average_[i] = new float_array_module_output();
		}
	}

	mixer::~mixer() {
		for (uint_fast8_t i = 0; i < 32; i++) {
			delete average_[i];
		}
	}

	uint16_t mixer::get_id()
	{
		return id_;
	}

	std::string mixer::get_name()
	{
		return name_;
	}

	processor_module_type mixer::get_type()
	{
		return processor_module_type::middle;
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

		average_[0]->note_id = id_;

		if (nb_inputs_ > 0) {

			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > average_[0]->values.nb_values)
			{
				if (average_[0]->values.nb_values > 0)
				{
					delete average_[0]->values.values;
				}
				average_[0]->values.values = new float[output_process_data.num_samples];
				average_[0]->values.nb_values = output_process_data.num_samples;
			}

			for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
			{
				float amplitude = .0f;

				float nb_amplitude_for_frame = 0;
				for (uint_fast32_t amplitude_id = 0; amplitude_id < nb_inputs_; amplitude_id++)
				{
					const float_array& generic = inputs_[amplitude_id];
					if (generic.nb_values > frame)
					{
						nb_amplitude_for_frame++;
						amplitude += generic.values[frame];
					}
				}
				if (nb_amplitude_for_frame > 0)
				{
					average_[0]->values.values[frame] = amplitude / nb_amplitude_for_frame;
				}
				else
				{
					average_[0]->values.values[frame] = 0;
				}
			}
		}
		
	}

	uint_fast8_t mixer::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		switch (slot_id) {
		case average_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(average_);
			return nb_inputs_;
		}
		
		throw std::invalid_argument("Invalid slot id");
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

		throw std::invalid_argument("Invalid slot name");
	}

	void mixer::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		switch(slot_id)
		{
		case generics_input_id:
			for (nb_inputs_ = 0; nb_inputs_ < nb_values; nb_inputs_++)
			{
				inputs_[nb_inputs_] = values[nb_inputs_]->to_float_array();
			}

			can_process_ = true;
			return;
		}
		
		throw std::invalid_argument("Invalid slot name");
	}

	uint_fast32_t mixer::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == generics_input_id)
		{
			return 32;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	void mixer::set_parameter(parameter parameter)
	{

	}
} // namespace
