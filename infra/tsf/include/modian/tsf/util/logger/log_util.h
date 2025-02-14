#pragma once

#include <WeakReference.h>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"
#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#error "No filesystem support"
#endif

namespace modian::tsf::util::logger {
    static constexpr auto ascii_modian_ime = R"(
        =======================================================================================================================

         ,ggg, ,ggg,_,ggg,                                                               ,a8a,  ,ggg, ,ggg,_,ggg,     ,ggggggg,
        dP""Y8dP""Y88P""Y8b                     8I                                      ,8" "8,dP""Y8dP""Y88P""Y8b  ,dP""""""Y8b
        Yb, `88'  `88'  `88                     8I                                      d8   8bYb, `88'  `88'  `88  d8'    a  Y8
         `"  88    88    88                     8I   gg                                 88   88 `"  88    88    88  88     "Y8P'
             88    88    88                     8I   ""                                 88   88     88    88    88  `8baaaa
             88    88    88    ,ggggg,    ,gggg,8I   gg     ,gggg,gg   ,ggg,,ggg,       Y8   8P     88    88    88 ,d8P""""
             88    88    88   dP"  "Y8gggdP"  "Y8I   88    dP"  "Y8I  ,8" "8P" "8,      `8, ,8'     88    88    88 d8"
             88    88    88  i8'    ,8I i8'    ,8I   88   i8'    ,8I  I8   8I   8I 8888  "8,8"      88    88    88 Y8,
             88    88    Y8,,d8,   ,d8',d8,   ,d8b,_,88,_,d8,   ,d8b,,dP   8I   Yb,`8b,  ,d8b,      88    88    Y8,`Yba,,_____,
             88    88    `Y8P"Y8888P"  P"Y8888P"`Y88P""Y8P"Y8888P"`Y88P'   8I   `Y8  "Y88P" "Y8     88    88    `Y8  `"Y8888888

        =======================================================================================================================
    )";

    static bool g_loggerInitialized{false};

    inline void init_logger() {
        if (g_loggerInitialized)
            return;

        // 检查是否已经存在同名日志器
        auto existing_logger = spdlog::get("modian_logger");
        if (existing_logger) {
            spdlog::set_default_logger(existing_logger);
            spdlog::set_level(spdlog::level::debug);
            g_loggerInitialized = true;
            spdlog::debug("Using existing logger: modian_logger");
            return;
        }

        // 正常的初始化流程…
        const char* userprofile = std::getenv("USERPROFILE");
        if (!userprofile) {
            spdlog::error("Failed to get USERPROFILE environment variable.");
            return;
        }

        std::string log_dir = std::string(userprofile) + "/Modian/Log";
        std::string log_path = log_dir + "/modian.log";

        try {
            if (!fs::exists(log_dir)) {
                // TODO: when testing should use macro to decide print to console instead of use directory existence
                const auto logger = spdlog::stdout_color_mt("modian_logger");
                spdlog::set_default_logger(logger);
                spdlog::set_level(spdlog::level::debug);
                spdlog::debug("Console logger initialized.");
                spdlog::info("\n{}", ascii_modian_ime);
                g_loggerInitialized = true;
                return;
            }

            const auto logger = spdlog::basic_logger_mt("modian_logger", log_path);
            spdlog::set_default_logger(logger);
            spdlog::set_level(spdlog::level::debug);
            spdlog::flush_on(spdlog::level::info);
            g_loggerInitialized = true;
            spdlog::debug("File logger initialized.");
        } catch (const fs::filesystem_error &err) {
            spdlog::error("Filesystem error: {}", err.what());
        } catch (const spdlog::spdlog_ex &ex) {
            spdlog::error("Spdlog initialization failed: {}", ex.what());
        }
    }

    inline std::string wstring_to_string(const std::wstring& wstr) {
        if (wstr.empty()) {
            return {};
        }
        const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
        std::string str(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &str[0], size_needed, nullptr, nullptr);
        return str;
    }
}