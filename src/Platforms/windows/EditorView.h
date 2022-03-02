#pragma once

#define UNICODE
#include <windows.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

namespace live::tritone::vie {
	class EditorView
	{
	public:
		EditorView(void* parent);
		~EditorView();

		void setNuklearContext(nk_context* nuklearContext);
		void* getHandle();

	private:
		static LONG_PTR WINAPI MessageProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LONG_PTR WINAPI HandleWin32Message(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

		HWND hWnd;
		nk_context* m_pNkCtx;
	};
}
