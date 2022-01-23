#pragma once

#include "Parameter.h"

namespace live::tritone::vie {
	class FrequencyParameter : public Parameter
	{
	public:
		FrequencyParameter();
		FrequencyParameter(const FrequencyParameter& other);

		virtual double toPlainValue(double valueNormalized);
		virtual double toNormalizedValue(double plainValue);
	};
}