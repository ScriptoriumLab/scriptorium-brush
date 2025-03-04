#include "modian/ui/win32/renderer/candidate_renderer.h"

namespace modian::infra::ui::win32::renderer {
	void candidate_renderer::begin_frame(const HDC& hdc) {
		hdc_ = hdc;
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
		SelectObject(hdc_, font_);
	}

	void candidate_renderer::draw_list(const std::vector<std::wstring>& items) {
		for (size_t i = 0; i < items.size(); ++i) {
			TextOutW(hdc_, 10, 20 * i, items[i].c_str(), items[i].length());
		}
	}

	void candidate_renderer::end_frame() {
		DeleteObject(font_);
	}
}
