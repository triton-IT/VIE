#include "TriToneView.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace cycfi::elements;

namespace tech::tritonit::tritone {
	TriToneView::TriToneView(FrequencyParameter* frequencyParameter) :
		nbRef_(0),
		frame_(nullptr),
		width_(1024),
		height_(600),
		frequencyParameter_(frequencyParameter),
		view_(nullptr),
		parent_(nullptr)
	{
	}

	TriToneView::~TriToneView()
	{
	}

	template <bool is_vertical>
	auto TriToneView::make_markers()
	{
		auto track = basic_track<5, is_vertical>();
		return slider_labels<10>(
			slider_marks<40>(track),         // Track with marks
			0.8,                             // Label font size (relative size)
			"0", "1", "2", "3", "4",         // Labels
			"5", "6", "7", "8", "9", "10"
			);
	}

	auto TriToneView::make_vslider(int index)
	{
		vsliders[index] = share(slider(
			basic_thumb<25>(),
			make_markers<true>(),
			(index + 1) * 0.25
		));
		vmargin_rect margin_offset{ 20, 20 };

		vsliders[index]->on_change =
			[index, this](double val)
		{
			frequencyParameter_->setNormalized(val);
		};

		return align_center(vmargin(margin_offset, hold(vsliders[index])));
	}

	auto TriToneView::deserialisePane(nlohmann::json& paneJson) {
		return pane("Vertical Sliders", make_vslider(0), 0.8f);
	}

	auto TriToneView::deserialiseMargin(nlohmann::json& marginJson) {
		float left = marginJson.find("left").value().get<float>();
		float top = marginJson.find("top").value().get<float>();
		float right = marginJson.find("right").value().get<float>();
		float bottom = marginJson.find("bottom").value().get<float>();

		//auto child = marginJson.find("child").value();
		//auto pane = deserialisePane(child.find("pane").value());

		//return margin({ left, top, right, bottom }, pane);
		auto p = pane("Frequency", make_vslider(0), 0.8f);
		return margin({ left, top, right, bottom },
			p
		);
	}

	auto TriToneView::make_controls()
	{
		auto viewDescription = R"(
		{
			"margin": {
				"top": 20,
				"left": 20,
				"bottom": 20,
				"right": 20,
				"child": {
					"pane": {
						"title": "Frequency",
						"child": {
							"vslider": {
							}
						}
					}
				}
			}
		}
		)"_json;

		for (auto& [key, value] : viewDescription.items()) {
			return deserialiseMargin(value);
		}
	}

	tresult PLUGIN_API TriToneView::queryInterface(const TUID iid, void** obj)
	{
		if (::Steinberg::FUnknownPrivate::iidEqual(iid, ::FUnknown::iid)) {
			addRef();
			*obj = static_cast<TriToneView*>(this);
			return ::Steinberg::kResultOk;
		}

		return kResultFalse;
	}

	uint32 PLUGIN_API TriToneView::addRef()
	{
		return ++nbRef_;
	}

	uint32 PLUGIN_API TriToneView::release()
	{
		return --nbRef_;
	}

	tresult PLUGIN_API TriToneView::isPlatformTypeSupported(FIDString type)
	{
		if (strcmp(type, "HWND") == 0) {
			return kResultTrue;
		}

		return kResultFalse;
	}

	tresult PLUGIN_API TriToneView::attached(void* parent, FIDString /*type*/)
	{
		parent_ = parent;

#ifdef WIN32
		view_ = new view((HWND) parent);
#elif
		view_ = new view(parent);
#endif // WIN32

		color bkd_color = rgba(0, 0, 0, 255);
		auto background = fixed_size(
			{ 200, 200 }, box(bkd_color));
		view_->content(
			make_controls(),
			background
		);

		return kResultTrue;
	}

	tresult PLUGIN_API TriToneView::removed()
	{
		if (view_ != nullptr) {
			delete view_;
			view_ = nullptr;
		}

		return kResultTrue;
	}

	tresult PLUGIN_API TriToneView::onWheel(float /*distance*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::onKeyDown(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::onKeyUp(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::getSize(ViewRect* size)
	{
		size->left = 0;
		size->top = 0;
		size->right = width_;
		size->bottom = height_;

		RECT bounds;
		GetClientRect((HWND)parent_, &bounds);

		MoveWindow(
			view_->host(),
			0, 0,
			bounds.right,
			bounds.bottom,
			TRUE);

		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::onSize(ViewRect* newSize)
	{
		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::onFocus(TBool /*state*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::setFrame(IPlugFrame* frame)
	{
		frame_ = frame;

		return kResultOk;
	}

	tresult PLUGIN_API TriToneView::canResize()
	{
		return kResultTrue;
	}
	
	tresult PLUGIN_API TriToneView::checkSizeConstraint(ViewRect* /*rect*/)
	{
		return kResultOk;
	}
} // namespace
