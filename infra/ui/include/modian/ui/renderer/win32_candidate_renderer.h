#pragma once

#include "modian/ui/renderer/candidate_renderer.h"

namespace modian::infra::ui::renderer {
	class win32_candidate_renderer final : public candidate_renderer {
	public:
		void render(HWND hwnd, const std::vector<std::wstring>& candidates) override;
	};
}
