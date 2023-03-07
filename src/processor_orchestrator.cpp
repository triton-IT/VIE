#include "processor_orchestrator.hpp"

#include <set>

#include "application.hpp"

#include "modules/oscillator.hpp"
#include "modules/envelope.hpp"
#include "modules/multiplier.hpp"
#include "modules/mixer.hpp"
#include "modules/audio_output.hpp"

using namespace std;

using namespace nlohmann;
using namespace live::tritone::vie::processor::module;

namespace live::tritone::vie
{
	processor_orchestrator::processor_orchestrator() : nb_modules_(0),
	                                                   processor_modules_{}, nb_midi_input_modules_(0),
													   sources_midi_input_modules_{},
													   nb_audio_input_modules_(0),
													   sources_audio_input_modules_{},
												       processing_setup_(),
	                                                   relations_{},
													   bypass_(false)
	{
		memset(nb_module_relations_, 0, 128);
	}

	void processor_orchestrator::initialize()
	{
		terminate();
	}

	void processor_orchestrator::add_processor_module(processor_module& processor)
	{
		if (processor.get_type() == processor_module_type::event_input)
		{
			sources_midi_input_modules_[nb_midi_input_modules_] = &dynamic_cast<midi_input&>(processor);
			nb_midi_input_modules_++;
		}
		if (processor.get_type() == processor_module_type::audio_input)
		{
			sources_audio_input_modules_[nb_audio_input_modules_] = &dynamic_cast<audio_input&>(processor);
			nb_audio_input_modules_++;
		}

		processor_modules_[nb_modules_] = &processor;

		nb_modules_++;
	}

	processor_module** processor_orchestrator::get_processor_modules(int* nb_modules)
	{
		*nb_modules = nb_modules_;
		return processor_modules_;
	}

	void processor_orchestrator::add_relation(module_relation* relation)
	{
		uint16_t source_module_id = relation->source_module->get_id();
		
		module_relation** module_relations = relations_[source_module_id];
		const int nb_module_relations = nb_module_relations_[source_module_id];
		
		auto module_relation = module_relations[nb_module_relations];
		module_relation = relation;
		
		nb_module_relations_[source_module_id] = nb_module_relations + 1;
	}

	void processor_orchestrator::terminate()
	{
		//Release processor modules.
		for (int i = 0; i < nb_modules_; i++)
		{
			//orchestrator is not owner of module.
			//const auto* module = processor_modules_[i];
			//delete module;
			processor_modules_[i] = nullptr;
		}

		nb_modules_ = 0;
		nb_midi_input_modules_ = 0;
		nb_audio_input_modules_ = 0;
		
		for(int i = 0; i < 32; i++)
		{
			nb_module_relations_[i] = 0;
		}
	}

	void processor_orchestrator::setup_processing(const processing_setup& setup)
	{
		processing_setup_.sample_rate = setup.sample_rate;
		processing_setup_.processing_mode = setup.processing_mode;
		processing_setup_.max_samples_per_block = setup.max_samples_per_block;
		processing_setup_.sample_size = setup.sample_size;

		for (int i = 0; i < nb_modules_; i++)
		{
			auto* module = processor_modules_[i];
			module->set_sample_rate(processing_setup_.sample_rate);
		}
	}

	void processor_orchestrator::process_input_event(event& event) const
	{
		midi_input* midi_input_module = get_midi_input_module_for_event(event);
		switch (event.type)
		{
		case event::type::note_on:
			{
				note_event& note_on_event = event.core_event.note_on;
				midi_input_module->note_on(note_on_event);
			}
			break;
		case event::type::note_off:
			{
				note_event& note_off_event = event.core_event.note_off;
				midi_input_module->note_off(note_off_event);
			}
			break;
		case event::type::data_event:
		{
			note_event& note_off_event = event.core_event.note_off;
			midi_input_module->note_off(note_off_event);
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
		auto audio_input = get_audio_input_module_for_buffer(buffer_id);
		audio_input->set_buffer(buffer);
	}

	void processor_orchestrator::process(output_process_data& output_process_data)
	{
		if (!bypass_) {
			for (int i = 0; i < nb_modules_; i++)
			{
				auto* module = processor_modules_[i];
				module->preprocess();
			}
			for (int i = 0; i < nb_midi_input_modules_; i++)
			{
				auto* midi_input_module = sources_midi_input_modules_[i];
				//TODO: Multi-thread call to this method.
				process(midi_input_module, output_process_data);
			}
			for (int i = 0; i < nb_audio_input_modules_; i++)
			{
				auto* audio_input_module = sources_audio_input_modules_[i];
				//TODO: Multi-thread call to this method.
				process(audio_input_module, output_process_data);
			}
		}
	}

	void processor_orchestrator::process(processor_module* source_module,
	                                     output_process_data& output_process_data)
	{
		// If all input of source module are not filled in by parent module, we cannot process it.
		// We need to wait for each parent to process before this module.
		// The last parent of this module triggers the process.
		if (source_module->can_process())
		{
			//Process source module
			source_module->process(output_process_data);

			//Get all children of source modules
			module_relation** module_relations = relations_[source_module->get_id()];

			//Process all children.
			const int nb_relations = nb_module_relations_[source_module->get_id()];
			for (int i = 0; i < nb_relations; i++)
			{
				const auto [relation_module, source_slot_id, target_module, target_slot_id] = *(module_relations[i]);

				std::array<module_output*, 32> source_output_values;
				
				//Because module has been processed, we can get its output values.
				uint_fast8_t nb_outputs = source_module->get_output_values(
					source_slot_id, source_output_values);

				//And then pass output values to input values of next module.
				target_module->set_input_values(target_slot_id, source_output_values, nb_outputs);

				// And process next module.
				process(target_module, output_process_data);
			}
		}
	}

	void processor_orchestrator::parameter_changed(const unsigned long parameter_id, long sample_offset, double parameter_value)
	{
		unsigned int component_id = parameter_id >> 16;
		unsigned int component_parameter_id = parameter_id & 0xffff;
		//FIXME: use component_parameter_id
		float_module_output input(0, parameter_value);
	}

	midi_input* processor_orchestrator::get_midi_input_module_for_event(const event& event) const
	{
		return sources_midi_input_modules_[event.bus_index];
	}

	audio_input* processor_orchestrator::get_audio_input_module_for_buffer(int32_t buffer_id) const
	{
		return sources_audio_input_modules_[buffer_id];
	}
} // namespace
