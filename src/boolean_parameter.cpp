#include "boolean_parameter.hpp"
#include "application.hpp"

namespace live::tritone::vie {
	boolean_parameter::boolean_parameter(
		unsigned long id,
		const wchar_t title[128],
		const wchar_t short_title[128],
		const wchar_t unit[128],
		double default_value) : 
		parameter(id, title, short_title, unit, 1, default_value) {
	}

	double boolean_parameter::to_plain_value(double value_normalized) {
		if (value_normalized >= .5) {
			return 1.0;
		}
		return 0.0;
	}

	double boolean_parameter::to_normalized_value(double plain_value) {
		if (plain_value >= .5) {
			return 1.0;
		}
		return 0.0;
	}
}
