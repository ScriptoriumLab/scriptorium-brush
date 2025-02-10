#pragma once

#include <spdlog/spdlog-inl.h>
#include <spdlog/sinks/basic_file_sink.h>
#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "No filesystem support"
#endif

namespace modian::util::logger {
    static constexpr auto ascii_modian_ime = R"(
         __  __           _ _             _____ __  __ ______
        |  \/  |         | (_)           |_   _|  \/  |  ____|
        | \  / | ___   __| |_  __ _ _ __   | | | \  / | |__
        | |\/| |/ _ \ / _` | |/ _` | '_ \  | | | |\/| |  __|
        | |  | | (_) | (_| | | (_| | | | |_| |_| |  | | |____
        |_|  |_|\___/ \__,_|_|\__,_|_| |_|_____|_|  |_|______|
    )";

    static bool g_loggerInitialized{false};

    inline void init_logger() {
        if (g_loggerInitialized)
            return;

        const char* userprofile = std::getenv("USERPROFILE");
        if (!userprofile) {
            // 如果没有获取到环境变量，可以选择一个默认路径或者直接返回
            return;
        }

        std::string log_dir = std::string(userprofile) + "/Modian/Log";
        std::string log_path = log_dir + "/modian.log";

        try {
            if (!fs::exists(log_dir)) {
                fs::create_directories(log_dir);
            }
        } catch (const fs::filesystem_error &err) {
            // 处理错误，比如打印调试信息
            return;
        }

        try {
            const auto logger = spdlog::basic_logger_mt("file_logger", log_path);
            set_default_logger(logger);
            spdlog::set_level(spdlog::level::info);
            g_loggerInitialized = true;
            spdlog::info("spdlog initialized. Logging to {}", log_path);
        } catch (const spdlog::spdlog_ex &ex) {
            // TODO: add failed exception handler
        }
    }
}