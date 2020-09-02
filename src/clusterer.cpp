#include "clusterer.hpp"

using namespace otus;
using namespace std;

void Clusterer::operator()(int numberOfClusters) {
  dlib::kkmeans<KernelType> kMeans(kCentroid);

  vector<DataType> initialCentersI;
  vector<DataType> & initialCenters { initialCentersI };

  kMeans.set_number_of_centers(numberOfClusters);
  pick_initial_centers(
      numberOfClusters,
      initialCenters,
      data,
      kMeans.get_kernel());

  kMeans.train(data, initialCenters);

  //for (auto const &i: data) cout << kMeans(i) << DataType(i)(0) << endl;
}
