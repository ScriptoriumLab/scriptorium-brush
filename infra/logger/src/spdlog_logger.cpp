#include "modian/logger/spdlog_logger.h"

#include <iostream>
#include <spdlog/sinks/rotating_file_sink.h>
#include "spdlog/sinks/basic_file_sink.h"

namespace modian::logger {
	spdlog_logger::spdlog_logger() {
        // 检查是否已经存在同名日志器
        if (const auto existing_logger = spdlog::get("modian_logger")) {
            spdlog::set_default_logger(existing_logger);
            spdlog::set_level(spdlog::level::debug);
            spdlog::debug("Using existing logger: modian_logger");
            return;
        }
		try {
			char* userprofile{nullptr};
			size_t size = 0;

			if (const errno_t err = _dupenv_s(&userprofile, &size, "USERPROFILE"); err != 0 || userprofile == nullptr) {
			 spdlog::error("Failed to retrieve USERPROFILE.");
			 return;
			}

			const std::string log_dir = std::string(userprofile) + "/Modian/Log";
			const std::string log_path = log_dir + "/modian.log";
			logger_ = spdlog::basic_logger_mt("modian_logger", log_path);;

			// 设置为默认 logger，确保在其他地方可以通过 spdlog 调用
			spdlog::set_default_logger(logger_);
            spdlog::set_level(spdlog::level::debug);
            spdlog::flush_on(spdlog::level::info);
            logger_->debug("File logger initialized.");
		} catch (const std::exception& e) {
			std::cerr << "Logger initialization failed: " << e.what() << std::endl;
		}
	}

    void spdlog_logger::debug(const std::string& message) {
	    logger_->debug(message);
    }

    void spdlog_logger::error(const std::string& message) {
	    logger_->error(message);
    }

    void spdlog_logger::info_impl(const std::string& message) {
	    logger_->info(message);
    }

    void spdlog_logger::info_impl(const std::string& message, const std::string& arg) {
		logger_->info(fmt::runtime(message), arg);
    }
}
