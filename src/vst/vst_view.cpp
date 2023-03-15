#include "vst_view.hpp"

using namespace Steinberg;
using namespace Vst;

using namespace nlohmann;

namespace live::tritone::vie::vst
{
	vst_view::vst_view(host_callback* host_callback) :
		nb_ref_(0),
		ptr_frame_(nullptr),
		width_(1024),
		height_(600)
	{
		editor_view_.set_host_callback(host_callback);
	}

	tresult __stdcall vst_view::queryInterface(const TUID iid, void** obj)
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

	vst_view::addRef()
	{
		return ++nb_ref_;
	}

	uint32 __stdcall

	vst_view::release()
	{
		//FIXME: Try to delete this if 0.
		return --nb_ref_;
	}

	tresult __stdcall vst_view::isPlatformTypeSupported(FIDString type)
	{
		if (
			strcmp(type, "HWND") == 0)
		{
			return kResultTrue;
		}

		return kResultFalse;
	}

	tresult __stdcall vst_view::attached(void* parent, FIDString /*type*/)
	{
		editor_view_.attached(parent);

		return kResultTrue;
	}

	tresult __stdcall vst_view::removed()
	{
		editor_view_.removed();

		return kResultTrue;
	}

	tresult __stdcall vst_view::onWheel(float /*distance*/)
	{
		return kResultOk;
	}

	tresult __stdcall vst_view::onKeyDown(char16
	                    key, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult __stdcall vst_view::onKeyUp(char16 /*key*/, int16 /*keyCode*/, int16 /*modifiers*/)
	{
		return kResultOk;
	}

	tresult __stdcall vst_view::getSize(ViewRect* size)
	{
		size->left = 0;
		size->top = 0;
		size->right = width_;
		size->bottom = height_;

		return kResultOk;
	}

	tresult __stdcall vst_view::onSize(ViewRect* new_size)
	{
		return kResultOk;
	}

	tresult __stdcall vst_view::onFocus(TBool /*state*/)
	{
		return kResultOk;
	}

	tresult __stdcall vst_view::setFrame(IPlugFrame* frame)
	{
		ptr_frame_ = frame;

		return kResultOk;
	}

	tresult __stdcall vst_view::canResize()
	{
		return kResultTrue;
	}

	tresult __stdcall vst_view::checkSizeConstraint(ViewRect* /*rect*/)
	{
		return kResultOk;
	}

	tresult vst_view::set_state(IBStream* state)
	{
		return kResultTrue;
	}

	tresult vst_view::get_state(IBStream* state)
	{
		return kResultTrue;
	}

	void vst_view::set_host_callback(host_callback* callback)
	{
		editor_view_.set_host_callback(callback);
	}
	
	void vst_view::initialize()
	{
		editor_view_.initialize();
	}

	nlohmann::json vst_view::serialize()
	{		
		return editor_view_.serialize();
	}

	void vst_view::deserialize(nlohmann::json definition)
	{
		editor_view_.deserialize(definition);
	}
	
	void vst_view::add_module(nlohmann::json module)
	{
		editor_view_.add_module(module);
	}

	void vst_view::delete_module(int id)
	{
		editor_view_.delete_module(id);
	}

	void vst_view::clear()
	{
		editor_view_.clear();
	}

	void vst_view::render()
	{
	}
} // namespace
