#include "modian/ui/win32/renderer/candidate_renderer.h"

namespace modian::infra::ui::win32::renderer {
	void win32_candidate_renderer::render(HWND hwnd, const std::vector<std::wstring>& candidates) {
		PAINTSTRUCT ps;
		const HDC hdc = BeginPaint(hwnd, &ps);
		// 简单绘制，每行显示一个候选词
		int y = 10;
		for (const auto& candidate : candidates) {
			TextOutW(hdc, 10, y, candidate.c_str(), static_cast<int>(candidate.size()));
			y += 20;
		}
		EndPaint(hwnd, &ps);
	}
}