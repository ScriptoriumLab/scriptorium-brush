#include "modian/core/logger/console_logger.h"

#include <iostream>

void modian::core::console_logger::debug(const std::string& message) {
	std::cout << "[debug] " << message << '\n';
}

void modian::core::console_logger::error(const std::string& message) {
	std::cout << "[error] " << message << '\n';
}

void modian::core::console_logger::info_impl(const std::string& message) {
	std::cout << "[info] " << message << '\n';
}
