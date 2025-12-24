#include <gtest/gtest.h>
#include <windows.h>
#include <filesystem>
#include <string>
#include <thread>
#include "modian/ipc/ipc_client.h"

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

TEST_F(InkstoneIntegrationTest, ConnectAndInput) {
    modian::brush::infra::ipc::ipc_client client;

    std::string response = client.send_and_wait("n");
    response = client.send_and_wait("i");
    EXPECT_EQ(response, "你");

    client.send_and_wait("h");
    client.send_and_wait("a");
    response = client.send_and_wait("o");
    EXPECT_EQ(response, "好");


    client.send_and_wait("m");
    response = client.send_and_wait("o");
    EXPECT_EQ(response, "墨");

    client.send_and_wait("d");
    client.send_and_wait("i");
    client.send_and_wait("a");
    response = client.send_and_wait("n");
    EXPECT_EQ(response, "点");
}