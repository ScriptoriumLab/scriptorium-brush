#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include "modian/common/core/logger/base_logger.h"

namespace modian::brush::infra::logger {
	class spdlog_logger final : public common::core::base_logger {
	public:
		spdlog_logger();
		~spdlog_logger() override;

		void sink_it(common::core::log_level level, std::string_view msg) override;

		[[nodiscard]] std::string_view type() const override { return "spdlog_logger"; }

	private:
		std::shared_ptr<spdlog::logger> logger_;
	};
}
