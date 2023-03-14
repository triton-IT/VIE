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

	struct float_array {
		uint_fast16_t nb_values;
		float* values;
	};
	
	struct module_output
	{		
		module_output(uint32_t id) : note_id(id)
		{
		}

		uint32_t note_id;

		virtual bool to_boolean() = 0;

		virtual float to_float() = 0;

		virtual float_array to_float_array() = 0;
	};

	/**
	* Used to transfer output event from one module to another.
	* For example, a note on event (that does not contain any additional data).
	*/
	struct novalue_module_output : public module_output
	{
		novalue_module_output() : module_output(0)
		{
		}
		
		novalue_module_output(uint32_t note_id) : module_output(note_id)
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

	struct float_module_output : public module_output
	{
		float_module_output() : module_output(0), value(.0f)
		{
			value = .0f;
		}
		
		float_module_output(uint32_t note_id, float value) : module_output(note_id), value(value)
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

	struct float_array_module_output : public module_output
	{
		float_array_module_output() : module_output(0)
		{
			values.nb_values = 0;
			values.values = nullptr;
		}
		
		float_array_module_output(uint32_t note_id, float_array values) : module_output(note_id), values(values)
		{
		}

		float_array values;

		bool to_boolean() override
		{
			return values.nb_values > 0;
		}

		float to_float() override
		{
			return static_cast<float>(values.nb_values);
		}

		float_array to_float_array() override
		{
			return values;
		}
	};

	enum class processor_module_type
	{
		event_input,
		audio_input,
		audio_output,
		middle
	};

	class processor_module;
	struct module_link
	{
		/**
		* Id of output slot in source module.
		**/
		uint_fast8_t source_slot_id;
		/**
		* Pointer to target module.
		**/
		std::shared_ptr<processor_module> target_module;
		/**
		* Id of input slot in target module.
		**/
		uint_fast8_t target_slot_id;
	};

	/**
	* A module is a processor that can be linked to other modules.
	* It can be an input, an output or a middle module.
	**/
	class processor_module
	{
	public:
		/**
		* throw std::runtime_error when construction fails.
		*/
		processor_module() = default;
		processor_module(const processor_module&) = default;
		processor_module(processor_module&&) = default;

		virtual ~processor_module();

		processor_module& operator=(const processor_module&) = default;
		processor_module& operator=(processor_module&&) = default;

		/**
		* Get this processor identifier.
		*/
		virtual uint16_t get_id() = 0;

		/**
		* Get this processor unique name.
		*/
		virtual std::string get_name() = 0;

		/**
		* Get module type. (input, output or middle).
		*/
		virtual processor_module_type get_type() = 0;

		/**
		* Set sample rate.
		*/
		virtual void set_sample_rate(double sample_rate) = 0;

		/**
		* Get the input identifier for the given input name.
		*/
		virtual uint_fast16_t get_slot_id(const std::string& slot_name) = 0;

		/**
		* Set the values for the given slot.
		* For a given slot, multiple notes can be played (one value by note).
		* The input type of the requested slot is unknown by this interface.
		*/
		virtual void set_input_values(uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values) = 0;

		/**
		* Get the maximum number of values accepted for the given input identifier.
		*/
		virtual uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) = 0;

		/**
		* Pre-process the module.
		* Called only once per process loop.
		* If the module has multiple inputs links, 
		* the "process" method and the "get_output_values" are called once by input link.
		* The "preprocess" method is called only once whatever the number of input links.
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
		* Get result of processing for specific slot.
		* For a given slot, multiple notes can be played (one value by note).
		* The output type of the requested slot is unknown by this interface.
		*/
		virtual uint_fast8_t get_output_values(uint_fast16_t slot_id, std::array<module_output*, 32>&) = 0;

		/**
		 * Has this processor finished the processing ?
		 * The processor should return true when it has processed its latest output value.
		 */
		virtual bool has_finished() = 0;

		/**
		* Set a parameter to this processor.
		*/
		virtual void set_parameter(parameter parameter) = 0;

		/**
		* serialize this processor.
		*/
		virtual nlohmann::json serialize() = 0;
		
		/**
		* Initialize module with specified configuration.
		*/
		void initialize(nlohmann::json processor_definition);

		/**
		* Link a slot id of this processor to a target slot of a processor.
		*/
		uint_fast8_t link(uint_fast8_t source_slot_id, std::shared_ptr<processor_module> target_module, uint_fast8_t target_slot_id);

		/**
		* Unlink a slot id of this processor from a target slot of a processor.
		*/
		uint_fast8_t unlink(uint_fast8_t source_slot_id, std::shared_ptr<processor_module> target_module, uint_fast8_t target_slot_id);

		/**
		* Unlink all slots pointing to the target processor.
		*/
		void unlink(std::shared_ptr<processor_module> target_module);
		
		/**
		* Get links to output_modules.
		*/
		std::array<std::shared_ptr<module_link>, 32>& get_modules_links(uint_fast8_t& nb_module_links);

#ifdef UNIT_TESTING
	public:
#else
	private:
#endif
		/**
		* List of target module input slots linked to this module output slots.
		**/
		std::array<std::shared_ptr<module_link>, 32> modules_links_;
		uint_fast8_t nb_modules_links_;
	};
} // namespace
