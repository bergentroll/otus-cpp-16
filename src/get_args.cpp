#include <cstring>

#include "get_args.hpp"


using namespace std;

namespace otus {
  pair<int,string> get_args(int argc, char const *argv[]) {
    int N;
    size_t idx;

    if (argc != 3) throw(InvalidArgument("two aguments expected"));

    try {
      N = stoi(argv[1], &idx);
    } catch (invalid_argument &e) {
      throw InvalidArgument("first argument must be integer");
    } catch (out_of_range &e) {
      throw InvalidArgument("first argument is out of range");
    }

    if (idx < strlen(argv[1])) {
      throw InvalidArgument("first argument must be integer");
    }
    if (N < 1) {
      throw InvalidArgument("first argument must be greater than zero");
    }

    return make_pair(N, argv[2]);
  }
}
