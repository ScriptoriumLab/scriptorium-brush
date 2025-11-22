#include "modian/ipc/ipc_client.h"

#include <windows.h>
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::ipc {
    const std::wstring PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";

    ipc_client::ipc_client() = default;

    ipc_client::~ipc_client() {
        close();
    }

    void ipc_client::close() {
        if (pipe_handle_ && pipe_handle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(static_cast<HANDLE>(pipe_handle_));
            pipe_handle_ = nullptr;
        }
    }

    bool ipc_client::ensure_connection() {
        if (pipe_handle_ != nullptr && pipe_handle_ != INVALID_HANDLE_VALUE) {
            return true;
        }

        HANDLE hPipe = CreateFileW(
            PIPE_NAME.c_str(),
            GENERIC_WRITE,
            0,              // No sharing
            nullptr,        // Default security attributes
            OPEN_EXISTING,  // Opens existing pipe
            0,              // Default attributes
            nullptr         // No template file
        );

        if (hPipe != INVALID_HANDLE_VALUE) {
            pipe_handle_ = hPipe;
            core::logger_service::logger()->info("IPC Connected to Inkstone!");
            return true;
        }

        return false;
    }

    void ipc_client::send(std::string_view message) {
        std::lock_guard lock(mutex_);

        if (!ensure_connection()) {
            return;
        }

        DWORD bytesWritten;
        BOOL success = WriteFile(
            static_cast<HANDLE>(pipe_handle_),
            message.data(),
            static_cast<DWORD>(message.size()),
            &bytesWritten,
            nullptr
        );

        if (!success) {
            core::logger_service::logger()->error("IPC Write failed. Error: {}", GetLastError());
            close(); // 关闭句柄，下次重试连接
        }
    }
}