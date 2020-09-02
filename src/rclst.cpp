#include <iostream>
#include <limits>

#include "clusterer.hpp"
#include "get_args.hpp"

using namespace std;
using namespace otus;

int main(int argc, char const ** argv) {
  int numberOfClusters;
  string outputFileName;

  try {
    auto result { get_args(argc, argv) };
    numberOfClusters = result.first;
    outputFileName = result.second;

  }
  catch (InvalidArgument const &e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  std::vector<Flat> data { };

  string buf;

  while (getline(cin, buf)) data.push_back(buf);

  Clusterer clusterer { data, 0.001 };
  data.clear();

  clusterer(numberOfClusters);
}
