#include <atomic>
#include <new>

#include "modian/logger/spdlog_logger.h"
#include "modian/core/logger/logger_service.h"
#include "modian/tsf/dll/info/registry_info.h"
#include "modian/tsf/class_factory.h"
#include "modian/tsf/dll/register.h"
#include "modian/tsf/dll/dll_util.h"


STDAPI DllCanUnloadNow() {
	modian::brush::core::logger_service::logger()->info("Start unloading...");
	return (modian::brush::infra::tsf::g_server_lock == 0 && modian::brush::infra::tsf::g_active_objects == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
    modian::brush::core::logger_service::update_logger([](){
        return std::make_shared<modian::brush::infra::logger::spdlog_logger>();
    });

    modian::brush::core::logger_service::logger()->info("Getting class object...");

    if (!ppv) return E_POINTER;
    *ppv = nullptr;

    if (!IsEqualGUID(rclsid, modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    auto* p_factory = new (std::nothrow) modian::brush::infra::tsf::class_factory();
    if (!p_factory) return E_OUTOFMEMORY;

    const HRESULT hr = p_factory->QueryInterface(riid, ppv);

    p_factory->Release();

    return hr;
}

STDAPI DllUnregisterServer() {
    modian::brush::core::logger_service::logger()->info("Unregistering Modian IME dll...");

    modian::brush::infra::tsf::dll::com_registration::unregister_profiles();
    modian::brush::infra::tsf::dll::com_registration::unregister_categories();
    modian::brush::infra::tsf::dll::com_registration::unregister_server();

    const std::wstring key_path = std::wstring(modian::brush::infra::tsf::dll::info::MODIAN_REGISTRY_CLSID_ROOT_PATH)
                                + modian::brush::infra::tsf::dll::util::convert_clsid_to_string(modian::brush::infra::tsf::dll::info::MODIAN_IME_CLSID);

    const LSTATUS status = RegDeleteTreeW(HKEY_LOCAL_MACHINE, key_path.c_str());

    modian::brush::core::logger_service::logger()->info("Successfully unregister Modian IME dll");

    modian::brush::core::logger_service::shutdown();

    return (status == ERROR_SUCCESS || status == ERROR_FILE_NOT_FOUND) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
    modian::brush::core::logger_service::update_logger([](){
        return std::make_shared<modian::brush::infra::logger::spdlog_logger>();
    });

    modian::brush::core::logger_service::print_logo();
	modian::brush::core::logger_service::logger()->info("Registering Modian IME dll...");

    if (!modian::brush::infra::tsf::dll::com_registration::register_server()
     || !modian::brush::infra::tsf::dll::com_registration::register_profiles()
     || !modian::brush::infra::tsf::dll::com_registration::register_categories()) {

        modian::brush::core::logger_service::logger()->error("Failed to register Modian IME dll!");

        // 注册失败回滚
        DllUnregisterServer();
        return E_FAIL;
    }

    modian::brush::core::logger_service::logger()->info("Successfully register Modian IME dll");
    return S_OK;
}