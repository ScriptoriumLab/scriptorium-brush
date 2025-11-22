#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "modian/core/logger/base_logger.h"

namespace modian::brush::infra::logger {
	class spdlog_logger final : public core::base_logger {
	public:
		spdlog_logger();
		~spdlog_logger() override;
		void debug(std::string_view message) override;
		void error(std::string_view message) override;

		[[nodiscard]] std::string_view type() const override { return "spdlog_logger"; }

	protected:
		void info_impl(std::string_view message) override;
		void info_impl(std::string_view message, std::string_view arg) override;
		void info_impl(std::string_view message, std::wstring_view arg) override;
		void info_impl(std::string_view message, const int& arg) override;

	private:
        std::shared_ptr<spdlog::logger> logger_;
	};
}
