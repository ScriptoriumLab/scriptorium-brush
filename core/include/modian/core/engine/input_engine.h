#pragma once

#include <vector>
#include <string>

namespace modian::core {
	class input_engine {
  	public:
          virtual ~input_engine() = default;

          virtual std::vector<std::wstring> convert(const std::wstring& input) = 0;
	};
}