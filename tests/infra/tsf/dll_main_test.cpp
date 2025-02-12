#include <msctf.h>
#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"
#include "modian/tsf/tsf_text_service.h"
#include "modian/tsf/dll/register.h"
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
        spdlog::shutdown();
    }
private:
    modian::tsf::dll::auto_com ac{};
};

TEST_F(modian_registry_test, should_get_basic_info_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    EXPECT_TRUE(is_reg_key_exists(HKEY_LOCAL_MACHINE, CLSID_KEY));

    EXPECT_EQ(read_reg_string(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, L"Description"), L"Modian Input Method");
}

TEST_F(modian_registry_test, should_get_categories_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

	ITfCategoryMgr* category_mgr{nullptr};
	EXPECT_TRUE(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, reinterpret_cast<void**>(&category_mgr)) == S_OK);

    IEnumGUID* categories;
    EXPECT_TRUE(category_mgr->EnumCategoriesInItem(modian::tsf::dll::MODIAN_IME_CLSID, &categories) == S_OK);

    GUID guid;
    ULONG fetched{0};
    size_t size{0};
    while (categories->Next(1, &guid, &fetched) == S_OK) {
        ASSERT_TRUE(std::ranges::find(modian::tsf::dll::MODIAN_SUPPORT_CATEGORIES.begin(), modian::tsf::dll::MODIAN_SUPPORT_CATEGORIES.end(), guid) != modian::tsf::dll::MODIAN_SUPPORT_CATEGORIES.end());
        ++size;
    }
    ASSERT_EQ(size, modian::tsf::dll::MODIAN_SUPPORT_CATEGORIES.size());
}

TEST_F(modian_registry_test, should_successfully_unregister_modian_after_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);
    ASSERT_EQ(DllUnregisterServer(), S_OK);

    EXPECT_FALSE(is_reg_key_exists(HKEY_LOCAL_MACHINE, CLSID_KEY));
    EXPECT_FALSE(is_reg_key_exists(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY));
}

TEST_F(modian_registry_test, should_successfully_create_input_processor) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    modian::tsf::tsf_text_service* tsf_text_service = nullptr;
    const auto hr = CoCreateInstance(modian::tsf::dll::MODIAN_IME_CLSID, nullptr, CLSCTX_INPROC_SERVER, IID_ITfTextInputProcessor, reinterpret_cast<void**>(&tsf_text_service));

    ASSERT_EQ(hr, S_OK);
}