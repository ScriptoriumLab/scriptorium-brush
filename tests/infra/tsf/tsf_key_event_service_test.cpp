#include "gtest/gtest.h"

#include "modian/core/engine/pinyin_engine.h"
#include "modian/tsf/tsf_key_event_service.h"

TEST(key_event_service_test, should_get_candidates_when_input_is_ni) {
	modian::manager::engine_manager engine_manager;
	engine_manager.add_new_engine("test pinyin engine", []() {
		return std::make_shared<modian::core::pinyin_engine>(modian::core::pinyin_engine::get_instance(std::string{PROJECT_SOURCE_DIR}.append("/data/pinyin_dictionary.txt")));
	});
	engine_manager.select_engine("test pinyin engine");
	modian::tsf::tsf_key_event_service event_service{engine_manager};

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