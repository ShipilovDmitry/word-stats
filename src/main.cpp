#include "utils.hpp"

int main(int argc, char *argv[]) {
  auto const cmdArguments = getCmdAgruments(argc, argv);
  if (!cmdArguments) {
    return -1;
  }

  return 0;
}
