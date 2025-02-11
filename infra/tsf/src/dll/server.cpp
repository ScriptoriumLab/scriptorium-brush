#include <modian/util/log_util.h>
#include <spdlog/spdlog.h>

#include "../../../../cmake-build-debug/_deps/spdlog-src/include/spdlog/fmt/bundled/chrono.h"
#include "modian/info/registry_info.h"
#include "modian/tsf/class_factory.h"
#include "modian/tsf/dll/register.h"

volatile long g_server_lock{0};
volatile long g_active_objects{0};

STDAPI DllCanUnloadNow() {
	return (g_server_lock == 0 && g_active_objects == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
	spdlog::info("Getting class object...");
	if (!ppv) return E_POINTER;

	auto* pFactory = new (std::nothrow) class_factory();
	if (!pFactory) return E_OUTOFMEMORY;

	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	spdlog::info("Class factory returned with {:x}", hr);
	pFactory->Release(); // 避免内存泄漏
	return hr;
}

STDAPI DllUnregisterServer() {
	spdlog::info("Unregistering Modian IME dll...");

	modian::tsf::dll::com_registration::unregister_profiles();
	modian::tsf::dll::com_registration::unregister_categories();
	modian::tsf::dll::com_registration::unregister_server();

	const auto hr = RegDeleteTreeW(HKEY_LOCAL_MACHINE, CLSID_KEY);

	spdlog::info("Successfully unregister Modian IME dll");
	// TODO: should shutdown spdlog here, but this will make tests fail, fix this later
	// spdlog::shutdown();

	return hr == ERROR_SUCCESS ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
	modian::util::logger::init_logger();
	spdlog::info("\n{}", modian::util::logger::ascii_modian_ime);

	spdlog::info("Registering Modian IME dll...");

	if (!modian::tsf::dll::com_registration::register_server()
	 || !modian::tsf::dll::com_registration::register_profiles()
	 || !modian::tsf::dll::com_registration::register_categories()) {
		spdlog::error("Failed to register Modian IME dll!");

		DllUnregisterServer();
		return E_FAIL;
	}

	spdlog::info("Successfully register Modian IME dll");

	return S_OK;
}