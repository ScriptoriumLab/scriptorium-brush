#pragma once

#include "modian/core/input_engine.h"

#include <unordered_map>
#include <vector>

namespace modian::core {
	class pinyin_engine : public input_engine {
	public:
		std::vector<std::wstring> convert(const std::wstring& input) override;
		void load_dictionary(const std::string& path) override;

	private:
		std::unordered_map<std::wstring, std::vector<std::wstring>> dictionary_;
	};
}