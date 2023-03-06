#include "bus.hpp"

namespace live::tritone::vie {
	bus::bus(std::wstring name,
		const bus_type type,
		processor_module& processor_module,
		const short channel_count,
		const bool is_default_active,
		const bool is_control_voltage):
	name_(std::move(name)),
	type_(type), channel_count_(channel_count),
	is_default_active_(is_default_active),
	is_control_voltage_(is_control_voltage),
	active_(false),
	arrangement_(0),
	processor_module_(processor_module) {
	}

	std::wstring bus::get_name()
	{
		return name_;
	}

	bus_type bus::get_type() const
	{
		return type_;
	}

	short bus::get_channel_count() const
	{
		return channel_count_;
	}

	bool bus::is_default_active() const
	{
		return is_default_active_;
	}

	bool bus::is_control_voltage() const
	{
		return is_control_voltage_;
	}

	bool bus::get_active() const
	{
		return active_;
	}

	void bus::set_active(const bool active) {
		active_ = active;
	}

	uint64_t bus::get_arrangement() const
	{
		return arrangement_;
	}

	void bus::set_arrangement(const uint64_t arrangement) {
		arrangement_ = arrangement;
	}

	processor_module& bus::get_processor_module() const
	{
		return processor_module_;
	}
}
