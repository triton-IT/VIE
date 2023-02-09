#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

#include <stdio.h>
#include <string>

#include "NodeEditor.h"

namespace live::tritone::vie {

    NodeEditor::NodeEditor()
    {
        begin = NULL;
        end = NULL;
        node_count = 0;
        link_count = 0;
        scrolling.x = 0;
        scrolling.y = 0;
        ids = 0;
        show_grid = nk_true;
        add("Source", nk_rect(0, 0, 180, 220), nk_rgb(255, 0, 0), 0, 1);
        add("Source", nk_rect(40, 260, 180, 220), nk_rgb(0, 255, 0), 0, 1);
        add("Combine", nk_rect(200, 100, 180, 220), nk_rgb(0, 0, 255), 2, 2);
        link(0, 0, 2, 0);
        link(1, 0, 2, 1);
    }

    void NodeEditor::add(const char* name, struct nk_rect bounds,
        struct nk_color col, int in_count, int out_count)
    {
        Node* node;
        node = &node_buf[node_count++];
        node->ID = ids++;
        node->value = 0;
        node->color = nk_rgb(255, 0, 0);
        node->input_count = in_count;
        node->output_count = out_count;
        node->color = col;
        node->bounds = bounds;
        strcpy(node->name, name);
        push(node);
    }

    void NodeEditor::link(int in_id, int in_slot,
        int out_id, int out_slot)
    {
        NodeLink* lnk;
        lnk = &links[link_count++];
        lnk->input_id = in_id;
        lnk->input_slot = in_slot;
        lnk->output_id = out_id;
        lnk->output_slot = out_slot;
    }

    int NodeEditor::render(struct nk_context* ctx)
    {
        int n = 0;
        struct nk_rect total_space;
        const struct nk_input* in = &ctx->input;
        struct nk_command_buffer* canvas;
        Node* updated = 0;

        if (nk_begin(ctx, "Node editor", nk_rect(200, 0, 400, 400),
            NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE))
        {
            /* allocate complete window space */
            canvas = nk_window_get_canvas(ctx);
            total_space = nk_window_get_content_region(ctx);
            nk_layout_space_begin(ctx, NK_STATIC, total_space.h, node_count);
            {
                Node* it = begin;
                struct nk_rect size = nk_layout_space_bounds(ctx);
                struct nk_panel* panel = 0;

                if (show_grid) {
                    /* display grid */
                    float x, y;
                    const float grid_size = 32.0f;
                    const struct nk_color grid_color = nk_rgb(50, 50, 50);
                    for (x = (float)fmod(size.x - scrolling.x, grid_size); x < size.w; x += grid_size)
                        nk_stroke_line(canvas, x + size.x, size.y, x + size.x, size.y + size.h, 1.0f, grid_color);
                    for (y = (float)fmod(size.y - scrolling.y, grid_size); y < size.h; y += grid_size)
                        nk_stroke_line(canvas, size.x, y + size.y, size.x + size.w, y + size.y, 1.0f, grid_color);
                }

                /* execute each node as a movable group */
                while (it) {
                    /* calculate scrolled node window position and size */
                    nk_layout_space_push(ctx, nk_rect(it->bounds.x - scrolling.x,
                        it->bounds.y - scrolling.y, it->bounds.w, it->bounds.h));

                    /* execute node window */
                    if (nk_group_begin(ctx, it->name, NK_WINDOW_MOVABLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE))
                    {
                        /* always have last selected node on top */

                        panel = nk_window_get_panel(ctx);
                        if (nk_input_mouse_clicked(in, NK_BUTTON_LEFT, panel->bounds) &&
                            (!(it->prev && nk_input_mouse_clicked(in, NK_BUTTON_LEFT,
                                nk_layout_space_rect_to_screen(ctx, panel->bounds)))) &&
                            end != it)
                        {
                            updated = it;
                        }

                        /* ================= NODE CONTENT =====================*/
                        nk_layout_row_dynamic(ctx, 25, 1);
                        nk_button_color(ctx, it->color);
                        it->color.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, it->color.r, 255, 1, 1);
                        it->color.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, it->color.g, 255, 1, 1);
                        it->color.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, it->color.b, 255, 1, 1);
                        it->color.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, it->color.a, 255, 1, 1);
                        /* ====================================================*/
                        nk_group_end(ctx);
                    }
                    {
                        /* node connector and linking */
                        float space;
                        struct nk_rect bounds;
                        bounds = nk_layout_space_rect_to_local(ctx, panel->bounds);
                        bounds.x += scrolling.x;
                        bounds.y += scrolling.y;
                        it->bounds = bounds;

                        /* output connector */
                        space = panel->bounds.h / (float)((it->output_count) + 1);
                        for (n = 0; n < it->output_count; ++n) {
                            struct nk_rect circle;
                            circle.x = panel->bounds.x + panel->bounds.w - 4;
                            circle.y = panel->bounds.y + space * (float)(n + 1);
                            circle.w = 8; circle.h = 8;
                            nk_fill_circle(canvas, circle, nk_rgb(100, 100, 100));

                            /* start linking process */
                            if (nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, circle, nk_true)) {
                                linking.active = nk_true;
                                linking.node = it;
                                linking.input_id = it->ID;
                                linking.input_slot = n;
                            }

                            /* draw curve from linked node slot to mouse position */
                            if (linking.active && linking.node == it &&
                                linking.input_slot == n) {
                                struct nk_vec2 l0 = nk_vec2(circle.x + 3, circle.y + 3);
                                struct nk_vec2 l1 = in->mouse.pos;
                                nk_stroke_curve(canvas, l0.x, l0.y, l0.x + 50.0f, l0.y,
                                    l1.x - 50.0f, l1.y, l1.x, l1.y, 1.0f, nk_rgb(100, 100, 100));
                            }
                        }

                        /* input connector */
                        space = panel->bounds.h / (float)((it->input_count) + 1);
                        for (n = 0; n < it->input_count; ++n) {
                            struct nk_rect circle;
                            circle.x = panel->bounds.x - 4;
                            circle.y = panel->bounds.y + space * (float)(n + 1);
                            circle.w = 8; circle.h = 8;
                            nk_fill_circle(canvas, circle, nk_rgb(100, 100, 100));
                            if (nk_input_is_mouse_released(in, NK_BUTTON_LEFT) &&
                                nk_input_is_mouse_hovering_rect(in, circle) &&
                                linking.active && linking.node != it) {
                                linking.active = nk_false;
                                link(linking.input_id, linking.input_slot, it->ID, n);
                            }
                        }
                    }
                    it = it->next;
                }

                /* reset linking connection */
                if (linking.active && nk_input_is_mouse_released(in, NK_BUTTON_LEFT)) {
                    linking.active = nk_false;
                    linking.node = NULL;
                    fprintf(stdout, "linking failed\n");
                }

                /* draw each link */
                for (n = 0; n < link_count; ++n) {
                    NodeLink* lnk = &links[n];
                    Node* ni = find(lnk->input_id);
                    Node* no = find(lnk->output_id);
                    float spacei = panel->bounds.h / (float)((ni->output_count) + 1);
                    float spaceo = panel->bounds.h / (float)((no->input_count) + 1);
                    struct nk_vec2 l0 = nk_layout_space_to_screen(ctx,
                        nk_vec2(ni->bounds.x + ni->bounds.w, 3.0f + ni->bounds.y + spacei * (float)(lnk->input_slot + 1)));
                    struct nk_vec2 l1 = nk_layout_space_to_screen(ctx,
                        nk_vec2(no->bounds.x, 3.0f + no->bounds.y + spaceo * (float)(lnk->output_slot + 1)));

                    l0.x -= scrolling.x;
                    l0.y -= scrolling.y;
                    l1.x -= scrolling.x;
                    l1.y -= scrolling.y;
                    nk_stroke_curve(canvas, l0.x, l0.y, l0.x + 50.0f, l0.y,
                        l1.x - 50.0f, l1.y, l1.x, l1.y, 1.0f, nk_rgb(100, 100, 100));
                }

                if (updated) {
                    /* reshuffle nodes to have least recently selected node on top */
                    pop(updated);
                    push(updated);
                }

                /* node selection */
                if (nk_input_mouse_clicked(in, NK_BUTTON_LEFT, nk_layout_space_bounds(ctx))) {
                    it = begin;
                    selected = NULL;
                    bounds = nk_rect(in->mouse.pos.x, in->mouse.pos.y, 100, 200);
                    while (it) {
                        struct nk_rect b = nk_layout_space_rect_to_screen(ctx, it->bounds);
                        b.x -= scrolling.x;
                        b.y -= scrolling.y;
                        if (nk_input_is_mouse_hovering_rect(in, b))
                            selected = it;
                        it = it->next;
                    }
                }

                /* contextual menu */
                if (nk_contextual_begin(ctx, 0, nk_vec2(100, 220), nk_window_get_bounds(ctx))) {
                    const char* grid_option[] = { "Show Grid", "Hide Grid" };
                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (nk_contextual_item_label(ctx, "New", NK_TEXT_CENTERED))
                        add("New", nk_rect(400, 260, 180, 220),
                            nk_rgb(255, 255, 255), 1, 2);
                    if (nk_contextual_item_label(ctx, grid_option[show_grid], NK_TEXT_CENTERED))
                        show_grid = !show_grid;
                    nk_contextual_end(ctx);
                }
            }
            nk_layout_space_end(ctx);

            /* window content scrolling */
            if (nk_input_is_mouse_hovering_rect(in, nk_window_get_bounds(ctx)) &&
                nk_input_is_mouse_down(in, NK_BUTTON_MIDDLE)) {
                scrolling.x += in->mouse.delta.x;
                scrolling.y += in->mouse.delta.y;
            }
        }
        nk_end(ctx);
        return !nk_window_is_closed(ctx, "NodeEdit");
    }

    void NodeEditor::push(Node* node)
    {
        if (!begin) {
            node->next = NULL;
            node->prev = NULL;
            begin = node;
            end = node;
        }
        else {
            node->prev = end;
            if (end)
                end->next = node;
            node->next = NULL;
            end = node;
        }
    }

    void NodeEditor::pop(Node* node)
    {
        if (node->next)
            node->next->prev = node->prev;
        if (node->prev)
            node->prev->next = node->next;
        if (end == node)
            end = node->prev;
        if (begin == node)
            begin = node->next;
        node->next = NULL;
        node->prev = NULL;
    }

    Node* NodeEditor::find(int ID)
    {
        Node* iter = begin;
        while (iter) {
            if (iter->ID == ID)
                return iter;
            iter = iter->next;
        }
        return NULL;
    }
}