#pragma once

#include <string_view>
#include <mutex>

namespace modian::brush::infra::ipc {
	constexpr std::wstring_view DEFAULT_PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";

	class ipc_client {
	public:
		explicit ipc_client(std::wstring_view pipe_name = DEFAULT_PIPE_NAME);
		~ipc_client();

		ipc_client(const ipc_client&) = delete;
		ipc_client& operator=(const ipc_client&) = delete;

		std::string send_and_wait(std::string_view message);

	private:
		bool ensure_connection();
		void close();

		std::wstring pipe_name_;
		void* pipe_handle_;
		std::mutex mutex_;
	};
}