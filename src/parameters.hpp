#pragma once

#include "Parameter.hpp"

#include <memory>
#include <string>
#include <map>
#include <vector>

namespace live::tritone::vie {
	class parameters {
	public:
		parameters() = default;
		parameters(const parameters&) = default;
		parameters(parameters&&) = default;

		virtual ~parameters();

		parameters& operator=(const parameters&) = default;
		parameters& operator=(parameters&&) = default;

		parameter add_parameter(unsigned long id, parameter parameter);
		parameter add_parameter(unsigned long id, std::string name, std::string short_name, std::string type, std::string unit, float value);

		parameter get_parameter(unsigned long id);
		parameter get_parameter_by_index(long index);

		long count();

	private:

	protected:
		std::map<unsigned long, parameter*> parameters_map;
		std::vector<parameter*> parameters_list;
	};
}