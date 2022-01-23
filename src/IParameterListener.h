#pragma once

#include <pluginterfaces/vst/vsttypes.h>

namespace live::tritone::vie {
	class IParameterListener
	{
	public:
		virtual void parameterValueChanged(Steinberg::Vst::ParamID parameterId, Steinberg::Vst::ParamValue normalizedValue) = 0;
	};
} // namespace

