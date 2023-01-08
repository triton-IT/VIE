#pragma once

#include "parameter.hpp"

namespace live::tritone::vie {
	class wave_form_parameter final : public parameter {
	public:
		wave_form_parameter();
		wave_form_parameter(const wave_form_parameter& other) = default;
		wave_form_parameter(wave_form_parameter&&) = default;

		~wave_form_parameter() override = default;

		wave_form_parameter& operator=(const wave_form_parameter&) = default;
		wave_form_parameter& operator=(wave_form_parameter&&) = default;

		double to_plain_value(double value_normalized) override;

		double to_normalized_value(double plain_value) override;
	};
}