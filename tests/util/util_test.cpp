#include "gtest/gtest.h"
#include "util.h"

constexpr CLSID CLSID_MODIAN_TEXT_SERVICE{0xf7a3b6d1, 0xec88, 0x41a2, {0x9f, 0x5d, 0x7a, 0xe, 0x3c, 0x8a, 0x7b, 0x89}};

TEST(convert_clsid_to_wchart_test, should_successfully_convert_to_wchart_when_pass_a_clsid) {
	constexpr auto res = convert_clsid_to_wchar_t(CLSID_MODIAN_TEXT_SERVICE);

	ASSERT_EQ(res.size(), 39);
	ASSERT_STREQ(res.data(), L"{F7A3B6D1-EC88-41A2-9F5D-7A0E3C8A7B89}\0");
}