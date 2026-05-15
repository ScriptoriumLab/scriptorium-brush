#include "gtest/gtest.h"

#include <msctf.h>
#include <windows.h>
#include <string>

#include "dll/registry_operator.h"
#include "modian/tsf/tsf_text_service.h"
#include "modian/tsf/dll/info/registry_info.h"
#include "modian/tsf/dll/dll_util.h"
#include "modian/common/core/logger/logger_service.h"

STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();

class modian_registry_test : public ::testing::Test {
protected:
    void SetUp() override {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        ASSERT_TRUE(SUCCEEDED(hr));

        _ = DllUnregisterServer();
        modian::common::core::update_logger_times = 1;
    }

    void TearDown() override {
        _ = DllUnregisterServer();

        CoUninitialize();
    }
private:
    HRESULT _{S_OK};
};

TEST_F(modian_registry_test, should_get_basic_info_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    EXPECT_TRUE(modian::brush::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(modian::brush::infra::tsf::dll::info::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::brush::infra::tsf::dll::util::convert_clsid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID)).c_str()));

    EXPECT_EQ(modian::brush::tests::registry_operator::read_reg_string(HKEY_LOCAL_MACHINE, (std::wstring(modian::brush::infra::tsf::dll::info::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::brush::infra::tsf::dll::util::convert_clsid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID) + L"\\LanguageProfile\\0x00000804\\" + modian::brush::infra::tsf::dll::util::convert_guid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_GUID_PROFILE)).c_str(), L"Description"), L"Modian Input Method");

    EXPECT_EQ(modian::brush::tests::registry_operator::read_reg_string(HKEY_CLASSES_ROOT, (modian::brush::infra::tsf::dll::info::MODIAN_IME_REGINFO_PREFIX_CLSID.data() + modian::brush::infra::tsf::dll::util::convert_clsid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID) + L"\\InProcServer32").c_str(), L"ThreadingModel"), modian::brush::infra::tsf::dll::info::MODIAN_IME_MODEL);
}

TEST_F(modian_registry_test, should_get_categories_when_successfully_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

	ITfCategoryMgr* category_mgr{nullptr};
	EXPECT_TRUE(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, reinterpret_cast<void**>(&category_mgr)) == S_OK);

    IEnumGUID* categories;
    EXPECT_TRUE(category_mgr->EnumCategoriesInItem(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID, &categories) == S_OK);

    GUID guid;
    ULONG fetched{0};
    size_t size{0};
    while (categories->Next(1, &guid, &fetched) == S_OK) {
        ASSERT_TRUE(std::ranges::find(modian::brush::infra::tsf::dll::info::MODIAN_SUPPORT_CATEGORIES.begin(), modian::brush::infra::tsf::dll::info::MODIAN_SUPPORT_CATEGORIES.end(), guid) != modian::brush::infra::tsf::dll::info::MODIAN_SUPPORT_CATEGORIES.end());
        ++size;
    }
    ASSERT_EQ(size, modian::brush::infra::tsf::dll::info::MODIAN_SUPPORT_CATEGORIES.size());
}

TEST_F(modian_registry_test, should_successfully_unregister_modian_after_register_modian) {
    ASSERT_EQ(DllRegisterServer(), S_OK);
    ASSERT_EQ(DllUnregisterServer(), S_OK);

    EXPECT_FALSE(modian::brush::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(modian::brush::infra::tsf::dll::info::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::brush::infra::tsf::dll::util::convert_clsid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID)).c_str()));
    EXPECT_FALSE(modian::brush::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(modian::brush::infra::tsf::dll::info::MODIAN_REGISTRY_CLSID_ROOT_PATH) + modian::brush::infra::tsf::dll::util::convert_clsid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID) + L"\\LanguageProfile\\0x00000804\\" + modian::brush::infra::tsf::dll::util::convert_guid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_GUID_PROFILE)).c_str()));
}

TEST_F(modian_registry_test, should_successfully_create_input_processor) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    IClassFactory* p_factory = nullptr;
    HRESULT hr = DllGetClassObject(
        modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID,
        IID_IClassFactory,
        reinterpret_cast<void**>(&p_factory)
    );

    ASSERT_EQ(hr, S_OK);
    ASSERT_NE(p_factory, nullptr);

    modian::brush::infra::tsf::tsf_text_service* tsf_text_service = nullptr;
    hr = p_factory->CreateInstance(
        nullptr,
        IID_ITfTextInputProcessor,
        reinterpret_cast<void**>(&tsf_text_service)
    );

    ASSERT_EQ(hr, S_OK);
    ASSERT_NE(tsf_text_service, nullptr);

    if (tsf_text_service) tsf_text_service->Release();
    if (p_factory) p_factory->Release();
}
