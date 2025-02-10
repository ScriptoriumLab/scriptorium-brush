#pragma once

#include <msctf.h>
#include <vector>

#include "util.h"

inline HINSTANCE g_h_instance{nullptr};

constexpr size_t CLSID_KEY_SIZE{66};
constexpr size_t LANGUAGE_PROFILE_KEY_SIZE{132};

constexpr CLSID CLSID_MODIAN_TEXT_SERVICE{0xf7a3b6d1, 0xec88, 0x41a2, {0x9f, 0x5d, 0x7a, 0xe, 0x3c, 0x8a, 0x7b, 0x89}};
inline wchar_t CLSID_KEY[CLSID_KEY_SIZE]{L"SOFTWARE\\Microsoft\\CTF\\TIP\\"};
// TODO: extract concat later
inline auto concat_key_path_res = wcscat_s(CLSID_KEY, convert_clsid_to_wchar_t(CLSID_MODIAN_TEXT_SERVICE).data());

inline wchar_t LANGUAGE_PROFILE_KEY[LANGUAGE_PROFILE_KEY_SIZE]{L""};
inline auto concat_lang_profile_step1 = wcscat_s(LANGUAGE_PROFILE_KEY, CLSID_KEY);
inline auto concat_lang_profile_res = wcscat_s(LANGUAGE_PROFILE_KEY, L"\\LanguageProfile\\0x00000804\\{C00E97BF-4DD6-4C08-9D8D-BA67265F4997}");

namespace modian::tsf::dll {
	static constexpr auto MODIAN_IME_LANG_ID{MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)};

	static constexpr WCHAR MODIAN_IME_DESC[]{L"Modian Input Method"};
	static constexpr CLSID MODIAN_IME_CLSID{0xf7a3b6d1, 0xec88, 0x41a2, {0x9f, 0x5d, 0x7a, 0xe, 0x3c, 0x8a, 0x7b, 0x89}};
	static constexpr GUID MODIAN_IME_GUID_PROFILE = {0xC00E97BF, 0x4DD6, 0x4C08, {0x9D, 0x8D, 0xBA, 0x67, 0x26, 0x5F, 0x49, 0x97}};

	static const std::vector MODIAN_SUPPORT_CATEGORIES = {
		GUID_TFCAT_TIP_KEYBOARD,
		GUID_TFCAT_DISPLAYATTRIBUTEPROVIDER,
		GUID_TFCAT_TIPCAP_UIELEMENTENABLED,
		GUID_TFCAT_TIPCAP_SECUREMODE,
		GUID_TFCAT_TIPCAP_COMLESS,
		GUID_TFCAT_TIPCAP_INPUTMODECOMPARTMENT,
		GUID_TFCAT_TIPCAP_IMMERSIVESUPPORT,
		GUID_TFCAT_TIPCAP_SYSTRAYSUPPORT,
	};
}
