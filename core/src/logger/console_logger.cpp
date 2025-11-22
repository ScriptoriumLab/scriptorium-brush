#include "modian/core/logger/console_logger.h"

#include <codecvt>
#include <iostream>

namespace modian::brush::core {
	void console_logger::debug(const std::string& message) {
		std::cout << "[debug] " << message << std::endl;
	}

	void console_logger::error(const std::string& message) {
		std::cout << "[error] " << message << std::endl;
	}

	void console_logger::info_impl(const std::string& message) {
		std::cout << "[info] " << message << std::endl;
	}

	void console_logger::info_impl(const std::string& message, const std::string& arg) {
		std::cout << "[info] " << message << ", " << arg << std::endl;
	}

	void console_logger::info_impl(const std::string& message, const std::wstring& arg) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string arg_str = converter.to_bytes(arg);

		std::cout << "[info] " << message << ", " << arg_str << std::endl;
	}

	void console_logger::info_impl(const std::string& message, const int& arg) {
		std::cout << "[info] " << message << ", " << arg << std::endl;
	}
}
