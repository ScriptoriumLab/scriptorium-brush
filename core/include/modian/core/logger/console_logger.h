#pragma once

#include "modian/core/logger/base_logger.h"

namespace modian::core {
	class console_logger final : public base_logger {
	public:
		~console_logger() override = default;

        void info(const std::string& message) override;
        void debug(const std::string& message) override;
        void error(const std::string& message) override;
		std::string type{"console_logger"};
	};
}