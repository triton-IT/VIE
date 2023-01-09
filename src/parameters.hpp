#pragma once

#include "parameter.hpp"

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

		long count();

	private:

	protected:
		parameter* parameters_[255] = { nullptr };
		uint_fast8_t nb_parameters_ = 0;
	};
}