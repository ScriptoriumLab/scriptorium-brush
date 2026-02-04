#include <gtest/gtest.h>
#include <thread>
#include <windows.h>
#include "modian/ipc/ipc_client.h"

using namespace modian::brush::infra::ipc;

const std::wstring TEST_PIPE_NAME = L"\\\\.\\pipe\\modian_test_pipe_001";

class IpcClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        server_ready_ = false;
    }

    void TearDown() override {
        join_server();
    }

    void run_mock_server(const std::string& response_data) {
        join_server();
        server_ready_ = false;

        server_thread_ = std::thread([this, response_data] {
            const auto h_named_pipe = CreateNamedPipeW(
                TEST_PIPE_NAME.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                1,
                1024, 1024, 0, nullptr
            );

            if (h_named_pipe == INVALID_HANDLE_VALUE) {
                server_ready_ = true;
                return;
            }

            server_ready_ = true;

            if (ConnectNamedPipe(h_named_pipe, nullptr) || GetLastError() == ERROR_PIPE_CONNECTED) {
                char buffer[1024];
                DWORD bytes_read;

                if (ReadFile(h_named_pipe, buffer, sizeof(buffer), &bytes_read, nullptr)) {
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                DWORD bytes_written;
                WriteFile(h_named_pipe, response_data.c_str(), response_data.size(), &bytes_written, nullptr);
            }

            FlushFileBuffers(h_named_pipe);
            DisconnectNamedPipe(h_named_pipe);
            CloseHandle(h_named_pipe);
        });
    }

    void wait_for_server_ready() const {
        int timeout_ms = 2000;
        while (!server_ready_ && timeout_ms > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            timeout_ms -= 10;
        }

        if (!server_ready_) {
            FAIL() << "Mock Server failed to start in time!";
        }
    }

    void join_server() {
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

    std::thread server_thread_;
    std::atomic<bool> server_ready_{false};
};

TEST_F(IpcClientTest, should_return_empty_string_when_server_is_not_running) {
    ipc_client client(TEST_PIPE_NAME);

    const auto response = client.send_and_wait("hello");

    ASSERT_TRUE(response.empty());
}

TEST_F(IpcClientTest, should_get_response_successfully_when_server_is_running) {
    const std::string expected_response = "C:你";

    run_mock_server(expected_response);
    wait_for_server_ready();

    ipc_client client(TEST_PIPE_NAME);

    const auto actual_response = client.send_and_wait("ni");

    ASSERT_EQ(actual_response, expected_response);

    join_server();
}

TEST_F(IpcClientTest, should_successfully_reconnect_when_server_restart) {
    run_mock_server("Response 1");
    wait_for_server_ready();
    ipc_client client(TEST_PIPE_NAME);
    ASSERT_EQ(client.send_and_wait("Req 1"), "Response 1");
    join_server();

    run_mock_server("Response 2");
    wait_for_server_ready();
    ASSERT_EQ(client.send_and_wait("Req 2"), "Response 2");
    join_server();
}