#pragma once

#include <json.hpp>

#include "module_descriptor.hpp"
#include "../processor_module.hpp"

namespace live::tritone::vie::processor::module
{
	struct audio_input_descriptor : public module_descriptor {
		audio_input_descriptor()
		{
			id = 0;
			name = L"audio-in";
			icon = L"<svg width=\"16\" height=\"16\" viewBox=\"0 0 16 16\" xmlns=\"http://www.w3.org/2000/svg\" fill=\"currentColor\">  <path id=\"mainPath\" d=\"M15 5.5a4.394 4.394 0 0 1-4 4.5 2.955 2.955 0 0 0-.2-1A3.565 3.565 0 0 0 14 5.5a3.507 3.507 0 0 0-7-.3A3.552 3.552 0 0 0 6 5a4.622 4.622 0 0 1 4.5-4A4.481 4.481 0 0 1 15 5.5zM5.5 6a4.5 4.5 0 1 0 0 9.001 4.5 4.5 0 0 0 0-9z\"/></svg>";
			input_slots = {
				{0, L"audio-in"}
			};
			nb_input_slots = 1;
			output_slots = {
				{0, L"audio-out"}
			};
			nb_output_slots = 1;
		}
	};
	
	class audio_input final : public processor_module
	{
	public:
		explicit audio_input(nlohmann::json audio_input_definition);
		audio_input(const audio_input&) = default;
		audio_input(audio_input&&) = default;

		~audio_input() override;

		audio_input& operator=(const audio_input&) = default;
		audio_input& operator=(audio_input&&) = default;

		uint16_t get_id() override;

		std::string get_name() override;

		processor_module_type get_type() override;

		void set_sample_rate(double audio_input_rate) override;

		void preprocess() override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		uint_fast8_t get_output_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;

		void set_parameter(parameter parameter) override;

		nlohmann::json serialize() override;

		void set_buffer(audio_bus_buffers* buffer);

	private:
		static constexpr const char* onoff_input_name = "on/off input";
		static constexpr int onoff_input_id = 0;

		static constexpr const char* audio_output_name = "Audio output";
		static constexpr int audio_output_id = 1;

		uint16_t id_;
		std::string name_;
		double sample_rate_;

		struct audio_input_info {
			uint32_t nb_channels;
			uint32_t nb_frames;
			uint32_t rate;
			int format;
			float* buffers[8]; //8 channels max for now to handle surround 7.1.
 		};

		std::unordered_map<int, audio_input_info> buffer;

		struct audio_input_state {
			bool activated;
			uint32_t position;
		};

		float_array_module_output empty_array_module_;

		std::array<float_array_module_output*, 32> outputs_;

		audio_bus_buffers* buffer_;
	};
} // namespace
