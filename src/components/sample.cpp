#include "sample.hpp"

#include "../application.hpp"

#include <sndfile.h>
#include <sndfile.hh>

namespace live::tritone::vie::processor::component
{
	sample::sample(nlohmann::json sample_definition) :
		id_(sample_definition["id"]),
		name_(sample_definition["name"]),
		can_process_(false)
	{
		std::string file_path = sample_definition["file_path"];

		loadFile(file_path);
	}

	sample::~sample()
	{
	}

    void sample::loadFile(std::string filename)
    {
		SndfileHandle sndfileHandle = SndfileHandle(filename);

        // Create the sample descriptor
        sample_descriptor sample_descriptor;
        sample_descriptor.nb_channels = sndfileHandle.channels();
        sample_descriptor.nb_frames = sndfileHandle.frames();
		sample_descriptor.rate = sndfileHandle.samplerate();
		sample_descriptor.format = sndfileHandle.format();
		sample_descriptor.buffer = new float[sample_descriptor.nb_frames * sample_descriptor.nb_channels];

		sndfileHandle.read(sample_descriptor.buffer, sample_descriptor.nb_frames * sample_descriptor.nb_channels);
		
		samples_descriptors_[0] = sample_descriptor;
    }

	uint16_t sample::get_id()
	{
		return id_;
	}

	std::string sample::get_name()
	{
		return name_;
	}

	processor_component_type sample::get_type()
	{
		return processor_component_type::middle;
	}

	void sample::set_sample_rate(const double sample_rate)
	{
		sample_rate_ = sample_rate;
	}

	//FIXME: Can't know if all inputs are set-up. So, we should check note by note if it can be processed. 
	bool sample::can_process()
	{
		return can_process_;
	}

	void sample::process(output_process_data& output_process_data)
	{
		can_process_ = false;

		nb_outputs_ = 0;
		/*for (auto& [note_id, sample_descriptor] : *current_samples_descriptors_)
		{
			nb_outputs_++;
		}*/
	}

	uint_fast32_t sample::get_output_values(const uint_fast16_t slot_id, void* output_values[])
	{
		*output_values = outputs_;

		return nb_outputs_;
	}

	bool sample::has_finished()
	{
		return false;
	}

	void sample::get_zombie_notes_ids(std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	void sample::set_zombie_notes_ids(const std::unordered_set<uint32_t>& zombie_notes_ids)
	{
	}

	uint_fast16_t sample::get_slot_id(const std::string& slot_name)
	{
		if (slot_name == onoff_input_name)
		{
			return onoff_input_id;
		}
		else if (slot_name == sample_on_input_name)
		{
			return sample_on_input_id;
		}
		else if (slot_name == sample_name_input_name)
        {
            return sample_name_input_id;
        }

		return -1;
	}

	void sample::set_input_values(const uint_fast16_t slot_id, void* values, const uint_fast32_t nb_values)
	{
		if (slot_id == sample_on_input_id)
		{
			/*(nb_values <= 32);
			for (uint_fast16_t i = 0; i < nb_values; i++)
			{
				const auto& component_output = static_cast<novalue_component_output*>(values)[i];

				uint32_t input_id = component_output.output_id;

				if (const auto current_sample_descriptor = current_samples_descriptors_->find(input_id); current_sample_descriptor != current_samples_descriptors_->end())
				{
					//If synth was already set before, we don't want to erase it.
					//So, keep the old one.
					sample_descriptor<void>& sample_descriptor = next_samples_descriptors_->operator[](input_id);
					sample_descriptor.note_mode = component_output.note_mode;
					sample_descriptor.sample = current_sample_descriptor->second.sample;
				}
				else
				{
					//If synth is a new one, create it.
					sample_descriptor<void>& sample_descriptor = next_samples_descriptors_->operator[](input_id);
					sample_descriptor.note_mode = component_output.note_mode;
					sample_descriptor.sample = new q::white_sample_synth{};
				}
			}

			//Switching between current and next synth is done to simplify the deletion of synths no more used.
			std::unordered_map<uint_fast16_t, sample_descriptor<void>>* tmp_samples_descriptors = current_samples_descriptors_;
			current_samples_descriptors_ = next_samples_descriptors_;
			next_samples_descriptors_ = tmp_samples_descriptors;

			//Clear the phases for next call.
			next_samples_descriptors_->clear();

			can_process_ = true;*/
		}
	}

	uint_fast32_t sample::get_max_nb_input_values(const uint_fast16_t slot_id)
	{
		if (slot_id == sample_on_input_id)
		{
			return 32;
		}

		return -1;
	}

    void sample::set_parameter(parameter parameter) {
        /*if (parameter.get_title() == "sample_name") {
            name_ = parameter.get_title();
        }*/
    }

	/*void sample::set_sample_type(sample_type sample_type) {
		sample_type_ = sample_type;
	}*/
} // namespace
