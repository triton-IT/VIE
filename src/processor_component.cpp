#include "processor_component.hpp"

#include "application.hpp"

using namespace nlohmann;

namespace live::tritone::vie
{
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