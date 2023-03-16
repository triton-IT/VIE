#pragma once

#include <unordered_set>

#include "processor_definition.hpp"
#include "processor_module.hpp"
#include "modules/midi_input.hpp"
#include "modules/audio_input.hpp"

namespace live::tritone::vie {
	class processor_orchestrator {
	public:
		processor_orchestrator();

		void initialize();

		const std::shared_ptr<processor_module> add_processor_module(nlohmann::json processor_definition);

		void delete_processor(uint_fast8_t processor_id);

		uint_fast8_t get_nb_processors();

		void clear();

		std::shared_ptr<processor_module> get_processor(int id);

		std::array<std::shared_ptr<processor_module>, 128> get_processor_modules(uint_fast8_t& nb_modules /*out*/);

		uint16_t link_modules(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id);
		void unlink_modules(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id);
		void enable_modules_link(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id);
		void disable_modules_link(uint_fast8_t source_module_id, uint_fast16_t source_slot_id, uint_fast8_t target_module_id, uint_fast16_t target_slot_id);
		
		std::array<std::array<std::shared_ptr<module_link>, 32>, 128>& get_modules_links(uint_fast8_t& nb_modules /*out*/, std::array<uint_fast8_t, 32>& nb_links /* out */);

		void terminate();

		void setup_processing(const processing_setup& setup);

		void process_input_event(event& event) const;

		void process_input_audio(audio_bus_buffers* buffer, int32_t buffer_id) const;

		void process(output_process_data& output_process_data);

		void parameter_changed(const unsigned long parameter_id, long sample_offset, double parameter_value);

#ifdef UNIT_TESTING
	public:
#else
	private:
#endif
		//TODO: compute nb of modules when parsing config file instead of using constant.
		std::array<std::shared_ptr<processor_module>, 128> processor_modules_;
		uint_fast8_t nb_modules_;

		//TODO: compute nb of midi modules when parsing config file instead of using constant.
		std::array<std::shared_ptr<processor::module::midi_input>, 16> sources_midi_input_modules_;
		uint_fast8_t nb_midi_input_modules_;

		//TODO: compute nb of audio input modules when parsing config file instead of using constant.
		std::array<std::shared_ptr<processor::module::audio_input>, 16> sources_audio_input_modules_;
		uint_fast8_t nb_audio_input_modules_;

		processing_setup processing_setup_;

		bool bypass_;

		void add_processor_module(std::shared_ptr<processor_module> processor);
		
		void process(std::shared_ptr<processor_module>, output_process_data& output_process_data);
		
		[[nodiscard]] std::shared_ptr<processor::module::midi_input> get_midi_input_module_for_event(const event& event) const;
		[[nodiscard]] std::shared_ptr<processor::module::audio_input> get_audio_input_module_for_buffer(int32_t buffer_id) const;
	};
} // namespace
