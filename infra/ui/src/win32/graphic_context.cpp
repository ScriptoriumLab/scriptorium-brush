#include "modian/ui/win32/graphic_context.h"

namespace modian::infra::ui {
	win32::win32_graphic_context::win32_graphic_context(const HWND& hwnd) : hwnd_{hwnd} {}
}
