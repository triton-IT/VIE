#include "recorder.hpp"

#include "../application.hpp"

namespace live::tritone::vie::processor::module
{
	recorder::recorder(nlohmann::json recorder_definition) : processor_module(),
		id_(recorder_definition["id"]),
		sample_rate_(.0),
		can_process_(false),
		on_(true),
		nb_outputs_(0)
	{
		if (recorder_definition.contains("name")) {
			name_ = recorder_definition["name"];
		}
		else {
			name_ = "Recorder";
		}
		
		//auto& parameters_definition = recorder_definition["parameters"];

		std::string file_path = recorder_definition["file_path"];
		
		SF_INFO info;
		info.channels = 1;
		info.samplerate = 44100;
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

		// Ouvrir un fichier audio en écriture
		output_file_ = sf_open(file_path.c_str(), SFM_WRITE, &info);
	}

	recorder::~recorder()
	{
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

	processor_module_type recorder::get_type()
	{
		return processor_module_type::middle;
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
			//TODO: Handle more channels than mono.
			auto output = output_[0];
			output->note_id = id_;
			
			if (on_)
			{
				sf_writef_float(output_file_, output->values.values, output->values.nb_values);
			}
		}
	}

	uint_fast8_t recorder::get_output_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values)
	{
		switch (slot_id) {
		case amplitude_output_id:
			values = reinterpret_cast<std::array<module_output*, 32>&>(output_);
			return nb_outputs_;
		}
		
		throw std::invalid_argument("Invalid slot id");
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

		throw std::invalid_argument("Invalid slot name");
	}

	void recorder::set_input_values(const uint_fast16_t slot_id, std::array<module_output*, 32>& values, uint_fast8_t nb_values)
	{
		switch (slot_id)
		{
		case onoff_input_id:
			assert(nb_values == 1);

			on_ = reinterpret_cast<std::array<float_module_output*, 32>&>(values)[0]->to_boolean();

			return;
		case amplitude_input_id:
			assert(nb_values <= 32);
			nb_outputs_ = nb_values;
			
			if (nb_outputs_ > 0)
			{
				output_ = reinterpret_cast<std::array<float_array_module_output*, 32>&>(values);
			}

			can_process_ = true;
			
			return;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	uint_fast32_t recorder::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == onoff_input_id)
		{
			return 1;
		}
		if (slot_id == amplitude_input_id)
		{
			return 32;
		}

		throw std::invalid_argument("Invalid slot name");
	}

	void recorder::set_parameter(parameter parameter)
	{

	}

	nlohmann::json recorder::serialize()
	{
		nlohmann::json root;
		root["id"] = id_;
		root["name"] = name_;
		root["type"] = "recorder";

		return root;
	}
} // namespace
