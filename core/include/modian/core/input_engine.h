#ifndef INPUT_ENGINE_H
#define INPUT_ENGINE_H

#include <vector>
#include <string>

namespace modian::core {
	class input_engine {
  	public:
          virtual ~input_engine() = default;

          virtual std::vector<std::wstring> convert(const std::wstring& input) = 0;

          virtual void load_dictionary(const std::string& path) = 0;
	};
}

#endif //INPUT_ENGINE_H
