#pragma once

#include <dlib/clustering.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace otus {
  constexpr int dataSize = 7;
  using DataType = dlib::matrix<float, dataSize, 1>;

  class ParsingError: public std::runtime_error {
  public:
    ParsingError(std::string const & message):
    std::runtime_error(message) { }
  };

  template <typename T>
  class InputValidator {
  public:
    class InvalidToken: public ParsingError {
    public:
      InvalidToken(std::string const & message):
      ParsingError(message) { }
    };

    InputValidator(T const & value): expected(value) { }
  private:
    T expected;

    template <typename U>
    friend std::istream & operator>>(std::istream & is, InputValidator<U> & val);
  };

  template <typename T>
  std::istream & operator>>(std::istream & is, InputValidator<T> & val) {
    char buf;
    is >> buf;
    if (buf != val.expected) {
      std::stringstream ss { };
      ss
        << "expected \"" << val.expected << '"'
        << ", but \"" << buf << "\" given";
      ss.seekp(-1);
      throw typename InputValidator<T>::InvalidToken(ss.str());
    }
    return is;
  }

  class Flat: public DataType {
  public:
    using Cluster = std::optional<unsigned long>;

    Flat() = default;

    Flat(std::string const &description);

    float distanceTo(DataType const & other) const;

    operator std::string() const;
  };
}
