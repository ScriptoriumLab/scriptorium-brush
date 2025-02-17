#pragma once

#include <Windows.h>

namespace modian::tsf::dll::util {
	inline std::wstring convert_clsid_to_string(const CLSID& clsid) {
		LPOLESTR clsid_str = nullptr;
		if (const auto hr = StringFromCLSID(clsid, &clsid_str); FAILED(hr)) {
			throw std::runtime_error{"cannot convert thi clsid"};
		}

		return clsid_str;
	}
}