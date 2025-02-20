#include <utility>

#include "modian/core/logger/logger_service.h"

#include "modian/core/logger/console_logger.h"

namespace modian::core {
	std::shared_ptr<base_logger> logger_service::instance = std::make_shared<console_logger>();
	std::mutex logger_service::mutex_;

	void logger_service::update_logger(std::shared_ptr<base_logger> new_logger) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (instance->type == "console_logger" || instance->type == "base_logger") {
			instance = std::move(new_logger);
		}
	}

	std::shared_ptr<base_logger> logger_service::logger() {
		std::lock_guard<std::mutex> lock(mutex_);
		return instance;
	}
}
