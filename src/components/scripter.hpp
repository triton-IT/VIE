#pragma once

#include <json.hpp>
#include <Python.h>

#include "../processor_component.hpp"

namespace live::tritone::vie::processor::component
{
	class python_scripter final : public processor_component
	{
	public:
		explicit python_scripter(nlohmann::json noise_definition);
		python_scripter(const python_scripter&) = default;
		python_scripter(python_scripter&&) = default;

		~python_scripter() override;

		python_scripter& operator=(const python_scripter&) = default;
		python_scripter& operator=(python_scripter&&) = default;

		uint16_t get_id() override;

		std::string get_name() override;

		processor_component_type get_type() override;

		void set_sample_rate(double sample_rate) override;

		void preprocess() override;

		bool can_process() override;

		void process(output_process_data& output_process_data) override;

		component_output** get_outputs_pool(uint_fast16_t slot_id) override;

		uint_fast32_t get_output_values(uint_fast16_t slot_id, component_output* output_values[32]) override;

		bool has_finished() override;

		uint_fast16_t get_slot_id(const std::string& slot_name) override;

		void set_input_values(uint_fast16_t slot_id, component_output* values[32], uint_fast32_t nb_values) override;

		uint_fast32_t get_max_nb_input_values(uint_fast16_t slot_id) override;
		
		void set_parameter(parameter parameter) override;

	private:
		PyObject* global_variables;
		PyObject* local_variables;
		PyObject* compiled_code;

		uint16_t id_;
		std::string name_;
		
		PyThreadState* thread_state;
		
		uint_fast8_t nb_outputs_;
		float_array_component_output* outputs_[32];
	};
} // namespace
