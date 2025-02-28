#include "gtest/gtest.h"

#include "modian/core/engine/pinyin_engine.h"
#include "modian/tsf/tsf_key_event_service.h"

#include <codecvt>

class key_event_observer final : public modian::core::candidate_observer {
public:
	void on_candidate_update(const std::vector<std::wstring>& candidates) override {
		candidates_ = candidates;
	}

	std::vector<std::wstring> candidates_;
};

TEST(key_event_service_test, should_get_candidates_when_input_is_ni) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	auto observer = std::make_shared<key_event_observer>();

	modian::manager::candidate_manager candidate_manager;
	candidate_manager.add_observer(observer);

	modian::manager::engine_manager engine_manager{candidate_manager};
	engine_manager.add_new_engine("test pinyin engine", []() {
		return std::make_shared<modian::core::pinyin_engine>(modian::core::pinyin_engine::get_instance(std::string{PROJECT_SOURCE_DIR}.append("/data/pinyin_dictionary.txt")));
	});
	engine_manager.select_engine("test pinyin engine");

	auto event_service = modian::tsf::tsf_key_event_service{engine_manager};

	ITfContext* context = nullptr;
	LPARAM l_param = 0;
	BOOL pf_eaten = FALSE;
	HRESULT hr;

	WPARAM character = 'n';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'i';
	hr = event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), std::string{"你"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), std::string{"尼"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), std::string{"泥"});

	character = 'h';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'a';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'o';
	hr = event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), std::string{"好"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), std::string{"号"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), std::string{"豪"});


	character = 'm';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'o';
	hr = event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), std::string{"墨"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), std::string{"莫"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), std::string{"末"});

	character = 'd';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'i';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'a';
	event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	character = 'n';
	hr = event_service.OnKeyDown(context, character, l_param, &pf_eaten);
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), std::string{"点"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), std::string{"店"});
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), std::string{"电"});
}