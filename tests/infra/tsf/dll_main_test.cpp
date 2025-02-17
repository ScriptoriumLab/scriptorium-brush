#include <msctf.h>
#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <vector>
#include "spdlog/spdlog.h"

#include "dll/registry_operator.h"
#include "modian/tsf/tsf_text_service.h"
#include "modian/tsf/dll/register.h"
#include "modian/tsf/dll/info/registry_info.h"
#include "modian/tsf/dll/dll_util.h"

STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();

class modian_registry_test : public ::testing::Test {
protected:
    void SetUp() override {
        _ = DllUnregisterServer();
    }

    void TearDown() override {
        _ = DllUnregisterServer();
    }
private:
    modian::tsf::dll::auto_com ac{};
    HRESULT _{S_OK};
};

TEST_F(modian_registry_test, should_get_basic_info_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    EXPECT_TRUE(modian::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(modian::tsf::dll::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::tsf::dll::util::convert_clsid_to_string(modian::tsf::dll::MODIAN_IME_CLSID)).c_str()));

    EXPECT_EQ(modian::tests::registry_operator::read_reg_string(HKEY_LOCAL_MACHINE, (std::wstring(modian::tsf::dll::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::tsf::dll::util::convert_clsid_to_string(modian::tsf::dll::MODIAN_IME_CLSID) + L"\\LanguageProfile\\0x00000804\\" + modian::tsf::dll::util::convert_guid_to_string(modian::tsf::dll::MODIAN_IME_GUID_PROFILE)).c_str(), L"Description"), L"Modian Input Method");

    EXPECT_EQ(modian::tests::registry_operator::read_reg_string(HKEY_CLASSES_ROOT, (modian::tsf::dll::MODIAN_IME_REGINFO_PREFIX_CLSID.data() + modian::tsf::dll::util::convert_clsid_to_string(modian::tsf::dll::MODIAN_IME_CLSID) + L"\\InProcServer32").c_str(), L"ThreadingModel"), modian::tsf::dll::MODIAN_IME_MODEL);
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

    EXPECT_FALSE(modian::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(modian::tsf::dll::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::tsf::dll::util::convert_clsid_to_string(modian::tsf::dll::MODIAN_IME_CLSID)).c_str()));
    EXPECT_FALSE(modian::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(modian::tsf::dll::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::tsf::dll::util::convert_clsid_to_string(modian::tsf::dll::MODIAN_IME_CLSID) + L"\\LanguageProfile\\0x00000804\\" + modian::tsf::dll::util::convert_guid_to_string(modian::tsf::dll::MODIAN_IME_GUID_PROFILE)).c_str()));
}

TEST_F(modian_registry_test, should_successfully_create_input_processor) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    modian::tsf::tsf_text_service* tsf_text_service = nullptr;
    const auto hr = CoCreateInstance(modian::tsf::dll::MODIAN_IME_CLSID, nullptr, CLSCTX_INPROC_SERVER, IID_ITfTextInputProcessor, reinterpret_cast<void**>(&tsf_text_service));

    ASSERT_EQ(hr, S_OK);
}