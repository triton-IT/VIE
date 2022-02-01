#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

#include "UserinterfaceStyle.h"

namespace live::tritone::vie {
    void UserInterfaceStyle::set_style(struct nk_context* ctx, enum theme theme)
    {
        struct nk_color colors[NK_COLOR_COUNT];
        switch (theme) {
        case UserInterfaceStyle::theme::THEME_LIGHT:
                colors[NK_COLOR_TEXT] = nk_rgba(70, 70, 70, 255);
                colors[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
                colors[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
                colors[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
                colors[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
                colors[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
                colors[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
                colors[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
                colors[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
                colors[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
                colors[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
                colors[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
                colors[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
                colors[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
                colors[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
                colors[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
                colors[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
                colors[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
                colors[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
                colors[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
                colors[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
                colors[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
                colors[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
                colors[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
                colors[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
                colors[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
                colors[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
                colors[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
                nk_style_from_table(ctx, colors);
                break;
            
        case UserInterfaceStyle::theme::THEME_DARK:
                colors[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
                colors[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
                colors[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
                colors[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
                colors[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
                colors[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
                colors[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
                colors[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
                colors[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
                colors[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
                colors[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
                colors[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
                colors[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
                colors[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
                colors[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
                colors[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
                colors[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
                colors[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
                colors[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
                colors[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
                colors[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
                colors[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
                colors[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
                colors[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
                colors[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
                colors[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
                colors[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
                colors[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
                nk_style_from_table(ctx, colors);
                break;

        default:
                nk_style_default(ctx);
        }
    }
}