#pragma once

#include <cstdint>

namespace live::tritone::vie {
	struct data_event {
		uint32_t size;
		uint32_t type;
		const uint8_t* byte;
	};
} // namespace
