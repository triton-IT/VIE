#pragma once

#include <string>

namespace live::tritone::vie::processor::module
{
	struct slot_descriptor {
		uint_fast8_t id;
		std::wstring name;
	};

	struct module_descriptor {		
		uint_fast16_t id;
		std::wstring name;
		std::wstring icon;

		std::array<slot_descriptor, 32> input_slots;
		uint_fast8_t nb_input_slots;
		std::array<slot_descriptor, 32> output_slots;
		uint_fast8_t nb_output_slots;

		std::wstring to_json();
	};
} // namespace
