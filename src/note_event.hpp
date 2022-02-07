#pragma once

#include <cstdint>

namespace live::tritone::vie {
	struct note_event {
		uint32_t id;
		int_fast16_t channel;
		int_fast16_t pitch;
		float tuning;
		float velocity;
	};
} // namespace
