#include "parameter.hpp"

#include <cmath>
#include <cwchar>
#include <limits>
#include <cstring>

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
		  normalized_value_(0),
		  listener_(nullptr)
	{
	#if defined(_WIN32) || defined(_WIN64) 
		wcscpy(title_, title);
		wcscpy(short_title_, short_title);
		wcscpy(units_, units);
	#else
		wcscpy((wchar_t*)title_, title);
		wcscpy((wchar_t*)short_title_, short_title);
		wcscpy((wchar_t*)units_, units);
	#endif
	}

	unsigned long parameter::get_id() const
	{
		return id_;
	}

	void parameter::get_title(wchar_t out[128]) const
	{
	#if defined(_WIN32) || defined(_WIN64) 
		wcscpy(out, title_);
	#else
		wcscpy(out, (wchar_t*)title_);
	#endif
	}

	void parameter::get_short_title(wchar_t out[128]) const
	{
	#if defined(_WIN32) || defined(_WIN64) 
		wcscpy(out, short_title_);
	#else
		wcscpy(out, (wchar_t*)title_);
	#endif
	}

	void parameter::get_units(wchar_t out[128]) const
	{
	#if defined(_WIN32) || defined(_WIN64) 
		wcscpy(out, units_);
	#else
		wcscpy(out, (wchar_t*)title_);
	#endif
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

			if (listener_ != nullptr) {
				listener_->parameter_value_changed(id_, normalized_value_);
			}

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
		swprintf(string, 128, L"%.4f", normalized_value);
	}

	bool parameter::from_string(const wchar_t* string, double& normalized_value) {
		const int nb_items = swscanf(string, L"%.4f", &normalized_value);
		return nb_items > 0;
	}

	void parameter::set_listener(i_parameter_listener* listener) {
		listener_ = listener;
	}
}
