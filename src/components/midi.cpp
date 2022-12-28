#include "midi.hpp"

#include <unordered_set>

#include "../application.hpp"
#include "../constants.hpp"

using namespace std;

namespace live::tritone::vie::processor::component
{
	midi::midi(nlohmann::json midi_definition) : processor_component(),
		id_(midi_definition["id"]),
		name_(midi_definition["name"]),
		nb_values_(0),
		frequencies_outputs_(),
		velocities_outputs_(),
		nb_notes_on_values_(0),
		notes_on_outputs_(),
		nb_notes_off_values_(0),
		notes_off_outputs_()
	{
		for (int i = 0; i < 32; i++)
			frequencies_outputs_[i] = new float_component_output();
		
		for (int i = 0; i < 32; i++)
			velocities_outputs_[i] = new float_component_output();
		
		for(int i = 0; i < 32; i++)
			notes_on_outputs_[i] = new novalue_component_output();
		
		for (int i = 0; i < 32; i++)
			notes_off_outputs_[i] = new novalue_component_output();
	}

	midi::~midi() {
		for (int i = 0; i < 32; i++) {
			delete frequencies_outputs_[i];
		}

		for (int i = 0; i < 32; i++) {
			delete velocities_outputs_[i];
		}

		for (int i = 0; i < 32; i++) {
			delete notes_on_outputs_[i];
		}

		for (int i = 0; i < 32; i++) {
			delete notes_off_outputs_[i];
		}
	}

	std::uint16_t midi::get_id()
	{
		return id_;
	}

	std::string midi::get_name()
	{
		return name_;
	}

	processor_component_type midi::get_type()
	{
		return processor_component_type::event_input;
	}

	void midi::set_sample_rate(double sample_rate)
	{
	}

    uint_fast16_t midi::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == frequencies_output_name)
		{
			return frequencies_output_id;
		}
		else if (slot_name == velocities_output_name)
		{
			return velocities_output_id;
		}
		else if (slot_name == notes_on_output_name)
		{
			return notes_on_output_id;
		}
		else if (slot_name == notes_off_output_name)
		{
			return notes_off_output_id;
		}

		return -1;
	}

	void midi::set_input_values(uint_fast16_t slot_id, component_output* values[32], uint_fast32_t nb_values)
	{
	}

	uint_fast32_t midi::get_max_nb_input_values(uint_fast16_t slot_id)
	{
		return 0;
	}

	void midi::preprocess() {
	}

	bool midi::can_process()
	{
		return true;
	}

	void midi::process(output_process_data& output_process_data)
	{
		if (is_on) {
			//Get all outputs information from midi event.
			nb_values_ = 0;
			for (auto& [note_id, note_on_event] : notes_)
			{
				float_component_output* frequencies_output = frequencies_outputs_[nb_values_];
				frequencies_output->note_id = note_id;
				const float frequency = midi_notes_frequencies[note_on_event.id];
				frequencies_output->value = frequency;

				float_component_output* velocities_output = velocities_outputs_[nb_values_];
				velocities_output->note_id = note_id;
				velocities_output->value = note_on_event.velocity;

				nb_values_++;
			}
		}
	}

	component_output** midi::get_outputs_pool(uint_fast16_t slot_id) {
		switch (slot_id)
		{
		case frequencies_output_id:
			return (component_output**) frequencies_outputs_;
		case velocities_output_id:
			return (component_output**) velocities_outputs_;
		case notes_on_output_id:
			return (component_output**) notes_on_outputs_;
		case notes_off_output_id:
			return (component_output**) notes_off_outputs_;
		default:
			break;
		}
	}

	uint_fast32_t midi::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		uint_fast32_t nb_values = -1;
		switch (slot_id)
		{
		case frequencies_output_id:
			output_values = (component_output**) frequencies_outputs_;
			nb_values = nb_values_;
			break;
		case velocities_output_id:
			output_values = (component_output**) velocities_outputs_;
			nb_values = nb_values_;
			break;
		case notes_on_output_id:
			output_values = (component_output**) notes_on_outputs_;
			nb_values = nb_notes_on_values_;
			nb_notes_on_values_ = 0;
			break;
		case notes_off_output_id:
			output_values = (component_output**) notes_off_outputs_;
			nb_values = nb_notes_off_values_;
			nb_notes_off_values_ = 0;
			break;
		default:
			break;
		}
		return nb_values;
	}

	bool midi::has_finished()
	{
		return true;
	}

	void midi::note_on(note_event& note_on_event)
	{
#ifdef VIE_DEBUG
		debugLogger.write("Midi: Note on emitted" + note_on_event.id);
#endif
		const uint32_t midi_note_id = (get_id() << sizeof(uint16_t)) + note_on_event.pitch;
		note_on_event.id = midi_note_id;
		notes_.emplace(midi_note_id, note_on_event);
		notes_on_outputs_[nb_notes_on_values_]->note_id = midi_note_id;
		nb_notes_on_values_++;
	}

	void midi::note_off(note_event& note_off_event)
	{
#ifdef VIE_DEBUG
		debugLogger.write("Midi: Note off emitted" + note_off_event.id);
#endif
		const uint32_t midi_note_id = (get_id() << sizeof(uint16_t)) + note_off_event.pitch;
		note_off_event.id = midi_note_id;
		
		notes_off_outputs_[nb_notes_off_values_]->note_id = midi_note_id;
		nb_notes_off_values_++;
		
		//Erase note from notes on.
		notes_.erase(midi_note_id);
	}

	void midi::set_parameter(parameter parameter)
	{
		if (parameter.get_title() == L"on") {
			if (parameter.get_normalized_value() == 0.0f) {
				is_on = false;
			}
			else {
				is_on = true;
			}
		}
	}
} // namespace
