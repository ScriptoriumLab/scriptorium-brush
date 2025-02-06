#include <new>
#include <windows.h>

#include "modian/info/register_table_info.h"

#include "modian/tsf/tsf_text_service.h"
#include "modian/tsf/class_factory.h"

HINSTANCE g_h_instance{nullptr};
volatile long g_server_lock{0};
volatile long g_active_objects{0};

extern "C" {
STDAPI DllCanUnloadNow() {
	return (g_server_lock == 0 && g_active_objects == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
	if (!ppv) return E_POINTER;

	auto* pFactory = new (std::nothrow) class_factory();
	if (!pFactory) return E_OUTOFMEMORY;

	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release(); // 避免内存泄漏
	return hr;
}

STDAPI DllRegisterServer() {
	// 注册输入法
	HKEY hkey;
	if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, CLSID_KEY, 0, nullptr, 0, KEY_WRITE, nullptr, &hkey, nullptr) != ERROR_SUCCESS) {
		return E_ACCESSDENIED;
	}

	// 设置输入法描述
	const auto description = L"Modian";
	RegSetValueExW(hkey, L"Description", 0, REG_SZ, reinterpret_cast<const BYTE*>(description), (wcslen(description) + 1) * sizeof(wchar_t));

	// 注册语言配置文件
	if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, LANGUAGE_PROFILE_KEY, 0, nullptr, 0, KEY_WRITE, nullptr, &hkey, nullptr) != ERROR_SUCCESS) {
		return E_ACCESSDENIED;
	}

	// 设置语言配置文件的描述
	RegSetValueExW(hkey, L"Description", 0, REG_SZ, reinterpret_cast<const BYTE*>(description), (wcslen(description) + 1) * sizeof(wchar_t));

	// 设置输入法的图标文件路径（如果有）
	const wchar_t* icon_file = L"C:\\Path\\To\\Your\\Icon.ico"; // 请替换为实际路径
	RegSetValueExW(hkey, L"IconFile", 0, REG_SZ, reinterpret_cast<const BYTE*>(icon_file), (wcslen(icon_file) + 1) * sizeof(wchar_t));

	// 设置语言配置文件的启用状态
	DWORD enable = 1; // 1 表示启用，0 表示禁用
	RegSetValueExW(hkey, L"Enable", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&enable), sizeof(enable));

	RegCloseKey(hkey);

	return S_OK;
}

STDAPI DllUnregisterServer() {
	return RegDeleteTreeW(HKEY_LOCAL_MACHINE, CLSID_KEY) == ERROR_SUCCESS ? S_OK : S_FALSE;
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
	default:;
	}
	return TRUE;
}
