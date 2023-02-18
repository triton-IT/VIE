#include "noise.hpp"

#include "../application.hpp"

#include <q/synth/noise.hpp>

using namespace cycfi;
using namespace q::literals;

namespace live::tritone::vie::processor::module
{
	noise::noise(nlohmann::json noise_definition) :
		id_(noise_definition["id"]),
		name_(noise_definition["name"]),
		current_noises_descriptors_(new std::unordered_map<uint_fast16_t, noise_descriptor<void>>),
		next_noises_descriptors_(new std::unordered_map<uint_fast16_t, noise_descriptor<void>>),
		sample_rate_(0.0),
		noise_type_(noise_type::white),
		can_process_(false),
		nb_outputs_(0)
	{
		for (uint_fast8_t i = 0; i < 32; i++) {
			outputs_[i] = new float_array_module_output();
		}
	}

	noise::~noise()
	{
		for (uint_fast8_t i = 0; i < 32; i++) {
			delete outputs_[i];
		}
	}

	uint16_t noise::get_id()
	{
		return id_;
	}

	std::string noise::get_name()
	{
		return name_;
	}

	processor_module_type noise::get_type()
	{
		return processor_module_type::middle;
	}

	void noise::set_sample_rate(const double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	void noise::preprocess() {
	}

	//FIXME: Can't know if all inputs are set-up. So, we should check note by note if it can be processed. 
	bool noise::can_process()
	{
		return can_process_;
	}

	void noise::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		nb_outputs_ = 0;
		for (auto& [note_id, noise_descriptor] : *current_noises_descriptors_)
		{
			float_array_module_output* output = outputs_[nb_outputs_];

			//If nb of samples is greater than the ones currently allocated, reallocate.
			if (output_process_data.num_samples > output->values.nb_values)
			{
				if (output->values.nb_values > 0)
				{
					delete[] output->values.values;
				}
				output->values.values = new float[output_process_data.num_samples];
				output->values.nb_values = output_process_data.num_samples;
			}

			output->note_id = note_id;

			void* synth = noise_descriptor.noise_synth;
			switch (noise_type_)
			{
			case noise_type::white:
			{
				auto white_noise_synth = static_cast<cycfi::q::white_noise_synth*>(synth);
				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output->values.values[frame] = white_noise_synth->operator()();
				}
			}
				break;
			case noise_type::pink:
			{
				auto pink_noise_synth = static_cast<cycfi::q::pink_noise_synth*>(synth);
				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output->values.values[frame] = pink_noise_synth->operator()();
				}
			}
				break;
			case noise_type::brown:
			{
				auto brown_noise_synth = static_cast<cycfi::q::brown_noise_synth*>(synth);
				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output->values.values[frame] = brown_noise_synth->operator()();
				}
			}
				break;
			}
			nb_outputs_++;
		}
	}

	uint_fast8_t noise::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		switch (slot_id)
		{
		case amplitudes_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(outputs_);
			return nb_outputs_;
		}
		
		throw std::invalid_argument("Invalid slot id");
	}

	bool noise::has_finished()
	{
		return false;
	}

	uint_fast16_t noise::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	void noise::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		switch (slot_id)
		{
		case onoff_input_id:
			assert(nb_values > 0);
			for (uint_fast16_t i = 0; i < nb_values; i++)
			{
				const auto module_output = values[i];

				uint32_t input_id = module_output->note_id;

				if (const auto current_noise_descriptor = current_noises_descriptors_->find(input_id); current_noise_descriptor != current_noises_descriptors_->end())
				{
					//If synth was already set before, we don't want to erase it.
					//So, keep the old one.
					noise_descriptor<void>& noise_descriptor = next_noises_descriptors_->operator[](input_id);
					noise_descriptor.noise_synth = current_noise_descriptor->second.noise_synth;
				}
				else
				{
					//If synth is a new one, create it.
					noise_descriptor<void>& noise_descriptor = next_noises_descriptors_->operator[](input_id);
					noise_descriptor.noise_synth = new q::white_noise_synth{};
				}
			}

			//Switching between current and next synth is done to simplify the deletion of synths no more used.
			std::unordered_map<uint_fast16_t, noise_descriptor<void>>* tmp_noises_descriptors = current_noises_descriptors_;
			current_noises_descriptors_ = next_noises_descriptors_;
			next_noises_descriptors_ = tmp_noises_descriptors;

			//Clear the phases for next call.
			next_noises_descriptors_->clear();

			can_process_ = true;

			return;
		}
		
		throw std::invalid_argument("Invalid slot name");
	}

	uint_fast32_t noise::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == onoff_input_id)
		{
			return 32;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	void noise::set_noise_type(noise_type noise_type) {
		noise_type_ = noise_type;
	}
} // namespace
