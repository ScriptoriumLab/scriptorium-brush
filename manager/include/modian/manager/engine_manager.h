#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

#include "candidate_manager.h"
#include "modian/core/engine/input_engine.h"

namespace modian::brush::manager {
	class engine_manager {
	public:
		explicit engine_manager(candidate_manager can_manager);

		void add_new_engine(const std::pair<std::string, std::function<std::shared_ptr<core::input_engine>()>>& engine_detail);
		void select_engine(const std::string& engine_name);

		void update_input_state(const wchar_t& character);
	private:
		std::unordered_map<std::string, std::function<std::shared_ptr<core::input_engine>()>> engine_list_;
		std::shared_ptr<core::input_engine> current_engine_{nullptr};

		std::wstring input_pinyin_;

		candidate_manager candidate_manager_;
	};
}