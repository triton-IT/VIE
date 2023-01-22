#pragma once

#include <json.hpp>

#include <string>
#include <cstdint>
#include <unordered_set>

#include "processor_definition.hpp"
#include "parameter.hpp"

namespace live::tritone::vie
{
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

	typedef struct {
		uint_fast16_t nb_values;
		float* values;
	} float_array;
	
	struct component_output
	{		
		component_output(uint32_t id) : note_id(id)
		{
		}

		uint32_t note_id;

		virtual bool to_boolean() = 0;

		virtual float to_float() = 0;

		virtual float_array to_float_array() = 0;
	};

	/**
	* Used to transfer output event from one component to another.
	* For example, a note on event (that does not contain any additional data).
	*/
	struct novalue_component_output : public component_output
	{
		novalue_component_output() : component_output(0)
		{
		}
		
		novalue_component_output(uint32_t note_id) : component_output(note_id)
		{
		}

		bool to_boolean() override
		{
			return false;
		}

		float to_float() override
		{
			return 0.0f;
		}

		float_array to_float_array() override
		{
			return { 0, nullptr };
		}
	};

	struct float_component_output : public component_output
	{
		float_component_output() : component_output(0), value(.0f)
		{
			value = .0f;
		}
		
		float_component_output(uint32_t note_id, float value) : component_output(note_id), value(value)
		{
		}

		float value;

		bool to_boolean() override
		{
			return value >= 0.5f;
		}

		float to_float() override
		{
			return value;
		}

		float_array to_float_array() override
		{
			return { 1, &value };
		}
	};

	struct float_array_component_output : public component_output
	{
		float_array_component_output() : component_output(0)
		{
			values.nb_values = 0;
			values.values = nullptr;
		}
		
		float_array_component_output(uint32_t note_id, float_array values) : component_output(note_id), values(values)
		{
		}

		float_array values;

		bool to_boolean() override
		{
			return values.nb_values > 0;
		}

		float to_float() override
		{
			return values.nb_values;
		}

		float_array to_float_array() override
		{
			return values;
		}
	};

	enum class processor_component_type
	{
		event_input,
		audio_input,
		audio_output,
		middle
	};

	class processor_component
	{
	public:
		/**
		* throw std::runtime_error when construction fails.
		*/
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
		* Pre-process the component.
		* Called only once per process loop.
		* If the component has multiple inputs relations, 
		* the "process" method and the "get_output_values" are called once by input relation.
		* The "preprocess" method is called only once whatever the number of input relations.
		*/
		virtual void preprocess() = 0;

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
		* Get a pool of output values for the given slot.
		* The pool must be allocated with the right structure for the slot.
		* The pool do not need to be reallocated on each request. The less allocation, the better.
		* This pool will be used to populate output values.
		* The pool must be allocated with enough objects to contain output values.
		*/
		virtual component_output** get_outputs_pool(uint_fast16_t slot_id) = 0;
		
		/**
		* Get result of processing for specific slot.
		* For a given slot, multiple notes can be played (one value by note).
		* The output type of the requested slot is unknown by this interface.
		*/
		virtual uint_fast32_t get_output_values(uint_fast16_t slot_id, component_output* values[32]) = 0;

		/**
		 * Has this processor finished the processing ?
		 * The processor should return true when it has processed its latest output value.
		 */
		virtual bool has_finished() = 0;

		/**
		* Get the input identifier for the given input name.
		*/
		virtual uint_fast16_t get_slot_id(const std::string& slot_name) = 0;

		/**
		* Set the values for the given slot.
		* For a given slot, multiple notes can be played (one value by note).
		* The input type of the requested slot is unknown by this interface.
		*/
		virtual void set_input_values(uint_fast16_t slot_id, component_output* values[32], uint_fast32_t nb_values) = 0;

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
