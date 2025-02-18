#pragma once

#include <string>

namespace modian::core {
	class i_logger {
	public:
		virtual ~i_logger() = default;
        virtual void info(const std::string& message) = 0;
        virtual void debug(const std::string& message) = 0;
        virtual void error(const std::string& message) = 0;
	};
}