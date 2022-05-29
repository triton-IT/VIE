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

		~multiplier() override = default;

		uint16_t get_id() override;

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

	private:
		constexpr static const char* multipliers_input_name = "multipliers input";
		constexpr static int multipliers_input_id = 0;
		constexpr static const char* multiplicands_input_name = "multiplicands input";
		constexpr static int multiplicands_input_id = 1;

		constexpr static const char* products_output_name = "products output";
		constexpr static int products_output_id = 2;

		uint16_t id_;
		std::string name_;
		std::string type_;

		uint_fast16_t nb_inputs_multipliers_;
		float_array_component_output* multipliers_;
		uint_fast16_t nb_inputs_multiplicands_;
		float_array_component_output* multiplicands_;

		bool multipliers_filled_;
		bool multiplicands_filled_;

		uint_fast8_t nb_products_;
		float_array_component_output products_[32];
	};
} // namespace
