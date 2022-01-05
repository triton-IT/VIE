#include <stdio.h>
#include <string.h>
#include <climits>
#include <time.h>
#include <cmath>

#include "UserInterface.h"

namespace live::tritone::vie {
    int UserInterface::render(nk_context* ctx)
    {
        static nk_flags window_flags = 0;

        if (nk_begin(ctx, "Overview", nk_rect(10, 10, 400, 600), window_flags))
        {
            static int check = nk_true;
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Nuklear", NK_TEXT_LEFT);
            nk_label(ctx, "By Micha Mettke", NK_TEXT_LEFT);
            nk_label(ctx, "nuklear is licensed under the public domain License.",  NK_TEXT_LEFT);
            nk_checkbox_label(ctx, "check", &check);
        }
        nk_end(ctx);

        return (int) !nk_window_is_closed(ctx, "Overview");
    }
}