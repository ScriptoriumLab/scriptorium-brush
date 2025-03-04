#pragma once

#include <Windows.h>
#include <vector>
#include <string>

namespace modian::infra::ui::core::renderer {
	class candidate_renderer {
	public:
		virtual ~candidate_renderer() = default;

		virtual void begin_frame(const HDC& hdc) = 0;
		virtual void draw_list(const std::vector<std::wstring>& items) = 0;
		virtual void end_frame() = 0;
	};
}