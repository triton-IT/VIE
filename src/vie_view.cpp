#include "vie_view.hpp"

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie
{
	vie_view::vie_view(std::vector<parameter*>& parameters, Steinberg::Vst::IComponentHandler* handler) :
		parameters_(parameters),
		nb_ref_(0),
		ptr_frame_(nullptr),
		width_(1024),
		height_(600),
		handler_(handler)
	{
		editor_view.set_component_handler(handler);
	}

	tresult __stdcall vie_view::queryInterface(const TUID iid, void** obj)
	{
		if (FUnknownPrivate::iidEqual(iid, FUnknown::iid))
		{
			addRef();
			*obj = this;
			return kResultOk;
		}

		return kResultFalse;
	}

	uint32 __stdcall

	vie_view::addRef()
	{
		return ++nb_ref_;
	}

	uint32 __stdcall

	vie_view::release()
	{
		//FIXME: Try to delete this if 0.
		return --nb_ref_;
	}

	tresult __stdcall vie_view::isPlatformTypeSupported(FIDString type)
	{
		if (
			strcmp(type, "HWND") == 0)
		{
			return kResultTrue;
		}

		return kResultFalse;
	}

	tresult __stdcall vie_view::attached(void* parent, FIDString /*type*/)
	{
		editor_view.attached(parent);

		return kResultTrue;
	}

	tresult __stdcall vie_view::removed()
	{
		editor_view.removed();

		return kResultTrue;
	}

	tresult __stdcall vie_view::onWheel(float /*distance*/)
	{
		return kResultOk;
	}

	tresult __stdcall vie_view::onKeyDown(char16
	                    key, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult __stdcall vie_view::onKeyUp(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult __stdcall vie_view::getSize(ViewRect* size)
	{
		size->left = 0;
		size->top = 0;
		size->right = width_;
		size->bottom = height_;

		return kResultOk;
	}

	tresult __stdcall vie_view::onSize(ViewRect* new_size)
	{
		return kResultOk;
	}

	tresult __stdcall vie_view::onFocus(TBool /*state*/)
	{
		return kResultOk;
	}

	tresult __stdcall vie_view::setFrame(IPlugFrame* frame)
	{
		ptr_frame_ = frame;

		return kResultOk;
	}

	tresult __stdcall vie_view::canResize()
	{
		return kResultTrue;
	}

	tresult __stdcall vie_view::checkSizeConstraint(ViewRect* /*rect*/)
	{
		return kResultOk;
	}

	tresult vie_view::set_state(IBStream* state)
	{
		return kResultTrue;
	}

	tresult vie_view::get_state(IBStream* state)
	{
		return kResultTrue;
	}

	void vie_view::set_component_handler(Steinberg::Vst::IComponentHandler* handler)
	{
		handler_ = handler;
	}

	void vie_view::render()
	{
	}
} // namespace
