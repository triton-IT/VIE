#include "midi.hpp"

#include <unordered_set>

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
		if (slot_name == frequencies_output_name)
		{
			return frequencies_output_id;
		}
		if (slot_name == velocities_output_name)
		{
			return velocities_output_id;
		}
		if (slot_name == notes_on_output_name)
		{
			return notes_on_output_id;
		}
		if (slot_name == notes_off_output_name)
		{
			return notes_off_output_id;
		}

		return -1;
	}

	void midi::set_input_values(uint_fast16_t slot_id, void* values, uint_fast32_t nb_values)
	{
	}

	uint_fast32_t midi::get_max_nb_input_values(uint_fast16_t slot_id)
	{
		return 0;
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
				//If element is found as 'note on' it's not a zombie anymore.
				zombie_notes_.erase(note_id);

				handle_processing_(note_id, note_mode::normal, note_on_event);
			}

			for (auto& [note_id, note_on_event] : zombie_notes_)
			{
				handle_processing_(note_id, note_mode::zombie, note_on_event);
			}
		}
	}

	void midi::handle_processing_(uint32_t note_id, note_mode note_mode, const note_event& note_event)
	{
		float_component_output& frequencies_output = frequencies_outputs_[nb_values_];
		frequencies_output.output_id = note_id;
		frequencies_output.note_mode = note_mode;
		const float frequency = midi_notes_frequencies[note_event.id];
		frequencies_output.value = frequency;

		float_component_output& velocities_output = velocities_outputs_[nb_values_];
		velocities_output.output_id = note_id;
		velocities_output.note_mode = note_mode;
		velocities_output.value = note_event.velocity;

		nb_values_++;
	}

	uint_fast32_t midi::get_output_values(const uint_fast16_t slot_id, void* output_values[])
	{
		uint_fast32_t nb_values = -1;
		switch (slot_id)
		{
		case frequencies_output_id:
			*output_values = frequencies_outputs_;
			nb_values = nb_values_;
			break;
		case velocities_output_id:
			*output_values = velocities_outputs_;
			nb_values = nb_values_;
			break;
		case notes_on_output_id:
			*output_values = notes_on_outputs_;
			nb_values = nb_notes_on_values_;
			nb_notes_on_values_ = 0;
			break;
		case notes_off_output_id:
			*output_values = notes_off_outputs_;
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

	void midi::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	void midi::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids)
	{
		zombie_notes_.clear();

		for (auto note_id : zombie_notes_ids) {
			auto note_iterator = notes_.find(note_id);
			if (note_iterator != notes_.end()) {
				zombie_notes_.emplace(note_id, note_iterator->second);
			}
		}

		// Delete all notes that are off and not zombie.
		for (auto iterator = notes_ids_to_delete_.begin(), end = notes_ids_to_delete_.end(); iterator != end;) {
			auto& note_id_to_delete = *iterator;
			if (zombie_notes_ids.find(note_id_to_delete) == zombie_notes_ids.end()) {
				notes_.erase(note_id_to_delete);
				iterator = notes_ids_to_delete_.erase(iterator);
			}
			else {
				iterator++;
			}
		}
	}

	void midi::note_on(note_event& note_on_event)
	{
		const uint32_t midi_note_id = (get_id() << sizeof(uint16_t)) + note_on_event.pitch;
		note_on_event.id = midi_note_id;
		notes_.emplace(midi_note_id, note_on_event);
		note_event_component_output note_on_event_output;
		note_on_event_output.output_id = midi_note_id;
		note_on_event_output.value = note_on_event;
		notes_on_outputs_[nb_notes_on_values_] = note_on_event_output;
		nb_notes_on_values_++;
	}

	void midi::note_off(note_event& note_off_event)
	{
		const uint32_t midi_note_id = (get_id() << sizeof(uint16_t)) + note_off_event.pitch;
		note_off_event.id = midi_note_id;
		
		note_event_component_output note_off_event_output;
		note_off_event_output.output_id = midi_note_id;
		note_off_event_output.value = note_off_event;
		notes_off_outputs_[nb_notes_off_values_] = note_off_event_output;
		nb_notes_off_values_++;

		// We cannot erase note from notes_ because a zombie note could still need its definition.
		// So mark it as deleted and delete if not required by a zombie note.
		notes_ids_to_delete_.emplace(midi_note_id);
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
