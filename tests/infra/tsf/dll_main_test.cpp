#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <vector>

// 导入 DLL 导出的注册函数
extern "C" {
__declspec(dllimport) STDAPI DllRegisterServer();
__declspec(dllimport) STDAPI DllUnregisterServer();
}

const auto CLSID_KEY = L"SOFTWARE\\Microsoft\\CTF\\TIP\\{F7A3B6D1-EC88-41A2-9F5D-7A0E3C8A7B89}";
const auto LANGUAGE_PROFILE_KEY = L"SOFTWARE\\Microsoft\\CTF\\TIP\\{F7A3B6D1-EC88-41A2-9F5D-7A0E3C8A7B89}\\LanguageProfile\\0x00000804\\{C00E97BF-4DD6-4C08-9D8D-BA67265F4997}";

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
    DWORD size = 0;
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

// 辅助函数：读取注册表 DWORD 值
DWORD RegReadDWORD(HKEY root, const wchar_t* path, const wchar_t* valueName) {
    HKEY hKey;
    DWORD value = 0;
    DWORD size = sizeof(DWORD);

    if (RegOpenKeyExW(root, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegQueryValueExW(hKey, valueName, nullptr, nullptr, (LPBYTE)&value, &size);
        RegCloseKey(hKey);
    }
    return value;
}

class InputMethodRegistryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 确保测试前环境干净
        DllUnregisterServer();
    }

    void TearDown() override {
        // 清理测试产生的注册表项
        DllUnregisterServer();
    }
};

TEST_F(InputMethodRegistryTest, BasicRegistration) {
    // 执行注册
    ASSERT_EQ(DllRegisterServer(), S_OK);

    // 验证主注册表项
    EXPECT_TRUE(is_reg_key_exists(HKEY_LOCAL_MACHINE, CLSID_KEY));

    // 验证描述信息
    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, CLSID_KEY, L"Description"), L"Modian");
}

TEST_F(InputMethodRegistryTest, LanguageProfileConfiguration) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    // 验证语言配置文件路径
    EXPECT_TRUE(is_reg_key_exists(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY));

    // 验证描述信息
    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"Description"), L"Modian");

    // 验证启用状态
    EXPECT_EQ(RegReadDWORD(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"Enable"), 1UL);
}

TEST_F(InputMethodRegistryTest, IconFileConfiguration) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    const std::wstring expectedIcon = L"C:\\Path\\To\\Your\\Icon.ico";
    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"IconFile"), expectedIcon);
}

TEST_F(InputMethodRegistryTest, Unregistration) {
    // 先注册再测试注销
    ASSERT_EQ(DllRegisterServer(), S_OK);
    ASSERT_EQ(DllUnregisterServer(), S_OK);

    // 验证所有相关项已删除
    EXPECT_FALSE(is_reg_key_exists(HKEY_LOCAL_MACHINE, CLSID_KEY));
    EXPECT_FALSE(is_reg_key_exists(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY));
}