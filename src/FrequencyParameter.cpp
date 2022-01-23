#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstunits.h>

#include "frequencyParameter.h"
#include "Application.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace live::tritone::vie {
	FrequencyParameter::FrequencyParameter() : Parameter(kFrequencyId, L"Frequency", L"Freq", L"Hz", 0, .0f, kRootUnitId, ParameterInfo::kCanAutomate)
	{
	}

	FrequencyParameter::FrequencyParameter(const FrequencyParameter& other) : Parameter(other) {
	}

	double FrequencyParameter::toPlainValue(double valueNormalized) {
		return valueNormalized - .5f;
	}

	double FrequencyParameter::toNormalizedValue(double plainValue) {
		return plainValue + .5f;
	}
}
