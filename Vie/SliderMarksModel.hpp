#pragma once

#include "BasicTrackModel.hpp"

namespace live::tritone::vie::ui::model {
    struct SliderMarksModel {
        static auto deserialize() {
            auto track = BasicTrackModel::deserialize();
            return cycfi::elements::slider_marks<40>(track);
        }
    };
}