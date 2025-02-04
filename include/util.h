#ifndef UTIL_H
#define UTIL_H

#include <Windows.h>
#include <vector>
#include <array>

// 将一个 4 位数字转换为对应的十六进制宽字符（大写字母）
constexpr wchar_t nibble_to_wchar(uint8_t nibble) {
	return nibble < 10 ? L'0' + nibble : L'A' + (nibble - 10);
}

constexpr std::array<wchar_t, 39> convert_clsid_to_wchar_t(const CLSID& guid) {
	// 格式：{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
	// 总长度 38 个字符，加上结尾的 null 字符共 39
	std::array<wchar_t, 39> str{};

	str[0] = L'{';

	// Data1: 8 个十六进制数字，位于位置 1-8
	for (int i = 0; i < 8; ++i) {
		int shift = (7 - i) * 4;
		uint8_t nib = static_cast<uint8_t>((guid.Data1 >> shift) & 0xF);
		str[1 + i] = nibble_to_wchar(nib);
	}
	str[9] = L'-';

	// Data2: 4 个十六进制数字，位置 10-13
	for (int i = 0; i < 4; ++i) {
		int shift = (3 - i) * 4;
		uint8_t nib = static_cast<uint8_t>((guid.Data2 >> shift) & 0xF);
		str[10 + i] = nibble_to_wchar(nib);
	}
	str[14] = L'-';

	// Data3: 4 个十六进制数字，位置 15-18
	for (int i = 0; i < 4; ++i) {
		int shift = (3 - i) * 4;
		uint8_t nib = static_cast<uint8_t>((guid.Data3 >> shift) & 0xF);
		str[15 + i] = nibble_to_wchar(nib);
	}
	str[19] = L'-';

	// Data4: 前 2 字节形成第四组，共 4 个十六进制数字，位置 20-23
	for (int i = 0; i < 4; ++i) {
		uint8_t byte = guid.Data4[i];
		str[20 + 2 * i]     = nibble_to_wchar((byte >> 4) & 0xF);
		str[20 + 2 * i + 1] = nibble_to_wchar(byte & 0xF);
	}
	str[24] = L'-';

	// Data4: 剩下 6 字节形成第五组，共 12 个十六进制数字，位置 23-34
	for (int i = 2; i < 8; ++i) {
		uint8_t byte = guid.Data4[i];
		int pos = 25 + (i - 2) * 2;
		str[pos]     = nibble_to_wchar((byte >> 4) & 0xF);
		str[pos + 1] = nibble_to_wchar(byte & 0xF);
	}
	str[37] = L'}';
	str[38] = L'\0';

	return str;
}

// TODO: 有时间可以加上 concept 限制能够转为 span 的类型，做提前检查
template <typename X, typename Y>
std::vector<wchar_t> concat(const X& a, const Y& b) {
	const auto A = std::span<const wchar_t>(a);
	const auto B = std::span<const wchar_t>(b);
	const size_t L = A.size();
	const size_t R = B.size();

	std::vector<wchar_t> res(L + R - 1);
	for (size_t i = 0; i < L - 1; ++i) {
		res[i] = A[i];
	}
	for (size_t i = 0; i < R; ++i) {
		res[L - 1 + i] = B[i];
	}
	return res;
}

#endif //UTIL_H
