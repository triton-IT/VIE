#pragma once

#include <unordered_set>

#include "processor_definition.hpp"
#include "processor_module.hpp"
#include "modules/midi_input.hpp"
#include "modules/audio_input.hpp"

namespace live::tritone::vie {
	/**
	* A source module output slot can be linked to multiple target modules input slots.
	**/
	struct module_relation {
		/**
		* Pointer to source module.
		**/
		processor_module* source_module;
		/**
		* Id of slot in source module.
		**/
		uint_fast16_t source_slot_id;
		/**
		* Pointer to target module.
		**/
		processor_module* target_module;
		/**
		* Id of slot in target module.
		**/
		uint_fast16_t target_slot_id;
	};

	class processor_orchestrator {
	public:
		processor_orchestrator();

		void add_processor_module(processor_module& processor);

		processor_module** get_processor_modules(int* nb_modules);

		void add_relation(module_relation* relation);

		void terminate() const;

		void setup_processing(const processing_setup& setup);

		void process_input_event(event& event) const;

		void process_input_audio(audio_bus_buffers* buffer, int32_t buffer_id) const;

		void process(output_process_data& output_process_data);

		void parameter_changed(const unsigned long parameter_id, long sample_offset, double parameter_value);


	private:
		int nb_modules_;
		//TODO: compute nb of modules when parsing config file instead of using constant.
		processor_module* processor_modules_[128];

		int nb_midi_input_modules_;
		//TODO: compute nb of midi modules when parsing config file instead of using constant.
		processor::module::midi_input* sources_midi_input_modules_[16];

		int nb_audio_input_modules_;
		//TODO: compute nb of audio input modules when parsing config file instead of using constant.
		processor::module::audio_input* sources_audio_input_modules_[16];

		processing_setup processing_setup_;
		
		int nb_module_relations_[32]{};
		//Contains an array of output relations for the given module id.
		//TODO: compute nb of modules when parsing config file instead of using constant.
		//TODO: compute max nb of relations for modules when parsing config file instead of using constant.
		module_relation* relations_[128][32];

		void process(processor_module* source_module, output_process_data& output_process_data);
		
		[[nodiscard]] processor::module::midi_input* get_midi_input_module_for_event(const event& event) const;
		[[nodiscard]] processor::module::audio_input* get_audio_input_module_for_buffer(int32_t buffer_id) const;

		bool bypass_;
	};
} // namespace
