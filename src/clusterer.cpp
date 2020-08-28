#include "clusterer.hpp"

using namespace otus;
using namespace std;

void Clusterer::operator()(int numberOfClusters) {
  dlib::kkmeans<kernel_type> kMeans(kCentroid);

  vector<sample_type> samples;
  vector<sample_type> initialCenters;

  for (auto const & point: data)
    samples.push_back({ point.x, point.y });

  data.clear();

  kMeans.set_number_of_centers(numberOfClusters);
  pick_initial_centers(
      numberOfClusters,
      initialCenters,
      samples,
      kMeans.get_kernel());

  kMeans.train(samples, initialCenters);

  for (auto const &i: samples) data.push_back({kMeans(i), i(0), i(1)});
}
