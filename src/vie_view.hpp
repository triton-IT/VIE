#pragma once

#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>

#include <thread>
#include <string>
#include <vector>

#include <json.hpp>

#include "parameter.hpp"

#include "host_callback.hpp"

#ifdef _WIN32

#include "windows/editor_view.hpp"

#endif

namespace live::tritone::vie
{
	class vie_view final : public Steinberg::IPlugView
	{
	public:
		explicit vie_view(host_callback* host_callback);

		virtual ~vie_view() = default;

		Steinberg::tresult __stdcall queryInterface(const Steinberg::TUID iid, void** obj) override;
		Steinberg::uint32 __stdcall addRef() override;
		Steinberg::uint32 __stdcall release() override;
		Steinberg::tresult __stdcall isPlatformTypeSupported(Steinberg::FIDString type) override;
		Steinberg::tresult __stdcall attached(void* parent, Steinberg::FIDString type) override;
		Steinberg::tresult __stdcall removed() override;
		Steinberg::tresult __stdcall onWheel(float distance) override;
		Steinberg::tresult __stdcall onKeyDown(Steinberg::char16 key, Steinberg::int16 key_code, Steinberg::int16 modifiers) override;
		Steinberg::tresult __stdcall onKeyUp(Steinberg::char16 key, Steinberg::int16 key_code, Steinberg::int16 modifiers) override;
		Steinberg::tresult __stdcall getSize(Steinberg::ViewRect * size) override;
		Steinberg::tresult __stdcall onSize(Steinberg::ViewRect * new_size) override;
		Steinberg::tresult __stdcall onFocus(Steinberg::TBool state) override;
		Steinberg::tresult __stdcall setFrame(Steinberg::IPlugFrame * frame) override;
		Steinberg::tresult __stdcall canResize() override;
		Steinberg::tresult __stdcall checkSizeConstraint(Steinberg::ViewRect* rect) override;

		Steinberg::tresult set_state(Steinberg::IBStream* state);
		Steinberg::tresult get_state(Steinberg::IBStream* state);

		void set_host_callback(host_callback* callback);

		nlohmann::json serialize();

		void load(nlohmann::json instrument);
		
		void add_module(nlohmann::json module);

		void render();

	private:
		void parse_modules(nlohmann::json modules_definitions);
		void parse_module(nlohmann::json module_definition);
		void parse_relations(nlohmann::json relations_definitions);
		void parse_relation(nlohmann::json relation_definition);
		void parse_ui(nlohmann::json ui_definition);
		
		Steinberg::uint32 nb_ref_;
		Steinberg::IPlugFrame* ptr_frame_;

		editor_view editor_view;

		int width_;
		int height_;
	};
}
