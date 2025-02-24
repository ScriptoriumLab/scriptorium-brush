#include "modian/manager/engine_manager.h"

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/logger/logger_service.h"

modian::manager::engine_manager::engine_manager() {
	engine_list_.emplace("pinyin engine", []() -> std::shared_ptr<core::input_engine> {
		char* userprofile{nullptr};
		size_t size = 0;

		if (const errno_t err = _dupenv_s(&userprofile, &size, "USERPROFILE"); err != 0 || userprofile == nullptr) {
			core::logger_service::logger()->error("Failed to retrieve USERPROFILE.");
			return std::shared_ptr<core::input_engine>{};
		}

		const std::string dictionary_path = std::string(userprofile) + "/Modian/Local/pinyin_dictionary.txt";

		return std::make_shared<core::pinyin_engine>(core::pinyin_engine::get_instance(dictionary_path));
	});
}

void modian::manager::engine_manager::add_new_engine(const std::string& engine_name, const std::function<std::shared_ptr<core::input_engine>()>& lazy_load_engine) {
	engine_list_.emplace(engine_name, lazy_load_engine);
}

void modian::manager::engine_manager::select_engine(const std::string& engine_name) {
	current_engine_ = engine_list_[engine_name]();
}

std::shared_ptr<modian::core::input_engine> modian::manager::engine_manager::get_engine() {
	if (current_engine_ == nullptr) {
		current_engine_ = engine_list_.begin()->second();
	}

	return current_engine_;
}
