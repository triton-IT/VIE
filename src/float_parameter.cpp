#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "float_parameter.hpp"
#include "application.hpp"

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie {
	float_parameter::float_parameter(
		unsigned long id,
		const wchar_t title[128],
		const wchar_t short_title[128],
		const wchar_t unit[128],
		double default_value) : 
		parameter(id, title, short_title, unit, 0, default_value, kRootUnitId, ParameterInfo::kCanAutomate) {
	}

	double float_parameter::to_plain_value(const double value_normalized) {
		return value_normalized - .5;
	}

	double float_parameter::to_normalized_value(const double plain_value) {
		return plain_value + .5;
	}
}
