#pragma once

#include <wrl.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>

#include "WebView2.h"
#include "wil/com.h"

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
		wchar_t appdata_path[4096];

		ICoreWebView2Controller* ptr_web_view_controller_;
		ICoreWebView2* ptr_web_view_window_;
		EventRegistrationToken web_message_received_token_;

		Steinberg::Vst::IComponentHandler* handler_;
	};
}
