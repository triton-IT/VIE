#pragma once

#include "parameter.hpp"

namespace live::tritone::vie {
	class boolean_parameter final : public parameter {
	public:
		boolean_parameter(
			unsigned long id,
			const wchar_t title[128],
			const wchar_t short_title[128],
			const wchar_t unit[128],
			double default_value);
		boolean_parameter(const boolean_parameter& other) = default;
		boolean_parameter(boolean_parameter&&) = default;

		~boolean_parameter() override = default;

		boolean_parameter& operator=(const boolean_parameter&) = default;
		boolean_parameter& operator=(boolean_parameter&&) = default;

		double to_plain_value(double value_normalized) override;

		double to_normalized_value(double plain_value) override;
	};
}