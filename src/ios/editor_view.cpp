#include "editor_view.hpp"

#include <functional>
#include <json.hpp>

#include <locale>
#include <codecvt>
#include <sstream>

#include "../application.hpp"
#include "../processor_module.hpp"

using namespace std;
using json = nlohmann::json;

namespace live::tritone::vie
{
	editor_view::editor_view() :
		handler_(nullptr)
	{
	}

	editor_view::~editor_view()
	{
	}

	void editor_view::attached(void* parent) {
	}

	void editor_view::removed()
	{
	}

	void editor_view::set_component_handler(Steinberg::Vst::IComponentHandler* handler)
	{
		handler_ = handler;
	}
} // namespace
