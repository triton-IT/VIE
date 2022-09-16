#pragma once

#include <json.hpp>

#include "../processor_component.hpp"

namespace live::tritone::vie::processor::component
{
	class noise final : public processor_component
	{
	public:
		enum class noise_type {
			white,
			pink,
			brown
		};

		explicit noise(nlohmann::json noise_definition);
		noise(const noise&) = default;
		noise(noise&&) = default;

		~noise() override;

		noise& operator=(const noise&) = default;
		noise& operator=(noise&&) = default;

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

		void set_noise_type(noise_type noise_type);

	private:
		constexpr static const char* noise_on_input_name = "On input";
		constexpr static int noise_on_input_id = 0;

		constexpr static const char* amplitudes_output_name = "amplitudes output";
		constexpr static int amplitudes_output_id = 1;

		uint16_t id_;
		std::string name_;
		std::string type_;
		double sample_rate_;

		template <typename Synth>
		struct noise_descriptor {
			note_mode note_mode;
			Synth* noise_synth;
		};

		std::unordered_map<uint_fast16_t, noise_descriptor<void>>* current_noises_descriptors_;
		std::unordered_map<uint_fast16_t, noise_descriptor<void>>* next_noises_descriptors_;

		noise_type noise_type_;

		bool can_process_;

		uint_fast8_t nb_outputs_;
		float_array_component_output outputs_[32];
	};
} // namespace
