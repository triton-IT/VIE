#pragma once

#include <public.sdk/source/vst/vstparameters.h>

namespace live::tritone::vie {
	class IParameterListener
	{
	public:
		virtual void parameterValueChanged(Steinberg::int32 parameterId, Steinberg::Vst::ParamValue normalizedValue) = 0;
	};
} // namespace

