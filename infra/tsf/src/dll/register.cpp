#include "modian/tsf/dll/register.h"

#include <msctf.h>
#include <wrl/client.h>
#include <string>
#include <vector>

#include "modian/tsf/dll/info/registry_info.h"
#include "modian/tsf/dll/dll_util.h"

using Microsoft::WRL::ComPtr;

namespace modian::brush::infra::tsf::dll {
    constexpr UINT DEFAULT_ICON_INDEX = static_cast<UINT>(-12);

    bool com_registration::register_profiles() {
        ComPtr<ITfInputProcessorProfileMgr> profile_mgr;

        HRESULT hr = CoCreateInstance(
            CLSID_TF_InputProcessorProfiles,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&profile_mgr)
        );

        if (FAILED(hr)) {
            return false;
        }

        std::vector<WCHAR> file_name_buf(MAX_PATH);
        const DWORD copied_len = GetModuleFileNameW(modian_instance, file_name_buf.data(), static_cast<DWORD>(file_name_buf.size()));

        if (copied_len == 0) {
            return false;
        }

        hr = profile_mgr->RegisterProfile(
            info::MODIAN_IME_CLSID,
            info::MODIAN_IME_LANG_ID,
            info::MODIAN_IME_GUID_PROFILE,
            info::MODIAN_IME_DESC.data(),
            static_cast<ULONG>(info::MODIAN_IME_DESC.size()),
            file_name_buf.data(), // 图标文件路径 (DLL 本身)
            static_cast<ULONG>(copied_len),
            DEFAULT_ICON_INDEX,   // 图标索引
            nullptr,
            0,
            TRUE,
            0
        );

        return hr == S_OK;
    }

    void com_registration::unregister_profiles() {
        ComPtr<ITfInputProcessorProfileMgr> profile_mgr;
        const HRESULT hr = CoCreateInstance(
            CLSID_TF_InputProcessorProfiles,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&profile_mgr)
        );

        if (FAILED(hr)) {
            return;
        }

        profile_mgr->UnregisterProfile(
            info::MODIAN_IME_CLSID,
            info::MODIAN_IME_LANG_ID,
            info::MODIAN_IME_GUID_PROFILE,
            0
        );
    }

    bool com_registration::register_categories() {
        ComPtr<ITfCategoryMgr> category_mgr;
        HRESULT hr = CoCreateInstance(
            CLSID_TF_CategoryMgr,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&category_mgr)
        );

        if (FAILED(hr)) {
            return false;
        }

        for (const auto& guid : info::MODIAN_SUPPORT_CATEGORIES) {
            hr = category_mgr->RegisterCategory(
                info::MODIAN_IME_CLSID,
                guid,
                info::MODIAN_IME_CLSID
            );
        }

        return (hr == S_OK);
    }

    void com_registration::unregister_categories() {
        ComPtr<ITfCategoryMgr> category_mgr;
        if (FAILED(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&category_mgr)))) {
            return;
        }

        for (const auto& guid : info::MODIAN_SUPPORT_CATEGORIES) {
            category_mgr->UnregisterCategory(
                info::MODIAN_IME_CLSID,
                guid,
                info::MODIAN_IME_CLSID
            );
        }
    }

    bool com_registration::register_server() {
        const std::wstring key_path = info::MODIAN_IME_REGINFO_PREFIX_CLSID.data() +
                                      util::convert_clsid_to_string(info::MODIAN_IME_CLSID);

        HKEY h_key = nullptr;
        HKEY h_sub_key = nullptr;
        bool success = false;

        // 辅助 lambda，用于关闭 key，实现简单的 RAII
        auto close_keys = [&](HKEY k1, HKEY k2) {
            if (k1) RegCloseKey(k1);
            if (k2) RegCloseKey(k2);
        };

        if (RegCreateKeyEx(HKEY_CLASSES_ROOT, key_path.c_str(), 0, nullptr,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &h_key, nullptr) != ERROR_SUCCESS) {
            return false;
        }

        if (RegSetValueEx(h_key, nullptr, 0, REG_SZ,
            reinterpret_cast<const BYTE*>(info::MODIAN_IME_DESC.data()),
            static_cast<DWORD>(info::MODIAN_IME_DESC.size() * sizeof(WCHAR))) != ERROR_SUCCESS) {
            close_keys(h_key, nullptr);
            return false;
        }

        if (RegCreateKeyEx(h_key, info::MODIAN_IME_REGINFO_KEY_INPROSVR32.data(), 0, nullptr,
            REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &h_sub_key, nullptr) != ERROR_SUCCESS) {
            close_keys(h_key, nullptr);
            return false;
        }

        std::vector<WCHAR> file_name(MAX_PATH);
        DWORD len = GetModuleFileNameW(modian_instance, file_name.data(), static_cast<DWORD>(file_name.size()));

        if (RegSetValueEx(h_sub_key, nullptr, 0, REG_SZ,
            reinterpret_cast<const BYTE*>(file_name.data()),
            len * sizeof(WCHAR)) == ERROR_SUCCESS) {

            if (RegSetValueEx(h_sub_key, info::MODIAN_IME_REGINFO_KEY_THREADMODEL.data(), 0, REG_SZ,
                reinterpret_cast<const BYTE*>(info::MODIAN_IME_MODEL.data()),
                static_cast<DWORD>(info::MODIAN_IME_MODEL.size() * sizeof(WCHAR))) == ERROR_SUCCESS) {
                success = true;
            }
        }

        close_keys(h_key, h_sub_key);
        return success;
    }

    void com_registration::unregister_server() {
        const std::wstring key_path = info::MODIAN_IME_REGINFO_PREFIX_CLSID.data() +
                                      util::convert_clsid_to_string(info::MODIAN_IME_CLSID);
        RegDeleteTreeW(HKEY_CLASSES_ROOT, key_path.c_str());
    }
}