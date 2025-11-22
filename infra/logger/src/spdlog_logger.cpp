#include "modian/logger/spdlog_logger.h"
#include "modian/core/utils/utils.h"

#include <iostream>
#include <filesystem>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace fs = std::filesystem;

namespace modian::brush::infra::logger {
    spdlog_logger::spdlog_logger() {
        if (const auto existing_logger = spdlog::get("modian_logger")) {
            logger_ = existing_logger;

            spdlog::set_default_logger(logger_);
            spdlog::set_level(spdlog::level::debug);
            return;
        }

        try {
            char* userprofile_raw{nullptr};
            size_t size = 0;

            if (_dupenv_s(&userprofile_raw, &size, "USERPROFILE") != 0 || userprofile_raw == nullptr) {
                std::cerr << "[Fatal] Failed to retrieve USERPROFILE environment variable." << std::endl;
                return;
            }

            const fs::path home_dir = userprofile_raw;
            free(userprofile_raw); // 【关键修复】防止内存泄漏

            const fs::path log_dir = home_dir / "Modian" / "Log";
            const fs::path log_path = log_dir / "modian.log";

            if (!fs::exists(log_dir)) {
                std::error_code ec;
                fs::create_directories(log_dir, ec);
                if (ec) {
                    std::cerr << "[Fatal] Failed to create log directory: " << ec.message() << std::endl;
                    return;
                }
            }

            logger_ = spdlog::basic_logger_mt("modian_logger", log_path.string());

            spdlog::set_default_logger(logger_);
            spdlog::set_level(spdlog::level::debug);

            // TODO: change to debug or error level after debugging
            spdlog::flush_on(spdlog::level::info);
            logger_->info("Logger initialized at: {}", log_path.string());
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "[Fatal] Log initialization failed: " << ex.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[Fatal] Unexpected error in logger init: " << e.what() << std::endl;
        }
    }

    spdlog_logger::~spdlog_logger() {
        spdlog::shutdown();
    }

    void spdlog_logger::debug(std::string_view message) {
        if (logger_) logger_->debug(message);
    }

    void spdlog_logger::error(std::string_view message) {
        if (logger_) logger_->error(message);
    }

    void spdlog_logger::info_impl(std::string_view message) {
        if (logger_) logger_->info(message);
    }

    void spdlog_logger::info_impl(std::string_view message, std::string_view arg) {
        // fmt::runtime 是必要的，因为 message 不是编译期常量
        if (logger_) logger_->info(fmt::runtime(message), arg);
    }

    void spdlog_logger::info_impl(std::string_view message, std::wstring_view arg) {
        if (logger_) {
            logger_->info(fmt::runtime(message), core::utils::to_utf8(arg));
        }
    }

    void spdlog_logger::info_impl(std::string_view message, const int& arg) {
        if (logger_) logger_->info(fmt::runtime(message), arg);
    }
}