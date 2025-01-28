#include "utils.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  // Suppose, that using exceptions is ok
  try {
    doLogic(argc, argv);
  } catch (std::exception const &ex) {
    std::cerr << "Exception while running: " << ex.what() << std::endl;
    return -1;
  }

  return 0;
}
