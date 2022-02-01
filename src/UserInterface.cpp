#include <stdio.h>
#include <string.h>
#include <climits>
#include <time.h>
#include <cmath>
#include <stdlib.h>

#include "UserInterface.h"

namespace live::tritone::vie {
    int UserInterface::render(nk_context* ctx,
        std::vector<Parameter*>& parameters_,
        std::string log)
    {
        static nk_flags window_flags = 0;

        if (nk_begin(ctx, "Overview", nk_rect(10, 10, 400, 600), window_flags))
        {
            static int check = nk_true;
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Logs", NK_TEXT_LEFT);
            
            nk_label(ctx, log.c_str(), NK_TEXT_LEFT);
            
            if (nk_checkbox_label(ctx, "check", &check)) {
                nk_label(ctx, "Check event.", NK_TEXT_LEFT);
                //FIXME: Pass right parameter.
                Parameter* parameter = parameters_.at(2);
                parameter->setNormalizedValue(check);
            }
        }
        nk_end(ctx);

        return (int) !nk_window_is_closed(ctx, "Overview");
    }
}