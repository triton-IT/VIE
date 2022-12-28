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
		velocities_filled_(false),
		notes_on_filled_(false),
		notes_off_filled_(false),
		nb_outputs_(0),
		outputs_()
	{
		for (int i = 0; i < 32; i++) {
			outputs_[i] = new float_array_component_output();
		}
		for (int i = 0; i < 32; i++) {
			velocities_[i] = new novalue_component_output();
		}
	}

	envelope::~envelope()
	{
		//FIXME: map can grow unexpectedly. Clear it from time to time instead of only in destructor.
		for (const auto& [note_id, envelope] : *envelopes_)
		{
			delete envelope;
		}

		delete envelopes_;

		for (int i = 0; i < 32; i++) {
			delete velocities_[i];
		}
		for (int i = 0; i < 32; i++) {
			delete outputs_[i];
		}
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

	void envelope::preprocess() {
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

		// FIXME: Trigger correctly (only once per note on event). 
		// Cannot be done in "note on" event because velocity is perhaps not initialized yet.
		// Cannot be done in "velocity" event because note on is perhaps not initialized yet.
		//q_envelope->trigger(velocity * 255);

		nb_outputs_ = 0;
		
		for (uint_fast32_t velocity_index = 0; velocity_index < nb_velocities_inputs_; velocity_index++)
		{
			const uint_fast32_t velocity_output_id = velocities_[velocity_index]->note_id;
			
			if (auto envelope_iterator = envelopes_->find(velocity_output_id); envelope_iterator != envelopes_->end())
			{
				uint32_t note_id = envelope_iterator->first;
				q::envelope* q_envelope = envelope_iterator->second;

				float_array_component_output& output = *(outputs_[velocity_index]);

				//If nb of samples is greater than the ones currently allocated, reallocate.
				//It means either that this is the first time we pass here or host changed the number of frames to process.
				if (output_process_data.num_samples > output.values.nb_values)
				{
					if (output.values.nb_values > 0)
					{
						delete output.values.values;
					}
					output.values.values = new float[output_process_data.num_samples];
					output.values.nb_values = output_process_data.num_samples;
				}

				output.note_id = velocity_output_id;
				output.values.nb_values = 1;

				for (uint_fast32_t frame = 0; frame < output_process_data.num_samples; frame++)
				{
					output.values.values[frame] = q_envelope->operator()();
				}

				if(q_envelope->state() == cycfi::q::envelope::note_off_state)
				{
					envelopes_->erase(envelope_iterator);
					delete q_envelope;
				}
			}

			nb_outputs_++;
		}
	}

	component_output** envelope::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) outputs_;
	}

	uint_fast32_t envelope::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		output_values = (component_output**) outputs_;

		return nb_outputs_;
	}

	bool envelope::has_finished()
	{
		return false;
	}

	uint_fast16_t envelope::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == velocities_input_name)
		{
			return velocities_input_id;
		}
		else if (slot_name == notes_on_input_name)
		{
			return notes_on_input_id;
		}
		else if (slot_name == notes_off_input_name)
		{
			return notes_off_input_id;
		}
		else if (slot_name == amplitudes_output_name)
		{
			return amplitudes_output_id;
		}

		return -1;
	}

	void envelope::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{		
		switch (slot_id)
		{
		case notes_on_input_id:
			assert(nb_values <= 32);
			for (uint_fast32_t note_index = 0; note_index < nb_values; note_index++)
			{
				component_output* note_event_component = values[note_index];
				uint32_t note_id = note_event_component->note_id;

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
			}
			notes_on_filled_ = true;
			break;
		case notes_off_input_id:
			assert(nb_values <= 32);
			for (uint_fast32_t note_index = 0; note_index < nb_values; note_index++)
			{
				const component_output* note_event_component = values[note_index];
				uint32_t note_id = note_event_component->note_id;
				q::envelope* q_envelope = envelopes_->operator[](note_id);
				q_envelope->release();
			}
			notes_off_filled_ = true;
			break;
		case velocities_input_id:
			assert(nb_values <= 32);
			// If nb of velocity is positive, set it. Otherwise, let it as-is, it will serve for the note off.
			if (nb_values > 0) {
				nb_velocities_inputs_ = nb_values;
				if (nb_values > 0) {
					for (int i = 0; i < 32; i++) {
						velocities_[i]->note_id = values[i]->note_id;
					}
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
