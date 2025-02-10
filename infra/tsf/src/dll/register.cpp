#include "modian/tsf/dll/register.h"

#include <msctf.h>
#include <iostream>
#include <strsafe.h>

#include "modian/info/registry_info.h"

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
	DWORD cch_a;
	cch_a = GetModuleFileName(g_h_instance, ach_icon_file, MAX_PATH);
	cch_a = cch_a >= MAX_PATH ? (MAX_PATH - 1) : cch_a;
	ach_icon_file[cch_a] = '\0';

	size_t len_of_desc{0};
	hr = StringCchLength(MODIAN_IME_DESC, STRSAFE_MAX_CCH, &len_of_desc);
	if (hr != S_OK) {
		goto Exit;
	}

	// TODO: extract -12 to variable
	hr = input_processor_profile_mgr->RegisterProfile(MODIAN_IME_CLSID, MODIAN_IME_LANG_ID, MODIAN_IME_GUID_PROFILE, MODIAN_IME_DESC, static_cast<ULONG>(len_of_desc), ach_icon_file, cch_a, static_cast<UINT>(-12), nullptr, 0, true, 0);

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
