#pragma once

#include <fstream>
#include <string>

namespace live::tritone::vie {
	class host_callback {
	public:
		virtual void beginEdit(uint32_t id) = 0;
		virtual void performEdit(uint32_t id, double valueNormalized) = 0;
		virtual void endEdit(uint32_t id) = 0;
		virtual void restartComponent(int32_t flags) = 0;
	};
}
