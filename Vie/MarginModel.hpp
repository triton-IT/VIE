#pragma once

#include "Model.hpp"
#include "SliderModel.hpp"
#include "DialModel.hpp"

namespace live::tritone::vie::ui::model {
	struct MarginModel : Model {
		float top;
		float left;
		float bottom;
		float right;

		std::string childId;

		static auto deserialize(nlohmann::json& json) {
			auto top = json["top"].get<float>();
			auto left = json["left"].get<float>();
			auto bottom = json["bottom"].get<float>();
			auto right = json["right"].get<float>();
			auto child = json["child"];

			auto childType = child["type"];
			if (childType.get<std::string>().compare("tile") == 0) {
				auto hTile = TileModel::deserialize(child);
				return cycfi::elements::margin({ left, top, right, bottom }, hTile);
			}
		}
	};
}