#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "WaveFormParameter.h"
#include "Application.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace live::tritone::vie {
	WaveFormParameter::WaveFormParameter() : Parameter(kWaveFormId, L"Wave form", L"Form", L"", 1, .0, kRootUnitId, ParameterInfo::kCanAutomate)
	{
	}

	WaveFormParameter::WaveFormParameter(const WaveFormParameter& other) : Parameter(other) {
	}

	double WaveFormParameter::toPlainValue(double valueNormalized) {
		if (valueNormalized >= .5) {
			return 1.0;
		}
		return 0.0;
	}

	double WaveFormParameter::toNormalizedValue(double plainValue) {
		if (plainValue >= .5) {
			return 1.0;
		}
		return 0.0;
	}
}
