#pragma once

#include <string>
#include <cstdint>

#include "processor_module.hpp"
#include <pluginterfaces/base/ftypes.h>

namespace live::tritone::vie {
	enum class media_type {
		audio,
		event
	};

	enum class bus_direction {
		input,
		output
	};

	enum class bus_type {
		main,
		aux
	};

	struct bus_info {
		media_type media_type;
		bus_direction direction;
		long channel_count;
		Steinberg::char16 name[128];
		bus_type bus_type;
		bool is_default_active;
		bool is_control_voltage;
	};

	class bus {
	public:
		bus(Steinberg::char16 name[128], bus_type type, processor_module* processor_module, short channel_count = 2,
		    bool is_default_active = true, bool is_control_voltage = false);

		Steinberg::char16* get_name();

		[[nodiscard]] bus_type get_type() const;

		[[nodiscard]] short get_channel_count() const;

		[[nodiscard]] bool is_default_active() const;

		[[nodiscard]] bool is_control_voltage() const;

		[[nodiscard]] bool get_active() const;

		void set_active(bool active);

		[[nodiscard]] ::uint64_t get_arrangement() const;

		void set_arrangement(::uint64_t arrangement);

		[[nodiscard]] processor_module* get_processor_module() const;

	private:
		Steinberg::char16 name_[128];
		bus_type type_;
		short channel_count_;
		bool is_default_active_;
		bool is_control_voltage_;
		bool active_;
		uint64_t arrangement_;
		processor_module* processor_module_;
	};
}