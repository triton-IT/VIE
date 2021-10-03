#include "VieView.h"
#include "BasicThumbModel.hpp"
#include "BasicTrackModel.hpp"
#include "ColorModel.hpp"
#include "AlignModel.hpp"
#include "MinSizeModel.hpp"
#include "StretchModel.hpp"
#include "TileModel.hpp"
#include "MarginModel.h"
#include "PaneModel.hpp"
#include "SliderModel.hpp"
#include "SliderLabelsModel.hpp"
#include "SliderMarksModel.hpp"
#include "MinSizeModel.hpp"

#include "Application.h"

#include <fstream>

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace cycfi::elements;
using namespace live::tritone::vie::ui::model;

namespace live::tritone::vie {
	VieView::VieView(FrequencyParameter* frequencyParameter) :
		nbRef_(0),
		frame_(nullptr),
		width_(1024),
		height_(600),
		frequencyParameter_(frequencyParameter),
		view_(nullptr),
		parent_(nullptr)
	{
	}

	VieView::~VieView()
	{
	}

	auto VieView::deserialise() {
		DLOG("Starting deserialise.");

		std::ifstream uiJsonFile(contentPath + "ui.json");
		nlohmann::json uiJson;
		uiJsonFile >> uiJson;

		deserialiseUI(uiJson);
	}

	void VieView::deserialiseUI(nlohmann::json& json)
	{
		auto root = json["root"];
		auto type = root["type"].get<std::string>();

		color bkd_color = rgba(255, 0, 0, 255);
		auto background = fixed_size(
			{ 200, 200 }, box(bkd_color));

		if (type.compare("margin") == 0) {
			auto rootElement = MarginModel::deserialize(root);

			view_->content(
				rootElement,
				background
			);
		}
	}

	tresult PLUGIN_API VieView::queryInterface(const TUID iid, void** obj)
	{
		if (::Steinberg::FUnknownPrivate::iidEqual(iid, ::FUnknown::iid)) {
			addRef();
			*obj = static_cast<VieView*>(this);
			return ::Steinberg::kResultOk;
		}

		return kResultFalse;
	}

	uint32 PLUGIN_API VieView::addRef()
	{
		return ++nbRef_;
	}

	uint32 PLUGIN_API VieView::release()
	{
		return --nbRef_;
	}

	tresult PLUGIN_API VieView::isPlatformTypeSupported(FIDString type)
	{
		if (strcmp(type, "HWND") == 0) {
			return kResultTrue;
		}

		return kResultFalse;
	}

	tresult PLUGIN_API VieView::attached(void* parent, FIDString /*type*/)
	{
		parent_ = parent;

#ifdef WIN32
		view_ = new view((HWND) parent);
#elif
		view_ = new view(parent);
#endif // WIN32

		deserialise();

		return kResultTrue;
	}

	tresult PLUGIN_API VieView::removed()
	{
		if (view_ != nullptr) {
			delete view_;
			view_ = nullptr;
		}

		return kResultTrue;
	}

	tresult PLUGIN_API VieView::onWheel(float /*distance*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::onKeyDown(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::onKeyUp(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::getSize(ViewRect* size)
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

	tresult PLUGIN_API VieView::onSize(ViewRect* newSize)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::onFocus(TBool /*state*/)
	{
		return kResultOk;
	}

	tresult PLUGIN_API VieView::setFrame(IPlugFrame* frame)
	{
		frame_ = frame;

		return kResultOk;
	}

	tresult PLUGIN_API VieView::canResize()
	{
		return kResultTrue;
	}
	
	tresult PLUGIN_API VieView::checkSizeConstraint(ViewRect* /*rect*/)
	{
		return kResultOk;
	}
} // namespace
