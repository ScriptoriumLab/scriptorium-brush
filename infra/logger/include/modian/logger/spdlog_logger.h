#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "modian/core/logger/base_logger.h"

namespace modian::brush::infra::logger {
	class spdlog_logger final : public core::base_logger {
	public:
		spdlog_logger();
		~spdlog_logger() override;
		void debug(const std::string& message) override;
		void error(const std::string& message) override;

		std::string type{"spdlog_logger"};

	protected:
		void info_impl(const std::string& message) override;
		void info_impl(const std::string& message, const std::string& arg) override;
		void info_impl(const std::string& message, const std::wstring& arg) override;
		void info_impl(const std::string& message, const int& arg) override;

	private:
		static std::string wstring_to_string(const std::wstring& wstr);
        std::shared_ptr<spdlog::logger> logger_;
	};
}
