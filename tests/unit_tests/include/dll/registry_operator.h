#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace modian::tests::registry_operator {
    inline bool is_reg_key_exists(const HKEY& root, const wchar_t* path) {
        HKEY hKey;
        if (const LONG result = RegOpenKeyExW(root, path, 0, KEY_READ, &hKey); result == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        return false;
    }

    inline std::wstring read_reg_string(const HKEY& root, const wchar_t* path, const wchar_t* valueName) {
        HKEY hKey;
        DWORD size{0};
        std::wstring result;

        if (RegOpenKeyExW(root, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            // 获取数据大小
            if (RegQueryValueExW(hKey, valueName, nullptr, nullptr, nullptr, &size) == ERROR_SUCCESS) {
                std::vector<wchar_t> buffer(size / sizeof(wchar_t) + 1);
                if (RegQueryValueExW(hKey, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer.data()), &size) == ERROR_SUCCESS) {
                    result = buffer.data();
                }
            }
            RegCloseKey(hKey);
        }
        return result;
    }
}