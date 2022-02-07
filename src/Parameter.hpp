#pragma once

#include "i_parameter_listener.hpp"

namespace live::tritone::vie {
	class parameter {
	public:
		parameter(
			unsigned long id,
			const wchar_t title[128],
			const wchar_t short_title[128],
			const wchar_t units[128],
			long step_count,
			double default_normalized_value,
			long unit_id,
			long flags);

		parameter(const parameter&) = default;
		parameter(parameter&&) = default;

		parameter& operator=(const parameter&) = default;
		parameter& operator=(parameter&&) = default;

		virtual ~parameter() = default;

		[[nodiscard]] unsigned long get_id() const;

		void get_title(wchar_t out[128]) const;

		void get_short_title(wchar_t out[128]) const;

		void get_units(wchar_t out[128]) const;

		[[nodiscard]] long get_step_count() const;

		[[nodiscard]] double get_default_normalized_value() const;

		[[nodiscard]] long get_unit_id() const;

		[[nodiscard]] long get_flags() const;

		[[nodiscard]] double get_normalized_value() const;

		bool set_normalized_value(double normalized_value);

		virtual double to_plain_value(double value_normalized);

		virtual double to_normalized_value(double plain_value);

		virtual void to_string(double normalized_value, wchar_t string[128]);

		virtual bool from_string(const wchar_t* string, double& normalized_value);

		virtual void set_listener(i_parameter_listener* listener);

	private:
		unsigned long id_;
		wchar_t title_[128]{};
		wchar_t short_title_[128]{};
		wchar_t units_[128]{};
		long step_count_;
		double default_normalized_value_;
		long unit_id_;
		long flags_;            //DAW dependent parameters

		double normalized_value_;

		i_parameter_listener* listener_;
	};
}