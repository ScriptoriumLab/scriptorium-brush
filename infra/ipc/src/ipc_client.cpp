#include "modian/ipc/ipc_client.h"

#include <windows.h>
#include <vector>
#include "modian/core/logger/logger_service.h"

namespace modian::brush::infra::ipc {
    constexpr DWORD BUFFER_SIZE = 4096;

    ipc_client::ipc_client(std::wstring_view pipe_name) : pipe_name_{pipe_name}, pipe_handle_(INVALID_HANDLE_VALUE) {}

    ipc_client::~ipc_client() {
        close();
    }

    void ipc_client::close() {
        if (pipe_handle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(static_cast<HANDLE>(pipe_handle_));
            pipe_handle_ = INVALID_HANDLE_VALUE;
        }
    }

    bool ipc_client::ensure_connection() {
        if (pipe_handle_ != INVALID_HANDLE_VALUE) {
            return true;
        }

        if (!WaitNamedPipeW(pipe_name_.c_str(), 20)) {
            if (GetLastError() != ERROR_FILE_NOT_FOUND) {
                core::logger_service::logger()->debug("retrying...");
            }
            return false;
        }

        HANDLE hPipe = CreateFileW(
            pipe_name_.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            core::logger_service::logger()->error("Failed to connect pipe. Error: {}", GetLastError());
            return false;
        }

        DWORD mode = PIPE_READMODE_MESSAGE;
        if (!SetNamedPipeHandleState(hPipe, &mode, nullptr, nullptr)) {
            core::logger_service::logger()->error("Failed to set pipe mode. Error: {}", GetLastError());
            CloseHandle(hPipe);
            return false;
        }

        pipe_handle_ = hPipe;
        core::logger_service::logger()->info("IPC Connected to Inkstone!");
        return true;
    }

    std::string ipc_client::send_and_wait(std::string_view message) {
        std::lock_guard lock(mutex_);

        if (!ensure_connection()) return "";

        DWORD bytesWritten;
        BOOL success = WriteFile(
            static_cast<HANDLE>(pipe_handle_),
            message.data(), static_cast<DWORD>(message.size()),
            &bytesWritten, nullptr
        );

        if (!success) {
            core::logger_service::logger()->debug("IPC Write failed. Error: {}. Retrying...", GetLastError());
            close();

            if (!ensure_connection()) {
                core::logger_service::logger()->error("IPC Reconnect failed.");
                return "";
            }

            success = WriteFile(
                static_cast<HANDLE>(pipe_handle_),
                message.data(), static_cast<DWORD>(message.size()),
                &bytesWritten, nullptr
            );

            if (!success) {
                core::logger_service::logger()->error("IPC Retry Write failed. Error: {}", GetLastError());
                close();
                return "";
            }
        }

        std::vector<char> buffer(BUFFER_SIZE);
        DWORD bytesRead;

        success = ReadFile(
            static_cast<HANDLE>(pipe_handle_),
            buffer.data(), static_cast<DWORD>(buffer.size()),
            &bytesRead, nullptr
        );

        if (success && bytesRead > 0) {
            return { buffer.data(), bytesRead };
        }

        if (!success) {
             core::logger_service::logger()->error("IPC Read failed. Error: {}", GetLastError());
             close();
        }

        return "";
    }
}