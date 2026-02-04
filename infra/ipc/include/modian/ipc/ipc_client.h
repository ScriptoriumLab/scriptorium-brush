#pragma once

#include <string_view>
#include <mutex>

namespace modian::brush::infra::ipc {
	class ipc_client {
	public:
		ipc_client();
		~ipc_client();

		ipc_client(const ipc_client&) = delete;
		ipc_client& operator=(const ipc_client&) = delete;

		std::string send_and_wait(std::string_view message);

	private:
		bool ensure_connection();
		void close();

		void* pipe_handle_;
		std::mutex mutex_;
	};
}