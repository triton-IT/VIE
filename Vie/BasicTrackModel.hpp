#pragma once

#include <elements.hpp>

#include "Model.hpp"

namespace live::tritone::vie::ui::model {
    struct BasicTrackModel {
        static auto deserialize() {
            return cycfi::elements::basic_track<5, true>();
        }
    };
}