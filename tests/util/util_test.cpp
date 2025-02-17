#include "gtest/gtest.h"
#include "util.h"

#include "modian/info/registry_info.h"

TEST(convert_clsid_to_wchart_test, should_successfully_convert_to_wchart_when_pass_a_clsid) {
	constexpr auto res = convert_clsid_to_wchar_t(modian::tsf::dll::MODIAN_IME_CLSID);

	ASSERT_EQ(res.size(), 39);
	ASSERT_STREQ(res.data(), L"{F7A3B6D1-EC88-41A2-9F5D-7A0E3C8A7B89}\0");
}