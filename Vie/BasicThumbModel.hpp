#pragma once

#include <elements.hpp>

#include "Model.hpp"

namespace live::tritone::vie::ui::model {
	struct BasicThumbModel {
		static auto deserialize() {
			return cycfi::elements::basic_thumb<25>();
		}
	};
}