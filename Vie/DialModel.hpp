#pragma once

#include <elements.hpp>

#include "Model.hpp"

namespace live::tritone::vie::ui::model {
	struct DialModel {
		static auto deserialize(nlohmann::json& json) {
            auto dial = cycfi::elements::share(
                cycfi::elements::dial(
                    cycfi::elements::radial_marks<20>(cycfi::elements::basic_knob<50>()),
                    0.25
                )
            );

            auto markers = cycfi::elements::radial_labels<15>(
                cycfi::elements::hold(dial),
                0.7,                                   // Label font size (relative size)
                "0", "1", "2", "3", "4",               // Labels
                "5", "6", "7", "8", "9", "10"
                );

            return cycfi::elements::align_center_middle(markers);
		}
	};
}