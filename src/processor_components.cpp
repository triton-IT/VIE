#include "processor_components.hpp"

#include "components/midi.hpp"
#include "components/oscillator.hpp"
#include "components/envelope.hpp"
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

processor_components::~processor_components() {

}

processor_component* processor_components::create(nlohmann::json processor_definition) {
	processor_component* processor = nullptr;

	const std::string type = processor_definition["type"];
	if (type == "midi")
	{
		processor = new midi(processor_definition);
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
    else if (type == "sample")
    {
        processor = new sample(processor_definition);
    }
    else if (type == "output")
    {
        processor = new output(processor_definition);
    }

	processor->initialize(processor_definition);

	by_id_.emplace(processor->get_id(), processor);
	by_name_.emplace(processor->get_name(), processor);
	//list_.push_back(processor);

	return processor;
}

processor_component& processor_components::get_by_id(unsigned long id)
{
	return *by_id_[id];
}

processor_component& processor_components::get_by_name(std::string name)
{
	return *by_name_[name];
}

processor_component& processor_components::get_by_index(long index) {
	return *list_[index];
}

long processor_components::count()
{
	return by_id_.size();
}