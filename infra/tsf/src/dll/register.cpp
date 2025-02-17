#include "modian/tsf/dll/register.h"

#include <msctf.h>
#include <strsafe.h>

#include "modian/info/registry_info.h"
#include "modian/tsf/dll/dll_util.h"

modian::tsf::dll::auto_com::auto_com() : b_init{false} {
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) return;
	b_init = true;
}

modian::tsf::dll::auto_com::~auto_com() {
	if (b_init) {
		CoUninitialize();
	}
}

bool modian::tsf::dll::com_registration::register_profiles() {
	HRESULT hr{S_FALSE};

	ITfInputProcessorProfileMgr* input_processor_profile_mgr{nullptr};
	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfileMgr, reinterpret_cast<void**>(&input_processor_profile_mgr));
	if (FAILED(hr)) {
		return false;
	}

	WCHAR ach_icon_file[MAX_PATH] = {'\0'};
	DWORD cch_a{0};
	cch_a = GetModuleFileName(modian_instance, ach_icon_file, MAX_PATH);
	cch_a = cch_a >= MAX_PATH ? (MAX_PATH - 1) : cch_a;
	ach_icon_file[cch_a] = '\0';

	size_t len_of_desc{0};
	hr = StringCchLength(MODIAN_IME_DESC, STRSAFE_MAX_CCH, &len_of_desc);
	if (hr != S_OK) {
		goto Exit;
	}

	// TODO: extract -12 to variable
	// hr = input_processor_profile_mgr->RegisterProfile(MODIAN_IME_CLSID, MODIAN_IME_LANG_ID, MODIAN_IME_GUID_PROFILE, MODIAN_IME_DESC, static_cast<ULONG>(len_of_desc), ach_icon_file, cch_a, static_cast<UINT>(-12), nullptr, 0, TRUE, 0);
	hr = input_processor_profile_mgr->RegisterProfile(MODIAN_IME_CLSID, MODIAN_IME_LANG_ID, MODIAN_IME_GUID_PROFILE, MODIAN_IME_DESC, static_cast<ULONG>(len_of_desc), nullptr, 0, static_cast<UINT>(-12), nullptr, 0, TRUE, 0);

	Exit:
	if (input_processor_profile_mgr) {
		input_processor_profile_mgr->Release();
	}

	return (hr == S_OK);
}

void modian::tsf::dll::com_registration::unregister_profiles() {
	HRESULT hr{S_OK};

	ITfInputProcessorProfileMgr* input_processor_profile_mgr{nullptr};
	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfileMgr, reinterpret_cast<void**>(&input_processor_profile_mgr));
	if (FAILED(hr)) {
		goto Exit;
	}

	input_processor_profile_mgr->UnregisterProfile(MODIAN_IME_CLSID, MODIAN_IME_LANG_ID, MODIAN_IME_GUID_PROFILE, 0);

	Exit:
	if (input_processor_profile_mgr) {
		input_processor_profile_mgr->Release();
	}
}

bool modian::tsf::dll::com_registration::register_categories() {
	ITfCategoryMgr* category_mgr{nullptr};
	HRESULT hr{S_OK};

	hr = CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, reinterpret_cast<void**>(&category_mgr));
	if (FAILED(hr)) {
		return false;
	}

	for (const auto& guid : MODIAN_SUPPORT_CATEGORIES) {
		hr = category_mgr->RegisterCategory(MODIAN_IME_CLSID, guid, MODIAN_IME_CLSID);
	}

	category_mgr->Release();

	return (hr == S_OK);
}

void modian::tsf::dll::com_registration::unregister_categories() {
	ITfCategoryMgr* category_mgr{nullptr};
	HRESULT hr{S_OK};

	hr = CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, reinterpret_cast<void**>(&category_mgr));
	if (FAILED(hr)) {
		return;
	}

	for (const auto& guid : MODIAN_SUPPORT_CATEGORIES) {
		category_mgr->UnregisterCategory(MODIAN_IME_CLSID, guid, MODIAN_IME_CLSID);
	}

	category_mgr->Release();
}

bool modian::tsf::dll::com_registration::register_server() {
	DWORD copied_string_len{0};
	HKEY reg_key_handle{nullptr};
	HKEY reg_sub_key_handle{nullptr};
	bool ret{false};
	const std::wstring ach_ime_key{MODIAN_IME_REGINFO_PREFIX_CLSID + util::convert_clsid_to_string(MODIAN_IME_CLSID)};
	WCHAR ach_file_name[MAX_PATH]{'\0'};

	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, ach_ime_key.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &reg_key_handle, &copied_string_len) == ERROR_SUCCESS) {
		if (RegSetValueEx(reg_key_handle, nullptr, 0, REG_SZ, reinterpret_cast<const BYTE*>(MODIAN_IME_DESC), (_countof(MODIAN_IME_DESC)) * sizeof(WCHAR)) != ERROR_SUCCESS) {
			goto Exit;
		}

		if (RegCreateKeyEx(reg_key_handle, MODIAN_IME_REGINFO_KEY_INPROSVR32.data(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &reg_sub_key_handle, &copied_string_len) == ERROR_SUCCESS) {
			copied_string_len = GetModuleFileNameW(modian_instance, ach_file_name, ARRAYSIZE(ach_file_name));
			copied_string_len = copied_string_len >= MAX_PATH - 1 ? MAX_PATH : ++copied_string_len;
			if (RegSetValueEx(reg_sub_key_handle, nullptr, 0, REG_SZ, reinterpret_cast<const BYTE*>(ach_file_name), copied_string_len * sizeof(WCHAR)) != ERROR_SUCCESS) {
				goto Exit;
			}

			if (RegSetValueEx(reg_sub_key_handle, MODIAN_IME_REGINFO_KEY_THREADMODEL, 0, REG_SZ, reinterpret_cast<const BYTE*>(MODIAN_IME_MODEL.c_str()), (MODIAN_IME_MODEL.size()) * sizeof(WCHAR)) != ERROR_SUCCESS) {
				goto Exit;
			}

			ret = TRUE;
		}
	}

	Exit:
	if (reg_sub_key_handle) {
		RegCloseKey(reg_sub_key_handle);
		reg_sub_key_handle = nullptr;
	}

	if (reg_key_handle) {
		RegCloseKey(reg_key_handle);
		reg_key_handle = nullptr;
	}

	return ret;
}

void modian::tsf::dll::com_registration::unregister_server() {
	const std::wstring ach_ime_key{MODIAN_IME_REGINFO_PREFIX_CLSID + util::convert_clsid_to_string(MODIAN_IME_CLSID)};

	recurse_delete_key(HKEY_CLASSES_ROOT, ach_ime_key.c_str());
}

LONG modian::tsf::dll::com_registration::recurse_delete_key(HKEY h_parent_key, LPCTSTR lpsz_key) {
	HKEY reg_key_handle = nullptr;
	LONG res = 0;
	FILETIME time;
	WCHAR string_buffer[256] = {'\0'};
	DWORD size = ARRAYSIZE(string_buffer);

	if (RegOpenKey(h_parent_key, lpsz_key, &reg_key_handle) != ERROR_SUCCESS) {
		return ERROR_SUCCESS;
	}

	res = ERROR_SUCCESS;
	while (RegEnumKeyEx(reg_key_handle, 0, string_buffer, &size, NULL, NULL, NULL, &time) == ERROR_SUCCESS) {
		string_buffer[ARRAYSIZE(string_buffer) - 1] = '\0';
		res = recurse_delete_key(reg_key_handle, string_buffer);
		if (res != ERROR_SUCCESS)
		{
			break;
		}
		size = ARRAYSIZE(string_buffer);
	}
	RegCloseKey(reg_key_handle);

	return res == ERROR_SUCCESS ? RegDeleteKey(h_parent_key, lpsz_key) : res;
}
