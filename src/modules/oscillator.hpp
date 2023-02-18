#pragma once

#include <json.hpp>

#include <q/support/phase.hpp>

#include "../processor_module.hpp"

namespace live::tritone::vie::processor::module
{
	class oscillator final : public processor_module
	{
	public:
		enum class signal_type
		{
			sin,
			saw
		};

		explicit oscillator(nlohmann::json oscillator_definition);
		oscillator(const oscillator&) = default;
		oscillator(oscillator&&) = default;

		~oscillator() override;

		oscillator& operator=(const oscillator&) = default;
		oscillator& operator=(oscillator&&) = default;

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
		
		constexpr static const char* frequency_input_name = "frequencies input";
		constexpr static int frequency_input_id = 1;

		constexpr static const char* signal_type_input_name = "signal type input";
		constexpr static int signal_type_input_id = 2;

		constexpr static const char* amplitudes_output_name = "amplitudes output";
		constexpr static int amplitudes_output_id = 3;

		uint16_t id_;
		std::string name_;
		std::string type_;
		double sample_rate_;

		struct phase_descriptor {
			cycfi::q::phase_iterator phase_iterator;
		};

		std::unordered_map<uint_fast16_t, phase_descriptor>* current_phases_descriptors_;
		std::unordered_map<uint_fast16_t, phase_descriptor>* next_phases_descriptors_;

		signal_type signal_type_;

		bool can_process_;

		uint_fast8_t nb_outputs_;
		std::array<float_array_module_output*, 32> outputs_;
	};
} // namespace
