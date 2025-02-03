#include <new>

#include "modian/tsf/tsf_text_service.h"
#include "modian/tsf/class_factory.h"

#include <windows.h>

constexpr CLSID CLSID_MODIAN_TEXT_SERVICE = {0xf7a3b6d1, 0xec88, 0x41a2, {0x9f, 0x5d, 0x7a, 0xe, 0x3c, 0x8a, 0x7b, 0x89}};

HINSTANCE g_h_instance = nullptr;
volatile long g_server_lock = 0;
volatile long g_active_objects = 0;

extern "C" {
__declspec(dllexport) STDAPI DllCanUnloadNow() {
	return (g_server_lock == 0 && g_active_objects == 0) ? S_OK : S_FALSE;
}

__declspec(dllexport) STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
	if (!ppv) return E_POINTER;

	class_factory* pFactory = new (std::nothrow) class_factory();
	if (!pFactory) return E_OUTOFMEMORY;

	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release(); // 避免内存泄漏
	return hr;
}

__declspec(dllexport) STDAPI DllRegisterServer() {
	wchar_t clsid_str[40];
	StringFromGUID2(CLSID_MODIAN_TEXT_SERVICE, clsid_str, ARRAYSIZE(clsid_str));

	HKEY hkey;
	if (const auto key_path = L"SOFTWARE\\Microsoft\\CTF\\TIP\\{F7A3B6D1-EC88-41A2-9F5D-7A0E3C8A7B89}"; RegCreateKeyExW(HKEY_LOCAL_MACHINE, key_path, 0, nullptr, 0, KEY_WRITE, nullptr, &hkey, nullptr) != ERROR_SUCCESS) {
		return E_ACCESSDENIED;
	}

	// 设置默认值
	RegSetValueExW(hkey, L"", 0, REG_SZ, reinterpret_cast<const BYTE*>(L"Modian Input Method"), (wcslen(L"Modian Input Method") + 1) * sizeof(wchar_t));

	// 设置 Description 值
	RegSetValueExW(hkey, L"Description", 0, REG_SZ, reinterpret_cast<const BYTE*>(L"Modian Input Method"), (wcslen(L"Modian Input Method") + 1) * sizeof(wchar_t));

	RegCloseKey(hkey);

	return S_OK;
}

__declspec(dllexport) STDAPI DllUnregisterServer() {
	const auto key_path = L"SOFTWARE\\Microsoft\\CTF\\TIP\\{F7A3B6D1-EC88-41A2-9F5D-7A0E3C8A7B89}";

	return RegDeleteTreeW(HKEY_LOCAL_MACHINE, key_path) == ERROR_SUCCESS ? S_OK : S_FALSE;
}
}

BOOL APIENTRY DllMain(HMODULE h_module, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		g_h_instance = h_module;
		DisableThreadLibraryCalls(h_module);
		break;
	case DLL_PROCESS_DETACH:
		g_h_instance = nullptr;
		break;
	}
	return TRUE;
}
