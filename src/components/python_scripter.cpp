#include "python_scripter.hpp"

#include "../application.hpp"

#include <iostream>

namespace live::tritone::vie::processor::component
{
	python_scripter::python_scripter(nlohmann::json scripter_definition) :
		id_(scripter_definition["id"]),
		name_(scripter_definition["name"]),
		nb_outputs_(0)
	{
		Py_Initialize();
		
		//Because DAW creates multiple threads, python interpreter must be able to lock/unlock them.
		//This constructor is called in the main thread, so we can safely call PyEval_InitThreads here.
		PyEval_InitThreads();
		
		global_variables = PyDict_New();
		local_variables = PyDict_New();
		const char* script = "print('Hello, World!')";
		compiled_code = Py_CompileString(script, "", Py_eval_input);

		//Save thread to be able to restore before finalizing.
		thread_state = PyEval_SaveThread();
	}

	python_scripter::~python_scripter()
	{
		PyEval_RestoreThread(thread_state);
		Py_Finalize();
	}

	uint16_t python_scripter::get_id()
	{
		return id_;
	}

	std::string python_scripter::get_name()
	{
		return name_;
	}

	processor_component_type python_scripter::get_type()
	{
		return processor_component_type::middle;
	}

	void python_scripter::set_sample_rate(const double sample_rate)
	{
	}

	void python_scripter::preprocess() {
	}

	bool python_scripter::can_process()
	{
		return true;
	}

	void python_scripter::process(output_process_data& output_process_data)
	{
		auto state = PyGILState_Ensure();
		PyObject* result = PyEval_EvalCode(compiled_code, global_variables, local_variables);
		PyGILState_Release(state);
	}

	component_output** python_scripter::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**)outputs_;
	}

	uint_fast32_t python_scripter::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		output_values = (component_output**)outputs_;

		return nb_outputs_;
	}

	bool python_scripter::has_finished()
	{
		return true;
	}

	uint_fast16_t python_scripter::get_slot_id(const std::string& slot_name)
	{
		return 0;
	}

	void python_scripter::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
	}

	uint_fast32_t python_scripter::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		return 32;
	}
	
	void python_scripter::set_parameter(parameter parameter) {}
} // namespace
