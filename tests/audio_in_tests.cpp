#include "../src/modules/audio_input.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace live::tritone::vie;
using namespace live::tritone::vie::processor::module;

SCENARIO("audio_in should output same sample than input one.", "[audio-in]") {
    GIVEN("An empty input buffer") {
        audio_input audio_in;
		
        audio_bus_buffers buffer;
        audio_in.set_buffer(&buffer);

        output_process_data data;

        std::array<float_array_module_output*, 32> outputs;
        for (auto output : outputs)
        {
            output = new float_array_module_output;
        }
		
        WHEN("input is processed") {
            audio_in.process(data);

            THEN("audio output slot value is empty") {
                auto output_modules = reinterpret_cast<std::array<module_output*, 32>&>(outputs);
				
                auto nb_values = audio_in.get_output_values(1, output_modules);
				
                REQUIRE(nb_values == 1);
                REQUIRE(output_modules[0]->to_float_array().nb_values == 0);
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

        std::array<float_array_module_output*, 32> modules;
        for (int i = 0; i <32; i++)
        {
            modules[i] = new float_array_module_output;
        }

        WHEN("input is processed") {
            audio_in.process(data);

            THEN("audio output slot value contains same data as input") {
                auto output_modules = reinterpret_cast<std::array<module_output*, 32>&>(modules);

                auto nb_values = audio_in.get_output_values(1, output_modules);

                REQUIRE(nb_values == 1);
                REQUIRE(output_modules[0]->to_float_array().nb_values == 8);
                REQUIRE(output_modules[0]->to_float_array().values[0] == 0.0f);
                REQUIRE(output_modules[0]->to_float_array().values[1] == 1.0f);
                REQUIRE(output_modules[0]->to_float_array().values[2] == 2.0f);
                REQUIRE(output_modules[0]->to_float_array().values[3] == 3.0f);
            }
        }
    }
}