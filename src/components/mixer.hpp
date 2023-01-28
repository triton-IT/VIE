#pragma once

#include <json.hpp>

#include "../processor_component.hpp"

namespace live::tritone::vie::processor::component
{
	/**
	* @brief A sample processor component.
	* 
	*/
	class mixer final : public processor_component
	{
	public:
		explicit mixer(nlohmann::json mixer_definition);

		~mixer();

		uint16_t get_id() override;

		std::string get_name() override;

		processor_component_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		void preprocess() override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		/**
		* Set a pair<string, float> where string is the id of this mixer and float is the mixer output value.
		*/
		uint_fast8_t get_output_values(uint_fast16_t slot_id, std::array<component_output*, 32>& values) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, std::array<component_output*, 32>& values, uint_fast8_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_parameter(parameter parameter) override;

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;
		
		constexpr static const char* generics_input_name = "generics input";
		constexpr static int generics_input_id = 1;

		constexpr static const char* average_output_name = "average output";
		constexpr static int average_output_id = 2;

		uint16_t id_;
		std::string name_;
		std::string type_;

		uint_fast32_t nb_inputs_;
		std::array<float_array, 32> inputs_;

		bool can_process_;

		std::array<float_array_component_output*, 32> average_;
	};
} // namespace
