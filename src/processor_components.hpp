#pragma once

#include "processor_component.hpp"

#include <json.hpp>

#include <memory>
#include <string>
#include <map>
#include <vector>

namespace live::tritone::vie {
	class processor_components {
	public:
		static processor_components& get_instance();

		processor_components(const processor_components&) = default;
		processor_components(processor_components&&) = default;

		~processor_components();

		/**
		* Create a processor component from json file.
		*/
		processor_component* create(nlohmann::json processor_definition);

		processor_components& operator=(const processor_components&) = default;
		processor_components& operator=(processor_components&&) = default;

		processor_component& get_by_id(uint_fast8_t id);

	private:
		processor_components();

		processor_component* components_[512];
		
		uint_fast8_t nb_components_ = 0;
	};
}