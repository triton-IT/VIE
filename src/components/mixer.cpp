#include "mixer.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	mixer::mixer(nlohmann::json mixer_definition) : id_(mixer_definition["id"]), name_(mixer_definition["name"]),
	                                                nb_generics_(0), generics_(nullptr), can_process_(true)
	{
	}

	uint16_t mixer::get_id()
	{
		return id_;
	}

	processor_component_type mixer::get_type()
	{
		return processor_component_type::middle;
	}

	void mixer::set_sample_rate(double sample_rate)
	{
	}

	bool mixer::can_process()
	{
		return can_process_;
	}

	void mixer::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		average_.output_id = id_;

		//If nb of samples is greater than the ones currently allocated, reallocate.
		if (output_process_data.num_samples > average_.nb_samples)
		{
			if (average_.nb_samples > 0)
			{
				delete average_.values;
			}
			average_.values = new float[output_process_data.num_samples];
			average_.nb_samples = output_process_data.num_samples;
		}

		for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
		{
			float amplitude = .0f;

			float nb_amplitude_for_frame = 0;
			for (uint_fast32_t amplitude_id = 0; amplitude_id < nb_generics_; amplitude_id++)
			{
				if (generics_[amplitude_id].nb_samples > frame)
				{
					nb_amplitude_for_frame++;
					amplitude += generics_[amplitude_id].values[frame];
				}
			}
			if (nb_amplitude_for_frame > 0)
			{
				average_.values[frame] = amplitude / nb_amplitude_for_frame;
			}
			else
			{
				average_.values[frame] = 0;
			}
		}
	}

	uint_fast32_t mixer::get_output_values(const uint_fast16_t slot_id, void* output_values[])
	{
		*output_values = &average_;

		return 1;
	}

	bool mixer::has_finished()
	{
		return true;
	}

	uint_fast8_t mixer::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
		return 0;
	}

	void mixer::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_idss)
	{
	}

	uint_fast16_t mixer::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == generics_input_name)
		{
			return generics_input_id;
		}
		if (slot_name == average_output_name)
		{
			return average_output_id;
		}

		return -1;
	}

	void mixer::set_input_values(const uint_fast16_t slot_id, void* values, const uint_fast32_t nb_values)
	{
		nb_generics_ = nb_values;

		if (slot_id == generics_input_id)
		{
			generics_ = static_cast<float_array_component_output*>(values);

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
} // namespace
