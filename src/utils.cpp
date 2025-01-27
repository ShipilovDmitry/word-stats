#include "utils.hpp"
#include <iostream>

std::optional<CmdArguments> getCmdAgruments(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " path/to/input.txt path/to/output.txt"
              << std::endl;
    return std::nullopt;
  }

  return CmdArguments{argv[1], argv[2]};
};
