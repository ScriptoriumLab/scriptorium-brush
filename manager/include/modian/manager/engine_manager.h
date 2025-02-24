#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

#include "modian/core/engine/input_engine.h"

namespace modian::manager {
	class engine_manager {
	public:
		engine_manager();
		void add_new_engine(const std::string& engine_name, const std::function<std::shared_ptr<core::input_engine>()>& lazy_load_engine);
		void select_engine(const std::string& engine_name);
		std::shared_ptr<core::input_engine> get_engine();
	private:
		std::unordered_map<std::string, std::function<std::shared_ptr<core::input_engine>()>> engine_list_;
		std::shared_ptr<core::input_engine> current_engine_{nullptr};
	};
}