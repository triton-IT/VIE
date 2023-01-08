#include "vie_view.hpp"

#include "application.hpp"

#include <wrl.h>

using namespace Steinberg;
using namespace Vst;

namespace live::tritone::vie
{
	vie_view::vie_view(std::vector<parameter*>& parameters) :
		parameters_(parameters),
		nb_ref_(0),
		frame_(nullptr),
		width_(1024),
		height_(600),
		web_view_controller(nullptr),
		web_view_window(nullptr)
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
		HWND hWnd = static_cast<HWND>(parent);

		auto web_view_controller_handle = &web_view_controller;
		auto web_view_window_handle = &web_view_window;

		CreateCoreWebView2EnvironmentWithOptions(nullptr, L"c:/tmp/", nullptr,
			Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
				[hWnd, web_view_controller_handle, web_view_window_handle](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

					// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
					env->CreateCoreWebView2Controller(hWnd, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
						[hWnd, web_view_controller_handle, web_view_window_handle](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
							if (controller != nullptr) {
								*web_view_controller_handle = controller;
								controller->AddRef();
								(*web_view_controller_handle)->get_CoreWebView2(web_view_window_handle);
								(*web_view_window_handle)->AddRef();

								// Resize WebView to fit the bounds of the parent window
								RECT bounds;
								GetClientRect(hWnd, &bounds);
								(*web_view_controller_handle)->put_Bounds(bounds);

								// Schedule an async task to navigate to Bing
								(*web_view_window_handle)->Navigate(L"https://www.bing.com/");
							}

							return S_OK;
						}).Get());
					return S_OK;
				}).Get());

		return kResultTrue;
	}

	tresult __stdcall vie_view::removed()
	{
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
