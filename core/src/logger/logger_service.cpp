#include "modian/core/logger/logger_service.h"

#include <utility>
#include <Windows.h>

#include "modian/core/logger/console_logger.h"

namespace modian::core {
	std::shared_ptr<base_logger> logger_service::instance = std::make_shared<console_logger>();
	std::mutex logger_service::mutex_;
	volatile long update_logger_times{0};

	void logger_service::update_logger(std::shared_ptr<base_logger> new_logger) {
		if (update_logger_times == 0) {
			std::lock_guard lock(mutex_);
			if (instance->type == "console_logger" || instance->type == "base_logger") {
				instance = std::move(new_logger);
			}
			InterlockedIncrement(&update_logger_times);
		}
	}

	std::shared_ptr<base_logger> logger_service::logger() {
		std::lock_guard lock(mutex_);
		return instance;
	}
}
