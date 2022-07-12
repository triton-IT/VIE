#pragma once

#include "note_event.hpp"

namespace live::tritone::vie {
    enum class sample_size {
        sample_size32,
        sample_size64
    };

    enum class processing_mode {
        realtime,
        prefetch,
        offline
    };

    struct processing_setup {
        processing_mode processing_mode;
        sample_size sample_size;
        long max_samples_per_block;
        double sample_rate;
    };

    struct event {
        union core_event {
            note_event note_on;
            note_event note_off;
        } core_event;

        enum class type {
            note_on,
            note_off,
            data_event,
            poly_pressure_event,
            note_expression_value_event,
            note_expression_text_event,
            chord_event,
            scale_event,
            legacy_midi_cc_out_event
        };

        long bus_index;
        long sample_offset;
        double position;
        bool is_live;
        type type;
    };
}