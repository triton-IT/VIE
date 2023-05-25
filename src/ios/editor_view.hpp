#pragma once

#include <pluginterfaces/vst/ivsteditcontroller.h>

namespace live::tritone::vie {
	class editor_view {
	public:
		explicit editor_view();

		editor_view(const editor_view&) = default;

		editor_view(editor_view&&) = default;

		editor_view& operator=(const editor_view&) = default;

		editor_view& operator=(editor_view&&) = default;

		~editor_view();

		void attached(void* parent);

		void removed();

		void set_component_handler(Steinberg::Vst::IComponentHandler* handler);

	private:

		Steinberg::Vst::IComponentHandler* handler_;
	};
}
