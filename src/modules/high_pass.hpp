#pragma once

#include <json.hpp>

#include "module_descriptor.hpp"
#include "../processor_module.hpp"

namespace live::tritone::vie::processor::module
{
	struct high_pass_descriptor : public module_descriptor {
		high_pass_descriptor()
		{
			id = 5;
			name = L"high-pass";
			icon = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 16 16\" xmlns=\"http://www.w3.org/2000/svg\" fill=\"currentColor\">  <path id=\"mainPath\" d=\"M15 5.5a4.394 4.394 0 0 1-4 4.5 2.955 2.955 0 0 0-.2-1A3.565 3.565 0 0 0 14 5.5a3.507 3.507 0 0 0-7-.3A3.552 3.552 0 0 0 6 5a4.622 4.622 0 0 1 4.5-4A4.481 4.481 0 0 1 15 5.5zM5.5 6a4.5 4.5 0 1 0 0 9.001 4.5 4.5 0 0 0 0-9z\"/></svg>";
			input_slots = { {
				{0, L"on/off"},
				{1, L"signal"},
				{2, L"cutoff"},
				{3, L"resonance"}
			} };
			nb_input_slots = 4;
			output_slots = { {
				{4, L"amplitudes"}
			} };
			nb_output_slots = 1;
		}
	};
	
	/**
	* @brief A sample processor module.
	* 
	*/
	class high_pass final : public processor_module
	{
	public:
		explicit high_pass(nlohmann::json high_pass_definition);

		~high_pass();

		uint16_t get_id() override;

		std::string get_name() override;

		processor_module_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		void preprocess() override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		/**
		* Set a pair<string, float> where string is the id of this high_pass and float is the high_pass output value.
		*/
		uint_fast8_t get_output_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_parameter(parameter parameter) override;

		nlohmann::json serialize() override;

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;
		
		constexpr static const char* generics_input_name = "signal input";
		constexpr static int generics_input_id = 1;

		constexpr static const char* cutoff_input_name = "cutoff input";
		constexpr static int cutoff_input_id = 2;

		constexpr static const char* resonance_input_name = "resonance input";
		constexpr static int resonance_input_id = 3;

		constexpr static const char* average_output_name = "filtered output";
		constexpr static int average_output_id = 4;

		uint16_t id_;
		std::string name_;
		std::string type_;

		uint_fast32_t nb_inputs_;
		std::array<float_array, 32> inputs_;
		bool inputs_set_;

		float cutoff_;
		bool cutoff_set_;
		float resonance_;
		bool resonance_set_;

		float omega;
		float sin_omega;
		float alpha;
		float cos_omega;
		float input_prev_[2]; // Entrées précédentes
		float output_prev_[2]; // Sorties précédentes
		float a1, a2, b0, b1, b2; // Coefficients du filtre

		bool input_modified_;

		double sample_rate_;

		std::array<float_array_module_output*, 32> filtered_;
	};
} // namespace
