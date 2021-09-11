#pragma once

#include "SliderModel.hpp"
#include "DialModel.hpp"

namespace live::tritone::vie::ui::model {
	struct PaneModel {
		using htile_composite = cycfi::elements::composite<std::vector<cycfi::elements::element_ptr>, cycfi::elements::htile_element>;

		using elements_ptrs_array = std::array<cycfi::elements::element_ptr, 2>;
		using layer_composite = cycfi::elements::composite<elements_ptrs_array, cycfi::elements::layer_element>;

		static layer_composite deserialize(nlohmann::json& json) {
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
					auto paneModel = PaneModel::deserialize(child);
					childrenElements.push_back(share(paneModel));
				}
			}

			htile_composite tile;
			tile = childrenElements;

			return pane("Pane", hmin_size(300, tile), 0.8f);
		}
	};
}
