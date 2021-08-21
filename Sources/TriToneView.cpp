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

	auto TriToneView::make_hslider(int index)
	{
		hsliders[index] = share(slider(
			basic_thumb<25>(),
			make_markers<false>(),
			(index + 1) * 0.25
		));
		hmargin_rect margin_offset{ 20, 20 };
		return align_middle(hmargin(margin_offset, hold(hsliders[index])));
	}

	auto TriToneView::make_hsliders()
	{
		return hmin_size(300,
			vtile(
				make_hslider(0),
				make_hslider(1),
				make_hslider(2)
			)
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

	auto TriToneView::make_vsliders()
	{
		return hmin_size(300,
			htile(
				make_vslider(0),
				make_vslider(1),
				make_vslider(2)
			)
		);
	}

	auto TriToneView::make_dial(int index)
	{
		dials[index] = share(
			dial(
				radial_marks<20>(basic_knob<50>()),
				(index + 1) * 0.25
			)
		);

		auto markers = radial_labels<15>(
			hold(dials[index]),
			0.7,                                   // Label font size (relative size)
			"0", "1", "2", "3", "4",               // Labels
			"5", "6", "7", "8", "9", "1"
			);

		return align_center_middle(markers);
	}

	auto TriToneView::make_dials()
	{
		return hmargin(20,
			vtile(
				make_dial(0),
				make_dial(1),
				make_dial(2)
			)
		);
	}

	auto TriToneView::make_controls()
	{
		rect sliders_margin = { 20, 20, 20, 20 };

		return margin({ 20, 10, 20, 10 },
			vmin_size(400,
				htile(
					margin(sliders_margin, pane("Vertical Sliders", make_vsliders(), 0.8f)),
					margin(sliders_margin, pane("Horizontal Sliders", make_hsliders(), 0.8f)),
					hstretch(0.5, margin({ 20, 20, 20, 20 }, pane("Knobs", make_dials(), 0.8f)))
				)
			)
		);
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
