#ifndef REGISTER_TABLE_INFO_H
#define REGISTER_TABLE_INFO_H

#include "util.h"

constexpr size_t KEY_PATH_SIZE{66};
constexpr CLSID CLSID_MODIAN_TEXT_SERVICE{0xf7a3b6d1, 0xec88, 0x41a2, {0x9f, 0x5d, 0x7a, 0xe, 0x3c, 0x8a, 0x7b, 0x89}};
inline wchar_t CLSID_KEY[KEY_PATH_SIZE]{L"SOFTWARE\\Microsoft\\CTF\\TIP\\"};
// TODO: extract concat later
inline auto concat_key_path_res = wcscat_s(CLSID_KEY, convert_clsid_to_wchar_t(CLSID_MODIAN_TEXT_SERVICE).data());

inline wchar_t LANGUAGE_PROFILE_KEY[132]{L""};
inline auto concat_lang_profile_step1 = wcscat_s(LANGUAGE_PROFILE_KEY, CLSID_KEY);
inline auto concat_lang_profile_res = wcscat_s(LANGUAGE_PROFILE_KEY, L"\\LanguageProfile\\0x00000804\\{C00E97BF-4DD6-4C08-9D8D-BA67265F4997}");

#endif //REGISTER_TABLE_INFO_H
