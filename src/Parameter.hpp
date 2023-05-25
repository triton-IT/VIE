#pragma once

#include <string>
#include <pluginterfaces/base/iupdatehandler.h>
#include <pluginterfaces/base/ftypes.h>

namespace live::tritone::vie {
	class parameter {
	public:
		parameter(
			unsigned long id,
			const Steinberg::char16 title[128],
			const Steinberg::char16 short_title[128],
			const Steinberg::char16 units[128],
			long step_count,
			double default_normalized_value);

		parameter(const parameter&) = default;
		parameter(parameter&&) = default;

		parameter& operator=(const parameter&) = default;
		parameter& operator=(parameter&&) = default;

		virtual ~parameter() = default;

		[[nodiscard]] unsigned long get_id() const;

		void get_title(Steinberg::char16 out[128]) const;

		void get_short_title(Steinberg::char16 out[128]) const;

		void get_units(Steinberg::char16 out[128]) const;

		[[nodiscard]] long get_step_count() const;

		[[nodiscard]] double get_default_normalized_value() const;

		[[nodiscard]] long get_unit_id() const;

		[[nodiscard]] long get_flags() const;

		[[nodiscard]] double get_normalized_value() const;

		bool set_normalized_value(double normalized_value);

		virtual double to_plain_value(double value_normalized);

		virtual double to_normalized_value(double plain_value);

		virtual void to_string(double normalized_value, Steinberg::char16 string[128]);

		virtual void from_string(const Steinberg::char16 string[128], double& normalized_value);

		bool is_title(const Steinberg::char16* expected);

	private:
		unsigned long id_;
		Steinberg::char16 title_[128]{};
		Steinberg::char16 short_title_[128]{};
		Steinberg::char16 units_[128]{};
		long step_count_;
		double default_normalized_value_;
		long unit_id_;
		long flags_;            //DAW dependent parameters

		double normalized_value_;
	};
}