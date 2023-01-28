#include "../src/components/audio_input.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace live::tritone::vie;
using namespace live::tritone::vie::processor::component;

SCENARIO("audio_in should output same sample than input one.", "[audio-in]") {
    GIVEN("An empty input buffer") {
        audio_input audio_in;
		
        audio_bus_buffers buffer;
        audio_in.set_buffer(&buffer);

        output_process_data data;

        std::array<float_array_component_output*, 32> outputs;
        for (auto output : outputs)
        {
            output = new float_array_component_output;
        }
		
        WHEN("input is processed") {
            audio_in.process(data);

            THEN("audio output slot value is empty") {
                auto output_components = reinterpret_cast<std::array<component_output*, 32>&>(outputs);
				
                auto nb_values = audio_in.get_output_values(1, output_components);
				
                REQUIRE(nb_values == 1);
                REQUIRE(output_components[0]->to_float_array().nb_values == 0);
            }
        }
    }
	
    GIVEN("An input buffer with data") {
        audio_input audio_in;

        audio_bus_buffers buffer;
        buffer.num_channels = 1;
        float first_channel_buffer[] = { 0.0f, 1.0f, 2.0f, 3.0f };
        float (*channels_buffer)[4] = &first_channel_buffer;
        buffer.channels_buffer = reinterpret_cast<void**>(&channels_buffer);
        audio_in.set_buffer(&buffer);

        output_process_data data;
        data.num_outputs = 1;
        data.num_samples = 8;

        std::array<float_array_component_output*, 32> outputs;
        for (int i = 0; i <32; i++)
        {
            outputs[i] = new float_array_component_output;
        }

        WHEN("input is processed") {
            audio_in.process(data);

            THEN("audio output slot value contains same data as input") {
                auto output_components = reinterpret_cast<std::array<component_output*, 32>&>(outputs);

                auto nb_values = audio_in.get_output_values(1, output_components);

                REQUIRE(nb_values == 1);
                REQUIRE(output_components[0]->to_float_array().nb_values == 8);
                REQUIRE(output_components[0]->to_float_array().values[0] == 0.0f);
                REQUIRE(output_components[0]->to_float_array().values[1] == 1.0f);
                REQUIRE(output_components[0]->to_float_array().values[2] == 2.0f);
                REQUIRE(output_components[0]->to_float_array().values[3] == 3.0f);
            }
        }
    }
}