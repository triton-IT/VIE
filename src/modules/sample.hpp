#pragma once

#include <json.hpp>

#include <sndfile.h>
#include <sndfile.hh>

#include "../processor_module.hpp"

namespace live::tritone::vie::processor::module
{
	class sample final : public processor_module
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
		void loadFile(std::string filename);
		
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

		static const uint_fast8_t max_nb_descriptors_ = 128;
		static const uint_fast8_t max_nb_outputs_ = 32;

		uint16_t id_;
		std::string name_;
		double sample_rate_;
		bool can_process_;
		/**
		* Because the module has multiple inputs. Multiples relations can be bound to it.
		* It means that the "process" method of the module can be called multiple times.
		* We only want to process the sample once for all "process" calls.
		*/
		bool already_processed_;

		struct sample_descriptor {
			bool activated;
			SndfileHandle* handle;
			uint_fast16_t nb_frames;
			float* buffer;
 		};

		std::array<sample_descriptor, max_nb_descriptors_> samples_descriptors_;

		uint_fast8_t nb_outputs_;
		std::array<float_array_module_output*, max_nb_outputs_> outputs_;
	};
} // namespace
