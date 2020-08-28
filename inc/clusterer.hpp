#pragma once

#include <dlib/clustering.h>
#include <optional>
#include <vector>

namespace otus {
  class Flat {
  public:
    using Cluster = std::optional<unsigned long>;

    Flat(std::string const &description) {
    }

  private:
    Cluster cluster { };
    // TODO Consider to use dlib::matrix.
    float latitude, longitude, price, square, kitchen;
    int rooms, storeys, floor;
  };

  class Clusterer {
  public:
    Clusterer(std::vector<Point> &data, float gamma=0.1, float accuracy=0.01):
    data(data),
    kCentroid(kernel_type(gamma), accuracy) { }

    void operator()(int numberOfClusters);

  private:
    using sample_type = dlib::matrix<float, 2, 1>;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    std::vector<Point> & data;
    dlib::kcentroid<kernel_type> kCentroid;
  };
}
