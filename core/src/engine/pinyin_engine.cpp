#include "modian/core/engine/pinyin_engine.h"

#include <codecvt>
#include <fstream>
#include <sstream>

#include "modian/core/logger/logger_service.h"

modian::core::pinyin_engine::pinyin_engine() {
	char* userprofile{nullptr};
	size_t size = 0;

	if (const errno_t err = _dupenv_s(&userprofile, &size, "USERPROFILE"); err != 0 || userprofile == nullptr) {
		logger_service::logger()->error("Failed to retrieve USERPROFILE.");
	}

	const std::string dictionary_path = std::string(userprofile) + "/Modian/Local/pinyin_dictionary.txt";
	load_dictionary(dictionary_path);
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
