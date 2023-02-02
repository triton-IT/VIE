#pragma once

#include <json.hpp>

#include "../processor_module.hpp"

#include <q/synth/envelope.hpp>

namespace live::tritone::vie::processor::module
{
	class envelope final : public processor_module
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

		processor_module_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		void preprocess() override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast8_t get_output_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_parameter(parameter parameter) override;

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;
		constexpr static const char* velocities_input_name = "velocities input";
		constexpr static int velocities_input_id = 1;
		constexpr static const char* notes_on_input_name = "notes on input";
		constexpr static int notes_on_input_id = 2;
		constexpr static const char* notes_off_input_name = "notes off input";
		constexpr static int notes_off_input_id = 3;

		constexpr static const char* amplitudes_output_name = "amplitudes output";
		constexpr static int amplitudes_output_id = 4;

		constexpr static const char* notes_off_output_name = "Note off output";
		constexpr static int notes_off_output_id = 5;

		constexpr static const char* sustains_starts_output_name = "Sustain start output";
		constexpr static int sustains_starts_output_id = 6;

		constexpr static const char* sustains_ends_output_name = "Sustain end output";
		constexpr static int sustains_ends_output_id = 7;

		constexpr static const char* sustains_loops_output_name = "Sustain loop output";
		constexpr static int sustains_loops_output_id = 8;

		uint16_t id_;
		std::string name_;
		std::string type_;

		typedef struct envelope_info {
			envelope_info() : is_processed(false), envelope(nullptr), position(0) {};
			cycfi::q::envelope* envelope;
			cycfi::q::envelope::config config;
			bool is_processed;
			cycfi::q::duration position;
		} envelope_info;
		
		typedef std::unordered_map<uint32_t, envelope_info> envelope_by_id;
		envelope_by_id envelopes_;

		double sample_rate_;

		uint_fast16_t nb_velocities_inputs_;
		std::array<float_module_output*, 32> velocities_;

		bool velocities_filled_;
		bool notes_on_filled_;
		bool notes_off_filled_;

		uint_fast16_t nb_notes_off_ = 0;
		std::array<novalue_module_output*, 32> notes_off_;

		uint_fast8_t nb_outputs_;
		std::array<float_array_module_output*, 32> outputs_;

		uint_fast16_t nb_sustains_starts_ = 0;
		/**
		* Send an event when marker of sustain start is reached.
		* Value is the offset of the sustain start marker.
		*/
		std::array<float_module_output*, 32> sustains_starts_;

		uint_fast16_t nb_sustains_ends_ = 0;
		/**
		* Send an event when marker of sustain end is reached.
		* Value is the offset of the sustain end marker.
		*/
		std::array<float_module_output*, 32> sustains_ends_;

		uint_fast8_t nb_sustains_loops_ = 0;
		/**
		* Send an event when marker of sustain end is reached.
		* Value is the offset of the sustain start marker.
		* This event can be used to move a sample to a start marker when a sustain end marker is reached.
		*/
		std::array<float_module_output*, 32> sustains_loops_;
	};
} // namespace
