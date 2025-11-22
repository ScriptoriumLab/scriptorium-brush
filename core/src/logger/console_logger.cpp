#include "modian/core/logger/console_logger.h"

#include <codecvt>
#include <iostream>

#include "modian/core/utils/utils.h"

namespace modian::brush::core {
	void console_logger::debug(std::string_view message) {
		std::cout << "[debug] " << message << "\n";
	}

	void console_logger::error(std::string_view message) {
		std::cout << "[error] " << message << "\n";
	}

	void console_logger::info_impl(std::string_view message) {
		std::cout << "[info] " << message << "\n";
	}

	void console_logger::info_impl(std::string_view message, std::string_view arg) {
		std::cout << "[info] " << message << ", " << arg << "\n";
	}

	void console_logger::info_impl(std::string_view message, std::wstring_view arg) {
		const auto arg_str {utils::to_utf8(arg)};
		std::cout << "[info] " << message << ", " << arg_str << "\n";
	}

	void console_logger::info_impl(const std::string_view message, const int& arg) {
		std::cout << "[info] " << message << ", " << arg << "\n";
	}
}
