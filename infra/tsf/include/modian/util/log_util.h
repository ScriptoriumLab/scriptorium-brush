#pragma once

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
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
            spdlog::set_default_logger(logger);
            spdlog::set_level(spdlog::level::info);
            g_loggerInitialized = true;
            spdlog::info("spdlog initialized. Logging to {}", log_path);
        } catch (const spdlog::spdlog_ex &ex) {
            // TODO: add failed exception handler
        }
    }
}