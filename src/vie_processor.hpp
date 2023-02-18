#pragma once

#include <json.hpp>

#include <vector>
#include <cstdint>

#include "processor_definition.hpp"
#include "processor_module.hpp"
#include "processor_orchestrator.hpp"
#include "bus.hpp"


namespace live::tritone::vie {
	class vie_processor {
	public:
		vie_processor();

		void initialize();

		void load(nlohmann::json instrument);

		void terminate();

		long get_bus_count(media_type media_type, bus_direction bus_direction);

		bool get_bus_info(media_type media_type, bus_direction bus_direction, long index, bus_info& bus_info /*out*/);

		bool activate_bus(media_type media_type, bus_direction bus_direction, long index, bool state);

		bool set_active(bool state);

		bool set_bus_arrangement(const uint64_t* inputs, long num_ins, const uint64_t* outputs, long num_outs) const;

		bool get_bus_arrangement(bus_direction bus_direction, long index, uint64_t& arr /*out*/);

		bool setup_processing(const processing_setup& setup);

		bool set_processing(bool state);

		void input_parameter_changed(unsigned long parameter_id, long sample_offset, double parameter_value);

		void process_input_event(event& event) const;

		void process_input_audio(audio_bus_buffers* buffer, int32_t buffer_id) const;

		bool process_output_data(output_process_data& output_process_data);

	private:
		void parse_processors(nlohmann::json processors_definitions);

		void parse_relations(nlohmann::json relations_definitions);

		std::vector<bus*>* get_buses(media_type media_type, bus_direction bus_direction);

		bus* get_bus(media_type media_type, bus_direction bus_direction, int index);

		processor_orchestrator orchestrator_;

		bool active_;
		bool processing_;

		std::vector<bus*> audio_input_buses_;
		std::vector<bus*> audio_output_buses_;
		std::vector<bus*> event_input_buses_;
		std::vector<bus*> event_output_buses_;
	};
} // namespace
