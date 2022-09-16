#include "envelope.hpp"

#include "../application.hpp"

#include <q/support/literals.hpp>

using namespace cycfi;
using namespace q::literals;

namespace live::tritone::vie::processor::component
{
	envelope::envelope(nlohmann::json envelope_definition) : processor_component(),
		id_(envelope_definition["id"]),
		name_(envelope_definition["name"]),
		envelopes_(new envelope_by_id),
		sample_rate_(0.0),
		nb_velocities_inputs_(0),
		velocities_(nullptr),
		velocities_filled_(false),
		notes_on_filled_(false),
		notes_off_filled_(false),
		nb_outputs_(0),
		nb_zombie_notes_(0)
	{
	}

	envelope::~envelope()
	{
		//FIXME: map can grow unexpectedly. Clear it from time to time instead of only in destructor.
		for (const auto& [note_id, envelope] : *envelopes_)
		{
			delete envelope;
		}

		delete envelopes_;

		//FIXME: delete outputs_ allocated values.
	}

	uint16_t envelope::get_id()
	{
		return id_;
	}

	std::string envelope::get_name()
	{
		return name_;
	}

	processor_component_type envelope::get_type()
	{
		return processor_component_type::middle;
	}

	void envelope::set_sample_rate(const double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	//FIXME: Can't know if all inputs are set-up. So, we should check note by note if it can be processed. 
	bool envelope::can_process()
	{
		return velocities_filled_ && notes_on_filled_ && notes_off_filled_;
	}

	void envelope::process(output_process_data& output_process_data)
	{
		velocities_filled_ = false;
		notes_on_filled_ = false;
		notes_off_filled_ = false;

		nb_outputs_ = 0;
		
		for (uint_fast32_t velocity_index = 0; velocity_index < nb_velocities_inputs_; velocity_index++)
		{
			const uint_fast32_t velocity_input_id = velocities_->output_id;
			
			if (auto envelope_iterator = envelopes_->find(velocity_input_id); envelope_iterator != envelopes_->end())
			{
				uint32_t note_id = envelope_iterator->first;
				q::envelope* q_envelope = envelope_iterator->second;

				float_array_component_output& output = outputs_[velocity_index];

				//If nb of samples is greater than the ones currently allocated, reallocate.
				//It means either that this is the first time we pass here or host changed the number of frames to process.
				if (output_process_data.num_samples > output.nb_samples)
				{
					if (output.nb_samples > 0)
					{
						delete output.values;
					}
					output.values = new float[output_process_data.num_samples];
					output.nb_samples = output_process_data.num_samples;
				}

				output.output_id = velocity_input_id;
				output.nb_samples = 1;

				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output.values[frame] = q_envelope->operator()();
				}

				if(q_envelope->state() == cycfi::q::envelope::note_off_state)
				{
					zombie_notes_ids_.erase(note_id);
					envelopes_->erase(envelope_iterator);
					delete q_envelope;
				}
			}

			nb_outputs_++;
		}
	}

	uint_fast32_t envelope::get_output_values(const uint_fast16_t slot_id, void* output_values[])
	{
		*output_values = outputs_;

		return nb_outputs_;
	}

	bool envelope::has_finished()
	{
		return false;
	}

	void envelope::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
		for(auto zombie_note_id: zombie_notes_ids_)
		{
			zombie_notes_ids.insert(zombie_note_id);
		}
	}

	void envelope::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids)
	{
		//Method only useful for inputs, for them to replay zombie note.
	}

	uint_fast16_t envelope::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == velocities_input_name)
		{
			return velocities_input_id;
		}
		if (slot_name == notes_on_input_name)
		{
			return notes_on_input_id;
		}
		if (slot_name == notes_off_input_name)
		{
			return notes_off_input_id;
		}
		if (slot_name == amplitudes_output_name)
		{
			return amplitudes_output_id;
		}

		return -1;
	}

	void envelope::set_input_values(const uint_fast16_t slot_id, void* values, const uint_fast32_t nb_values)
	{		
		switch (slot_id)
		{
		case notes_on_input_id:
			assert(nb_values <= 32);
			for (uint_fast32_t note_index = 0; note_index < nb_values; note_index++)
			{
				note_event_component_output& note_event_component = static_cast<note_event_component_output*>(values)[
					note_index];
				const auto& [id, channel, pitch, tuning, velocity] = note_event_component.value;
				uint32_t note_id = note_event_component.output_id;

				//FIXME: Remove this zombie notes.
				zombie_notes_ids_.insert(note_id);

				q::envelope* q_envelope;
				if (auto envelope_iterator = envelopes_->find(note_id); envelope_iterator != envelopes_->end())
				{
					q_envelope = envelope_iterator->second;
				}
				else
				{
					q::envelope::config config;
					config.release_rate = 1_s;
					q_envelope = new q::envelope(config, static_cast<uint32_t>(sample_rate_));
					envelopes_->emplace(note_id, q_envelope);
				}
				q_envelope->trigger(velocity * 255);
			}
			notes_on_filled_ = true;
			break;
		case notes_off_input_id:
			assert(nb_values <= 32);
			for (uint_fast32_t note_index = 0; note_index < nb_values; note_index++)
			{
				const note_event_component_output& note_event_component = static_cast<note_event_component_output*>(values)[
					note_index];
				uint32_t note_id = note_event_component.output_id;
				q::envelope* q_envelope = envelopes_->operator[](note_id);
				q_envelope->release();
			}
			notes_off_filled_ = true;
			break;
		case velocities_input_id:
			assert(nb_values <= 32);
			// If nb of velocity is positive, set it. Otherwise, let is as-is, it will serve for the note off.
			if (nb_values > 0) {
				nb_velocities_inputs_ = nb_values;
				if (nb_values > 0) {
					velocities_ = static_cast<float_component_output*>(values);
				}
			}
			velocities_filled_ = true;
			break;
		default:
			break;
		}
	}

	uint_fast32_t envelope::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == notes_on_input_id)
		{
			return 32;
		}

		if (slot_id == notes_off_input_id)
		{
			return 32;
		}

		return -1;
	}

	void envelope::set_parameter(parameter parameter)
	{

	}
} // namespace
