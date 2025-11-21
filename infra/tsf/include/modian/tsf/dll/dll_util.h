#pragma once

#include <stdexcept>
#include <string>
#include <Windows.h>

namespace modian::infra::tsf::dll::util {
	constexpr size_t GUID_STRING_BUFFER_SIZE{39};
	inline std::wstring convert_guid_to_string(const GUID& guid) {
		wchar_t guid_str[GUID_STRING_BUFFER_SIZE]{};

		if (StringFromGUID2(guid, guid_str, ARRAYSIZE(guid_str)) == 0) {
			throw std::runtime_error{"Failed to convert GUID to string"};
		}

		return guid_str;
	}

	inline std::wstring convert_clsid_to_string(const CLSID& clsid) {
		return convert_guid_to_string(clsid);
	}
}