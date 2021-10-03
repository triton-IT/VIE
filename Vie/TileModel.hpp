#pragma once

#include <elements.hpp>

#include "PaneModel.hpp"
#include "DialModel.hpp"

namespace live::tritone::vie::ui::model {
	using htile_composite = cycfi::elements::composite<std::vector<cycfi::elements::element_ptr>, cycfi::elements::htile_element>;

	struct TileModel : Model {
		std::list<std::string> childrenIds;

		static auto deserialize(nlohmann::json& json) {
			auto children = json["children"];

			std::vector<cycfi::elements::element_ptr> childrenElements;

			for (auto& child : children) {
				auto type = child["type"].get<std::string>();

				if (type.compare("slider") == 0) {
					auto slider = SliderModel::deserialize(child);
					childrenElements.push_back(share(slider));
				}
				else if (type.compare("knob") == 0) {
					auto knob = DialModel::deserialize(child);
					childrenElements.push_back(share(knob));
				}
				else if (type.compare("pane") == 0) {
					auto pane = PaneModel::deserialize(child);
					childrenElements.push_back(share(pane));
				}
			}

			htile_composite htile;
			htile = childrenElements;
			return htile;
		}
	};
}