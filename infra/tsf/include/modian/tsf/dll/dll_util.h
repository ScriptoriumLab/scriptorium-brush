#pragma once

#include <stdexcept>
#include <string>
#include <Windows.h>

namespace modian::infra::tsf::dll::util {
	constexpr int GUID_MAX_LEN{39};

	inline std::wstring convert_guid_to_string(const GUID& guid) {
		wchar_t guid_str[GUID_MAX_LEN];
		if (const auto res = StringFromGUID2(guid, guid_str, GUID_MAX_LEN); res == 0) {
			throw std::runtime_error{"Cannot convert this guid"};
		}

		return guid_str;
	}

	inline std::wstring convert_clsid_to_string(const CLSID& clsid) {
		LPOLESTR clsid_str = nullptr;
		if (const auto hr = StringFromCLSID(clsid, &clsid_str); FAILED(hr)) {
			throw std::runtime_error{"cannot convert thi clsid"};
		}

		return clsid_str;
	}
}