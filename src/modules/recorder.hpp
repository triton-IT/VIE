#pragma once

#include <json.hpp>
#include <sndfile.h>
#include <sndfile.hh>

#include "../processor_module.hpp"

namespace live::tritone::vie::processor::module
{
	class recorder final : public processor_module
	{
	public:
		enum class signal_type
		{
			sin,
			saw
		};

		explicit recorder(nlohmann::json recorder_definition);
		recorder(const recorder&) = default;
		recorder(recorder&&) = default;

		~recorder() override;

		recorder& operator=(const recorder&) = default;
		recorder& operator=(recorder&&) = default;

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
		
		constexpr static const char* amplitude_input_name = "amplitudes input";
		constexpr static int amplitude_input_id = 1;

		constexpr static const char* amplitude_output_name = "amplitudes output";
		constexpr static int amplitude_output_id = 2;

		uint16_t id_;
		std::string name_;
		std::string type_;
		double sample_rate_;
		bool on_;

		bool can_process_;

		std::array<float_array_module_output*, 32> output_;
		uint_fast32_t nb_outputs_;
		
		SNDFILE* output_file_;
	};
} // namespace
