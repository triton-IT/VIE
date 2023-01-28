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

		void preprocess() override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast8_t get_output_values(uint_fast16_t slot_id, std::array<component_output*, 32>& values) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, std::array<component_output*, 32>& values, uint_fast8_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_noise_type(noise_type noise_type);

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;

		constexpr static const char* amplitudes_output_name = "amplitudes output";
		constexpr static int amplitudes_output_id = 1;

		uint16_t id_;
		std::string name_;
		std::string type_;
		double sample_rate_;

		template <typename Synth>
		struct noise_descriptor {
			Synth* noise_synth;
		};

		std::unordered_map<uint_fast16_t, noise_descriptor<void>>* current_noises_descriptors_;
		std::unordered_map<uint_fast16_t, noise_descriptor<void>>* next_noises_descriptors_;

		noise_type noise_type_;

		bool can_process_;

		uint_fast8_t nb_outputs_;
		std::array<float_array_component_output*, 32> outputs_;
	};
} // namespace
