#include <new>

#include "scriptorium/felt/infra/logger/spdlog_logger.h"
#include "scriptorium/felt/core/logger/logger_service.h"
#include "scriptorium/tsf/dll/info/registry_info.h"
#include "scriptorium/tsf/class_factory.h"
#include "scriptorium/tsf/dll/register.h"
#include "scriptorium/tsf/dll/dll_util.h"


STDAPI DllCanUnloadNow() {
	scriptorium::felt::core::logger_service::logger()->info("Start unloading...");
	return (scriptorium::brush::infra::tsf::g_server_lock == 0 && scriptorium::brush::infra::tsf::g_active_objects == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
    scriptorium::felt::core::logger_service::update_logger([](){
        return std::make_shared<scriptorium::felt::infra::logger::spdlog_logger>("brush");
    });

    scriptorium::felt::core::logger_service::logger()->info("Getting class object...");

    if (!ppv) return E_POINTER;
    *ppv = nullptr;

    if (!IsEqualGUID(rclsid, scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    auto* p_factory = new (std::nothrow) scriptorium::brush::infra::tsf::class_factory();
    if (!p_factory) return E_OUTOFMEMORY;

    const HRESULT hr = p_factory->QueryInterface(riid, ppv);

    p_factory->Release();

    return hr;
}

STDAPI DllUnregisterServer() {
    scriptorium::felt::core::logger_service::logger()->info("Unregistering Scriptorium IME dll...");

    scriptorium::brush::infra::tsf::dll::com_registration::unregister_profiles();
    scriptorium::brush::infra::tsf::dll::com_registration::unregister_categories();
    scriptorium::brush::infra::tsf::dll::com_registration::unregister_server();

    const std::wstring key_path = std::wstring(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_REGISTRY_CLSID_ROOT_PATH)
                                + scriptorium::brush::infra::tsf::dll::util::convert_clsid_to_string(scriptorium::brush::infra::tsf::dll::info::SCRIPTORIUM_IME_CLSID);

    const LSTATUS status = RegDeleteTreeW(HKEY_LOCAL_MACHINE, key_path.c_str());

    scriptorium::felt::core::logger_service::logger()->info("Successfully unregister Scriptorium IME dll");

    scriptorium::felt::core::logger_service::shutdown();

    return (status == ERROR_SUCCESS || status == ERROR_FILE_NOT_FOUND) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
    scriptorium::felt::core::logger_service::update_logger([](){
        return std::make_shared<scriptorium::felt::infra::logger::spdlog_logger>("brush");
    });

    scriptorium::felt::core::logger_service::print_logo();
	scriptorium::felt::core::logger_service::logger()->info("Registering Scriptorium IME dll...");

    if (!scriptorium::brush::infra::tsf::dll::com_registration::register_server()
     || !scriptorium::brush::infra::tsf::dll::com_registration::register_profiles()
     || !scriptorium::brush::infra::tsf::dll::com_registration::register_categories()) {

        scriptorium::felt::core::logger_service::logger()->error("Failed to register Scriptorium IME dll!");

        // 注册失败回滚
        DllUnregisterServer();
        return E_FAIL;
    }

    scriptorium::felt::core::logger_service::logger()->info("Successfully register Scriptorium IME dll");
    return S_OK;
}
