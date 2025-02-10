#include <windows.h>
#include <modian/util/log_util.h>

#include "modian/tsf/dll/register.h"
#include "modian/info/registry_info.h"

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD dw_reason, LPVOID pv_reserved) {
	modian::tsf::dll::auto_com ac;

	switch (dw_reason) {
	case DLL_PROCESS_ATTACH:
		g_h_instance = h_instance;

		modian::util::logger::init_logger();
		spdlog::info("\n{}", modian::util::logger::ascii_modian_ime);

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
