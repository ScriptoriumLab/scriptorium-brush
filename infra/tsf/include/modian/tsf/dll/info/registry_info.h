#pragma once

#include <msctf.h>
#include <string_view>
#include <vector>

namespace modian::tsf::dll::info {

	static constexpr std::wstring_view MODIAN_IME_MODEL{L"Apartment"};

	static constexpr std::wstring_view MODIAN_IME_REGINFO_PREFIX_CLSID{L"CLSID\\"};
	static constexpr std::wstring_view MODIAN_REGISTRY_CLSID_ROOT_PATH{L"SOFTWARE\\Microsoft\\CTF\\TIP\\"};
	static constexpr std::wstring_view MODIAN_IME_REGINFO_KEY_INPROSVR32{L"InProcServer32"};
	static constexpr std::wstring_view MODIAN_IME_REGINFO_KEY_THREADMODEL{L"ThreadingModel"};

	static constexpr auto MODIAN_IME_LANG_ID{MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)};

	static constexpr std::wstring_view MODIAN_IME_DESC{L"Modian Input Method"};
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
