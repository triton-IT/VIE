#pragma once

#include "BasicThumbModel.hpp"
#include "SliderLabelsModel.hpp"

#include "Model.hpp"

namespace live::tritone::vie::ui::model {
	struct SliderModel {
		static auto deserialize(nlohmann::json& json) {
			return cycfi::elements::align_center(
				cycfi::elements::vmargin(
					{ 20, 20 }, 
					cycfi::elements::hold(
						cycfi::elements::share(
							cycfi::elements::slider(
								BasicThumbModel::deserialize(),
								SliderLabelsModel::deserialize(),
								0.25
							)
						)
					)
				)
			);
		}
	};
}