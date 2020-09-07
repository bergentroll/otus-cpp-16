#pragma once

#include <cmath>
#include <dlib/clustering.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility.hpp>

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

    Flat(std::string const &description) {
      float latitude, longitude, price, square, kitchen;
      int rooms, storeys, floor;

      std::stringstream ss { description };
      auto sepNum = std::count(description.begin(), description.end(), ';');

      InputValidator<char> semiColon { ';' };

      bool isNotBoundary { false };

      ss
        >> latitude >> semiColon >> longitude >> semiColon
        >> rooms >> semiColon >> price >> semiColon
        >> square >> semiColon >> kitchen >> semiColon;

      if (sepNum == 7) {
        ss >> floor >> semiColon >> storeys;
        if (floor > 1 && floor < storeys) isNotBoundary = true;
      }
      else if (sepNum == 6) {
        ss >> isNotBoundary;
      }
      else {
        throw ParsingError("invalid fileds number");
      }

      if (latitude < -90 || latitude > 90) {
        throw ParsingError(
            "invalid latitude " + std::to_string(latitude) + " given");
      }

      if (longitude < 0 || longitude > 180) {
        throw ParsingError(
            "invalid longitude " + std::to_string(longitude) + " given");
      }


      (*this)(0) = latitude;
      (*this)(1) = longitude;
      (*this)(2) = static_cast<float>(rooms);
      (*this)(3) = price;
      (*this)(4) = square;
      (*this)(5) = kitchen;
      (*this)(6) = static_cast<float>(isNotBoundary);
    }

    //operator DataType() const { return data; }
    float distanceTo(DataType const & other) const {
      using namespace std;

      auto fi1 { degToRad((*this)(0)) };
      auto fi2 { degToRad(other(0)) };
      auto la1 { degToRad((*this)(1)) };
      auto la2 { degToRad(other(1)) };

      return acos(sin(fi1) * sin(fi2) + cos(fi1) * cos(fi2) * cos(la1 - la2));
    }


    operator std::string() const {
      std::stringstream ss { };

      for (auto &i: *this) {
        ss << i << ';';
      }

      std::string result { ss.str() };
      result.pop_back();

      return result;
    }
  };
}
