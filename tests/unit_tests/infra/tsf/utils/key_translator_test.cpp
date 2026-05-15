#include "gtest/gtest.h"

#include "modian/tsf/utils/key_translator.h"

TEST(key_translator_test, should_translate_to_backspace_command_when_user_input_is_vb_back) {
	constexpr WPARAM key{VK_BACK};
	const auto request = modian::brush::infra::tsf::utils::translate_os_key(key);

	ASSERT_EQ(request, "cmd:backspace");
}

TEST(key_translator_test, should_translate_to_right_command_when_user_input_is_vb_right) {
	constexpr WPARAM key{VK_RIGHT};
	const auto request = modian::brush::infra::tsf::utils::translate_os_key(key);

	ASSERT_EQ(request, "cmd:right");
}

TEST(key_translator_test, should_translate_to_left_command_when_user_input_is_vb_left) {
	constexpr WPARAM key{VK_LEFT};
	const auto request = modian::brush::infra::tsf::utils::translate_os_key(key);

	ASSERT_EQ(request, "cmd:left");
}

TEST(key_translator_test, should_translate_to_space_command_when_user_input_is_vb_space) {
	constexpr WPARAM key{VK_SPACE};
	const auto request = modian::brush::infra::tsf::utils::translate_os_key(key);

	ASSERT_EQ(request, "cmd:space");
}

TEST(key_translator_test, should_translate_to_normal_input_when_user_input_other_keys) {
	constexpr WPARAM key{'N'};
	const auto request = modian::brush::infra::tsf::utils::translate_os_key(key);

	ASSERT_EQ(request, "N");
}

