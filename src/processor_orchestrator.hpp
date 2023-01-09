#pragma once

#include <unordered_set>

#include "processor_definition.hpp"
#include "processor_component.hpp"
#include "components/midi.hpp"

namespace live::tritone::vie {
	/**
	* A source component output slot can be linked to multiple target components input slots.
	**/
	struct component_relation {
		/**
		* Pointer to source component.
		**/
		processor_component* source_component;
		/**
		* Id of slot in source component.
		**/
		uint_fast16_t source_slot_id;
		/**
		* Pointer to target component.
		**/
		processor_component* target_component;
		/**
		* Id of slot in target component.
		**/
		uint_fast16_t target_slot_id;
	};

	class processor_orchestrator {
	public:
		processor_orchestrator();

		void add_processor_component(processor_component* processor);

		component_relation& add_relation(nlohmann::json relation_definition);

		void terminate() const;

		void setup_processing(const processing_setup& setup);

		void process_event(event& event) const;

		void process(output_process_data& output_process_data);

		void parameter_changed(const unsigned long parameter_id, long sample_offset, double parameter_value);


	private:
		int nb_components_;
		//TODO: compute nb of components when parsing config file instead of using constant.
		processor_component* processor_components_[128];

		int nb_midi_components_;
		//TODO: compute nb of midi components when parsing config file instead of using constant.
		processor::component::midi* sources_components_[16];

		processing_setup processing_setup_;
		
		int nb_component_relations_[32]{};
		//Contains an array of output relations for the given component id.
		//TODO: compute nb of components when parsing config file instead of using constant.
		//TODO: compute max nb of relations for components when parsing config file instead of using constant.
		component_relation relations_[128][32];

		void process(processor_component* source_component, output_process_data& output_process_data);
		[[nodiscard]] processor::component::midi* get_midi_component_for_event(const event& event) const;

		bool bypass_;
	};
} // namespace
