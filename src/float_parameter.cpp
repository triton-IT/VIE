#include "float_parameter.hpp"
#include "application.hpp"

namespace live::tritone::vie {
	float_parameter::float_parameter(
		unsigned long id,
		const Steinberg::char16 title[128],
		const Steinberg::char16 short_title[128],
		const Steinberg::char16 unit[128],
		double default_value) : 
		parameter(id, title, short_title, unit, 0, default_value) {
	}

	double float_parameter::to_plain_value(const double value_normalized) {
		return value_normalized - .5;
	}

	double float_parameter::to_normalized_value(const double plain_value) {
		return plain_value + .5;
	}
}
