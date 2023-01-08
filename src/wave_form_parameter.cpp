#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "wave_form_parameter.hpp"
#include "application.hpp"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace live::tritone::vie {
	wave_form_parameter::wave_form_parameter() : parameter(wave_form_id, L"Wave form", L"Form", L"", 1, .0, kRootUnitId,
		ParameterInfo::kCanAutomate) {
	}

	double wave_form_parameter::to_plain_value(double value_normalized) {
		if (value_normalized >= .5) {
			return 1.0;
		}
		return 0.0;
	}

	double wave_form_parameter::to_normalized_value(double plain_value) {
		if (plain_value >= .5) {
			return 1.0;
		}
		return 0.0;
	}
}
