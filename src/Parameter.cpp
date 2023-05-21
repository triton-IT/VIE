#include "parameter.hpp"

#include <cwchar>
#include <limits>
#include <pluginterfaces/base/ustring.h>

namespace live::tritone::vie {
	parameter::parameter(
		const unsigned long id,
		const Steinberg::char16 title[128],
		const Steinberg::char16 short_title[128],
		const Steinberg::char16 units[128],
		const long step_count,
		const double default_normalized_value)
		: id_(id),
		  step_count_(step_count),
		  default_normalized_value_(default_normalized_value),
		  unit_id_(0), //Equivalent to kRootUnitId
		  flags_(1), //Equivalent ParameterInfo::kCanAutomate
		  normalized_value_(default_normalized_value)
	{
		std::char_traits<Steinberg::char16>::copy(title_, title, std::char_traits<Steinberg::char16>::length(title) + 1);
		std::char_traits<Steinberg::char16>::copy(short_title_, short_title, std::char_traits<Steinberg::char16>::length(short_title) + 1);
		std::char_traits<Steinberg::char16>::copy(units_, units, std::char_traits<Steinberg::char16>::length(units) + 1);
	}

	unsigned long parameter::get_id() const
	{
		return id_;
	}

	void parameter::get_title(Steinberg::char16 out[128]) const
	{
		std::char_traits<Steinberg::char16>::copy(out, title_, std::char_traits<Steinberg::char16>::length(title_) + 1);
	}

	void parameter::get_short_title(Steinberg::char16 out[128]) const
	{
		std::char_traits<Steinberg::char16>::copy(out, short_title_, std::char_traits<Steinberg::char16>::length(short_title_) + 1);
	}

	void parameter::get_units(Steinberg::char16 out[128]) const
	{
		std::char_traits<Steinberg::char16>::copy(out, units_, std::char_traits<Steinberg::char16>::length(units_) + 1);
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

	void parameter::to_string(const double normalized_value, Steinberg::char16 out[128]) {
		Steinberg::char16 out_buffer[128];
		Steinberg::UString128 ustr_out(out_buffer, 128);
		ustr_out.scanFloat(*const_cast<double*>(&normalized_value));
	}

	void parameter::from_string(const Steinberg::char16 str_value[128], double& normalized_value) {
		Steinberg::UString u_string(const_cast<Steinberg::char16*>(str_value), 128);
		u_string.printFloat(normalized_value);
	}

	bool parameter::is_title(Steinberg::char16* expected) {
		return std::memcmp(title_, expected, sizeof(title_)) == 0;
	}
}
