#include "vie_processor.hpp"

#include "application.hpp"
#include "modules/audio_output.hpp"

#include <cstdint>

using namespace std;
using namespace live::tritone::vie::processor::module;

using json = nlohmann::json;

namespace live::tritone::vie {
	vie_processor::vie_processor() :
		active_(false),
		processing_(false)
	{
	}

	void vie_processor::initialize()
	{
		terminate();
		
		orchestrator_.initialize();
		
		active_= false;
		processing_ = false;
	}

	nlohmann::json vie_processor::serialize()
	{
		json root_json = json();
		json modules_json = json();
		int nb_modules;
		auto modules = orchestrator_.get_processor_modules(&nb_modules);
		
		for (int i = 0; i < nb_modules; i++)
		{
			auto module = modules[i];
			modules_json.push_back(module->serialize());
		}
		root_json["modules"] = modules_json;

		json links_json = json();
		std::array<int_fast8_t, 32> nb_links_per_module;
		std::array<std::array<modules_link*, 32>, 128>& links = orchestrator_.get_modules_links(&nb_links_per_module);

		for (int i = 0; i < nb_modules; i++)
		{
			int_fast8_t nb_links = nb_links_per_module[i];
			for (int_fast8_t j = 0; j < nb_links; j++)
			{
				auto modules_link = links[i][j];
				
				json link_json = json();
				link_json["source_module"] = modules_link->source_module->get_id();
				link_json["source_slot"] = modules_link->source_slot_id;
				link_json["target_module"] = modules_link->target_module->get_id();
				link_json["target_slot"] = modules_link->target_slot_id;
				
				links_json.push_back(link_json);
			}
		}
		root_json["links"] = links_json;
		
		return root_json;
	}

	void vie_processor::add_processor(processor_module& processor)
	{
		orchestrator_.add_processor_module(processor);
		switch (processor.get_type()) {
		case processor_module_type::event_input: {
			auto event_input_bus = new bus(std::wstring(L"Event input"), bus_type::main, processor);
			event_input_buses_.push_back(event_input_bus);
			break;
		}
		case processor_module_type::audio_input: {
			auto audio_input_bus = new bus(std::wstring(L"Audio input"), bus_type::main, processor);
			audio_input_buses_.push_back(audio_input_bus);
			break;
		}
		case processor_module_type::audio_output: {
			//TODO: Create another bus type if this one do not need a module as parameter.
			auto audio_output_bus = new bus(std::wstring(L"Audio output"), bus_type::main, processor);
			audio_output_buses_.push_back(audio_output_bus);
			dynamic_cast<audio_output&>(processor).set_output_bus_id(static_cast<uint_fast16_t>(audio_output_buses_.size()) - 1);
			break;
		}
		case processor_module_type::middle:
			break;
		}
	}

	void vie_processor::link_modules(modules_link& link) {
		orchestrator_.link_modules(link);
	}

	void vie_processor::terminate() {
		orchestrator_.terminate();

		//Release processor audio input buses.
        for (auto bus: audio_input_buses_) {
			delete bus;
		}
		audio_input_buses_.clear();

		//Release audio output buses.
		for (auto bus : audio_output_buses_) {
			delete bus;
		}
		audio_output_buses_.clear();

		//Release event input buses.
		for (auto bus : event_input_buses_) {
			delete bus;
		}
		event_input_buses_.clear();

		//Release event output buses.
		for (auto bus : event_output_buses_) {
			delete bus;
		}
		event_output_buses_.clear();
	}

	long vie_processor::get_bus_count(const media_type media_type, const bus_direction bus_direction) {
		const std::vector<bus*>* buses = get_buses(media_type, bus_direction);
		const unsigned long long nb_buses = buses->size();

		return static_cast<long>(nb_buses);
	}

	bool vie_processor::get_bus_info(const media_type media_type, const bus_direction bus_direction, const long index, bus_info& bus_info /*out*/) {
		if (bus* bus = get_bus(media_type, bus_direction, index)) {
			bus_info.media_type = media_type;
			bus_info.direction = bus_direction;
			bus_info.channel_count = bus->get_channel_count();
            wcscpy_s(bus_info.name, bus->get_name().c_str());
			switch (bus->get_type()) {
			case bus_type::main:
				bus_info.bus_type = bus_type::main;
				break;
			case bus_type::aux:
				bus_info.bus_type = bus_type::aux;
				break;
			}
			bus_info.is_default_active = bus->is_default_active();
			bus_info.is_control_voltage = bus->is_control_voltage();

			return true;
		}

		return false;
	}

	bool vie_processor::activate_bus(const media_type media_type, const bus_direction bus_direction, const long index, bool state) {
		if (bus* bus = get_bus(media_type, bus_direction, index)) {
			bus->set_active(true);

			return true;
		}

		return false;
	}

	bool vie_processor::set_active(const bool state) {
		active_ = state == 1 ? true : false;

		return true;
	}

	bool vie_processor::set_bus_arrangement(const uint64_t* inputs, const long num_ins, const uint64_t* outputs, const long num_outs) const
	{
		// We support the arrangements the host wants us to support;
		for (long i = 0; i < static_cast<long> (num_ins); ++i) {
			bus* bus = audio_input_buses_.at(i);
			bus->set_arrangement(inputs[i]);
		}

		for (long i = 0; i < static_cast<long> (num_outs); ++i) {
			bus* bus = audio_output_buses_.at(i);
			bus->set_arrangement(outputs[i]);
		}

		return true;
	}

	bool vie_processor::get_bus_arrangement(const bus_direction bus_direction, const long index, uint64_t& arr /*out*/) {
		if (const bus* bus = get_bus(media_type::audio, bus_direction, index)) {
			arr = bus->get_arrangement();
			return true;
		}
		return false;
	}

	bool vie_processor::setup_processing(const processing_setup& setup) {
		orchestrator_.setup_processing(setup);

		return true;
	}

	bool vie_processor::set_processing(bool state) {
		processing_ = state;

		return true;
	}

	void vie_processor::input_parameter_changed(const unsigned long parameter_id, long sample_offset, const double parameter_value) {
		orchestrator_.parameter_changed(parameter_id, sample_offset, parameter_value);
	}

	void vie_processor::process_input_event(event& event) const
	{
		orchestrator_.process_input_event(event);
	}

	void vie_processor::process_input_audio(audio_bus_buffers* buffer, int32_t buffer_id) const
	{
		orchestrator_.process_input_audio(buffer, buffer_id);
	}
	
	bool vie_processor::process_output_data(output_process_data& output_process_data) {
		orchestrator_.process(output_process_data);

		return true;
	}

	std::vector<bus*>* vie_processor::get_buses(const media_type media_type, const bus_direction bus_direction) {
		std::vector<bus*>* buses = nullptr;
		switch (media_type) {
		case media_type::audio:
			switch (bus_direction) {
			case bus_direction::input:
				buses = &audio_input_buses_;
				break;
			case bus_direction::output:
				buses = &audio_output_buses_;
				break;
			}
			break;
		case media_type::event:
			switch (bus_direction) {
			case bus_direction::input:
				buses = &event_input_buses_;
				break;
			case bus_direction::output:
				buses = &event_output_buses_;
				break;
			}
			break;
		}

		return buses;
	}

	bus* vie_processor::get_bus(const media_type media_type, const bus_direction bus_direction, const int index) {
		if (const std::vector<bus*>* buses = get_buses(media_type, bus_direction); buses->size() > static_cast<uint64_t>(index)) {
			return buses->at(index);
		}

		return nullptr;
	}
} // namespace
