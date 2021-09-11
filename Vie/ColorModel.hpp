#pragma once

#include <elements.hpp>

namespace live::tritone::vie::ui::model {
	struct ColorModel {
		float red;
		float green;
		float blue;
		float alpha;

		using elementsMap = std::map<std::string const, void*>;

		auto deserialize(elementsMap& elements) {
			return cycfi::elements::colors::black;
		}
	};
}