#pragma once

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

namespace live::tritone::vie {
	class NodeLink {
    public:
        int input_id;
        int input_slot;
        int output_id;
        int output_slot;
        struct nk_vec2 in;
        struct nk_vec2 out;
	};
}