#include <pluginterfaces/base/ustring.h>

#include "frequencyParameter.h"
#include "Application.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace tech::tritonit::tritone {
	FrequencyParameter::FrequencyParameter() : listener_(nullptr)
	{
		info.id = kFrequencyId;
		UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Frequency"));
		UString(info.shortTitle, USTRINGSIZE(info.shortTitle)).assign(USTRING("Freq"));
		UString(info.units, USTRINGSIZE(info.units)).assign(USTRING("Hz"));
		info.stepCount = 0;
		info.defaultNormalizedValue = .0f;
		info.unitId = kRootUnitId;
		info.flags = ParameterInfo::kCanAutomate;
	}
	
	bool FrequencyParameter::setNormalized(ParamValue v) {
		bool changed = Parameter::setNormalized(v);

		if (changed && listener_ != nullptr) {
			listener_->parameterValueChanged(info.id, v);
		}

		return changed;
	}

	void FrequencyParameter::toString(ParamValue normValue, String128 string) const
	{
		char text[32];
		sprintf(text, "%.4f", normValue);

		UString(string, 128).fromAscii(text);
	}

	bool FrequencyParameter::fromString(const TChar* string, ParamValue& normValue) const
	{
		UString wrapper((TChar*)string, 128);
		wrapper.scanFloat(normValue);

		return true;
	}

	void FrequencyParameter::setListener(IParameterListener* listener) {
		listener_ = listener;
	}
}