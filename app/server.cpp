#include <modian/ui/core/platform/ui_platform.h>

#include "modian/logger/spdlog_logger.h"
#include "modian/core/logger/logger_service.h"
#include "modian/tsf/dll/info/registry_info.h"
#include "modian/tsf/class_factory.h"
#include "modian/tsf/dll/register.h"
#include "modian/tsf/dll/dll_util.h"

volatile long modian::infra::tsf::g_server_lock{0};
volatile long modian::infra::tsf::g_active_objects{0};

STDAPI DllCanUnloadNow() {
	modian::core::logger_service::logger()->info("Start unloading...");
	modian::infra::ui::core::platform::ui_platform::instance()->stop_ui_thread();
	return (modian::infra::tsf::g_server_lock == 0 && modian::infra::tsf::g_active_objects == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
	modian::core::logger_service::update_logger([](){ return std::make_shared<modian::infra::logger::spdlog_logger>(); });

	modian::core::logger_service::logger()->info("Getting class object...");
	if (!ppv) return E_POINTER;

	auto* pFactory = new (std::nothrow) modian::infra::tsf::class_factory();
	if (!pFactory) return E_OUTOFMEMORY;

	const HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release(); // 避免内存泄漏
	return hr;
}

STDAPI DllUnregisterServer() {
	modian::core::logger_service::logger()->info("Unregistering Modian IME dll...");
	modian::infra::ui::core::platform::ui_platform::instance()->stop_ui_thread();

	modian::infra::tsf::dll::com_registration::unregister_profiles();
	modian::infra::tsf::dll::com_registration::unregister_categories();
	modian::infra::tsf::dll::com_registration::unregister_server();

	const auto hr = RegDeleteTreeW(
		HKEY_LOCAL_MACHINE,
		(std::wstring(modian::infra::tsf::dll::info::MODIAN_REGISTRY_CLSID_ROOT_PATH)
               + modian::infra::tsf::dll::util::convert_clsid_to_string(modian::infra::tsf::dll::info::MODIAN_IME_CLSID)).c_str()
    );

	modian::core::logger_service::logger()->info("Successfully unregister Modian IME dll");

	return hr == ERROR_SUCCESS ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
	modian::core::logger_service::update_logger([](){ return std::make_shared<modian::infra::logger::spdlog_logger>(); });

    modian::core::logger_service::logger()->info(modian::core::ascii_modian_ime);
	modian::core::logger_service::logger()->info("Registering Modian IME dll...");

	if (!modian::infra::tsf::dll::com_registration::register_server()
	 || !modian::infra::tsf::dll::com_registration::register_profiles()
	 || !modian::infra::tsf::dll::com_registration::register_categories()) {
		modian::core::logger_service::logger()->error("Failed to register Modian IME dll!");

		DllUnregisterServer();
		return E_FAIL;
	}

	modian::core::logger_service::logger()->info("Successfully register Modian IME dll");

	return S_OK;
}