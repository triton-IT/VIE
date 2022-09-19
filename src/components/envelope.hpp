#pragma once

#include <json.hpp>

#include "../processor_component.hpp"

#include <q/synth/envelope.hpp>

namespace live::tritone::vie::processor::component
{
	class envelope final : public processor_component
	{
	public:
		explicit envelope(nlohmann::json envelope_definition);

		envelope(const envelope&) = default;

		envelope(envelope&&) = default;

		~envelope() override;

		envelope& operator=(const envelope&) = default;

		envelope& operator=(envelope&&) = default;

		uint16_t get_id() override;

		std::string get_name() override;

		processor_component_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast32_t get_output_values(uint_fast16_t slot_id, void* output_values[]) override;

		bool has_finished() override;

		void get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids) override;

		void set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids) override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, void* values, uint_fast32_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_parameter(parameter parameter) override;

	private:
		constexpr static const char* velocities_input_name = "velocities input";
		constexpr static int velocities_input_id = 0;
		constexpr static const char* notes_on_input_name = "notes on input";
		constexpr static int notes_on_input_id = 1;
		constexpr static const char* notes_off_input_name = "notes off input";
		constexpr static int notes_off_input_id = 2;

		constexpr static const char* amplitudes_output_name = "amplitudes output";
		constexpr static int amplitudes_output_id = 3;

		uint16_t id_;
		std::string name_;
		std::string type_;

		typedef std::unordered_map<uint32_t, cycfi::q::envelope*> envelope_by_id;
		envelope_by_id* envelopes_;

		double sample_rate_;

		uint_fast16_t nb_velocities_inputs_;
		float_component_output* velocities_;

		bool velocities_filled_;
		bool notes_on_filled_;
		bool notes_off_filled_;

		uint_fast8_t nb_outputs_;
		float_array_component_output outputs_[32];

		uint_fast8_t nb_zombie_notes_;

		/**
		 * A given zombie note is a zombie note that is set as input.
		 */
		//std::unordered_set<uint32_t> given_zombie_notes_ids_;
		/**
		 * A requested zombie note is a zombie note that this component requests to caller.
		 */
		std::unordered_set<uint32_t> zombie_notes_ids_;
	};
} // namespace
