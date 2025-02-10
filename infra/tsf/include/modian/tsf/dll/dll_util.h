#pragma once

#include <Windows.h>

#include "modian/info/registry_info.h"

namespace modian::tsf::dll::util {
	constexpr BYTE guid_symbols[] = {3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15};

	constexpr WCHAR hex_digits[] = L"0123456789ABCDEF";

	inline BOOL clsid_to_string(REFGUID ref_guid, _Out_writes_(39) WCHAR *p_clsid_string)
	{
		WCHAR *p_temp = p_clsid_string;
		const auto p_bytes = reinterpret_cast<const BYTE*>(&ref_guid);

		DWORD j = 0;
		p_temp[j++] = L'{';
		for (int i = 0; i < sizeof(guid_symbols) && j < (CLSID_STRLEN - 2); i++)
		{
			if (guid_symbols[i] == '-')
			{
				p_temp[j++] = L'-';
			}
			else
			{
				p_temp[j++] = hex_digits[(p_bytes[guid_symbols[i]] & 0xF0) >> 4];
				p_temp[j++] = hex_digits[(p_bytes[guid_symbols[i]] & 0x0F)];
			}
		}

		p_temp[j++] = L'}';
		p_temp[j] = L'\0';

		return TRUE;
	}
}