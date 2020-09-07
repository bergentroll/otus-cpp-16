#include <iostream>
#include <limits>

#include "clusterer.hpp"
#include "get_args.hpp"
#include "utility.hpp"

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

  vector<Flat> data { };

  string buf;

  while (getline(cin, buf)) {
    try {
      data.push_back(buf);
    }
    catch (ParsingError const &e) {
      cerr << "Invalid input \"" << buf << "\": " <<  e.what() << endl;
    }
  }

  Clusterer clusterer { data, numberOfClusters, 0.001 };
  data.clear();

  try {
    clusterer.save(outputFileName);
  }
  catch (Clusterer::IOError const & e) {
    cerr << "Error while saving data: " << uncapitalize(e.what()) << endl;
    return EXIT_FAILURE;
  }
}
