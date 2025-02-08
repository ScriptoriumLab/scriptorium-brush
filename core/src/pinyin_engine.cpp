#include "modian/core/pinyin_engine.h"

#include <codecvt>
#include <fstream>
#include <sstream>

// TODO: change to get no param
modian::core::pinyin_engine& modian::core::pinyin_engine::get_instance(const std::string& path) {
	static pinyin_engine instance{path};
	return instance;
}

std::vector<std::wstring> modian::core::pinyin_engine::convert(const std::wstring& input) {
	if (auto it = dictionary_.find(input); it != dictionary_.end()) {
		return it->second;
	}
	return {};
}

void modian::core::pinyin_engine::load_dictionary(const std::string& path) {
	std::wifstream file(path);
	file.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));

	std::wstring line;
	while(std::getline(file, line)) {
		std::wistringstream iss(line);
		if (std::wstring pinyin; iss >> pinyin) {
			std::wstring word;
			auto& words = dictionary_[pinyin];
			while (iss >> word) {
				words.emplace_back(word);
			}
		}
	}
}

modian::core::pinyin_engine::pinyin_engine(const std::string& path) {
	// TODO: 考虑未来异步加载词库
	this->pinyin_engine::load_dictionary(path);
}
