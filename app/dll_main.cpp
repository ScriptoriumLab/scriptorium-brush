#include <windows.h>

#include "modian/core/logger/logger_service.h"
#include "modian/tsf/dll/register.h"
#include "modian/tsf/dll/info/registry_info.h"

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD dw_reason, [[maybe_unused]] LPVOID pv_reserved) {
	switch (dw_reason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(h_instance);
		modian::brush::infra::tsf::dll::modian_instance = h_instance;

		break;
	case DLL_PROCESS_DETACH:
		modian::brush::core::logger_service::shutdown();

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}

	return TRUE;
}
