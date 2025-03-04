#pragma once

#include <vector>
#include <string>
#include <memory>

#include "modian/ui/core/graphic_context.h"

namespace modian::infra::ui::core::renderer {
	class candidate_renderer {
	public:
		virtual ~candidate_renderer() = default;

		virtual void set_context(const std::shared_ptr<graphic_context>& context) = 0;
		virtual void begin_frame() = 0;
		virtual void draw_list(const std::vector<std::wstring>& items) = 0;
		virtual void end_frame() = 0;
	};
}