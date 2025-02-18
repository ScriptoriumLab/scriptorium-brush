#pragma once

#include "modian/core/engine/input_engine.h"

#include <unordered_map>
#include <vector>

namespace modian::core {
	class pinyin_engine final : public input_engine {
	public:
		static pinyin_engine& get_instance(const std::string& path);
		std::vector<std::wstring> convert(const std::wstring& input) override;
		void load_dictionary(const std::string& path) override;

	private:
		explicit pinyin_engine(const std::string& path);
		std::unordered_map<std::wstring, std::vector<std::wstring>> dictionary_;
	};
}