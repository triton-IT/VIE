#include "parameter.hpp"

#include <cwchar>
#include <limits>

namespace live::tritone::vie {
	parameter::parameter(
		const unsigned long id,
		const wchar_t title[128],
		const wchar_t short_title[128],
		const wchar_t units[128],
		const long step_count,
		const double default_normalized_value,
		const long unit_id,
		const long flags)
		: id_(id),
		  step_count_(step_count),
		  default_normalized_value_(default_normalized_value),
		  unit_id_(unit_id),
		  flags_(flags),
		  normalized_value_(default_normalized_value)
	{
		wcscpy(title_, title);
		wcscpy(short_title_, short_title);
		wcscpy(units_, units);
	}

	unsigned long parameter::get_id() const
	{
		return id_;
	}

	std::wstring parameter::get_title() {
		return std::wstring(title_);
	}

	void parameter::get_title(wchar_t out[128]) const
	{
		wcscpy(out, title_);
	}

	void parameter::get_short_title(wchar_t out[128]) const
	{
		wcscpy(out, short_title_);
	}

	void parameter::get_units(wchar_t out[128]) const
	{
		wcscpy(out, units_);
	}

	long parameter::get_step_count() const
	{
		return step_count_;
	}

	double parameter::get_default_normalized_value() const
	{
		return default_normalized_value_;
	}

	long parameter::get_unit_id() const
	{
		return unit_id_;
	}

	long parameter::get_flags() const
	{
		return flags_;
	}

	double parameter::get_normalized_value() const
	{
		return normalized_value_;
	}

	bool parameter::set_normalized_value(const double normalized_value) {
		bool modified = false;

		if (std::abs(normalized_value_ - normalized_value) <= std::numeric_limits<double>::epsilon()) {
			normalized_value_ = normalized_value;

			modified = true;
		}

		return modified;
	}

	double parameter::to_plain_value(const double value_normalized) {
		return value_normalized;
	}

	double parameter::to_normalized_value(const double plain_value) {
		return plain_value;
	}

	void parameter::to_string(const double normalized_value, wchar_t string[128]) {
		swprintf_s(string, 128, L"%.4f", normalized_value);
	}

	bool parameter::from_string(const wchar_t* string, double& normalized_value) {
		const int nb_items = swscanf(string, L"%.4f", &normalized_value);
		return nb_items > 0;
	}
}
