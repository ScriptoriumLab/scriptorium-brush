#include "modian/ui/win32/renderer/candidate_renderer.h"

#include "modian/ui/win32/graphic_context.h"

namespace modian::infra::ui::win32::renderer {
	void candidate_renderer::set_context(const std::shared_ptr<graphic_context>& context) {
		hwnd_ = std::static_pointer_cast<win32_graphic_context>(context)->hwnd_;
		listbox_ = CreateWindowW(
			L"LISTBOX", L"",
			WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
			0, 0, 400, 150,
			hwnd_, nullptr,
			GetModuleHandle(nullptr), nullptr
		);
	}

	void candidate_renderer::begin_frame() {
		font_ = CreateFontW(
            20, 0, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            L"微软雅黑"  // 使用中文字体
        );
		SendMessageW(listbox_, WM_SETFONT, reinterpret_cast<WPARAM>(font_), TRUE);
	}

	void candidate_renderer::draw_list(const std::vector<std::wstring>& items) {
		SendMessage(listbox_, LB_RESETCONTENT, 0, 0);
		for (const auto& str : items) {
			SendMessage(listbox_, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.c_str()));
		}
		// 自动调整窗口大小
		int itemHeight = SendMessage(listbox_, LB_GETITEMHEIGHT, 0, 0);
		RECT rc = {0, 0, 400, min(10, static_cast<int>(items.size())) * itemHeight + 4};
		AdjustWindowRectEx(&rc,
						   static_cast<DWORD>(GetWindowLongW(hwnd_, GWL_STYLE)),
						   FALSE,
						   static_cast<DWORD>(GetWindowLongW(hwnd_, GWL_EXSTYLE)));
		SetWindowPos(hwnd_, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
					 SWP_NOZORDER | SWP_NOMOVE);

		// get window's size
		RECT clientRect;
		GetClientRect(hwnd_, &clientRect);

		// according to window's position, change the size of candidate list box
		SetWindowPos(listbox_, nullptr,
					 0, 0,
					 clientRect.right - clientRect.left,
					 clientRect.bottom - clientRect.top,
					 SWP_NOZORDER | SWP_NOMOVE);
	}

	void candidate_renderer::end_frame() {
		DeleteObject(font_);
	}
}
