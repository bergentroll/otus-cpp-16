#include <cmath>

#include "flat.hpp"
#include "utility.hpp"

using namespace otus;
using namespace std;

Flat::Flat(string const &description) {
  float latitude, longitude, price, square, kitchen;
  int rooms, storeys, floor;

  stringstream ss { description };
  auto sepNum = count(description.begin(), description.end(), ';');

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
        "invalid latitude " + to_string(latitude) + " given");
  }

  if (longitude < 0 || longitude > 180) {
    throw ParsingError(
        "invalid longitude " + to_string(longitude) + " given");
  }

  (*this)(0) = latitude;
  (*this)(1) = longitude;
  (*this)(2) = static_cast<float>(rooms);
  (*this)(3) = price;
  (*this)(4) = square;
  (*this)(5) = kitchen;
  (*this)(6) = static_cast<float>(isNotBoundary);
}

float Flat::distanceTo(DataType const & other) const {
  auto fi1 { degToRad((*this)(0)) };
  auto fi2 { degToRad(other(0)) };
  auto la1 { degToRad((*this)(1)) };
  auto la2 { degToRad(other(1)) };

  return acos(sin(fi1) * sin(fi2) + cos(fi1) * cos(fi2) * cos(la1 - la2));
}

Flat::operator string() const {
  stringstream ss { };

  for (auto &i: *this) ss << i << ';';

  string result { ss.str() };
  result.pop_back();

  return result;
}
