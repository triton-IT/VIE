#include "processor_orchestrator.hpp"

#include <set>

#include "application.hpp"

#include "components/oscillator.hpp"
#include "components/envelope.hpp"
#include "components/multiplier.hpp"
#include "components/mixer.hpp"
#include "components/output.hpp"

using namespace std;

using namespace nlohmann;
using namespace live::tritone::vie::processor::component;

namespace live::tritone::vie
{
	processor_orchestrator::processor_orchestrator() : nb_components_(0),
	                                                   processor_components_{}, nb_midi_input_components_(0),
													   sources_midi_input_components_{},
													   nb_audio_input_components_(0),
													   sources_audio_input_components_{},
												       processing_setup_(),
	                                                   relations_{},
													   bypass_(false)
	{
		memset(nb_component_relations_, 0, 128);
	}

	void processor_orchestrator::add_processor_component(processor_component* processor)
	{
		if (processor->get_type() == processor_component_type::event_input)
		{
			sources_midi_input_components_[nb_midi_input_components_] = static_cast<midi_input*>(processor);
			nb_midi_input_components_++;
		}
		if (processor->get_type() == processor_component_type::audio_input)
		{
			sources_audio_input_components_[nb_audio_input_components_] = static_cast<audio_input*>(processor);
			nb_audio_input_components_++;
		}

		processor_components_[nb_components_] = processor;

		nb_components_++;
	}

	component_relation& processor_orchestrator::add_relation(json relation_definition)
	{
		const int source_component_id = relation_definition["sourceComponent"];

		component_relation* component_relations = relations_[source_component_id];
		const int nb_component_relations = nb_component_relations_[source_component_id];
		component_relation& component_relation = component_relations[nb_component_relations];
		nb_component_relations_[source_component_id] = nb_component_relations + 1;

		component_relation.source_component = processor_components_[source_component_id];

		const uint_fast16_t source_output_id = relation_definition["sourceOutputSlot"];
		component_relation.source_slot_id = source_output_id;

		const int target_component_id = relation_definition["targetComponent"];
		component_relation.target_component = processor_components_[target_component_id];

		const uint_fast16_t target_input_id = relation_definition["targetInputSlot"];
		component_relation.target_slot_id = target_input_id;

		return component_relation;
	}

	void processor_orchestrator::terminate() const
	{
		//Release processor components.
		for (int i = 0; i < nb_components_; i++)
		{
			const auto* component = processor_components_[i];
			delete component;
		}
	}

	void processor_orchestrator::setup_processing(const processing_setup& setup)
	{
		processing_setup_.sample_rate = setup.sample_rate;
		processing_setup_.processing_mode = setup.processing_mode;
		processing_setup_.max_samples_per_block = setup.max_samples_per_block;
		processing_setup_.sample_size = setup.sample_size;

		for (int i = 0; i < nb_components_; i++)
		{
			auto* component = processor_components_[i];
			component->set_sample_rate(processing_setup_.sample_rate);
		}
	}

	void processor_orchestrator::process_input_event(event& event) const
	{
		midi_input* midi_input_component = get_midi_input_component_for_event(event);
		switch (event.type)
		{
		case event::type::note_on:
			{
				note_event& note_on_event = event.core_event.note_on;
				midi_input_component->note_on(note_on_event);
			}
			break;
		case event::type::note_off:
			{
				note_event& note_off_event = event.core_event.note_off;
				midi_input_component->note_off(note_off_event);
			}
			break;
		case event::type::data_event:
		{
			note_event& note_off_event = event.core_event.note_off;
			midi_input_component->note_off(note_off_event);
		}
		break;
		case event::type::poly_pressure_event:
		case event::type::note_expression_value_event:
		case event::type::note_expression_text_event:
		case event::type::chord_event:
		case event::type::scale_event:
		case event::type::legacy_midi_cc_out_event:
			break;
		}
	}
	
	void processor_orchestrator::process_input_audio(audio_bus_buffers* buffer, int32_t buffer_id) const
	{
		auto audio_input = get_audio_input_component_for_buffer(buffer_id);
		audio_input->set_buffer(buffer);
	}

	void processor_orchestrator::process(output_process_data& output_process_data)
	{
		if (!bypass_) {
			for (int i = 0; i < nb_components_; i++)
			{
				auto* component = processor_components_[i];
				component->preprocess();
			}
			for (int i = 0; i < nb_midi_input_components_; i++)
			{
				auto* midi_input_component = sources_midi_input_components_[i];
				//TODO: Multi-thread call to this method.
				process(midi_input_component, output_process_data);
			}
			for (int i = 0; i < nb_audio_input_components_; i++)
			{
				auto* audio_input_component = sources_audio_input_components_[i];
				//TODO: Multi-thread call to this method.
				process(audio_input_component, output_process_data);
			}
		}
	}

	void processor_orchestrator::process(processor_component* source_component,
	                                     output_process_data& output_process_data)
	{
		// If all input of source component are not filled in by parent component, we cannot process it.
		// We need to wait for each parent to process before this component.
		// The last parent of this component triggers the process.
		if (source_component->can_process())
		{
			//Process source component
			source_component->process(output_process_data);

			//Get all children of source components
			const component_relation* component_relations = relations_[source_component->get_id()];

			//Process all children.
			const int nb_relations = nb_component_relations_[source_component->get_id()];
			for (int i = 0; i < nb_relations; i++)
			{
				const auto [relation_component, source_slot_id, target_component, target_slot_id] = component_relations[i];

				component_output** source_output_values = source_component->get_outputs_pool(source_slot_id);
				
				//Because component has been processed, we can get its output values.
				const uint_fast32_t nb_output_values = source_component->get_output_values(
					source_slot_id,
					source_output_values);

				//And then pass output values to input values of next component.
				target_component->set_input_values(target_slot_id, source_output_values, nb_output_values);

				// And process next component.
				process(target_component, output_process_data);
			}
		}
	}

	void processor_orchestrator::parameter_changed(const unsigned long parameter_id, long sample_offset, double parameter_value)
	{
		unsigned int component_id = parameter_id >> 16;
		unsigned int component_parameter_id = parameter_id & 0xffff;
		float_component_output input(0, parameter_value);
	}

	midi_input* processor_orchestrator::get_midi_input_component_for_event(const event& event) const
	{
		return sources_midi_input_components_[event.bus_index];
	}

	audio_input* processor_orchestrator::get_audio_input_component_for_buffer(int32_t buffer_id) const
	{
		return sources_audio_input_components_[buffer_id];
	}
} // namespace
