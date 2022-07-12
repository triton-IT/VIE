#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "frequency_parameter.hpp"
#include "application.hpp"

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie {
	frequency_parameter::frequency_parameter() : parameter(frequency_id, L"Frequency", L"Freq", L"Hz", 0, .0,
		kRootUnitId, ParameterInfo::kCanAutomate) {
	}

	double frequency_parameter::to_plain_value(const double value_normalized) {
		return value_normalized - .5;
	}

	double frequency_parameter::to_normalized_value(const double plain_value) {
		return plain_value + .5;
	}
}
