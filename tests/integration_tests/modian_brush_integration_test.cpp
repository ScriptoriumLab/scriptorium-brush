#include <gtest/gtest.h>
#include <windows.h>
#include <filesystem>
#include <string>
#include <thread>

#include "modian/common/infra/ipc/ipc_client_factory.h"

namespace fs = std::filesystem;

class InkstoneIntegrationTest : public ::testing::Test {
protected:
    PROCESS_INFORMATION pi_{};
    HANDLE hPipe_ = INVALID_HANDLE_VALUE;

    void SetUp() override {
        const std::string exe_path = INKSTONE_EXE_PATH;

        STARTUPINFOA si{};
        si.cb = sizeof(si);

        const fs::path exe_p(exe_path);
        std::string work_dir = exe_p.parent_path().string();

        std::cout << "Launching Inkstone at: " << exe_path << std::endl;

        const BOOL success = CreateProcessA(
            exe_path.c_str(),
            nullptr, nullptr, nullptr, FALSE,
            0, nullptr,
            work_dir.c_str(),
            &si, &pi_
        );

        ASSERT_TRUE(success) << "Failed to start Inkstone! Error: " << GetLastError();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void TearDown() override {
        if (pi_.hProcess) {
            TerminateProcess(pi_.hProcess, 0);
            WaitForSingleObject(pi_.hProcess, 1000);
            CloseHandle(pi_.hProcess);
            CloseHandle(pi_.hThread);
        }

        const std::string root_dir = MODIAN_TEST_ROOT_DIR;

        try {
            if (fs::exists(root_dir)) {
                fs::remove_all(root_dir);
                std::cout << "Environment cleaned up: " << root_dir << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to clean up environment: " << e.what() << std::endl;
        }
    }
};

TEST_F(InkstoneIntegrationTest, DISABLED_ConnectAndInput) {
	const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_input_protocol_pipe)";
    auto client = modian::common::infra::ipc::ipc_client_factory::create_sync_ipc_client(INPUT_PROTOCOL_PIPE_NAME);

    std::string response = client->sync_send("n");
    response = client->sync_send("i");
    EXPECT_EQ(response, "C:你");

    client->sync_send("h");
    client->sync_send("a");
    response = client->sync_send("o");
    EXPECT_EQ(response, "C:好");


    client->sync_send("m");
    response = client->sync_send("o");
    EXPECT_EQ(response, "C:墨");

    client->sync_send("d");
    client->sync_send("i");
    client->sync_send("a");
    response = client->sync_send("n");
    EXPECT_EQ(response, "C:点");
}
