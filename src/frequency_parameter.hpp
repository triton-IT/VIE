#pragma once

#include "parameter.hpp"

namespace live::tritone::vie
{
	class frequency_parameter final : public parameter
	{
	public:
		frequency_parameter();

		frequency_parameter(const frequency_parameter&) = default;

		frequency_parameter(frequency_parameter&&) = default;

		frequency_parameter& operator=(const frequency_parameter&) = default;

		frequency_parameter& operator=(frequency_parameter&&) = default;

		~frequency_parameter() override = default;

		double to_plain_value(double value_normalized) override;

		double to_normalized_value(double plain_value) override;
	};
}
