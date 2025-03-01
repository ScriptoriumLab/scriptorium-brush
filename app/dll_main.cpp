#include <windows.h>

#include "modian/tsf/dll/register.h"
#include "modian/tsf/dll/info/registry_info.h"

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD dw_reason, [[maybe_unused]] LPVOID pv_reserved) {
	modian::infra::tsf::dll::auto_com ac;

	switch (dw_reason) {
	case DLL_PROCESS_ATTACH:
		modian::infra::tsf::dll::modian_instance = h_instance;

		/**
		 * TODO: when introduce multi thread, need to add create critical logic:
		 * if (!InitializeCriticalSectionAndSpinCount(&cs, 0)) {
		 *     return FALSE;
		 * }
		 */

		/**
		 * TODO: when need to create a UI
		 * if (!register_window_class()) {
		 *     return FALSE;
		 * }
		 */

		break;
	case DLL_PROCESS_DETACH:
		/**
		 * TODO: when introduce multi thread, need to add delete critical section logic:
		 * DeleteCriticalSection(cs);
		 */
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}
