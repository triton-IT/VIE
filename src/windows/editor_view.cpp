#include <sstream>

#include "editor_view.hpp"

namespace live::tritone::vie
{
	editor_view::editor_view(void* parent)
	{
		const auto title = L"Virtual Instrument Engine";

		const WNDCLASSEX window_class_ex = {
			sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, message_proc,
			0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"VIE", nullptr
		};
		RegisterClassEx(&window_class_ex);

		RECT window_rect = {0, 0, 1024, 768};

		AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);
		h_wnd_ = CreateWindow(L"VIE", title,
		                      WS_CHILD | WS_VISIBLE,
		                      0, 0,
		                      window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		                      static_cast<HWND>(parent), NULL, GetModuleHandle(NULL), NULL);

		if (!h_wnd_)
		{
			MessageBox(nullptr, L"Cannot create window", L"Error", MB_OK | MB_ICONERROR);
			return;
		}

		SetWindowLongPtr(h_wnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(h_wnd_, 1);
	}

	editor_view::~editor_view()
	{
		if (h_wnd_)
		{
			DestroyWindow(h_wnd_);
			h_wnd_ = nullptr;
		}
		UnregisterClass(L"VIE", GetModuleHandle(nullptr));
	}

	LRESULT CALLBACK editor_view::message_proc(HWND window_handle,
	                                           const UINT message,
	                                           const WPARAM wide_parameter,
	                                           const LPARAM low_parameter
	)
	{
		handle_win32_message(window_handle, message, wide_parameter, low_parameter);

		return DefWindowProc(window_handle, message, wide_parameter, low_parameter);
	}

	LONG_PTR WINAPI editor_view::handle_win32_message(HWND
	                                                  window_handle,
	                                                  const UINT message, const WPARAM
	                                                  wide_param,
	                                                  const LPARAM low_param
	)
	{
		int result = 0;

		switch (message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			{
				const int ctrl = GetKeyState(VK_CONTROL) & (1 << 15);

				switch (wide_param)
				{
				case VK_SHIFT:
				case VK_LSHIFT:
				case VK_RSHIFT:
				case VK_DELETE:
				case VK_RETURN:
				case VK_TAB:
				case VK_LEFT:
				case VK_RIGHT:
				case VK_BACK:
				case VK_HOME:
				case VK_END:
				case VK_NEXT:
				case VK_PRIOR:
					result = 1;
					break;
				case 'C':
				case 'V':
				case 'X':
				case 'Z':
				case 'Y':
					if (ctrl)
					{
						result = 1;
					}
					break;
				default:
					break;
				}
			}
			break;
		case WM_ERASEBKGND:
		case WM_PAINT:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDBLCLK:
			result = 1;
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetCapture(window_handle);
			result = 1;
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			ReleaseCapture();
			result = 1;
			break;
		case WM_CHAR:
			if (wide_param >= 32)
			{
				result = 1;
			}
			break;
		default:
			result = 1;
			break;
		}

		return
			result;
	}

	void* editor_view::get_handle() const
	{
		return h_wnd_;
	}
} // namespace
