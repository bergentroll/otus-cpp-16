#pragma once

#include <cctype>
#include <cmath>
#include <string>

namespace otus {
  inline std::string uncapitalize(std::string_view input) {
    std::string result { input };
    if (result.size()) result[0] = std::tolower(result[0]);
    return result;
  }

  inline float degToRad(float angle) { return angle * M_PI / 180; }
}
