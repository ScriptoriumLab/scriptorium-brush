#pragma once

#include "modian/core/logger/logger.h"

namespace modian::core {
	class console_logger final : public logger {
		~console_logger() override = default;

        void info(const std::string& message) override;
        void debug(const std::string& message) override;
        void error(const std::string& message) override;
	};
}