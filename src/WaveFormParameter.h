#pragma once

#include "Parameter.h"

namespace live::tritone::vie {
	class WaveFormParameter : public Parameter
	{
	public:
		WaveFormParameter();
		WaveFormParameter(const WaveFormParameter& other);

		virtual double toPlainValue(double valueNormalized);
		virtual double toNormalizedValue(double plainValue);
	};
}