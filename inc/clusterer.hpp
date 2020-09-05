#pragma once

#include <algorithm>
#include <dlib/clustering.h>
#include <sstream>
#include <string>
#include <tuple>
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

      metaData.reserve(data.size());
      std::transform(
          data.begin(),
          data.end(),
          std::back_inserter(metaData),
          [](auto const &item) -> std::pair<std::string, ClusterNumType> {
            return std::make_pair(std::string(item), 0);
          });

      normalizeData();
      train(numberOfClusters);
    }

    Clusterer(std::string const & filename):
    kMeans(dlib::kcentroid(KernelType())) {
      // TODO Handle errors
      dlib::deserialize(filename + fileSuffix)
        >> kMeans >> k >> a >> metaData;
    }

    void save(std::string const & filename) {
      // TODO Handle errors
      dlib::serialize(filename + fileSuffix)
        << kMeans << k << a << metaData;
    }

    std::string operator()(Flat const & flat) const {
      std::stringstream ss { };
      DataType item = static_cast<DataType>(flat);
      normalize(item);

      auto cluster { kMeans(item) };
      ss << cluster << '\n';

      for (auto &md: metaData)
        if (md.second == cluster)
          ss << md.first << '\n';

      return ss.str();
    }

  private:
    using KernelType = dlib::radial_basis_kernel<DataType>;
    using DataSizeType = decltype(DataType().size());
    using ClusterNumType = unsigned long int;

    std::string const fileSuffix { ".dat" };
    std::vector<DataType> data;
    std::vector<std::pair<std::string, ClusterNumType>> metaData;
    dlib::kkmeans<KernelType> kMeans;
    /// For normalization function y = kx - a.
    std::vector<float> k, a; // TODO Make serializable functor.

    void normalizeData();

    void normalize(DataType & item) const {
        for (DataSizeType i { }; i < item.size(); ++i)
          item(i) = k[i] * (item(i) - a[i]);
    }

    void train(int numberOfClusters);
  };
}
