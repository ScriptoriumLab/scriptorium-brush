#pragma once

#include <string>
#include <string_view>

namespace modian::brush::core {
	class base_logger {
	public:
		virtual ~base_logger() = default;
        virtual void debug(std::string_view message) = 0;
        virtual void error(std::string_view message) = 0;

		[[nodiscard]] virtual std::string_view type() const { return "base_logger"; }

		void info(const std::string_view message) {
			info_impl(message);
		}

		template<typename... Args>
        void info(std::string_view message, Args... args) {
			info_impl(message, args...);
		}

	protected:
		virtual void info_impl(std::string_view message) = 0;
		virtual void info_impl(std::string_view message, std::string_view arg) = 0;
		virtual void info_impl(std::string_view message, std::wstring_view arg) = 0;
		virtual void info_impl(std::string_view message, const int& arg) = 0;
	};
}