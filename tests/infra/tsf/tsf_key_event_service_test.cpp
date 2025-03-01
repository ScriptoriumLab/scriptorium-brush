#include "gtest/gtest.h"

#include <codecvt>

#include "modian/core/engine/pinyin_engine.h"
#include "modian/tsf/tsf_key_event_service.h"

class key_event_observer final : public modian::core::candidate_observer {
public:
	void on_candidate_update(const std::vector<std::wstring>& candidates) override {
		candidates_ = candidates;
	}

	std::vector<std::wstring> candidates_;
};

HRESULT typing(modian::infra::tsf::tsf_key_event_service& key_event_service, const std::wstring& input);

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

	auto event_service = modian::infra::tsf::tsf_key_event_service{engine_manager};

	HRESULT hr = typing(event_service, L"ni");
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), "你");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), "尼");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), "泥");

	hr = typing(event_service, L"hao");
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), "好");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), "号");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), "豪");


	hr = typing(event_service, L"mo");
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]),"墨");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), "莫");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), "末");

	hr = typing(event_service, L"dian");
	ASSERT_EQ(hr, S_OK);
	ASSERT_EQ(observer->candidates_.size(), 3);
	ASSERT_EQ(converter.to_bytes(observer->candidates_[0]), "点");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[1]), "店");
	ASSERT_EQ(converter.to_bytes(observer->candidates_[2]), "电");
}

HRESULT typing(modian::infra::tsf::tsf_key_event_service& key_event_service, const std::wstring& input) {
	BOOL pf_eaten = FALSE;
	HRESULT hr = S_OK;

	for (const auto& character : input) {
		ITfContext* context = nullptr;
		constexpr LPARAM l_param = 0;
		hr = key_event_service.OnKeyDown(context, character, l_param, &pf_eaten);

		if (hr != S_OK) {
			return hr;
		}
	}

	return hr;
}

