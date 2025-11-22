#pragma once

#include "modian/core/logger/base_logger.h"

namespace modian::brush::core {
	class console_logger final : public base_logger {
	public:
		~console_logger() override = default;

        void debug(std::string_view message) override;
        void error(std::string_view message) override;

		[[nodiscard]] std::string_view type() const override { return "console_logger"; }

	protected:
		void info_impl(std::string_view message) override;
		void info_impl(std::string_view message, std::string_view arg) override;
		void info_impl(std::string_view message, std::wstring_view arg) override;
		void info_impl(std::string_view message, const int& arg) override;
	};
}