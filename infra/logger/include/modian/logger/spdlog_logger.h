#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "modian/core/logger/base_logger.h"

namespace modian::logger {
	class spdlog_logger final : public core::base_logger {
	public:
		spdlog_logger();
		~spdlog_logger() override = default;
		void info(const std::string& message) override;
		void debug(const std::string& message) override;
		void error(const std::string& message) override;
		std::string type{"spdlog_logger"};

	private:
        std::shared_ptr<spdlog::logger> logger_;
	};
}
