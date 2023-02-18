#include "editor_view.hpp"

#include <functional>
#include <OAIdl.h>

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
		ptr_web_view_controller_(nullptr),
		ptr_web_view_window_(nullptr),
		web_message_received_token_(),
		host_callback_(nullptr)
	{
		//Get appdata path which will be used to create temporary files required to use webview.
		char* appdata_ASCII_path = getenv("APPDATA");
		MultiByteToWideChar(CP_ACP, 0, appdata_ASCII_path, -1, appdata_path, 4096);
	}

	editor_view::~editor_view()
	{
	}

	void editor_view::attached(void* parent) {
		HWND hWnd = static_cast<HWND>(parent);

		// Create web view environment based on Appdata path.
		CreateCoreWebView2EnvironmentWithOptions(nullptr, appdata_path, nullptr,
			Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
				[hWnd, this]
		(HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
					// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
					env->CreateCoreWebView2Controller(hWnd, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
						[hWnd, this]
					(HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
							if (controller != nullptr) {
								ptr_web_view_controller_ = controller;
								controller->AddRef();
								ptr_web_view_controller_->get_CoreWebView2(&ptr_web_view_window_);
								ptr_web_view_window_->AddRef();
								ptr_web_view_window_->OpenDevToolsWindow();

								// Should use AddHostObjectToScript but can't make it work. So use messages instead.
								ptr_web_view_window_->add_WebMessageReceived(
									Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
										[this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) {
											if (host_callback_ != nullptr) {
												//Get message from webview.
												wil::unique_cotaskmem_string message_string;
												args->get_WebMessageAsJson(&message_string);

												//Transform message to string
												std::wstring json_message_wstring = message_string.get();
												on_message_received(sender, json_message_wstring);
											}
											return S_OK;
										}).Get(), &web_message_received_token_);

								// Resize WebView to fit the bounds of the parent window
								RECT bounds;
								GetClientRect(hWnd, &bounds);
								ptr_web_view_controller_->put_Bounds(bounds);

								// Open VIE HTML view.
								ptr_web_view_window_->Navigate(
									(std::wstring(L"file://") + content_path + L"/view/index.html").c_str()
								);
							}
							return S_OK;
						}).Get());
					return S_OK;
				}).Get());
	}

	void editor_view::removed()
	{
	}
	
	void editor_view::on_message_received(ICoreWebView2* sender, std::wstring json_wstring)
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &json_wstring[0], (int)json_wstring.size(), NULL, 0, NULL, NULL);
		std::string json_message_string(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &json_wstring[0], (int)json_wstring.size(), &json_message_string[0], size_needed, NULL, NULL);

		//Transform message string to json object.
		const char* json_message_char_ptr = json_message_string.c_str();
		json json_message = json::parse(json_message_char_ptr);

		//get parameter values from json message.
		const std::string action_name = json_message["action"];
		if (action_name.compare("get_projects") == 0) {
			on_message_get_projects(sender, json_message);
		}
		else if (action_name.compare("new_project") == 0) {
			on_message_new_project(sender, json_message);
		}
		else if (action_name.compare("save_project") == 0) {
			on_message_save_project(sender, json_message);
		}
		else if (action_name.compare("save_project_as") == 0) {
			on_message_save_project_as(sender, json_message);
		}
		else if (action_name.compare("load_project") == 0) {
			on_message_load_project(sender, json_message);
		}
		else if (action_name.compare("get_modules") == 0) {
			on_message_get_modules(sender, json_message);
		}
		else if (action_name.compare("add_module") == 0) {
			on_message_add_module(sender, json_message);
		}
		else if (action_name.compare("set_module_name") == 0) {
			on_message_set_module_name(sender, json_message);
		}
		else if (action_name.compare("delete_module") == 0) {
			on_message_delete_module(sender, json_message);
		}
		else if (action_name.compare("set_module_parameter_value") == 0) {
			on_message_set_module_parameter_value(sender, json_message);
		}
		else if (action_name.compare("link_modules") == 0) {
			on_message_link_modules(sender, json_message);
		}
		else if (action_name.compare("undo") == 0) {
			on_message_undo(sender, json_message);
		}
		else if (action_name.compare("redo") == 0) {
			on_message_redo(sender, json_message);
		}
	}

	void editor_view::set_host_callback(host_callback* host_callback)
	{
		host_callback_ = host_callback;
	}

	void editor_view::on_message_get_projects(ICoreWebView2* sender, json message) {
		int nb_projects;
		std::array<project, 32> projects = application_.get_projects(&nb_projects);

		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"get_projects_callback\",";
		reply += L"  \"response\": [";

		for (int i = 0; i < nb_projects; i++) {
			auto project = projects[i];

			reply += L"    {";
			reply += L"      \"id\": ";
			reply += L"      \"";
			reply += project.id;
			reply += L"\",";
			reply += L"      \"name\": ";
			reply += L"      \"";
			reply += project.name;
			reply += L"\"";
			reply += L"    }";
			if(i < nb_projects - 1)
			{
				reply += L",";
			}
		}

		reply += L"  ]";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_new_project(ICoreWebView2* sender, json message) {
		project project = application_.new_project();

		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"new_project_callback\",";
		reply += L"  \"response\": {";
		reply += L"    \"id\": ";
		reply += L"      \"";
		reply += project.id;
		reply += L"\",";
		reply += L"    \"name\": ";
		reply += L"      \"";
		reply += project.name;
		reply += L"\"";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_load_project(ICoreWebView2* sender, json message) {
		std::string project_path = message["body"]["path"];

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring project_path_w = converter.from_bytes(project_path);

		project project = application_.load_project(project_path_w);
		
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"load_project_callback\",";
		reply += L"  \"response\": {";
		reply += L"    \"id\": ";
		reply += L"      \"";
		reply += project.id;
		reply += L"\",";
		reply += L"    \"name\": ";
		reply += L"      \"";
		reply += project.name;
		reply += L"\",";
		reply += L"    \"description\": ";
		reply += L"      \"";
		reply += project.description;
		reply += L"\"";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_save_project(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"save_project_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_save_project_as(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"save_project_as_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_get_modules(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"get_modules_callback\",";
		reply += L"  \"response\": ";
		reply += L"  [";
		reply += L"  {";
		reply += L"    \"id\": ";
		reply += L"    \"424f7df3-9f36-411d-9535-f8323d4f372c\",";
		reply += L"    \"name\": ";
		reply += L"    \"Midi in\",";
		reply += L"    \"type_id\": ";
		reply += L"    \"midi-in\",";
		reply += L"    \"input_slots\": ";
		reply += L"    [";
		reply += L"      {";
		reply += L"        \"id\": ";
		reply += L"        \"378a9a4d-d7d0-4d30-a5f4-0a30e279399e\",";
		reply += L"        \"name\": ";
		reply += L"        \"project1\",";
		reply += L"        \"unit\": ";
		reply += L"        \"db\",";
		reply += L"        \"value\": ";
		reply += L"        1.0";
		reply += L"      }";
		reply += L"    ],";
		reply += L"    \"output_slots\": ";
		reply += L"    [";
		reply += L"      {";
		reply += L"        \"id\": ";
		reply += L"        \"916b23e7-7da9-4059-b72c-830b38da0fd5\",";
		reply += L"        \"name\": ";
		reply += L"        \"project1\",";
		reply += L"        \"unit\": ";
		reply += L"        \"db\",";
		reply += L"        \"value\": ";
		reply += L"        1.0";
		reply += L"      }";
		reply += L"    ],";
		reply += L"    \"position\": ";
		reply += L"    {\"x\":0, \"y\" : 0, \"z\" : 0},";
		reply += L"    \"icon\": ";
		reply += L"    {\"<svg width=\"16\" height=\"16\" viewBox=\"0 0 16 16\" xmlns=\"http://www.w3.org/2000/svg\" fill=\"currentColor\">  <path id=\"mainPath\" d=\"M15 5.5a4.394 4.394 0 0 1-4 4.5 2.955 2.955 0 0 0-.2-1A3.565 3.565 0 0 0 14 5.5a3.507 3.507 0 0 0-7-.3A3.552 3.552 0 0 0 6 5a4.622 4.622 0 0 1 4.5-4A4.481 4.481 0 0 1 15 5.5zM5.5 6a4.5 4.5 0 1 0 0 9.001 4.5 4.5 0 0 0 0-9z\"/></svg>\"},";
		reply += L"  }";
		reply += L"  ]";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_add_module(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"add_module_callback\",";
		reply += L"  \"response\": {";
		reply += L"    \"id\": ";
		reply += L"    \"097afe69-06ec-4dc4-ac34-aa81979cf899\"";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_delete_module(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"delete_module_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_set_module_name(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"set_module_name_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_set_module_parameter_value(ICoreWebView2* sender, json message) {
		nlohmann::json parameters = message["parameters"];
		uint_fast8_t module_id;
		string parameter_id;
		string parameter_value;
		bool module_found = false;
		bool parameter_id_found = false;
		bool parameter_value_found = false;

		for (auto& it : parameters.items()) {
			const std::string parameter_name = it.value()["id"];
			if (parameter_name.compare("module_id") == 0) {
				module_found = true;
				module_id = it.value().at("value");
			}
			else if (parameter_name.compare("parameter_id") == 0) {
				parameter_id_found = true;
				parameter_id = it.value().at("value");
			}
			else if (parameter_name.compare("parameter_value") == 0) {
				parameter_value_found = true;
				parameter_value = it.value().at("value");
			}
		}

		if (module_found && parameter_id_found && parameter_value_found) {
			processor_module& processor_module = application_.get_processor_components().get_by_id(module_id);
			uint16_t parameter_component_id = processor_module.get_id();
			uint16_t parameter_slot_id = processor_module.get_slot_id(parameter_id);

			unsigned long module_parameter_id = (parameter_component_id << 16) | parameter_slot_id;

			//Transmit parameter to host.
			host_callback_->beginEdit(module_parameter_id);
			host_callback_->performEdit(module_parameter_id, std::stof(parameter_value));
			host_callback_->endEdit(module_parameter_id);
		}
		else {
			if (!module_found) {
				sender->PostWebMessageAsJson(L"{\"action\": \"set_module_parameter_value_callback\", \"error\": 0}");
			}
			if (!parameter_id_found) {
				sender->PostWebMessageAsJson(L"{\"action\": \"set_module_parameter_value_callback\", \"error\": 1}");
			}
			if (!parameter_value_found) {
				sender->PostWebMessageAsJson(L"{\"action\": \"set_module_parameter_value_callback\", \"error\": 2}");
			}
		}
	}

	void editor_view::on_message_link_modules(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"link_modules_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_undo(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"undo_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}

	void editor_view::on_message_redo(ICoreWebView2* sender, json message) {
		std::wstring reply;
		reply += L"{";
		reply += L"  \"action\": \"redo_callback\",";
		reply += L"  \"response\": {";
		reply += L"  }";
		reply += L"}";
		sender->PostWebMessageAsJson(reply.c_str());
	}
} // namespace
