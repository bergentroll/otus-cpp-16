#pragma once

#include <cctype>
#include <string>

namespace otus {
  inline std::string uncapitalize(std::string_view input) {
    std::string result { input };
    if (result.size()) result[0] = std::tolower(result[0]);
    return result;
  }
}
