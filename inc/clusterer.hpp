#pragma once

#include <algorithm>
#include <dlib/clustering.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "flat.hpp"

namespace otus {
  class Clusterer {
  public:
    class IOError: public std::runtime_error {
    public:
      IOError(std::string const & message):
      std::runtime_error(message) { }
    };

    Clusterer(
        std::vector<Flat> const & data,
        int numberOfClusters,
        float gamma=0.1,
        float accuracy=0.01);

    Clusterer(std::string const & filename);

    void save(std::string const & filename);

    std::string operator()(Flat const & flat) const;

  private:
    using KernelType = dlib::radial_basis_kernel<DataType>;
    using DataSizeType = decltype(DataType().size());
    using ClusterNumType = unsigned long int;

    std::string const fileSuffix { ".dat" };
    std::vector<DataType> data;
    std::vector<std::pair<Flat, ClusterNumType>> metaData;
    dlib::kkmeans<KernelType> kMeans;
    /// For normalization function y = kx - a.
    std::vector<float> k, a; // TODO Make serializable functor.

    void normalizeData();

    void normalize(DataType & item) const;

    void train(int numberOfClusters);
  };
}
