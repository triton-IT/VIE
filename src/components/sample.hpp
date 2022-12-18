#pragma once

#include <json.hpp>

#include "../processor_component.hpp"

namespace live::tritone::vie::processor::component
{
	class sample final : public processor_component
	{
	public:
		explicit sample(nlohmann::json sample_definition);
		sample(const sample&) = default;
		sample(sample&&) = default;

		~sample() override;

		sample& operator=(const sample&) = default;
		sample& operator=(sample&&) = default;

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

		void set_parameter(parameter parameter) override;

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;

		constexpr static const char* sample_on_input_name = "On input";
		constexpr static int sample_on_input_id = 1;

		static constexpr const char* sample_name_input_name = "Sample name input";
		static constexpr int sample_name_input_id = 2;

		uint16_t id_;
		std::string name_;
		double sample_rate_;
		bool can_process_;
		bool has_finished_;

		struct sample_descriptor {
			uint32_t nb_channels;
			uint32_t nb_frames;
			uint32_t rate;
			int format;
			float* buffer;
		};

		std::unordered_map<int, sample_descriptor> samples_descriptors_;

		float_array_component_output* amplitudes_;

		float_array_component_output empty_array_component_;

		uint_fast8_t nb_outputs_;
		float_array_component_output outputs_[32];

		uint_fast16_t output_bus_id_;

		void loadFile(std::string filename);
	};
} // namespace
