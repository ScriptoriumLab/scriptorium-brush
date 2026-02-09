#pragma once

#include <string_view>
#include <mutex>

namespace modian::brush::infra::ipc {
	constexpr std::wstring_view DEFAULT_PIPE_NAME = LR"(\\.\pipe\modian_input_protocol_pipe)";

	class input_protocol_pipe_client {
	public:
		explicit input_protocol_pipe_client(std::wstring_view pipe_name = DEFAULT_PIPE_NAME);
		~input_protocol_pipe_client();

		input_protocol_pipe_client(const input_protocol_pipe_client&) = delete;
		input_protocol_pipe_client& operator=(const input_protocol_pipe_client&) = delete;

		std::string send_and_wait(std::string_view message);

	private:
		bool ensure_connection();
		void close();

		std::wstring pipe_name_;
		void* pipe_handle_;
		std::mutex mutex_;
	};
}