#pragma once

#include <memory>

#include "modian/ui/core/renderer/candidate_renderer.h"

namespace modian::infra::ui::win32 {
	class candidate_window {
	public:
		candidate_window(HINSTANCE h_instance, const std::shared_ptr<core::renderer::candidate_renderer>& renderer);
		~candidate_window();

		// 创建并显示窗口
		bool create();
		void show() const;
		void hide() const;

		// 更新候选词并重绘
		void update_candidates(const std::vector<std::wstring>& candidates);
	private:
		static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT handle_message(UINT message, WPARAM wParam, LPARAM lParam);

		HINSTANCE h_instance_;
		HWND hwnd_;
		HDC hdc_;
		std::vector<std::wstring> candidates_;
		std::shared_ptr<core::renderer::candidate_renderer> renderer_{};
	};
}
