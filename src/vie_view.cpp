#include "vie_view.hpp"

#include "application.hpp"

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie
{
	vie_view::vie_view(std::vector<parameter*>& parameters) :
		parameters_(parameters),
		m_p_editor_view_(nullptr),
		nb_ref_(0),
		frame_(nullptr),
		width_(1024),
		height_(600),
		is_renderer_running_(true)
	{
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
		m_p_editor_view_ = new editor_view(parent);

		auto view_renderer_thread = [](vie_view* view, const bool* is_running)
		{
			using ms = std::chrono::milliseconds;
			while (*is_running)
			{
				view->render();
				std::this_thread::sleep_for(ms(15));
			}
		};
		renderer_thread_.reset(new std::thread(view_renderer_thread, this, &is_renderer_running_));

		return kResultTrue;
	}

	tresult __stdcall vie_view::removed()
	{
		//Stop renderer loop.
		is_renderer_running_ = false;

		if (renderer_thread_)
		{
			renderer_thread_->join();
			renderer_thread_.reset();
			renderer_thread_ = nullptr;
		}

		delete m_p_editor_view_;
		m_p_editor_view_ = nullptr;

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
		frame_ = frame;

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

	void vie_view::render()
	{
	}
} // namespace
