#pragma once

#include <string>

namespace live::tritone::vie::view::module
{
	struct slot_view_descriptor {
		uint_fast8_t id;
		std::wstring name;
	};

	struct module_view_descriptor {		
		uint_fast16_t id;
		std::wstring name;
		std::wstring icon;

		std::array<slot_view_descriptor, 32> input_slots;
		uint_fast8_t nb_input_slots;
		std::array<slot_view_descriptor, 32> output_slots;
		uint_fast8_t nb_output_slots;

		std::wstring to_json();
	};

	struct module_view_instance {
		uint_fast16_t id;

		uint_fast16_t position[3];
	};
} // namespace
