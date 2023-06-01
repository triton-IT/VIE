#pragma once

#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>

#include <thread>
#include <string>
#include <vector>

#include "parameter.hpp"

#ifdef _WIN32
#include "windows/editor_view.hpp"
#else
#include "ios/editor_view.hpp"
#define __stdcall
#endif

namespace live::tritone::vie
{
	class vie_view final : public Steinberg::IPlugView
	{
	public:
		explicit vie_view(Steinberg::Vst::IComponentHandler* handler);

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

		void set_module_handler(Steinberg::Vst::IComponentHandler* handler);

		void render();

	private:
		Steinberg::uint32 nb_ref_;
		Steinberg::IPlugFrame* ptr_frame_;

		editor_view editor_view;

		int width_;
		int height_;

		Steinberg::Vst::IComponentHandler* handler_;
	};
}
