#include "clusterer.hpp"

using namespace otus;
using namespace std;

void Clusterer::operator()(int numberOfClusters) {
  vector<DataType> initialCenters;

  kMeans.set_number_of_centers(numberOfClusters);
  pick_initial_centers(
      numberOfClusters,
      initialCenters,
      data,
      kMeans.get_kernel());

  kMeans.train(data, initialCenters);
}

void Clusterer::normalize() {
  auto min = vector<float>(dataSize, numeric_limits<float>::max());
  auto max = vector<float>(dataSize, numeric_limits<float>::min());

  using DataSizeType = decltype(DataType().size());

  for (auto & item: data) {
    for (DataSizeType i { }; i < item.size(); ++i) {
      auto value = item(i);
      if (value < min[i]) min[i] = value;
      if (value > max[i]) max[i] = value;
    }
  }

  // y = kx - a
  vector<float> k { max }, a { min };

  for (size_t i { }; i < dataSize; ++i) {
    k[i] = 100 / (k[i] - a[i]);
  }

  for_each(
      data.begin(),
      data.end(),
      [&k, &a](auto & item) {
        for (DataSizeType i { }; i < item.size(); ++i)
          item(i) = k[i] * (item(i) - a[i]);
      });
}
