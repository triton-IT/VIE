#pragma once

#include <elements.hpp>

#include "Model.hpp"

namespace live::tritone::vie::ui::model {
	struct AlignModel : Model {
		float align;

		std::string childId;

		using elementsMap = std::map<std::string const, Model*>;

		auto deserialize(elementsMap& elements) {
			//return halign(0.5f, subject);
		}
	};
}
