#pragma once

#include <Windows.h>

#include "modian/ui/core/renderer/candidate_renderer.h"

namespace modian::infra::ui::win32::renderer {
	class candidate_renderer final : public core::renderer::candidate_renderer {
	public:
		void set_context(const std::shared_ptr<graphic_context>& context) override;
		void begin_frame() override;
		void draw_list(const std::vector<std::wstring>& items) override;
		void end_frame() override;
	private:
		HWND hwnd_{nullptr};
		HWND listbox_{nullptr};
		HDC hdc_{};
		HFONT font_{};
	};
}
