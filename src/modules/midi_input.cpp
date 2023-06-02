#include "midi_input.hpp"

#include <unordered_set>

#include "../application.hpp"
#include "../constants.hpp"

#ifdef _WIN32
#define ON_PARAMETER_ID L"on"
#else
#define ON_PARAMETER_ID u"on"
#endif

using namespace std;

namespace live::tritone::vie::processor::module
{
	midi_input::midi_input(nlohmann::json midi_definition) : processor_module(),
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
		for (int i = 0; i < 32; i++) {
			frequencies_outputs_[i] = new float_module_output();
			velocities_outputs_[i] = new float_module_output();
			notes_on_outputs_[i] = new novalue_module_output();
			notes_off_outputs_[i] = new novalue_module_output();
		}
	}

	midi_input::~midi_input() {
		for (int i = 0; i < 32; i++) {
			delete frequencies_outputs_[i];
			delete velocities_outputs_[i];
			delete notes_on_outputs_[i];
			delete notes_off_outputs_[i];
		}
	}

	std::uint16_t midi_input::get_id()
	{
		return id_;
	}

	std::string midi_input::get_name()
	{
		return name_;
	}

	processor_module_type midi_input::get_type()
	{
		return processor_module_type::event_input;
	}

	void midi_input::set_sample_rate(double sample_rate)
	{
	}

    uint_fast16_t midi_input::get_slot_id(const std::string& slot_name)
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

		throw std::invalid_argument("Invalid slot name");
	}

	void midi_input::set_input_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		throw std::invalid_argument("Invalid slot id");
	}

	uint_fast32_t midi_input::get_max_nb_input_values(uint_fast16_t slot_id)
	{
		throw std::invalid_argument("Invalid slot name");
	}

	void midi_input::preprocess() {
		nb_values_ = 0;
		
		nb_notes_on_values_ = 0;
		nb_notes_off_values_ = 0;
	}

	bool midi_input::can_process()
	{
		return true;
	}

	void midi_input::process(output_process_data& output_process_data)
	{
		//If module is on and has not already been processed.
		if (is_on && nb_values_ == 0) {
			//Get all outputs information from midi event.
			for (auto& [note_id, note_on_event] : notes_)
			{
				auto frequencies_output = frequencies_outputs_[nb_values_];
				frequencies_output->note_id = note_id;
				const float frequency = midi_notes_frequencies[note_on_event.id];
				frequencies_output->value = frequency;

				auto velocities_output = velocities_outputs_[nb_values_];
				velocities_output->note_id = note_id;
				velocities_output->value = note_on_event.velocity;

				nb_values_++;
			}
		}
	}

	uint_fast8_t midi_input::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		uint_fast32_t nb_values = -1;
		switch (slot_id)
		{
		case frequencies_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(frequencies_outputs_);
			return nb_values_;
		case velocities_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(velocities_outputs_);
			return nb_values_;
		case notes_on_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(notes_on_outputs_);
			note_on_processed = true;
			return nb_notes_on_values_;
		case notes_off_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(notes_off_outputs_);
			note_off_processed = true;
			return nb_notes_off_values_;
		default:
			break;
		}
		
		throw std::invalid_argument("Invalid slot id");
	}

	bool midi_input::has_finished()
	{
		return true;
	}

	void midi_input::note_on(note_event& note_on_event)
	{
#ifdef VIE_DEBUG
		debugLogger.write("Midi in: Note on emitted" + note_on_event.id);
#endif
		note_on_processed = false;
		
		if (nb_notes_on_values_ < 32)
		{
			const uint32_t midi_note_id = (get_id() << sizeof(uint16_t)) + note_on_event.pitch;
			note_on_event.id = midi_note_id;
			notes_.emplace(midi_note_id, note_on_event);
			notes_on_outputs_[nb_notes_on_values_]->note_id = midi_note_id;
			nb_notes_on_values_++;
		}
	}

	void midi_input::note_off(note_event& note_off_event)
	{
#ifdef VIE_DEBUG
		debugLogger.write("Midi in: Note off emitted" + note_off_event.id);
#endif
		note_off_processed = false;

		if (nb_notes_on_values_ < 32)
		{
			const uint32_t midi_note_id = (get_id() << sizeof(uint16_t)) + note_off_event.pitch;
			note_off_event.id = midi_note_id;

			notes_off_outputs_[nb_notes_off_values_]->note_id = midi_note_id;
			nb_notes_off_values_++;

			//Erase note from notes on.
			notes_.erase(midi_note_id);
		}
	}

	void midi_input::set_parameter(parameter parameter)
	{
		if (parameter.is_title(ON_PARAMETER_ID)) {
			if (parameter.get_normalized_value() == 0.0f) {
				is_on = false;
			}
			else {
				is_on = true;
			}
		}
	}
} // namespace
