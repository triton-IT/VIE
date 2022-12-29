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

		void preprocess() override;

		bool can_process() override;

		component_output** get_outputs_pool(uint_fast16_t slot_id) override;

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

		constexpr static const char* play_input_name = "Play input";
		constexpr static int play_input_id = 1;

		constexpr static const char* stop_input_name = "Stop input";
		constexpr static int stop_input_id = 2;

		static constexpr const char* name_input_name = "Sample name input";
		static constexpr int name_input_id = 3;

		static constexpr const char* play_at_input_name = "Play at output";
		static constexpr int play_at_input_id = 4;

		static constexpr const char* amplitude_output_name = "Sample amplitude output";
		static constexpr int amplitude_output_id = 5;

		uint16_t id_;
		std::string name_;
		double sample_rate_;
		bool can_process_;
		/**
		* Because the component has multiple inputs. Multiples relations can be bound to it.
		* It means that the "process" method of the component can be called multiple times.
		* We only want to process the sample once for all "process" calls.
		*/
		bool already_processed_;
		bool has_finished_;

		struct sample_descriptor {
			uint32_t nb_channels;
			uint32_t nb_frames;
			uint32_t rate;
			int format;
			float* buffers[8]; //8 channels max for now to handle surround 7.1.
 		};

		std::unordered_map<int, sample_descriptor> samples_descriptors_;

		struct sample_state {
			bool activated;
			uint32_t position;
		};

		//std::unordered_map<int, sample_state&> samples_states_;
		sample_state samples_states_[128] = {0};

		float_array_component_output* amplitudes_;

		float_array_component_output empty_array_component_;

		uint_fast8_t nb_outputs_;
		float_array_component_output* outputs_[32];

		uint_fast16_t output_bus_id_;

		void loadFile(std::string filename);
	};
} // namespace
