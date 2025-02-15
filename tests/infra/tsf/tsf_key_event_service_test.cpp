#include <modian/core/pinyin_engine.h>
#include <modian/tsf/tsf_key_event_service.h>

#include "gtest/gtest.h"

TEST(key_event_service_test, should_get_candidates_when_input_is_ni) {
	modian::tsf::tsf_key_event_service event_service{std::make_shared<modian::core::pinyin_engine>(modian::core::pinyin_engine::get_instance(std::string{PROJECT_SOURCE_DIR}.append("/data/pinyin_dictionary.txt")))};

	ITfContext* context = nullptr;
	WPARAM character = 'n';
	LPARAM l_param = 0;
	BOOL pf_eaten = FALSE;

	auto hr = event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	ASSERT_EQ(hr, S_OK);

	character = 'i';
	hr = event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	ASSERT_EQ(hr, S_OK);
}