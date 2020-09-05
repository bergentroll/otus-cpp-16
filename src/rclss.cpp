#include <iostream>
#include <limits>

#include "clusterer.hpp"
#include "get_args.hpp"

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

  Clusterer clusterer { inputFileName };
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
