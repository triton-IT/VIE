#pragma once

#include "SliderMarksModel.hpp"

namespace live::tritone::vie::ui::model {
    struct SliderLabelsModel {
        static auto deserialize() {
            auto marks = SliderMarksModel::deserialize();
            return cycfi::elements::slider_labels<10>(
                marks,
                0.8f,
                "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"
                );
        }
    };
}