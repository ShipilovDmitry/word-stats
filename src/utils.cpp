#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

std::optional<CmdArguments> getCmdAgruments(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " path/to/input.txt path/to/output.txt"
              << std::endl;
    return std::nullopt;
  }

  return CmdArguments{argv[1], argv[2]};
};

StatsTable getStatsTable(std::string const &input) {
  std::ifstream fin(input);
  if (!fin.is_open()) {
    std::cerr << "Incorrect input file!" << input << std::endl;
    throw std::logic_error{"Incorrect input file!"};
  }

    using StatsTable = std::map<std::string, size_t>;
  StatsTable table;
  std::string word;

  // TODO read more intellectual, not byte by byte
  while (!fin.eof()) {
    // Do we need check for fin.fail() or fin.bad()?

    // It is possible to use std::alpa()
    // but we don't want to cal `tolower` on lowe case letters
    char const c = fin.get();
    if (c >= 'a' && c <= 'z') {
      word.push_back(c);
      continue;
    }

    if (c >= 'A' && c <= 'Z') {
      word.push_back(std::tolower(c));
      continue;
    }

    // We met not symbol
    // or reached end of file
    if (!word.empty()) {
      ++table[word];
      word.clear();
      continue;
    }
  }

  return table;
}
