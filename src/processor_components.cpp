#include "Application.hpp"
#include "processor_components.hpp"

#include "components/midi_input.hpp"
#include "components/audio_input.hpp"
#include "components/oscillator.hpp"
#include "components/envelope.hpp"
#include "components/low_pass.hpp"
#include "components/high_pass.hpp"
#include "components/gain.hpp"
#include "components/multiplier.hpp"
#include "components/mixer.hpp"
#include "components/output.hpp"
#include "components/sample.hpp"

using namespace live::tritone::vie;
using namespace live::tritone::vie::processor::component;

processor_components& processor_components::get_instance() {
	static processor_components instance;
	return instance;
}

processor_components::processor_components() : components_{} {
}

processor_components::~processor_components() {

}

processor_component* processor_components::create(nlohmann::json processor_definition) {
	processor_component* processor = nullptr;
	
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
    else if (type == "output")
    {
        processor = new output(processor_definition);
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

	processor->initialize(processor_definition);

	components_[nb_components_] = processor;
	nb_components_++;

#ifdef VIE_DEBUG
	debugLogger.write("Added processor: " + processor->get_name());
#endif

	return processor;
}

processor_component& processor_components::get_by_id(uint_fast8_t id)
{
	return *components_[nb_components_];
}