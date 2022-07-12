#pragma once

#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/base/ibstream.h>

#include <thread>
#include <string>
#include <vector>

#include "parameter.hpp"

#ifdef _WIN32

#include "windows/editor_view.hpp"
#include "WebView2.h"

#endif

namespace live::tritone::vie
{
	class vie_view final : public Steinberg::IPlugView
	{
	public:
		explicit vie_view(std::vector<parameter*>& parameters);

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

		void render();

	private:
		std::vector<parameter*>& parameters_;

		Steinberg::uint32 nb_ref_;
		Steinberg::IPlugFrame* frame_;

		int width_;
		int height_;

		ICoreWebView2Controller* web_view_controller;
		ICoreWebView2* web_view_window;
	};
}
