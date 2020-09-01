#pragma once

#include <stdexcept>
#include <string>
#include <tuple>

namespace otus {
  class InvalidArgument: public std::logic_error {
  public:
    explicit InvalidArgument(const std::string &input):
    std::logic_error(input) { }
  };

  std::pair<int,std::string> get_args(int argc, char const *argv[]);
}
