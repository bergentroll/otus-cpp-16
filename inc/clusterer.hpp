#pragma once

#include <algorithm>
#include <dlib/clustering.h>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace otus {
  constexpr int dataSize = 7;
  using DataType = dlib::matrix<float, dataSize, 1>;

  class InvalidToken: public std::runtime_error {
  public:
    InvalidToken(std::string const & message):
    std::runtime_error(message) { }
  };

  template <typename T>
  class InputValidator {
  public:
    class ConcreteInvalidToken: public InvalidToken {
    public:
      ConcreteInvalidToken(std::string const & message):
      InvalidToken(message) { }
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
      throw typename InputValidator<T>::ConcreteInvalidToken(ss.str());
    }
    return is;
  }

  class Flat {
  public:
    using Cluster = std::optional<unsigned long>;

    Flat(std::string const &description) {
      float latitude, longitude, price, square, kitchen;
      int rooms, storeys, floor;

      std::stringstream ss { description };

      InputValidator<char> semiColon { ';' };

      ss
        >> latitude >> semiColon >> longitude >> semiColon
        >> rooms >> semiColon >> price >> semiColon
        >> square >> semiColon >> kitchen >> semiColon
        >> floor >> semiColon >> storeys;

      bool isNotBoundary { false };

      if (floor > 1 && floor < storeys) {
        isNotBoundary = true;
      }

      data = {
        latitude, longitude,
        static_cast<float>(rooms),
        price, square, kitchen,
        static_cast<float>(isNotBoundary) };
    }

    operator DataType() const { return data; }

  private:
    DataType data;;
  };

  class Clusterer {
  public:
    Clusterer(
        std::vector<Flat> & data,
        int numberOfClusters,
        float gamma=0.1,
        float accuracy=0.01):
    kMeans(dlib::kcentroid(KernelType(gamma), accuracy)) {
      this->data.reserve(data.size());
      // TODO Copy data.
      std::transform(
          data.begin(),
          data.end(),
          std::back_inserter(this->data),
          [](auto const &item) -> DataType {
            return static_cast<DataType>(item);
          });

      normalizeData();
      train(numberOfClusters);
    }

    Clusterer(std::string const & filename):
    kMeans(dlib::kcentroid(KernelType())) {
      // TODO Handle errors
      dlib::deserialize(filename + fileSuffix)
        >> kMeans >> k >> a;
    }

    void save(std::string const & filename) {
      // TODO Handle errors
      dlib::serialize(filename + fileSuffix)
        << kMeans << k << a;
    }

    void operator()(Flat const & flat) const {
      DataType item = static_cast<DataType>(flat);
      normalize(item);
      std::cerr << kMeans(item) << std::endl;
    }

  private:
    using KernelType = dlib::radial_basis_kernel<DataType>;
    using DataSizeType = decltype(DataType().size());

    std::string const fileSuffix { ".dat" };
    std::vector<DataType> data;
    dlib::kkmeans<KernelType> kMeans;
    /// For normalization function y = kx - a.
    std::vector<float> k, a;

    void normalizeData();

    void normalize(DataType & item) const {
        for (DataSizeType i { }; i < item.size(); ++i)
          item(i) = k[i] * (item(i) - a[i]);
    }

    void train(int numberOfClusters);
  };
}
