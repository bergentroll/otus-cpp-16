#pragma once

#include <algorithm>
#include <dlib/clustering.h>
#include <iostream>
#include <string>
#include <vector>

#include "flat.hpp"

namespace otus {
  class Clusterer {
  public:
    Clusterer(
        std::vector<Flat> const & data,
        int numberOfClusters,
        float gamma=0.1,
        float accuracy=0.01):
    kMeans(dlib::kcentroid(KernelType(gamma), accuracy)) {
      this->data.reserve(data.size());
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
