#pragma once

#include <string>

namespace live::tritone::vie {
	enum class BusType {
		main,
		aux
	};

	enum class BusFlags
	{
		defaultActive = 1 << 0,
		isControlVoltage = 1 << 1
	};

    class Bus {
    public:
		Bus(std::wstring name, BusType type, short channelCount = 2, short flag = 0);

		const std::wstring getName();
		BusType getType();
		short getChannelCount();
		short getFlags();
		bool getActive();
		void setActive(bool active);
		unsigned long int getArrangement();
		void setArrangement(unsigned long int arrangement);

	private:
		std::wstring name_;
		BusType type_;
		short channelCount_;
		// Any combination of BusFlags
		short flags_;
		bool active_;
		unsigned long int arrangement_;
    };
}