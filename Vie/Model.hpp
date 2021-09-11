#pragma once

namespace live::tritone::vie::ui::model {
	enum class ModelType {
		MARGIN,
		TILE,
		SLIDER,
		ALIGN,
		MIN_SIZE,
		STRETCH,
		PANE,
		DIAL
	};

	struct Model {
		ModelType type;

		using elementsMap = std::map<std::string const, Model*>;

		auto deserialize(nlohmann::json& json, elementsMap& elements);
	};
}