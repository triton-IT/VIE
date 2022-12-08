#include "Bus.h"

namespace live::tritone::vie {
	Bus::Bus(std::wstring name, BusType type, short channelCount, short flags) : active_(false), arrangement_(0)
	{
		name_ = name;
		type_ = type;
		channelCount_ = channelCount;
		flags_ = flags;
	}

	const std::wstring Bus::getName() {
		return name_;
	}

	BusType Bus::getType() {
		return type_;
	}

	short Bus::getChannelCount() {
		return channelCount_;
	}

	short Bus::getFlags() {
		return flags_;
	}

	bool Bus::getActive() {
		return active_;
	}

	void Bus::setActive(bool active) {
		active_ = active;
	}

	unsigned long int Bus::getArrangement() {
		return arrangement_;
	}

	void Bus::setArrangement(unsigned long int arrangement) {
		arrangement_ = arrangement;
	}
}
