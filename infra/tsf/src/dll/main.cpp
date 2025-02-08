#include <windows.h>

// TODO: maybe need to move to a global info namespace later
HINSTANCE g_h_instance{nullptr};

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD dw_reason, LPVOID pv_reserved) {
	switch (dw_reason) {
	case DLL_PROCESS_ATTACH:
		g_h_instance = h_instance;

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
	}

	return TRUE;
}
