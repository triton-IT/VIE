#pragma once

#include <json.hpp>

#include "../processor_component.hpp"

namespace live::tritone::vie::processor::component
{
	class multiplier final : public processor_component
	{
	public:
		explicit multiplier(nlohmann::json multiplier_definition);

		multiplier(const multiplier&) = default;

		multiplier(multiplier&&) = default;

		multiplier& operator=(const multiplier&) = default;

		multiplier& operator=(multiplier&&) = default;

		~multiplier();

		uint16_t get_id() override;

		std::string get_name() override;

		processor_component_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		void preprocess() override;

		component_output** get_outputs_pool(uint_fast16_t slot_id) override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast32_t get_output_values(uint_fast16_t slot_id, component_output* output_values[32]) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, component_output* values[32], uint_fast32_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_parameter(parameter parameter) override;

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;
		
		constexpr static const char* multipliers_input_name = "multipliers input";
		constexpr static int multipliers_input_id = 1;
		
		constexpr static const char* multiplicands_input_name = "multiplicands input";
		constexpr static int multiplicands_input_id = 2;

		constexpr static const char* products_output_name = "products output";
		constexpr static int products_output_id = 3;

		uint16_t id_;
		std::string name_;
		std::string type_;

		uint_fast16_t nb_inputs_multipliers_;
		float_array_component_output* multipliers_[32];
		uint_fast16_t nb_inputs_multiplicands_;
		float_array_component_output* multiplicands_[32];

		bool multipliers_filled_;
		bool multiplicands_filled_;

		uint_fast8_t nb_products_;
		float_array_component_output* products_[32];
	};
} // namespace
