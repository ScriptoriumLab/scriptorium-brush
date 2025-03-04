#pragma once

#include "modian/ui/core/renderer/candidate_renderer.h"

namespace modian::infra::ui::win32::renderer {
	class candidate_renderer final : public core::renderer::candidate_renderer {
	public:
		void begin_frame(const HDC& hdc) override;
		void draw_list(const std::vector<std::wstring>& items) override;
		void end_frame() override;
	private:
		HWND hwnd_{};
		HDC hdc_{};
		HFONT font_{};
	};
}
