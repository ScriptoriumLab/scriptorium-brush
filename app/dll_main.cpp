#include <windows.h>

#include "modian/tsf/dll/register.h"
#include "modian/tsf/dll/info/registry_info.h"

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD dw_reason, [[maybe_unused]] LPVOID pv_reserved) {
	switch (dw_reason) {
	case DLL_PROCESS_ATTACH:
		modian::infra::tsf::dll::modian_instance = h_instance;

		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	default:
		break;
	}

	return TRUE;
}
