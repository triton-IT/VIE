#pragma once

#include <json.hpp>

#include <q/support/phase.hpp>

#include "../processor_component.hpp"

namespace live::tritone::vie::processor::component
{
	class oscillator final : public processor_component
	{
	public:
		explicit oscillator(nlohmann::json oscillator_definition);
		oscillator(const oscillator&) = default;
		oscillator(oscillator&&) = default;

		oscillator& operator=(const oscillator&) = default;
		oscillator& operator=(oscillator&&) = default;

		~oscillator() override;

		uint16_t get_id() override;

		processor_component_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast32_t get_output_values(uint_fast16_t slot_id, void* output_values[]) override;

		bool has_finished() override;

		uint_fast8_t get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids) override;

		void set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids) override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, void* values, uint_fast16_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

	private:
		constexpr static const char* frequency_input_name = "frequencies input";
		constexpr static int frequency_input_id = 0;

		constexpr static const char* amplitudes_output_name = "amplitudes output";
		constexpr static int amplitudes_output_id = 1;

		uint16_t id_;
		std::string name_;
		std::string type_;
		double sample_rate_;

		std::unordered_map<uint_fast16_t, cycfi::q::phase_iterator>* current_phases_iterators_;
		std::unordered_map<uint_fast16_t, cycfi::q::phase_iterator>* next_phases_iterators_;

		enum class signal_type
		{
			sin,
			saw
		};

		signal_type signal_type_;
		bool can_process_;

		uint_fast8_t nb_outputs_;
		float_array_component_output outputs_[32];
	};
} // namespace
