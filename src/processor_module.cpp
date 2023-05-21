#include "processor_module.hpp"

#include "application.hpp"

#include <string>
#include <pluginterfaces/base/ustring.h>

using namespace nlohmann;

namespace live::tritone::vie
{
	void processor_module::initialize(nlohmann::json processor_definition) {
		for (auto& [index, parameter_definition] : processor_definition["parameters"].items()) {
			const unsigned long id = parameter_definition["id"];
			const std::string name = parameter_definition["name"];
			const std::string short_name = parameter_definition["short_name"];
			const std::string type = parameter_definition["type"];
			const std::string unit = parameter_definition["unit"];
			const float value = parameter_definition["value"];

			Steinberg::char16 u_name[128];
			Steinberg::UString ustr_name(u_name, 128);
			ustr_name.fromAscii(name.c_str(), name.size());

			Steinberg::char16 u_short_name[128];
			Steinberg::UString ustr_short_name(u_short_name, 128);
			ustr_short_name.fromAscii(short_name.c_str(), short_name.size());

			Steinberg::char16 u_type[128];
			Steinberg::UString ustr_type(u_type, 128);
			ustr_type.fromAscii(type.c_str(), type.size());

			Steinberg::char16 u_unit[128];
			Steinberg::UString ustr_unit(u_unit, 128);
			ustr_unit.fromAscii(unit.c_str(), unit.size());

			parameter parameter = application::add_parameter(id, 
				u_name,
				u_short_name, 
				u_type, 
				u_unit, 
				value);

			set_parameter(parameter);
		}
	}
}