#pragma once

#include "parameter.hpp"

namespace live::tritone::vie
{
	class float_parameter final : public parameter
	{
	public:
		float_parameter(
			unsigned long id,
			const Steinberg::char16 title[128],
			const Steinberg::char16 short_title[128],
			const Steinberg::char16 unit[128],
			double default_value);

		float_parameter(const float_parameter&) = default;

		float_parameter(float_parameter&&) = default;

		float_parameter& operator=(const float_parameter&) = default;

		float_parameter& operator=(float_parameter&&) = default;

		~float_parameter() override = default;

		double to_plain_value(double value_normalized) override;

		double to_normalized_value(double plain_value) override;
	};
}
