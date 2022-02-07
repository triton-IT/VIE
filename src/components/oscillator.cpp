#include "oscillator.hpp"

#include "../application.hpp"

#include <q/synth/sin.hpp>
#include <q/synth/saw.hpp>

using namespace cycfi;

namespace live::tritone::vie::processor::component
{
	oscillator::oscillator(nlohmann::json oscillator_definition) :
		id_(oscillator_definition["id"]),
		name_(oscillator_definition["name"]),
		sample_rate_(.0),
		current_phases_iterators_(new std::unordered_map<uint_fast16_t, q::phase_iterator>),
		next_phases_iterators_(new std::unordered_map<uint_fast16_t, q::phase_iterator>),
		can_process_(false),
		nb_outputs_(0)
	{
		auto& bindings_definition = oscillator_definition["bindings"];

		type_ = bindings_definition["type"];
		if (type_ == "sin")
		{
			signal_type_ = signal_type::sin;
		}
		if (type_ == "saw")
		{
			signal_type_ = signal_type::saw;
		}
	}

	oscillator::~oscillator()
	{
		for (uint_fast32_t i = 0; i < nb_outputs_; i++)
		{
			if (const float_array_component_output output = outputs_[nb_outputs_]; output.nb_samples > 0)
			{
				delete output.values;
			}
		}

		delete current_phases_iterators_;
		delete next_phases_iterators_;
	}

	uint16_t oscillator::get_id()
	{
		return id_;
	}

	processor_component_type oscillator::get_type()
	{
		return processor_component_type::middle;
	}

	void oscillator::set_sample_rate(const double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	bool oscillator::can_process()
	{
		return can_process_;
	}

	void oscillator::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		nb_outputs_ = 0;
		for (auto& [noteId, phaseIterator] : *current_phases_iterators_)
		{
			float_array_component_output& output = outputs_[nb_outputs_];

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

			output.output_id = noteId;
			switch (signal_type_)
			{
			case signal_type::sin:
				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output.values[frame] = q::sin(phaseIterator);
					++phaseIterator;
				}
				nb_outputs_++;
				break;
			case signal_type::saw:
				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output.values[frame] = q::saw(phaseIterator);
					++phaseIterator;
				}
				nb_outputs_++;
				break;
			}
		}
	}

	uint_fast32_t oscillator::get_output_values(const uint_fast16_t slot_id, void* output_values[])
	{
		*output_values = outputs_;
		return nb_outputs_;
	}

	bool oscillator::has_finished()
	{
		return true;
	}

	uint_fast8_t oscillator::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
		return 0;
	}

	void oscillator::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	uint_fast16_t oscillator::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == frequency_input_name)
		{
			return frequency_input_id;
		}
		if (slot_name == amplitudes_output_name)
		{
			return amplitudes_output_id;
		}

		//FIXME: Do not return -1 on a uint !
		return -1;
	}

	void oscillator::set_input_values(const uint_fast16_t slot_id, void* values, const uint_fast16_t nb_values)
	{
		if (slot_id == frequency_input_id)
		{
			for (uint_fast16_t i = 0; i < nb_values; i++)
			{
				const auto& component_output = static_cast<float_component_output*>(values)[i];

				const auto note_id = component_output.output_id;

				if (const auto phase_iterator = current_phases_iterators_->find(note_id); phase_iterator != current_phases_iterators_->end())
				{
					//If frequency was already set before, we don't want to erase its phase.
					//So, keep the old one.
					next_phases_iterators_->operator[](note_id) = phase_iterator->second;
				}
				else
				{
					//If frequency is a new one, create a phase for it.
					const auto raw_frequency = component_output.value;
					const auto frequency = q::frequency(static_cast<double>(raw_frequency));
					next_phases_iterators_->operator[](note_id) = q::phase_iterator(
						frequency, static_cast<std::uint32_t>(sample_rate_));
				}
			}

			//Switching between current and next phases is done to simplify the deletion of phases no more used.
			std::unordered_map<uint_fast32_t, q::phase_iterator>* tmp_phases_iterators = current_phases_iterators_;
			current_phases_iterators_ = next_phases_iterators_;
			next_phases_iterators_ = tmp_phases_iterators;

			//Clear the phases for next call.
			next_phases_iterators_->clear();

			can_process_ = true;
		}
	}

	uint_fast32_t oscillator::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == frequency_input_id)
		{
			return 32;
		}

		return -1;
	}
} // namespace
