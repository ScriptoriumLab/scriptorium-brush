#include <utility>

#include "modian/manager/engine_manager.h"

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/logger/logger_service.h"

namespace modian::brush::manager {
	engine_manager::engine_manager(candidate_manager can_manager) : candidate_manager_{std::move(can_manager)} {
		engine_list_.emplace(core::lazy_load_dictionary<core::pinyin_engine>());
	}

	void engine_manager::add_new_engine(const std::pair<std::string, std::function<std::shared_ptr<core::input_engine>()>>& engine_detail) {
		engine_list_.emplace(engine_detail);
	}

	void engine_manager::select_engine(const std::string& engine_name) {
		current_engine_ = engine_list_[engine_name]();
	}

	void engine_manager::update_input_state(const wchar_t& character) {
		input_pinyin_.push_back(towlower(character));

		if (current_engine_ == nullptr) {
			current_engine_ = engine_list_.begin()->second();
		}

		if (candidate_manager_.update_candidates(current_engine_->convert(input_pinyin_))) {
			input_pinyin_.clear();
		}
	}
}
