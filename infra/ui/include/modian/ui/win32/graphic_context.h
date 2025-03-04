#pragma once

#include <Windows.h>

#include "modian/ui/core/graphic_context.h"

namespace modian::infra::ui::win32 {
	class win32_graphic_context final : public graphic_context {
	public:
		explicit win32_graphic_context(const HWND& hwnd);

		HWND hwnd_;
	};
}