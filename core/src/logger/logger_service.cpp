#define MODIAN_LOGGER_EXPORTS
#include "modian/core/logger/logger_service.h"

#include <Windows.h>

#include "modian/core/logger/console_logger.h"

namespace modian::brush::core {
	std::shared_ptr<base_logger> logger_service::instance = std::make_shared<console_logger>();
	std::mutex logger_service::mutex_;
	volatile long update_logger_times{0};

	std::shared_ptr<base_logger> logger_service::logger() {
		std::lock_guard lock(mutex_);
		return instance;
	}
}
