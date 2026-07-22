#pragma once

#include <msctf.h>
#include <string>
#include <array>

namespace scriptorium::brush::infra::tsf::dll::info {
	constexpr std::wstring_view SCRIPTORIUM_IME_MODEL{L"Apartment"};
	constexpr std::wstring_view SCRIPTORIUM_IME_DESC{L"Scriptorium Input Method"};

	constexpr std::wstring_view SCRIPTORIUM_IME_REGINFO_PREFIX_CLSID{L"CLSID\\"};
	constexpr std::wstring_view SCRIPTORIUM_REGISTRY_CLSID_ROOT_PATH{L"SOFTWARE\\Microsoft\\CTF\\TIP\\"};
	constexpr std::wstring_view SCRIPTORIUM_IME_REGINFO_KEY_INPROSVR32{L"InProcServer32"};
	constexpr std::wstring_view SCRIPTORIUM_IME_REGINFO_KEY_THREADMODEL{L"ThreadingModel"};

	constexpr auto SCRIPTORIUM_IME_LANG_ID{MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)};

	constexpr CLSID SCRIPTORIUM_IME_CLSID{0xf7a3b6d1, 0xec88, 0x41a2, {0x9f, 0x5d, 0x7a, 0xe, 0x3c, 0x8a, 0x7b, 0x89}};
	constexpr GUID SCRIPTORIUM_IME_GUID_PROFILE = {0xC00E97BF, 0x4DD6, 0x4C08, {0x9D, 0x8D, 0xBA, 0x67, 0x26, 0x5F, 0x49, 0x97}};

	const std::array SCRIPTORIUM_SUPPORT_CATEGORIES = {
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
