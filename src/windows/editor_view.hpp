#pragma once

#include <wrl.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <json.hpp>

#include "WebView2.h"
#include "wil/com.h"

#include "../host_callback.hpp"
#include "../modules/module_view_descriptor.hpp"

namespace live::tritone::vie {
	class editor_view {
	public:
		explicit editor_view();

		editor_view(const editor_view&) = default;

		editor_view(editor_view&&) = default;

		editor_view& operator=(const editor_view&) = default;

		editor_view& operator=(editor_view&&) = default;

		~editor_view();

		void initialize();

		void attached(void* parent);

		void removed();

		void set_host_callback(host_callback* host_callback);
		
		nlohmann::json serialize();

		void deserialize(nlohmann::json definition);

		void add_module(nlohmann::json module_definition);

#ifdef UNIT_TESTING
	public:
#else
	private:
#endif
		wchar_t appdata_path[4096];

		ICoreWebView2Controller* ptr_web_view_controller_;
		ICoreWebView2* ptr_web_view_window_;
		EventRegistrationToken web_message_received_token_;

		host_callback* host_callback_;
		
		std::array<std::unique_ptr<view::module::module_view_instance>, 255> modules_views_instances_;
		uint_fast8_t nb_modules_;

#ifdef UNIT_TESTING //Because "attached" method contains a lot of non testable elements (webview2), we plug our tests to this method.
	public:
#endif // UNIT_TESTING
		void on_message_received(ICoreWebView2* sender, std::wstring json_wstring);
#ifdef UNIT_TESTING
	private:
#endif // UNIT_TESTING
		void on_message_get_projects(ICoreWebView2* sender, nlohmann::json message);
		void on_message_new_project(ICoreWebView2* sender, nlohmann::json message);
		void on_message_load_project(ICoreWebView2* sender, nlohmann::json message);
		void on_message_export_project(ICoreWebView2* sender, nlohmann::json message);
		void on_message_import_project(ICoreWebView2* sender, nlohmann::json message);
		void on_message_get_modules(ICoreWebView2* sender, nlohmann::json message);
		void on_message_add_module(ICoreWebView2* sender, nlohmann::json message);
		void on_message_link_modules(ICoreWebView2* sender, nlohmann::json message);
		void on_message_delete_module(ICoreWebView2* sender, nlohmann::json message);
		void on_message_set_module_name(ICoreWebView2* sender, nlohmann::json message);
		void on_message_set_module_parameter_value(ICoreWebView2* sender, nlohmann::json message);
		void on_message_undo(ICoreWebView2* sender, nlohmann::json message);
		void on_message_redo(ICoreWebView2* sender, nlohmann::json message);
	};
}
