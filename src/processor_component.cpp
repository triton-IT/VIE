#include "processor_component.hpp"

#include "components/midi.hpp"
#include "components/oscillator.hpp"
#include "components/noise.hpp"
#include "components/envelope.hpp"
#include "components/multiplier.hpp"
#include "components/mixer.hpp"
#include "components/output.hpp"

using namespace nlohmann;
using namespace live::tritone::vie::processor::component;

namespace live::tritone::vie
{
	processor_component* processor_component::create(nlohmann::json processor_definition) {
		processor_component* component = nullptr;

		const std::string type = processor_definition["type"];
		if (type == "midi")
		{
			component = new midi(processor_definition);
		}
		else if (type == "oscillator")
		{
			component = new oscillator(processor_definition);
		}
		else if (type == "noise")
		{
			component = new noise(processor_definition);
		}
		else if (type == "envelope")
		{
			component = new envelope(processor_definition);
		}
		else if (type == "multiplier")
		{
			component = new multiplier(processor_definition);
		}
		else if (type == "mixer")
		{
			component = new mixer(processor_definition);
		}
		else if (type == "output")
		{
			component = new output(processor_definition);
		}

		return component;
	}
}