#pragma once

#include <json.hpp>
#include <unordered_set>

#include "../processor_component.hpp"
#include "../note_event.hpp"

namespace live::tritone::vie::processor::component
{
	class midi final : public processor_component
	{
	public:
		/**
		* Initialize class based on given JSON configuration.
		*/
		explicit midi(nlohmann::json midi_definition);

		uint16_t get_id() override;

		processor_component_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		uint_fast32_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, void* values, uint_fast32_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast32_t get_output_values(uint_fast16_t slot_id, void* output_values[]) override;

		bool has_finished() override;

		uint_fast8_t get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids) override;

		void set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids) override;

		/* Receive a note on event from host and set its id.*/
		void note_on(note_event& note_on_event);

		/* Receive a note off event from host and set its id.*/
		void note_off(note_event& note_off_event);

	private:
		static constexpr const char* notes_on_output_name = "notes on output";
		static constexpr int notes_on_output_id = 0;
		static constexpr const char* notes_off_output_name = "notes off output";
		static constexpr int notes_off_output_id = 1;
		static constexpr const char* frequencies_output_name = "frequencies output";
		static constexpr int frequencies_output_id = 2;
		static constexpr const char* velocities_output_name = "velocities output";
		static constexpr int velocities_output_id = 3;

		uint16_t id_;

		std::string name_;
		std::unordered_map<uint32_t, note_event> notes_;

		uint_fast32_t nb_values_;
		float_component_output frequencies_outputs_[32];
		float_component_output velocities_outputs_[32];
		uint_fast32_t nb_notes_on_values_;
		note_event_component_output notes_on_outputs_[32];
		uint_fast32_t nb_notes_off_values_;
		note_event_component_output notes_off_outputs_[32];
		
		std::unordered_map<uint32_t, note_event> zombie_notes_;

		void handle_processing_(uint32_t note_id, note_mode note_mode, const note_event& note_on_event);
	};
} // namespace
