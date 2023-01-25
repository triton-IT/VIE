#include "recorder.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::component
{
	recorder::recorder(nlohmann::json recorder_definition) : processor_component(),
		id_(recorder_definition["id"]),
		name_(recorder_definition["name"]),
		sample_rate_(.0),
		can_process_(false),
		nb_outputs_(0)
	{
		//auto& parameters_definition = recorder_definition["parameters"];

		std::string file_path = recorder_definition["file_path"];
		
		output_ = new float_array_component_output();
		
		SF_INFO info;
		info.channels = 1;
		info.samplerate = 44100;
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

		// Ouvrir un fichier audio en �criture
		output_file_ = sf_open(file_path.c_str(), SFM_WRITE, &info);
	}

	recorder::~recorder()
	{
		if (output_->values.nb_values > 0)
		{
			delete[] output_->values.values;
		}
		delete output_;

		sf_close(output_file_);
	}

	uint16_t recorder::get_id()
	{
		return id_;
	}

	std::string recorder::get_name()
	{
		return name_;
	}

	processor_component_type recorder::get_type()
	{
		return processor_component_type::middle;
	}

	void recorder::set_sample_rate(const double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	void recorder::preprocess() {
	}

	bool recorder::can_process()
	{
		return can_process_;
	}

	void recorder::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		if (nb_outputs_ > 0) {
			output_->note_id = id_;
			
			sf_writef_float(output_file_, output_->values.values, output_->values.nb_values);
		}
	}

	component_output** recorder::get_outputs_pool(uint_fast16_t slot_id) {
		return (component_output**) &output_;
	}

	uint_fast32_t recorder::get_output_values(const uint_fast16_t slot_id, component_output* output_values[32])
	{
		output_values = (component_output**) &output_;
		return nb_outputs_;
	}

	bool recorder::has_finished()
	{
		return true;
	}

	uint_fast16_t recorder::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == amplitude_input_name)
		{
			return amplitude_input_id;
		}
		else if (slot_name == amplitude_output_name)
		{
			return amplitude_output_id;
		}

		//FIXME: Do not return -1 on a uint !
		return -1;
	}

	void recorder::set_input_values(const uint_fast16_t slot_id, component_output* values[32], const uint_fast32_t nb_values)
	{
		nb_outputs_ = 0;
		
		if (slot_id == amplitude_input_id)
		{
			assert(nb_values <= 32);
			
			if (nb_values > 0) {
				output_->values = values[0]->to_float_array();
				nb_outputs_ = nb_values;
			}

			can_process_ = true;
		}
	}

	uint_fast32_t recorder::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == amplitude_input_id)
		{
			return 1;
		}

		return -1;
	}

	void recorder::set_parameter(parameter parameter)
	{

	}
} // namespace