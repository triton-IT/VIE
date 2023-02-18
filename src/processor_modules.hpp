#pragma once

#include "processor_module.hpp"

#include <json.hpp>

#include <memory>
#include <string>
#include <map>
#include <vector>

namespace live::tritone::vie {
	class processor_modules {
	public:
		static processor_modules& get_instance();

		processor_modules(const processor_modules&) = default;
		processor_modules(processor_modules&&) = default;

		~processor_modules();

		/**
		* Create a processor module from json file.
		*/
		processor_module* create(nlohmann::json processor_definition);

		processor_modules& operator=(const processor_modules&) = default;
		processor_modules& operator=(processor_modules&&) = default;

		processor_module& get_by_id(uint_fast8_t id);

	private:
		processor_modules();

		processor_module* modules_[512];
		
		uint_fast8_t nb_modules_ = 0;
	};
}