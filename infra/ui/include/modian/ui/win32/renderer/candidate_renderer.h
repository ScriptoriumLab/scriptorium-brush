#pragma once

#include <Windows.h>
#include <vector>
#include <string>

namespace modian::infra::ui::renderer {
	class candidate_renderer {
	public:
		virtual ~candidate_renderer() = default;
		virtual void render(HWND hwnd, const std::vector<std::wstring>& candidates) = 0;
	};
}