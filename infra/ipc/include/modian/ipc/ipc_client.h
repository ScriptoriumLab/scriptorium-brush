#pragma once

#include <string_view>
#include <mutex>

namespace modian::brush::infra::ipc {
	class ipc_client {
	public:
		ipc_client();
		~ipc_client();

		// 禁止拷贝，防止句柄双重释放
		ipc_client(const ipc_client&) = delete;
		ipc_client& operator=(const ipc_client&) = delete;

		// 发送消息
		void send(std::string_view message);

	private:
		// 尝试连接管道
		bool ensure_connection();
		void close();

		void* pipe_handle_{nullptr}; // 实际上是 HANDLE
		std::mutex mutex_; // 保护 pipe_handle_ 的线程安全
	};
}