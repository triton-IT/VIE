#include "Application.hpp"
#include "processor_modules.hpp"

#include "modules/midi_input.hpp"
#include "modules/audio_input.hpp"
#include "modules/oscillator.hpp"
#include "modules/envelope.hpp"
#include "modules/low_pass.hpp"
#include "modules/high_pass.hpp"
#include "modules/gain.hpp"
#include "modules/multiplier.hpp"
#include "modules/mixer.hpp"
#include "modules/audio_output.hpp"
#include "modules/sample.hpp"
#include "modules/recorder.hpp"

using namespace live::tritone::vie;
using namespace live::tritone::vie::processor::module;

processor_modules& processor_modules::get_instance() {
	static processor_modules instance;
	return instance;
}

processor_modules::processor_modules() : modules_{} {
}

processor_modules::~processor_modules() {

}

processor_module* processor_modules::create(nlohmann::json processor_definition) {
	processor_module* processor = nullptr;
	
	const std::string type = processor_definition["type"];
	if (type == "midi in")
	{
		processor = new midi_input(processor_definition);
	}
	else if (type == "audio in")
	{
		processor = new audio_input(processor_definition);
	}
	else if (type == "oscillator")
	{
		processor = new oscillator(processor_definition);
	}
	else if (type == "envelope")
	{
		processor = new envelope(processor_definition);
	}
	else if (type == "multiplier")
	{
		processor = new multiplier(processor_definition);
	}
	else if (type == "mixer")
	{
		processor = new mixer(processor_definition);
	}
    else if (type == "sample")
    {
        processor = new sample(processor_definition);
    }
    else if (type == "audio out")
    {
		processor = new audio_output(processor_definition);
    }
	else if (type == "low-pass")
	{
		processor = new low_pass(processor_definition);
	}
	else if (type == "high-pass")
	{
		processor = new high_pass(processor_definition);
	}
	else if (type == "gain")
	{
		processor = new gain(processor_definition);
	}
	else if (type == "recorder")
	{
		processor = new recorder(processor_definition);
	}

	processor->initialize(processor_definition);

	modules_[nb_modules_] = processor;
	nb_modules_++;

#ifdef VIE_DEBUG
	debugLogger.write("Added processor: " + processor->get_name());
#endif

	return processor;
}

processor_module& processor_modules::get_by_id(uint_fast8_t id)
{
	return *modules_[nb_modules_];
}