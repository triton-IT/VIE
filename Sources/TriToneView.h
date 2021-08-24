#pragma once

#include <pluginterfaces\gui\iplugview.h>
#include <elements.hpp>

#include "Third-parties/json.hpp"

#include "FrequencyParameter.h"

namespace tech::tritonit::tritone {
	class TriToneView : public Steinberg::IPlugView
	{

	public:
		TriToneView(FrequencyParameter* frequencyParameter);
		~TriToneView();

		Steinberg::tresult PLUGIN_API queryInterface(const Steinberg::TUID iid, void** obj) SMTG_OVERRIDE;
		virtual Steinberg::uint32 PLUGIN_API addRef() SMTG_OVERRIDE;
		virtual Steinberg::uint32 PLUGIN_API release() SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API attached(void* parent, Steinberg::FIDString type) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API removed() SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onWheel(float distance) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onKeyDown(Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onKeyUp(Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API getSize(Steinberg::ViewRect* size) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onSize(Steinberg::ViewRect* newSize) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API onFocus(Steinberg::TBool state) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API setFrame(Steinberg::IPlugFrame* frame) SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API canResize() SMTG_OVERRIDE;
		virtual Steinberg::tresult PLUGIN_API checkSizeConstraint(Steinberg::ViewRect* rect) SMTG_OVERRIDE;

	private:
		Steinberg::uint32 nbRef_;
		Steinberg::IPlugFrame* frame_;
		cycfi::elements::view* view_;
		void* parent_;

		FrequencyParameter* frequencyParameter_;

		int width_;
		int height_;

		using dial_ptr = std::shared_ptr<cycfi::elements::dial_base>;
		dial_ptr dials[3];

		using slider_ptr = std::shared_ptr<cycfi::elements::basic_slider_base>;
		slider_ptr hsliders[3];
		slider_ptr vsliders[3];

		std::shared_ptr<cycfi::elements::element> rootElement;

		template <bool is_vertical>
		auto make_markers();
		auto make_vslider(int index);
		auto make_controls();

		auto deserialisePane(nlohmann::json &paneJson);
		auto deserialiseMargin(nlohmann::json &marginJson);
	};
}
