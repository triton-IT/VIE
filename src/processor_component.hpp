#pragma once

#include <json.hpp>

#include <string>
#include <cstdint>
#include <unordered_set>

#include "processor_definition.hpp"
#include "parameter.hpp"

namespace live::tritone::vie
{
	enum class note_mode
	{
		normal,
		zombie
	};

	struct audio_bus_buffers
	{
		long num_channels;
		unsigned long long silence_flags;
		void** channels_buffer;
		sample_size sample_size;
	};

	struct output_process_data
	{
		output_process_data() : num_samples(0), num_outputs(0), outputs()
		{
		}

		uint_fast32_t num_samples;
		uint_fast32_t num_outputs;
		audio_bus_buffers outputs[32];
	};

	template <typename T>
	struct simple_component_output
	{
		simple_component_output() : output_id(0), note_mode(note_mode::normal), value()
		{
		}

		uint32_t output_id;
		note_mode note_mode;
		T value;
	};

	using float_component_output = simple_component_output<float>;

	template <typename T>
	struct array_component_output
	{
		array_component_output() : output_id(0), note_mode(note_mode::normal), nb_samples(0), values(nullptr)
		{
		}

		uint32_t output_id;
		note_mode note_mode;
		uint_fast32_t nb_samples;
		T* values;
	};

	using float_array_component_output = array_component_output<float>;

	using note_event_component_output = simple_component_output<note_event>;

	enum class processor_component_type
	{
		event_input,
		audio_input,
		output,
		middle
	};

	class processor_component
	{
	public:
		processor_component() = default;
		processor_component(const processor_component&) = default;
		processor_component(processor_component&&) = default;

		virtual ~processor_component() = default;

		processor_component& operator=(const processor_component&) = default;
		processor_component& operator=(processor_component&&) = default;

		/**
		* Get this processor identifier.
		*/
		virtual uint16_t get_id() = 0;

		/**
		* Get this processor unique name.
		*/
		virtual std::string get_name() = 0;

		/**
		* Get component type. (input, output or middle).
		*/
		virtual processor_component_type get_type() = 0;

		/**
		* Set sample rate.
		*/
		virtual void set_sample_rate(double sample_rate) = 0;

		/**
		* Is this processor be able to process ?
		* The processor should be able to process when all its input are correctly filled in.
		*/
		virtual bool can_process() = 0;

		/**
		* Run this processor.
		*/
		virtual void process(output_process_data& output_process_data) = 0;

		/**
		* Get result of processing for specific slot.
		* The output type of the requested slot is unknown by this interface.
		*/
		virtual uint_fast32_t get_output_values(uint_fast16_t slot_id, void* output_values[]) = 0;

		/**
		 * Has this processor finished the processing ?
		 * The processor should return true when it has processed its latest output value.
		 */
		virtual bool has_finished() = 0;

		/**
		 * Get the identifiers of the zombie notes.
		 * A zombie note is a note that must continue to play even if it as been released (e.g. during the release phase of an envelope).
		 */
		virtual void get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids) = 0;

		/**
		 * Set the identifiers of zombie notes.
		 * zombie notes must be processed like any other note by middle components.
		 * If a middle component uses a zombie note to perform processing, then the output must be marked as zombie.
		 * If a middle component requested a zombie note and do not use zombie notes other than the one requested, then the output must not be marked as zombie.
		 */
		virtual void set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids) = 0;

		/**
		* Get the input identifier for the given input name.
		*/
		virtual uint_fast16_t get_slot_id(const std::string& slot_name) = 0;

		/**
		* Set the values for the given slot.
		* The input type of the requested slot is unknown by this interface.
		*/
		virtual void set_input_values(uint_fast16_t slot_id, void* values, uint_fast32_t nb_values) = 0;

		/**
		* Get the maximum number of values accepted for the given input identifier.
		*/
		virtual uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) = 0;

		/**
		* Set a parameter to this processor.
		*/
		virtual void set_parameter(parameter parameter) = 0;

	private:
		/**
		* Initialize component with specified configuration.
		*/
		void initialize(nlohmann::json processor_definition);

		friend class processor_components;
	};
} // namespace
