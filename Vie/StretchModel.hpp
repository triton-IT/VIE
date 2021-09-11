#pragma once

#include <elements.hpp>

#include "Model.hpp"

namespace live::tritone::vie::ui::model {
	struct StretchModel : Model {
		float stretch;

		std::string childId;

		using elementsMap = std::map<std::string const, Model*>;
		auto deserialize(elementsMap& elements) {
			//return hstretch(subject);
		}
	};
}