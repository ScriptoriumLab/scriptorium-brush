#include <windows.h>

#include "scriptorium/felt/core/logger/logger_service.h"
#include "scriptorium/tsf/dll/register.h"

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD dw_reason, [[maybe_unused]] LPVOID pv_reserved) {
	switch (dw_reason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(h_instance);
		scriptorium::brush::infra::tsf::dll::scriptorium_instance = h_instance;
		break;
	case DLL_PROCESS_DETACH:
		scriptorium::felt::core::logger_service::shutdown();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}

	return TRUE;
}
