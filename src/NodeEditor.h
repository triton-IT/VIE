#pragma once

#include "Node.h"
#include "NodeLink.h"
#include "NodeLinking.h"

namespace live::tritone::vie {
	class NodeEditor {
	public:
        NodeEditor();

        void add(const char* name, struct nk_rect bounds, struct nk_color col, int in_count, int out_count);
        void link(int in_id, int in_slot, int out_id, int out_slot);
		int render(struct nk_context* ctx);

	private:
        Node node_buf[32];
        NodeLink links[64];
        Node* begin;
        Node* end;
        int node_count;
        int link_count;
        struct nk_rect bounds;
        Node* selected;
        int show_grid;
        struct nk_vec2 scrolling;
        NodeLinking linking;

		void push(struct Node* node);
		void pop(struct Node* node);
		Node* find(int ID);
	};
}