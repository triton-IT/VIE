#include "vie_processor.hpp"

#include "application.hpp"
#include "components/output.hpp"

#include <cstdint>

using namespace std;
using namespace live::tritone::vie::processor::component;

using json = nlohmann::json;

namespace live::tritone::vie {
	vie_processor::vie_processor() :
		active_(false),
		processing_(false),
		frequency_multiplier_(0.),
		bypass_(false) {
	}

	void vie_processor::initialize() {
		ifstream defaultInstrumentStream(content_path + L"default_instrument.json");
		json defaultInstrument;
		defaultInstrumentStream >> defaultInstrument;

		parse_processors(defaultInstrument["processors"]);
		parse_relations(defaultInstrument["relations"]);
	}

	void vie_processor::terminate() {
		orchestrator_.terminate();

		//Release processor audio input buses.
		for (auto bus : audio_input_buses_)
		{
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
		if (active_ && processing_) {
			switch (parameter_id) {
			case bypass_id:
				bypass_ = (parameter_value > 0.5);
				break;

			case frequency_id:
				frequency_multiplier_ = parameter_value;
				break;

			case wave_form_id:
				//FIXME: removed while working on processor_component. Make it generic after.
				//waveForm_ = (parameterValue > 0.5f);
				//break;
			default:
				break;
			}
		}
	}

	void vie_processor::process_input_event(event& event) const
	{
		orchestrator_.process_event(event);
	}
	
	bool vie_processor::process_output_data(output_process_data& output_process_data) {
		if (!bypass_) {
			orchestrator_.process(output_process_data);
		}

		return true;
	}

	void vie_processor::parse_processors(json processors_definition) {
		for (auto& [index, processor] : processors_definition.items()) {
			switch (processor_component* component = orchestrator_.add_processor_component(processor); component->get_type()) {
			case processor_component_type::event_input: {
				auto event_input_bus = new bus(std::wstring(L"Event input"), bus_type::main, component);
				event_input_buses_.push_back(event_input_bus);
				break;
			}
			case processor_component_type::audio_input: {
				auto audio_input_bus = new bus(std::wstring(L"Audio input"), bus_type::main, component);
				audio_input_buses_.push_back(audio_input_bus);
				break;
			}
			case processor_component_type::output: {
				//TODO: Create another bus type if this one do not need a component as parameter.
				auto audio_output_bus = new bus(std::wstring(L"Audio output"), bus_type::main, component);
				audio_output_buses_.push_back(audio_output_bus);
				dynamic_cast<output*>(component)->set_output_bus_id(static_cast<uint_fast16_t>(audio_output_buses_.size()) - 1);
				break;
			}
			case processor_component_type::middle:
				break;
			}
		}
	}

	void vie_processor::parse_relations(json relations_definition) {
		const uint64_t nb_relations_definition = relations_definition.size();

		for (uint64_t i = 0; i < nb_relations_definition; i++) {
			const json& relation_definition = relations_definition[i];
			orchestrator_.add_relation(relation_definition);
		}	
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
