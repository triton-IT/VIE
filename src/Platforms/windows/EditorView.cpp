#include <sstream>

#include "../Application.h"
#include "EditorView.h"

namespace live::tritone::vie {
	EditorView::EditorView(void* parent) : m_pNkCtx(nullptr)
	{
		std::wstringstream TitleSS;
		TitleSS << L"Virtual Instrument Engine";
		auto Title = TitleSS.str();

		WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, MessageProc,
						   0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"VIE", NULL };
		RegisterClassEx(&wcex);

		RECT WndRect = { 0, 0, 1024, 768 };

		AdjustWindowRect(&WndRect, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow(L"VIE", Title.c_str(),
			WS_CHILD | WS_VISIBLE,
			0, 0,
			WndRect.right - WndRect.left, WndRect.bottom - WndRect.top,
			(HWND)parent, NULL, GetModuleHandle(NULL), NULL);

		if (!hWnd)
		{
			MessageBox(NULL, L"Cannot create window", L"Error", MB_OK | MB_ICONERROR);
			return;
		}

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)this);

		ShowWindow(hWnd, 1);
	}

	EditorView::~EditorView()
	{
		if (hWnd) {
			DestroyWindow(hWnd);
			hWnd = nullptr;
		}
		UnregisterClass(L"VIE", GetModuleHandle(NULL));
	}

	void EditorView::setNuklearContext(nk_context* nuklearContext) {
		m_pNkCtx = nuklearContext;
	}

	LRESULT CALLBACK EditorView::MessageProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		EditorView* view = (EditorView*)(LONG_PTR)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (view) {
			view->HandleWin32Message(hwnd, msg, wparam, lparam);
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	LONG_PTR WINAPI EditorView::HandleWin32Message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		int result = 0;

		if (m_pNkCtx) {
			switch (msg)
			{
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			{
				int down = !((lparam >> 31) & 1);
				int ctrl = GetKeyState(VK_CONTROL) & (1 << 15);

				switch (wparam)
				{
				case VK_SHIFT:
				case VK_LSHIFT:
				case VK_RSHIFT:
					nk_input_key(m_pNkCtx, NK_KEY_SHIFT, down);
					result = 1;
					break;
				case VK_DELETE:
					nk_input_key(m_pNkCtx, NK_KEY_DEL, down);
					result = 1;
					break;
				case VK_RETURN:
					nk_input_key(m_pNkCtx, NK_KEY_ENTER, down);
					result = 1;
					break;
				case VK_TAB:
					nk_input_key(m_pNkCtx, NK_KEY_TAB, down);
					result = 1;
					break;
				case VK_LEFT:
					if (ctrl)
						nk_input_key(m_pNkCtx, NK_KEY_TEXT_WORD_LEFT, down);
					else
						nk_input_key(m_pNkCtx, NK_KEY_LEFT, down);
					result = 1;
					break;
				case VK_RIGHT:
					if (ctrl)
						nk_input_key(m_pNkCtx, NK_KEY_TEXT_WORD_RIGHT, down);
					else
						nk_input_key(m_pNkCtx, NK_KEY_RIGHT, down);
					result = 1;
					break;
				case VK_BACK:
					nk_input_key(m_pNkCtx, NK_KEY_BACKSPACE, down);
					result = 1;
					break;
				case VK_HOME:
					nk_input_key(m_pNkCtx, NK_KEY_TEXT_START, down);
					result = 1;
					break;
				case VK_END:
					nk_input_key(m_pNkCtx, NK_KEY_TEXT_END, down);
					result = 1;
					break;
				case VK_NEXT:
					nk_input_key(m_pNkCtx, NK_KEY_SCROLL_DOWN, down);
					result = 1;
					break;
				case VK_PRIOR:
					nk_input_key(m_pNkCtx, NK_KEY_SCROLL_UP, down);
					result = 1;
					break;
				case 'C':
					if (ctrl)
					{
						nk_input_key(m_pNkCtx, NK_KEY_COPY, down);
						result = 1;
					}
					break;
				case 'V':
					if (ctrl)
					{
						nk_input_key(m_pNkCtx, NK_KEY_PASTE, down);
						result = 1;
					}
					break;
				case 'X':
					if (ctrl)
					{
						nk_input_key(m_pNkCtx, NK_KEY_CUT, down);
						result = 1;
					}
					break;
				case 'Z':
					if (ctrl)
					{
						nk_input_key(m_pNkCtx, NK_KEY_TEXT_UNDO, down);
						result = 1;
					}
					break;
				case 'Y':
					if (ctrl)
					{
						nk_input_key(m_pNkCtx, NK_KEY_TEXT_REDO, down);
						result = 1;
					}
					break;
				default:
					break;
				}
			}
			break;
			case WM_ERASEBKGND:
				result = 1;
				break;
			case WM_PAINT:
				result = 1;
				break;
			case WM_MOUSEMOVE:
				nk_input_motion(m_pNkCtx, (short)LOWORD(lparam), (short)HIWORD(lparam));
				result = 1;
				break;
			case WM_LBUTTONDOWN:
				nk_input_button(m_pNkCtx, NK_BUTTON_LEFT, (short)LOWORD(lparam), (short)HIWORD(lparam), 1);
				SetCapture(hwnd);
				result = 1;
				break;
			case WM_LBUTTONUP:
				nk_input_button(m_pNkCtx, NK_BUTTON_LEFT, (short)LOWORD(lparam), (short)HIWORD(lparam), 0);
				ReleaseCapture();
				result = 1;
				break;
			case WM_MBUTTONDOWN:
				nk_input_button(m_pNkCtx, NK_BUTTON_MIDDLE, (short)LOWORD(lparam), (short)HIWORD(lparam), 1);
				SetCapture(hwnd);
				result = 1;
				break;
			case WM_MBUTTONUP:
				nk_input_button(m_pNkCtx, NK_BUTTON_MIDDLE, (short)LOWORD(lparam), (short)HIWORD(lparam), 0);
				ReleaseCapture();
				result = 1;
				break;
			case WM_RBUTTONDOWN:
				nk_input_button(m_pNkCtx, NK_BUTTON_RIGHT, (short)LOWORD(lparam), (short)HIWORD(lparam), 1);
				SetCapture(hwnd);
				result = 1;
				break;
			case WM_RBUTTONUP:
				nk_input_button(m_pNkCtx, NK_BUTTON_RIGHT, (short)LOWORD(lparam), (short)HIWORD(lparam), 0);
				ReleaseCapture();
				result = 1;
				break;
			case WM_MOUSEWHEEL:
				nk_input_scroll(m_pNkCtx, nk_vec2(0, (float)(short)HIWORD(wparam) / WHEEL_DELTA));
				result = 1;
				break;
			case WM_LBUTTONDBLCLK:
				nk_input_button(m_pNkCtx, NK_BUTTON_DOUBLE, (short)LOWORD(lparam), (short)HIWORD(lparam), 1);
				result = 1;
				break;
			case WM_CHAR:
				if (wparam >= 32)
				{
					nk_input_unicode(m_pNkCtx, (nk_rune)wparam);
					result = 1;
				}
				break;
			}
		}

		return result;
	}

	void* EditorView::getHandle() {
		return hWnd;
	}
} // namespace
