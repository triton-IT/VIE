#pragma once

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE


#include <Windows.h>

namespace live::tritone::vie {
	class editor_view {
	public:
		explicit editor_view(void* parent);

		editor_view(const editor_view&) = default;

		editor_view(editor_view&&) = default;

		editor_view& operator=(const editor_view&) = default;

		editor_view& operator=(editor_view&&) = default;

		~editor_view();

		[[nodiscard]] void* get_handle() const;

	private:
		static LONG_PTR WINAPI message_proc(HWND window_handle, UINT message, WPARAM wide_parameter, LPARAM low_parameter);

		static LONG_PTR WINAPI handle_win32_message(HWND window_handle, UINT message, WPARAM wide_param, LPARAM low_param);

		HWND h_wnd_;
	};
}
