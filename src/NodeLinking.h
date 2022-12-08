#pragma once

#include "Node.h"

namespace live::tritone::vie {
	class NodeLinking {
    public:
        int active;
        Node* node;
        int input_id;
        int input_slot;
	};
}
