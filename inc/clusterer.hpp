#pragma once

#include <dlib/clustering.h>
#include <optional>
#include <sstream>
#include <vector>

namespace otus {
  using DataType = dlib::matrix<float, 7, 1>;

  class Flat {
  public:
    using Cluster = std::optional<unsigned long>;

    Flat(std::string const &description) {
      float latitude, longitude, price, square, kitchen;
      int rooms, storeys, floor;

      std::stringstream ss { description };

      /// TODO: Check input.
      char semiColon;

      ss
        >> latitude >> semiColon >> longitude >> semiColon
        >> rooms >> semiColon >> price >> semiColon
        >> square >> semiColon >> kitchen >> semiColon
        >> storeys >> semiColon >> floor >> semiColon;

      bool isNotBoundary { false };

      if (floor > 1 && floor < storeys) {
        isNotBoundary = true;
      }

      data = {
        latitude, longitude,
        static_cast<float>(rooms),
        price, square, kitchen,
        static_cast<float>(isNotBoundary) };

      std::cerr << data << std::endl;
    }

    operator DataType() const { return data; }

  private:
    DataType data;;
  };

  class Clusterer {
  public:
    Clusterer(std::vector<Flat> &data, float gamma=0.1, float accuracy=0.01):
    data(data),
    kCentroid(kernel_type(gamma), accuracy) { }

    void operator()(int numberOfClusters);

  private:
    using sample_type = DataType;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    std::vector<Flat> & data;
    dlib::kcentroid<kernel_type> kCentroid;
  };
}
