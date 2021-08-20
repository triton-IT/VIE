#pragma once

#include <pluginterfaces/base/ftypes.h>

#include "IParameterListener.h"

namespace tech::tritonit::tritone {
	class FrequencyParameter : public Steinberg::Vst::Parameter
	{
	public:
		FrequencyParameter();

		bool setNormalized(Steinberg::Vst::ParamValue v);

		void toString(Steinberg::Vst::ParamValue normValue, Steinberg::Vst::String128 string) const SMTG_OVERRIDE;
		bool fromString(const Steinberg::Vst::TChar* string, Steinberg::Vst::ParamValue& normValue) const SMTG_OVERRIDE;

		void setListener(IParameterListener* listener);

	private:
		IParameterListener* listener_;
	};
}