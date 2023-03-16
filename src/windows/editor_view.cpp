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
		host_callback_(nullptr),
		nb_modules_(0)
	{
		//Get appdata path which will be used to create temporary files required to use webview.
		char* appdata_ASCII_path = getenv("APPDATA");
		MultiByteToWideChar(CP_ACP, 0, appdata_ASCII_path, -1, appdata_path, 4096);
	}

	editor_view::~editor_view()
	{
	}

	void editor_view::initialize()
	{
		removed();
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
												on_message_received(json_message_wstring);
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
	
	void editor_view::on_message_received(std::wstring json_wstring)
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
			on_message_get_projects(json_message);
		}
		else if (action_name.compare("new_project") == 0) {
			on_message_new_project(json_message);
			application_.save_project();
		}
		else if (action_name.compare("load_project") == 0) {
			on_message_load_project(json_message);
		}
		else if (action_name.compare("export_project") == 0) {
			on_message_export_project(json_message);
		}
		else if (action_name.compare("import_project") == 0) {
			on_message_import_project(json_message);
		}
		else if (action_name.compare("get_modules") == 0) {
			on_message_get_modules(json_message);
		}
		else if (action_name.compare("add_module") == 0) {
			on_message_add_module(json_message);
			application_.save_project();
		}
		else if (action_name.compare("set_module_name") == 0) {
			on_message_set_module_name(json_message);
			application_.save_project();
		}
		else if (action_name.compare("delete_module") == 0) {
			on_message_delete_module(json_message);
			application_.save_project();
		}
		else if (action_name.compare("set_module_parameter_value") == 0) {
			on_message_set_module_parameter_value(json_message);
			application_.save_project();
		}
		else if (action_name.compare("link_modules") == 0) {
			on_message_link_modules(json_message);
			application_.save_project();
		}
		else if (action_name.compare("unlink_modules") == 0) {
			on_message_unlink_modules(json_message);
			application_.save_project();
		}
		else if (action_name.compare("enable_modules_link") == 0) {
			on_message_enable_modules_link(json_message);
			application_.save_project();
		}
		else if (action_name.compare("disable_modules_link") == 0) {
			on_message_disable_modules_link(json_message);
			application_.save_project();
		}
		else if (action_name.compare("move_module") == 0) {
			on_message_move_module(json_message);
			application_.save_project();
		}
		else if (action_name.compare("undo") == 0) {
			on_message_undo(json_message);
			application_.save_project();
		}
		else if (action_name.compare("redo") == 0) {
			on_message_redo(json_message);
			application_.save_project();
		}
	}

	void editor_view::set_host_callback(host_callback* host_callback)
	{
		host_callback_ = host_callback;
	}

	nlohmann::json editor_view::serialize()
	{
		nlohmann::json view_json = nlohmann::json();
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

		for (uint_fast8_t i = 0; i < nb_modules_; i++)
		{
			nlohmann::json module_view_json = nlohmann::json();

			module_view_json["id"] = modules_views_instances_[i]->id;
			module_view_json["name"] = converter.to_bytes(modules_views_instances_[i]->name);

			nlohmann::json position_json = nlohmann::json();
			position_json["x"] = modules_views_instances_[i]->position[0];
			position_json["y"] = modules_views_instances_[i]->position[1];
			position_json["z"] = modules_views_instances_[i]->position[2];

			module_view_json["position"] = position_json;

			view_json.push_back(module_view_json);
		}

		return view_json;
	}

	void editor_view::deserialize(nlohmann::json definition)
	{
		for (auto& [index, view_module] : definition.items())
		{
			add_module(view_module);
		}
	}

	void editor_view::clear()
	{
		nb_modules_ = 0;
	}
	
	void editor_view::add_module(nlohmann::json module_definition)
	{
		int i = nb_modules_;
		modules_views_instances_[i] = std::make_unique<view::module::module_view_instance>();
		modules_views_instances_[i]->id = module_definition["id"];

		if (module_definition.contains("name"))
		{
			std::string module_name = module_definition["name"];
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			modules_views_instances_[i]->name = converter.from_bytes(module_name);
		}
		modules_views_instances_[i]->position[0] = module_definition["position"]["x"];
		modules_views_instances_[i]->position[1] = module_definition["position"]["y"];
		modules_views_instances_[i]->position[2] = module_definition["position"]["z"];

		nb_modules_++;
	}

	void editor_view::move_module(uint_fast8_t module_id, std::array<uint_fast8_t, 3> position)
	{
		modules_views_instances_[module_id]->position[0] = position[0];
		modules_views_instances_[module_id]->position[1] = position[1];
		modules_views_instances_[module_id]->position[2] = position[2];
	}

	void editor_view::delete_module(int id)
	{
		for (uint_fast8_t i = id; i < nb_modules_ - 1; i++)
		{
			modules_views_instances_[i] = std::move(modules_views_instances_[i + 1]);

			std::wstringstream reply;
			reply << L"{";
			reply << L" \"action\": \"reset_module_identifier\", \"body\": { \"id\": ";
			reply << i+1;
			reply << L", \"new_id\": ";
			reply << i;
			reply << L" }";
			reply << L"}";
			ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
		}
		nb_modules_--;
	}
	
	void editor_view::on_message_get_projects(json message) {
		int nb_projects;
		std::array<project_info, 32> projects = application_.get_projects(&nb_projects);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"get_projects_callback\", \"body\": [";
		
		for (int i = 0; i < nb_projects; i++) {
			auto project = projects[i];

			reply << L" {";
			reply << L" \"id\": ";
			reply << project.id;
			reply << L", \"name\": \"";
			reply << project.name;
			reply << L"\", \"description\": \"";
			reply << project.description;
			reply << L"\" }";
			if(i < nb_projects - 1)
			{
				reply << L",";
			}
		}

		reply << L" ] }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_new_project(json message) {
		project_info project = application_.new_project();

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"new_project_callback\", \"body\": { \"id\": ";
		reply << project.id;
		reply << L", \"name\": \"";
		reply << project.name;
		reply << L"\" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_load_project(json message) {
		uint_fast16_t project_id = message["body"]["id"];
		project_info project = application_.load_project(project_id);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"load_project_callback\", \"body\": { \"id\": ";
		reply << project.id;
		reply << L", \"name\": \"";
		reply << project.name;
		reply << L"\", \"description\": \"";
		reply << project.description;
		reply << L"\" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_export_project(json message) {
		std::string project_path = message["body"]["path"];

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring project_path_w = converter.from_bytes(project_path);

		application_.export_project(project_path_w);
		
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"export_project_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_import_project(json message) {
		std::string project_path = message["body"]["path"];

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring project_path_w = converter.from_bytes(project_path);

		project_info project = application_.import_project(project_path_w);
		
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"import_project_callback\",";
		reply << L" \"body\": {";
		reply << L" \"id\": ";
		reply << project.id;
		reply << L",";
		reply << L" \"name\": \"";
		reply << project.name;
		reply << L"\",";
		reply << L" \"description\": \"";
		reply << project.description;
		reply << L"\"";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_get_modules(json message) {
		uint_fast8_t nb_modules;
		auto modules = application_.get_available_modules(&nb_modules);
		
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"get_modules_callback\",";
		reply << L" \"body\": [";
		
		for (uint_fast8_t module_id = 0; module_id < nb_modules; module_id++)
		{
			auto module = modules[module_id];
			
			reply << L" {";
			reply << L" \"id\": ";
			reply << module.id;
			reply << L", \"name\": \"";
			reply << module.name;
			reply << L"\", \"icon\": \"";
			reply << module.icon;
			reply << L"\", \"input_slots\": ";
			reply << L"[";

			for (uint_fast8_t input_slot_id = 0; input_slot_id < module.nb_input_slots; input_slot_id++)
			{
				auto input_slot = module.input_slots[input_slot_id];
				
				reply << L" {";
				reply << L" \"id\": ";
				reply << input_slot.id;
				reply << L", \"name\": \"";
				reply << input_slot.name;
				reply << L"\" }";
				if (input_slot_id < module.nb_input_slots - 1) {
					reply << L",";
				}
			}
			
			reply << L" ],";
			reply << L" \"output_slots\": ";
			reply << L"[";

			for (uint_fast8_t output_slot_id = 0; output_slot_id < module.nb_output_slots; output_slot_id++)
			{
				auto output_slot = module.output_slots[output_slot_id];
				
				reply << L" {";
				reply << L" \"id\": ";
				reply << output_slot.id;
				reply << L", \"name\": \"";
				reply << output_slot.name;
				reply << L"\" }";
				if (output_slot_id < module.nb_output_slots - 1) {
					reply << L",";
				}
			}
			
			reply << L" ]";
			reply << L" }";
			if (module_id < nb_modules - 1) {
				reply << L",";
			}
		}
		
		reply << L" ]";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_add_module(json message) {
		nlohmann::json module_json = message.at("body");

		uint16_t module_id = application_.add_module(module_json);
		
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"add_module_callback\",";
		reply << L" \"body\": {";
		reply << L" \"module\": {";
		reply << L" \"id\": ";
		reply << std::to_wstring(module_id);
		reply << L" }";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_delete_module(json message) {
		nlohmann::json module_json = message.at("body");

		application_.delete_module(module_json["module_id"]);
		
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"delete_module_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_set_module_name(json message)
	{
		int module_id = message["body"]["module_id"];
		std::string module_name = message["body"]["module_name"];

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring module_name_w = converter.from_bytes(module_name);
		
		modules_views_instances_[module_id]->name = module_name_w;

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"set_module_name_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_set_module_parameter_value(json message) {
		nlohmann::json body = message.at("body");
		uint_fast8_t module_id = body["module_id"];
		uint16_t parameter_id = body["parameter_id"];
		string parameter_value = body["value"];

		std::shared_ptr<processor_module> processor_module = application_.get_processor_by_id(module_id);
		uint16_t parameter_component_id = processor_module->get_id();
		uint16_t parameter_slot_id = parameter_id;

		unsigned long module_parameter_id = (parameter_component_id << 16) | parameter_slot_id;

		//Transmit parameter to host.
		host_callback_->beginEdit(module_parameter_id);
		host_callback_->performEdit(module_parameter_id, std::stof(parameter_value));
		host_callback_->endEdit(module_parameter_id);
	}

	void editor_view::on_message_link_modules(json message) {
		nlohmann::json body_json = message.at("body");
		uint_fast8_t source_module_id = body_json["source_module_id"];
		uint_fast16_t source_slot_id = body_json["source_slot_id"];
		uint_fast8_t target_module_id = body_json["target_module_id"];
		uint_fast16_t target_slot_id = body_json["target_slot_id"];

		uint16_t module_id = application_.link_modules(source_module_id, source_slot_id, target_module_id, target_slot_id);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"link_modules_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_unlink_modules(json message) {
		nlohmann::json body_json = message.at("body");
		uint_fast8_t source_module_id = body_json["source_module_id"];
		uint_fast16_t source_slot_id = body_json["source_slot_id"];
		uint_fast8_t target_module_id = body_json["target_module_id"];
		uint_fast16_t target_slot_id = body_json["target_slot_id"];

		application_.unlink_modules(source_module_id, source_slot_id, target_module_id, target_slot_id);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"unlink_modules_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_disable_modules_link(json message) {
		nlohmann::json body_json = message.at("body");
		uint_fast8_t source_module_id = body_json["source_module_id"];
		uint_fast16_t source_slot_id = body_json["source_slot_id"];
		uint_fast8_t target_module_id = body_json["target_module_id"];
		uint_fast16_t target_slot_id = body_json["target_slot_id"];

		application_.disable_modules_link(source_module_id, source_slot_id, target_module_id, target_slot_id);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"disable_modules_link_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_enable_modules_link(json message) {
		nlohmann::json body_json = message.at("body");
		uint_fast8_t source_module_id = body_json["source_module_id"];
		uint_fast16_t source_slot_id = body_json["source_slot_id"];
		uint_fast8_t target_module_id = body_json["target_module_id"];
		uint_fast16_t target_slot_id = body_json["target_slot_id"];

		application_.enable_modules_link(source_module_id, source_slot_id, target_module_id, target_slot_id);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"enable_modules_link_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_move_module(json message) {
		nlohmann::json body_json = message.at("body");

		uint_fast8_t module_id = body_json["module_id"];
		
		std::array<uint_fast8_t, 3> position;
		position[0] = body_json["position"]["x"];
		position[1] = body_json["position"]["y"];
		position[2] = body_json["position"]["z"];

		application_.move_module(module_id, position);

		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"move_module_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_undo(json message) {
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"undo_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}

	void editor_view::on_message_redo(json message) {
		std::wstringstream reply;
		reply << L"{";
		reply << L" \"action\": \"redo_callback\",";
		reply << L" \"body\": {";
		reply << L" }";
		reply << L" }";
		ptr_web_view_window_->PostWebMessageAsJson(reply.str().c_str());
	}
} // namespace
