#pragma once

#include <string>

namespace modian::brush::core {
	class base_logger {
	public:
		virtual ~base_logger() = default;
        virtual void debug(const std::string& message) = 0;
        virtual void error(const std::string& message) = 0;

		template<typename... Args>
        void info(const std::string& message, Args... args) {
			info_impl(message, args...);
		}

		std::string type{"base_logger"};

	protected:
		virtual void info_impl(const std::string& message) = 0;
		virtual void info_impl(const std::string& message, const std::string& arg) = 0;
		virtual void info_impl(const std::string& message, const std::wstring& arg) = 0;
		virtual void info_impl(const std::string& message, const int& arg) = 0;
	};
}