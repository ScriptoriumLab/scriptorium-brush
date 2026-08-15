#include "gtest/gtest.h"

#include <msctf.h>
#include <windows.h>
#include <string>

#include "dll/registry_operator.h"
#include "scriptorium/tsf/tsf_text_service.h"
#include "scriptorium/tsf/dll/info/registry_info.h"
#include "scriptorium/tsf/dll/dll_util.h"
#include "scriptorium/felt/core/logger/logger_service.h"

STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();

class scriptorium_registry_test : public ::testing::Test {
protected:
    void SetUp() override {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        ASSERT_TRUE(SUCCEEDED(hr));

        _ = DllUnregisterServer();
        scriptorium::felt::core::update_logger_times = 1;
    }

    void TearDown() override {
        _ = DllUnregisterServer();

        CoUninitialize();
    }
private:
    HRESULT _{S_OK};
};

TEST_F(scriptorium_registry_test, should_get_basic_info_when_successfully_register_scriptorium) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    EXPECT_TRUE(scriptorium::brush::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_REGISTRY_CLSID_ROOT_PATH) + scriptorium::brush::infra::tsf::dll::util::convert_clsid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID)).c_str()));

    EXPECT_EQ(scriptorium::brush::tests::registry_operator::read_reg_string(HKEY_LOCAL_MACHINE, (std::wstring(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_REGISTRY_CLSID_ROOT_PATH) + scriptorium::brush::infra::tsf::dll::util::convert_clsid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID) + L"\\LanguageProfile\\0x00000804\\" + scriptorium::brush::infra::tsf::dll::util::convert_guid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_GUID_PROFILE)).c_str(), L"Description"), L"Scriptorium Input Method");

    EXPECT_EQ(scriptorium::brush::tests::registry_operator::read_reg_string(HKEY_CLASSES_ROOT, (scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_REGINFO_PREFIX_CLSID.data() + scriptorium::brush::infra::tsf::dll::util::convert_clsid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID) + L"\\InProcServer32").c_str(), L"ThreadingModel"), scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_MODEL);
}

TEST_F(scriptorium_registry_test, should_get_categories_when_successfully_register_scriptorium) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

	ITfCategoryMgr* category_mgr{nullptr};
	EXPECT_TRUE(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, reinterpret_cast<void**>(&category_mgr)) == S_OK);

    IEnumGUID* categories;
    EXPECT_TRUE(category_mgr->EnumCategoriesInItem(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID, &categories) == S_OK);

    GUID guid;
    ULONG fetched{0};
    size_t size{0};
    while (categories->Next(1, &guid, &fetched) == S_OK) {
        ASSERT_TRUE(std::ranges::find(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_SUPPORT_CATEGORIES.begin(), scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_SUPPORT_CATEGORIES.end(), guid) != scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_SUPPORT_CATEGORIES.end());
        ++size;
    }
    ASSERT_EQ(size, scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_SUPPORT_CATEGORIES.size());
}

TEST_F(scriptorium_registry_test, should_successfully_unregister_scriptorium_after_register_scriptorium) {
    ASSERT_EQ(DllRegisterServer(), S_OK);
    ASSERT_EQ(DllUnregisterServer(), S_OK);

    EXPECT_FALSE(scriptorium::brush::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_REGISTRY_CLSID_ROOT_PATH) + scriptorium::brush::infra::tsf::dll::util::convert_clsid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID)).c_str()));
    EXPECT_FALSE(scriptorium::brush::tests::registry_operator::is_reg_key_exists(HKEY_LOCAL_MACHINE, (std::wstring(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_REGISTRY_CLSID_ROOT_PATH) + scriptorium::brush::infra::tsf::dll::util::convert_clsid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID) + L"\\LanguageProfile\\0x00000804\\" + scriptorium::brush::infra::tsf::dll::util::convert_guid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_GUID_PROFILE)).c_str()));
}

TEST_F(scriptorium_registry_test, should_successfully_create_input_processor) {
    ASSERT_EQ(DllRegisterServer(), S_OK);

    IClassFactory* p_factory = nullptr;
    HRESULT hr = DllGetClassObject(
        scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID,
        IID_IClassFactory,
        reinterpret_cast<void**>(&p_factory)
    );

    ASSERT_EQ(hr, S_OK);
    ASSERT_NE(p_factory, nullptr);

    ITfTextInputProcessor* p_processor = nullptr;
    hr = p_factory->CreateInstance(
        nullptr,
        IID_ITfTextInputProcessor,
        reinterpret_cast<void**>(&p_processor)
    );

    ASSERT_EQ(hr, S_OK);
    ASSERT_NE(p_processor, nullptr);

    if (p_processor) p_processor->Release();
    if (p_factory) p_factory->Release();
}
