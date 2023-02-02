#pragma once

#include <json.hpp>

#include "../processor_module.hpp"

namespace live::tritone::vie::processor::module
{
	class audio_output final : public processor_module
	{
	public:
		explicit audio_output(nlohmann::json output_definition);
		audio_output(const audio_output&) = default;
		audio_output(audio_output&&) = default;

		~audio_output() override;

		audio_output& operator=(const audio_output&) = default;
		audio_output& operator=(audio_output&&) = default;

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

		void set_output_bus_id(uint_fast16_t bus_id);

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;
		
		static constexpr const char* amplitudes_input_name = "amplitudes input";
		static constexpr int amplitudes_input_id = 1;

		uint16_t id_;
		std::string name_;
		std::string type_;
		uint_fast8_t nb_outputs_;

		std::array<float_array_module_output, 32> amplitudes_;

		bool can_process_;

		uint_fast16_t output_bus_id_;
	};
} // namespace
