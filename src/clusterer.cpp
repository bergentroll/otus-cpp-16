#include "clusterer.hpp"

using namespace otus;
using namespace std;

Clusterer::Clusterer(
    vector<Flat> const & data,
    int numberOfClusters,
    float gamma,
    float accuracy):
kMeans(dlib::kcentroid(KernelType(gamma), accuracy)) {
  this->data.reserve(data.size());
  transform(
      data.begin(),
      data.end(),
      back_inserter(this->data),
      [](auto const &item) -> DataType {
        return static_cast<DataType>(item);
      });

  metaData.reserve(data.size());
  transform(
      data.begin(),
      data.end(),
      back_inserter(metaData),
      [](auto const &item) -> pair<Flat, ClusterNumType> {
        return make_pair(item, 0);
      });

  normalizeData();
  train(numberOfClusters);
}

Clusterer::Clusterer(string const & filename):
kMeans(dlib::kcentroid(KernelType())) {
  try {
    dlib::deserialize(filename + fileSuffix)
      >> kMeans >> k >> a >> metaData;
  }
  catch (dlib::serialization_error const & e) {
    throw(IOError(e.what()));
  }
}

void Clusterer::save(string const & filename) {
  try {
    dlib::serialize(filename + fileSuffix)
      << kMeans << k << a << metaData;
  }
  catch (dlib::serialization_error const & e) {
    throw(IOError(e.what()));
  }
}

string Clusterer::operator()(Flat const & flat) const {
  stringstream ss { };
  DataType item = static_cast<DataType>(flat);
  normalize(item);

  auto cluster { kMeans(item) };

  vector<pair<float, string>> neighbours { };

  for (auto &md: metaData) {
    if (md.second == cluster) {
      neighbours.push_back(
          make_pair(flat.distanceTo(md.first), string(md.first)));
    }
  }

  sort(
      neighbours.begin(),
      neighbours.end(),
      [](auto &left, auto &right) {
        return left.first < right.first;
      });

  for (auto const &neighbour: neighbours) {
    ss << neighbour.second << '\n';
  }

  return ss.str();
}

void Clusterer::train(int numberOfClusters) {
  vector<DataType> initialCenters;

  kMeans.set_number_of_centers(numberOfClusters);
  pick_initial_centers(
      numberOfClusters,
      initialCenters,
      data,
      kMeans.get_kernel());

  kMeans.train(data, initialCenters);

  for (size_t i { }; i < data.size(); ++i) {
    metaData[i].second = kMeans(data[i]);
  }
}

void Clusterer::normalizeData() {
  auto min = vector<float>(dataSize, numeric_limits<float>::max());
  auto max = vector<float>(dataSize, numeric_limits<float>::min());

  for (auto & item: data) {
    for (DataSizeType i { }; i < item.size(); ++i) {
      auto value = item(i);
      if (value < min[i]) min[i] = value;
      if (value > max[i]) max[i] = value;
    }
  }

  k = max; a = min;

  for (size_t i { }; i < dataSize; ++i)
    k[i] = 100 / (k[i] - a[i]);

  for_each(
      data.begin(),
      data.end(),
      [this](auto & item) { normalize(item); });
}

void Clusterer::normalize(DataType & item) const {
    for (DataSizeType i { }; i < item.size(); ++i)
      item(i) = k[i] * (item(i) - a[i]);
}
