#pragma once

#include <json.hpp>

#include "TileModel.hpp"
#include "infra/support.hpp"

namespace live::tritone::vie::ui::model {
	using margin = typename cycfi::elements::margin_element<cycfi::elements::rect, cycfi::remove_cvref_t<htile_composite>>;

	struct MarginModel {
		float top;
		float left;
		float bottom;
		float right;

		std::string childId;

		static margin deserialize(nlohmann::json& json);
	};
}