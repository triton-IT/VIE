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
    class Node {
    public:
        int ID;
        char name[32];
        struct nk_rect bounds;
        float value;
        struct nk_color color;
        int input_count;
        int output_count;
        Node* next;
        Node* prev;
    };
}