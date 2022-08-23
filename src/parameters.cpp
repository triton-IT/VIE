#include "parameters.hpp"

#include "boolean_parameter.hpp"
#include "float_parameter.hpp"

using namespace live::tritone::vie;

parameters::~parameters() {

}

parameter parameters::add_parameter(unsigned long id, parameter parameter)
{
	auto parameter_ptr = new live::tritone::vie::parameter(parameter);

	parameters_map.emplace(id, parameter_ptr);
	parameters_list.push_back(parameter_ptr);

	return *parameter_ptr;
}

parameter parameters::add_parameter(unsigned long id, std::string name, std::string short_name, std::string type, std::string unit, float value) {
	if (type == "boolean") {
		//FIXME: use wstring in parameters and fund a way to parse wstring with json.
		boolean_parameter parameter(id, 
			std::wstring(name.begin(), name.end()).c_str(), 
			std::wstring(short_name.begin(), short_name.end()).c_str(),
			std::wstring(unit.begin(), unit.end()).c_str(),
			value);
		return add_parameter(id, parameter);
	}
	else {
		float_parameter parameter(id,
			std::wstring(name.begin(), name.end()).c_str(),
			std::wstring(short_name.begin(), short_name.end()).c_str(),
			std::wstring(unit.begin(), unit.end()).c_str(),
			value);
		return add_parameter(id, parameter);
	}
}

parameter parameters::get_parameter(unsigned long id)
{
	return *parameters_map[id];
}

parameter parameters::get_parameter_by_index(long index) {
	return *parameters_list[index];
}

long parameters::count()
{
	return parameters_map.size();
}