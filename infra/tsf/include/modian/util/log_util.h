#pragma once

#include <fstream>
#include <mutex>
#include <string>

// 全局互斥量，保证写日志时线程安全
static std::mutex g_logMutex;

inline void log_message(const std::wstring& msg) {
	std::lock_guard<std::mutex> lock(g_logMutex);
	std::wofstream ofs(LOG_ROOT, std::ios::app);
	if (ofs) {
		ofs << msg << std::endl;
	}
}
