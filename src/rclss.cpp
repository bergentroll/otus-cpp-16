#include <iostream>
#include <limits>
#include <memory>

#include "clusterer.hpp"
#include "get_args.hpp"
#include "utility.hpp"

using namespace std;
using namespace otus;

int main(int argc, char const ** argv) {
  string inputFileName;

  if (argc == 2) {
    inputFileName = argv[1];
  }
  else {
    cerr << "Error: exactly one argument expected" << endl;
    return 1;
  }

  unique_ptr<Clusterer> clustererPtr;
  try {
    clustererPtr = make_unique<Clusterer>(inputFileName);
  }
  catch (Clusterer::IOError const & e) {
    cerr << "Error while loading data: " << uncapitalize(e.what()) << endl;
    return EXIT_FAILURE;
  }
  Clusterer & clusterer { *clustererPtr };

  string buf;

  while (getline(cin, buf)) {
    try {
      Flat item { buf };
      cout << clusterer(item) << std::endl;
    }
    catch (InvalidToken const &e) {
      cerr << "Invalid input \"" << buf << "\": " <<  e.what() << endl;
    }
  }
}
