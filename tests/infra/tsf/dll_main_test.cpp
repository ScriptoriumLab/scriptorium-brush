#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <vector>

#include "modian/info/registry_info.h"

STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();

bool is_reg_key_exists(const HKEY& root, const wchar_t* path) {
    HKEY hKey;
    if (const LONG result = RegOpenKeyExW(root, path, 0, KEY_READ, &hKey); result == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

std::wstring read_reg_string(const HKEY& root, const wchar_t* path, const wchar_t* valueName) {
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

DWORD read_reg_dword(const HKEY& root, const wchar_t* path, const wchar_t* valueName) {
    HKEY hKey;
    DWORD value{0};
    DWORD size = sizeof(DWORD);

    if (RegOpenKeyExW(root, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueExW(hKey, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &size);
        RegCloseKey(hKey);
    }
    return value;
}

class modian_registry_test : public ::testing::Test {
protected:
    void SetUp() override {
        DllUnregisterServer();
    }

    void TearDown() override {
        DllUnregisterServer();
    }
};

TEST_F(modian_registry_test, should_get_basic_info_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    EXPECT_TRUE(is_reg_key_exists(HKEY_LOCAL_MACHINE, CLSID_KEY));

    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, CLSID_KEY, L"Description"), L"Modian");
}

TEST_F(modian_registry_test, should_get_language_profile_info_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    EXPECT_TRUE(is_reg_key_exists(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY));

    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"Description"), L"Modian");

    EXPECT_EQ(read_reg_dword(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"Enable"), 1UL);
}

TEST_F(modian_registry_test, should_get_icon_path_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    const std::wstring expectedIcon = L"C:\\Path\\To\\Your\\Icon.ico";
    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"IconFile"), expectedIcon);
}

TEST_F(modian_registry_test, should_successfully_unregister_modian_after_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);
    ASSERT_EQ(DllUnregisterServer(), S_OK);

    EXPECT_FALSE(is_reg_key_exists(HKEY_LOCAL_MACHINE, CLSID_KEY));
    EXPECT_FALSE(is_reg_key_exists(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY));
}