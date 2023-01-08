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

		processor_component& get_by_id(unsigned long id);
		processor_component& get_by_name(std::string name);
		processor_component& get_by_index(long index);

		long count();

	private:
		processor_components() = default;

		std::map<unsigned long, processor_component*> by_id_;
		std::map<std::string, processor_component*> by_name_;
		std::vector<processor_component*> list_;
	};
}