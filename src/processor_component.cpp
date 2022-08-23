#include "processor_component.hpp"

#include "application.hpp"
#include "components/midi.hpp"
#include "components/oscillator.hpp"
#include "components/envelope.hpp"
#include "components/multiplier.hpp"
#include "components/mixer.hpp"
#include "components/output.hpp"

using namespace nlohmann;
using namespace live::tritone::vie::processor::component;

namespace live::tritone::vie
{
	processor_component* processor_component::create(nlohmann::json processor_definition) {
		processor_component* component = nullptr;

		const std::string type = processor_definition["type"];
		if (type == "midi")
		{
			component = new midi(processor_definition);
		}
		else if (type == "oscillator")
		{
			component = new oscillator(processor_definition);
		}
		else if (type == "envelope")
		{
			component = new envelope(processor_definition);
		}
		else if (type == "multiplier")
		{
			component = new multiplier(processor_definition);
		}
		else if (type == "mixer")
		{
			component = new mixer(processor_definition);
		}
		else if (type == "output")
		{
			component = new output(processor_definition);
		}

		component->initialize(processor_definition);

		return component;
	}

	void processor_component::initialize(nlohmann::json processor_definition) {
		for (auto& [index, parameter_definition] : processor_definition["parameters"].items()) {
			const unsigned long id = parameter_definition["id"];
			const std::string name = parameter_definition["name"];
			const std::string short_name = parameter_definition["short_name"];
			const std::string type = parameter_definition["type"];
			const std::string unit = parameter_definition["unit"];
			const float value = parameter_definition["value"];

			parameters& parameters = application::get_parameters();
			parameter parameter = parameters.add_parameter(id, name, short_name, type, unit, value);
		
			set_parameter(parameter);
		}
	}
}