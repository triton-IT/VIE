#pragma once

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>
#include <vector>
#include <string>

#include "Parameter.h"

namespace live::tritone::vie {
	class UserInterface
	{
	public:
		int render(nk_context* ctx,
			std::vector<Parameter*>& parameters_,
			std::string log);
	};
}