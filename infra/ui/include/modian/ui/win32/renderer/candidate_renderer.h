#pragma once

#include "modian/ui/core/renderer/candidate_renderer.h"

namespace modian::infra::ui::win32::renderer {
	class win32_candidate_renderer final : public core::renderer::candidate_renderer {
	public:
		void render(HWND hwnd, const std::vector<std::wstring>& candidates) override;
	};
}
